/**
 * @file
 *  i2c.c
 * @author
 *  Brendan Nash
 * @date
 *  4/25/23
 *
 */


#include "i2c.h"


static I2C_STATE_MACHINE i2c_sm;
//***********************************************************************************
// public functions
//***********************************************************************************
/***************************************************************************//**
 * @brief
 *  i2c_open initializes/enables everything needed for i2c communication to function.
 *
 * @details
 *  initializes clocks for I2C0 or I2C1 peripheral.
 *  initializes i2c peripheral.
 *  maps values the I2C_OPEN_STRUCT to I2C_TypeDef.
 *  routes pins and location for SDA and SCL.
 *  enables NVIC for I2C0 or I2C1 peripheral.
 *  blocks energy mode 2 (energy mode 1 is active).
 *  calls i2c_bus_reset and passes either I2C0 or I2C1 peripheral to it.
 * @param
 *  I2C peripheral typedef  struct from HAL and I2C open struct.
 *
 ******************************************************************************/
void i2c_open(I2C_TypeDef *i2c_peripheral, I2C_OPEN_STRUCT *i2c_open)
{
  I2C_Init_TypeDef I2C_Init_Struct;

  if(i2c_peripheral == I2C0)
    {
      CMU_ClockEnable(cmuClock_I2C0, true);//enable i2c0 clock
    }
  else if(i2c_peripheral == I2C1)
    {
      CMU_ClockEnable(cmuClock_I2C1, true);//enable i2c1 clock
    }
  else
    {
      EFM_ASSERT(false);
    }
  if(!(i2c_peripheral->IF & I2C_IF_START))
    {
      i2c_peripheral->IFS = I2C_IF_START;//set the start interrupt flag
    }
  else if(i2c_peripheral->IF & I2C_IF_START)
    {
      i2c_peripheral->IFC = I2C_IF_START;//clear the start interrupt flac
    }

  I2C_Init_Struct.enable   = i2c_open->enable;
  I2C_Init_Struct.master   = i2c_open->master;
  I2C_Init_Struct.refFreq  = i2c_open->refFreq;
  I2C_Init_Struct.freq     = i2c_open->freq;
  I2C_Init_Struct.clhr     = i2c_open->c1hr;
  //initialize I2C peripheral
  I2C_Init(i2c_peripheral, &I2C_Init_Struct);

  //route locations for SDA/SCL
  i2c_peripheral->ROUTELOC0 |= i2c_open->out_pin_route_SCL;
  i2c_peripheral->ROUTELOC0 |= i2c_open->out_pin_route_SDA;
  //route pins for SDA/SCL
  i2c_peripheral->ROUTEPEN  |= i2c_open->out_pin_SCL_en;
  i2c_peripheral->ROUTEPEN  |= i2c_open->out_pin_SDA_en;

  //enable i2c interrupts
  i2c_peripheral->IFC = _I2C_IFC_MASK;
  i2c_peripheral->IEN |= I2C_IEN_RXDATAV;
  i2c_peripheral->IEN |= I2C_IEN_MSTOP;
  i2c_peripheral->IEN |= I2C_IEN_ACK;
  i2c_peripheral->IEN |= I2C_IEN_NACK;

  if(i2c_peripheral == I2C0)
  {
      NVIC_EnableIRQ(I2C0_IRQn);
  }
  else if(i2c_peripheral == I2C1)
  {
      NVIC_EnableIRQ(I2C1_IRQn);
  }

   sleep_block_mode(EM2); //block energy mode 2
   i2c_bus_reset(i2c_peripheral);
}
/***************************************************************************//**
 * @brief
 *  i2c_start begins i2c communication.
 *
 * @details
 *  sets all members of I2C_STATE_MACHINE struct via input parameters.
 *
 * @param
 *  i2c device address, read or write bit, busy status, current number of bytes, bytes,
 *  expected, command mode, i2c data, callback bit, i2c peripheral.
 *
 ******************************************************************************/
