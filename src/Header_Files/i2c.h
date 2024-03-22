/**
 * @file
 *  i2c.h
 * @author
 *  Brendan Nash
 * @date
 *  4/18/23
 *
 */


#ifndef I2C_HG
#define I2C_HG

#include "em_assert.h"
#include "em_cmu.h"
#include "em_i2c.h"
#include "scheduler.h"
#include "sleep_routines.h"
#include <stdbool.h>
#include "brd_config.h"
#include "Si7021.h"
#include "HW_delay.h"

/***************************************************************************//**
 * @brief
 *  tI2C_STATES enum contains the states to be moved through in I2C state machine
 ******************************************************************************/
typedef enum
{
  I2C_INIT,
  I2C_RX_SEND_CMD,
  I2C_RX_SEND_ADDR,
  I2C_RX_RECEIVE_DATA,
  I2C_CLOSED
}tI2C_STATES;
/***************************************************************************//**
 * @brief
 *  tI2C_CMD enum contains read and write bits for CMD register
 ******************************************************************************/
typedef enum
{
  WRITE_BIT,
  READ_BIT
}tI2C_CMD;

/***************************************************************************//**
 * @brief
 *  tI2C_OPEN_STRUCT contains members used to set i2c peripherals
 ******************************************************************************/
typedef struct
{
  bool enable;
  bool master;
  uint32_t refFreq;
  uint32_t freq;
  I2C_ClockHLR_TypeDef c1hr;
  uint32_t out_pin_route_SCL; //out 0 route to gpio port/pin
  uint32_t out_pin_route_SDA; //out 1 route to gpio port/pin
  uint32_t out_pin_SCL_en;        //enable out 0 route
  uint32_t out_pin_SDA_en;        //enable out 1 route
}I2C_OPEN_STRUCT;
/***************************************************************************//**
 * @brief
 *  I2C_STATE_MACHINE contains members used move through i2c state machine
 ******************************************************************************/
typedef struct
{

  tI2C_STATES current_state;
  I2C_TypeDef *i2c;
  volatile bool i2c_busy;
  uint32_t i2c_num_bytes;
  volatile uint32_t i2c_curr_bytes;
  uint32_t i2c_device_adress;
  uint32_t i2c_register_adress;
  uint32_t i2c_data;
  tI2C_CMD i2c_cmd;
  uint32_t i2c_cb;
}I2C_STATE_MACHINE;

/**function prototypes*/
void i2c_open(I2C_TypeDef *i2c_peripheral, I2C_OPEN_STRUCT *i2c_open);
void i2c_start(uint32_t device_address, uint32_t RW, uint32_t busy, uint32_t currbyte, uint32_t numbyte, uint32_t mode, uint32_t data, uint32_t cb, I2C_TypeDef *i2c_peripheral);
void i2c_bus_reset(I2C_TypeDef *i2c_peripheral);
void ic2_cb();
float get_data();


#endif