void i2c_start(uint32_t device_address, uint32_t RW, uint32_t busy, uint32_t currbyte, uint32_t numbyte, uint32_t mode, uint32_t data, uint32_t cb, I2C_TypeDef *i2c_peripheral)
{

  I2C_STATE_MACHINE i2c_start_sm;

  i2c_start_sm.i2c                 = i2c_peripheral;
  i2c_start_sm.i2c_device_adress   = device_address;
  i2c_start_sm.i2c_cmd             = RW;
  i2c_start_sm.i2c_busy            = busy;
  i2c_start_sm.i2c_curr_bytes      = currbyte;
  i2c_start_sm.i2c_num_bytes       = numbyte;
  i2c_start_sm.i2c_register_adress = mode;
  i2c_start_sm.i2c_data            = data;
  i2c_start_sm.i2c_cb              = cb;
  i2c_start_sm.current_state       = I2C_INIT;

  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  i2c_sm = i2c_start_sm;
  CORE_EXIT_CRITICAL();

  i2c_sm.i2c->CMD = I2C_CMD_START;

  i2c_peripheral->TXDATA = (device_address<<1)|WRITE_BIT;

}



/***************************************************************************//**
 * @brief
 *  i2c_bus_reset resets the communication bus.
 *
 * @details
 *  ensures i2c peripheral SM is available to accept a command.
 *  clears: IEN register, IFC register, transmit buffer from CMD register.
 *  checks status of transmit buffer and sets either start or stop bits if clear.
 *  checks if MSTOP interrupt flag is raised.
 *  clears(again): IEN register, IFC register, transmit buffer from CMD register.
 ******************************************************************************/
void i2c_bus_reset(I2C_TypeDef *i2c_peripheral)
{
  i2c_peripheral->CMD      = I2C_CMD_ABORT;       //ensure i2c peripheral SM is available to accept a command
  uint32_t I2C_IEN_STORE   = i2c_peripheral->IEN; //store state of IEN register
  i2c_peripheral->IEN      = _I2C_IEN_RESETVALUE;      //clear IEN register
  i2c_peripheral->IFC      = _I2C_IFC_MASK;//clear IFC register
  i2c_peripheral->CMD      = I2C_CMD_CLEARTX;     //clear i2c transmit buffer
  i2c_peripheral->CMD      = I2C_IFC_MSTOP;
  i2c_peripheral->CMD      = (I2C_CMD_START | I2C_CMD_STOP);

  /*if((i2c_peripheral->STATUS & I2C_STATUS_TXBL))   //set stop/start if the transmit buffer is cleared
  {
       i2c_peripheral->CMD = I2C_CMD_START | I2C_CMD_STOP;
  }*/


  while(!(i2c_peripheral->IF & I2C_IF_MSTOP));      //check if MSTOP IF is raised

  i2c_peripheral->IFC = _I2C_IFC_MASK;       //clear IFC
  i2c_peripheral->CMD = I2C_CMD_ABORT;
  i2c_peripheral->IEN = I2C_IEN_STORE;
}

/***************************************************************************//**
 * @brief
 *  get_data uses input data to calculate temperature
 * @details
 *  calculates temperature with equation from si7021 data sheet
 *      -could be abstracted away with function in si7021.c
 *  can be edited for other ic's
 ******************************************************************************/
float get_data()
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  float temperature_C = ((175.71 * (((float)i2c_sm.i2c_data) / 65536)) - 46.85);
  CORE_EXIT_CRITICAL();

  return temperature_C;
}

//***********************************************************************************
// private functions
//***********************************************************************************
/***************************************************************************//**
 * @brief
 *  send_i2c_ack is a manual ack
 * @details
 *  sets ack bit in command register
 * @param
 *  I2C_STATE_MACHINE
 ******************************************************************************/
static void send_i2c_ack (I2C_STATE_MACHINE *i2c_sm)
{
  //set ack bit in command register
  i2c_sm->i2c->CMD = I2C_CMD_ACK;
}
/***************************************************************************//**
 * @brief
 *  send_i2c_nack is a manual nack
 * @details
 *  sets nack bit in command register
 * @param
 *  I2C_STATE_MACHINE
 ******************************************************************************/
static void send_i2c_nack (I2C_STATE_MACHINE *i2c_sm)
{
  //set nack bit in command register
  i2c_sm->i2c->CMD = I2C_CMD_NACK;
}
/***************************************************************************//**
 * @brief
 *  send_i2c_stop is a manual stop
 * @details
 *  sets stop bit in command register
 * @param
 *  I2C_STATE_MACHINE
 ******************************************************************************/
static void send_i2c_stop (I2C_STATE_MACHINE *i2c_sm)
{
  //set nack bit in command register
  i2c_sm->i2c->CMD = I2C_CMD_STOP;
}

/***************************************************************************//**
 * @brief
 *  i2c_ack is a helper function used for ack interrupt
 *
 * @details
 *  state machine that handles i2c init, i2crx send command, and i2c send address
 * @param
 *  I2C_STATE_MACHINE
 ******************************************************************************/
static void i2c_ack (I2C_STATE_MACHINE *i2c_sm)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  switch(i2c_sm->current_state)
  {
    case(I2C_INIT):
     if(i2c_sm->i2c_num_bytes > 2)
      {
        i2c_sm->current_state = I2C_CLOSED;
        send_i2c_stop (i2c_sm);
        break;
       }
     i2c_sm->i2c->TXDATA = i2c_sm->i2c_register_adress;
     i2c_sm->current_state = I2C_RX_SEND_CMD;
     break;
    case(I2C_RX_SEND_CMD):
     i2c_sm->i2c->CMD = I2C_CMD_START;
     i2c_sm->i2c->TXDATA = (i2c_sm->i2c_device_adress<<1)|READ_BIT;
     i2c_sm->current_state = I2C_RX_SEND_ADDR;
     break;
    case(I2C_RX_SEND_ADDR):
     i2c_sm->current_state = I2C_RX_RECEIVE_DATA;
     break;
    default:
      EFM_ASSERT(false);
      break;

  }
  timer_delay(MILS_DELAY);
  CORE_EXIT_CRITICAL();
}
/***************************************************************************//**
 * @brief
 *  i2c_nack is a helper function used for nack interrupt
 *
 * @details
 *  state machine that handles i2c init, i2crx send command, and i2c send address
 * @param
 *  I2C_STATE_MACHINE
 ******************************************************************************/
static void i2c_nack (I2C_STATE_MACHINE *i2c_sm)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  switch(i2c_sm->current_state)
  {
    case(I2C_INIT):
      if(i2c_sm->i2c_num_bytes > 2)
        {
          i2c_sm->current_state = I2C_CLOSED;
          send_i2c_stop (i2c_sm);
          break;
         }
      i2c_sm->i2c->CMD = I2C_CMD_START;
      i2c_sm->i2c->TXDATA = (i2c_sm->i2c_device_adress<<1) | WRITE_BIT;
      break;
    case(I2C_RX_SEND_CMD):
      i2c_sm->i2c->CMD = I2C_CMD_CONT;
      i2c_sm->i2c->TXDATA = i2c_sm->i2c_register_adress;
      break;
    case(I2C_RX_SEND_ADDR):
      i2c_sm->i2c->CMD = I2C_CMD_START;
      i2c_sm->i2c->TXDATA = (i2c_sm->i2c_device_adress << 1) | READ_BIT;
      break;
    default:
      EFM_ASSERT(false);
      break;
  }
  timer_delay(MILS_DELAY);
  CORE_EXIT_CRITICAL();
}
/***************************************************************************//**
 * @brief
 *  i2c_rxdatav is a helper function used for rxdatav interrupt
 *
 * @details
 *  state machine that handles 12c rx receive data
 *
 * @param
 *  I2C_STATE_MACHINE
 ******************************************************************************/
static void i2c_rxdatav (I2C_STATE_MACHINE *i2c_sm)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  switch(i2c_sm->current_state)
  {
    case(I2C_RX_RECEIVE_DATA):

      i2c_sm->i2c_data |= (i2c_sm->i2c->RXDATA<< (8 * (i2c_sm->i2c_curr_bytes - 1)));

      if(i2c_sm->i2c_curr_bytes == i2c_sm->i2c_num_bytes)
        {
          i2c_sm->i2c_curr_bytes --;
          send_i2c_ack (i2c_sm);
        }
      else
        {
          if(i2c_sm->i2c->RXDATA){};
          i2c_sm->current_state = I2C_CLOSED;
          send_i2c_nack (i2c_sm);
          send_i2c_stop (i2c_sm);
        }
      break;
    default:
      EFM_ASSERT(false);
      break;
  }
  timer_delay(MILS_DELAY);
  CORE_EXIT_CRITICAL();
}
/***************************************************************************//**
 * @brief
 *  i2c_mstop is a helper function used for mstop interrupt
 *
 * @details
 *  state machine that handles 12c closed
 * @param
 *  I2C_STATE_MACHINE
 ******************************************************************************/
static void i2c_mstop (I2C_STATE_MACHINE *i2c_sm)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  switch(i2c_sm->current_state)
  {
    case(I2C_CLOSED):
      i2c_bus_reset(i2c_sm->i2c);
      i2c_sm->i2c_busy = 0;
      add_scheduled_event(i2c_sm->i2c_cb);
      sleep_unblock_mode(EM2);
      break;
    default:
      EFM_ASSERT(false);
      break;
  }
  timer_delay(MILS_DELAY);
  CORE_EXIT_CRITICAL();
}

//***********************************************************************************
// I2C IRQ handlers
//***********************************************************************************
/***************************************************************************//**
 * @brief
 *  I2C0_IRQHandler services ack, nack rxdatav, and mstop interrupts for i2c0
 *  peripheral.
 *
 * @details
 *  calls helper functions for their respective interrupts
 *
 ******************************************************************************/
void I2C0_IRQHandler()
{
  uint32_t flag = I2C0->IF & I2C0->IEN;

  if(flag & I2C_IF_ACK)
    {
      I2C0->IFC = I2C_IF_ACK;
      EFM_ASSERT(!(I2C0->IF & flag & I2C_IF_ACK));
      i2c_ack (&i2c_sm);
    }
  if(flag & I2C_IF_NACK)
      {
        I2C0->IFC = I2C_IF_NACK;
        EFM_ASSERT(!(I2C0->IF & flag & I2C_IF_NACK));
        i2c_nack (&i2c_sm);
      }
  if(flag & I2C_IF_RXDATAV)
      {
        i2c_rxdatav (&i2c_sm);
      }
  if(flag & I2C_IEN_MSTOP)
     {
       I2C0->IFC = I2C_IEN_MSTOP;
       EFM_ASSERT(!(I2C0->IF & flag & I2C_IEN_MSTOP));
       i2c_mstop (&i2c_sm);
     }
}
/***************************************************************************//**
 * @brief
 *  I2C1_IRQHandler services ack, nack rxdatav, and mstop interrupts for i2c1
 *  peripheral.
 *
 * @details
 *  calls helper functions for their respective interrupts
 *
 ******************************************************************************/
void I2C1_IRQHandler()
{
  uint32_t flag = I2C1->IF & I2C1->IEN;

  if(flag & I2C_IF_ACK)
    {
      I2C1->IFC |= (flag & I2C_IF_ACK);
      EFM_ASSERT(!(I2C1->IF & flag & I2C_IF_ACK));
      i2c_ack (&i2c_sm);
    }
  if(flag & I2C_IF_NACK)
      {
        I2C1->IFC |= (flag & I2C_IF_NACK);
        EFM_ASSERT(!(I2C1->IF & flag & I2C_IF_NACK));
        i2c_nack (&i2c_sm);
      }
  if(flag & I2C_IF_RXDATAV)
      {
        I2C1->IFC |= (flag & I2C_IF_RXDATAV);
        EFM_ASSERT(!(I2C1->IF & flag & I2C_IF_RXDATAV));
        i2c_rxdatav (&i2c_sm);
      }
  if(flag & I2C_IEN_MSTOP)
     {
       I2C1->IFC |= (flag & I2C_IEN_MSTOP);
       EFM_ASSERT(!(I2C1->IF & flag & I2C_IEN_MSTOP));
       i2c_mstop (&i2c_sm);
     }
}
