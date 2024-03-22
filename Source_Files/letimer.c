/**
 * @file
 *  letimer.c
 * @author
 *  Brendan Nash
 * @date
 *  4/4/23
 *
 */


//***********************************************************************************
// Include files
//***********************************************************************************
#include "letimer.h"


/**private variable*/
//***********************************************************************************
// private variables
//***********************************************************************************
static uint32_t scheduled_comp0_cb;
static uint32_t scheduled_comp1_cb;
static uint32_t scheduled_uf_cb;
/**global variable*/
//***********************************************************************************
// global variables
//***********************************************************************************
/**functions*/
//***********************************************************************************
// function
//***********************************************************************************

/***************************************************************************//**
 * @brief
 *  letimer_pwm_open enables the PWM from LETIMER.
 *
 * @details
 *  uses initialized members of APP_LETIMER_PWM_TypeDef struct to assign LETIMER_TypeDef
 *  registers.
 *
 * @param
 *    pointer to LETIMER_TypeDef struct, pointer to APP_LETIMER_PWM_TypeDef struct.
 ******************************************************************************/
void letimer_pwm_open(LETIMER_TypeDef *letimer, APP_LETIMER_PWM_TypeDef *app_letimer_struct)
{

  uint32_t period_init     = app_letimer_struct->period * 500;
  uint32_t act_period_init = app_letimer_struct->active_period * 500 ;
  scheduled_comp0_cb       = app_letimer_struct->comp0_cb;
  scheduled_comp1_cb       = app_letimer_struct->comp1_cb;
  scheduled_uf_cb          = app_letimer_struct->uf_cb;

  CMU_ClockEnable(cmuClock_LETIMER0, true);
  while (letimer->SYNCBUSY);

  //Write to a register
  letimer->CMD |= LETIMER_CMD_START;
  //Do an EFM_ASSERT() to check the write to the register happened properly.
  while (letimer->SYNCBUSY);
  EFM_ASSERT((letimer->STATUS & LETIMER_STATUS_RUNNING));
  // Reset the register
  letimer->CMD |= LETIMER_CMD_STOP;

  while (letimer->SYNCBUSY);
  letimer->CNT = 0;
  LETIMER_Init_TypeDef letimer_init;

  letimer_init.bufTop   = true;
  letimer_init.comp0Top = true;
  letimer_init.debugRun = app_letimer_struct->debugRun;
  letimer_init.enable   = app_letimer_struct->enable;
  letimer_init.out0Pol  = SET_IDLE_OUT_POL;
  letimer_init.out1Pol  = SET_IDLE_OUT_POL;
  letimer_init.repMode  = letimerRepeatFree;
  letimer_init.ufoa0    = letimerUFOAPwm;
  letimer_init.ufoa1    = letimerUFOAPwm;

  LETIMER_Init(letimer, &letimer_init);
  while (letimer->SYNCBUSY);


  LETIMER_CompareSet(letimer, 0, period_init);
  LETIMER_CompareSet(letimer, 1, act_period_init);

  letimer->ROUTELOC0 |= app_letimer_struct->out_pin_route0;
  letimer->ROUTELOC0 |= app_letimer_struct->out_pin_route1;
  letimer->ROUTEPEN  |= app_letimer_struct->out_pin_0_en;
  letimer->ROUTEPEN  |= app_letimer_struct->out_pin_1_en;

  letimer->REP0 |= REP_REGS_NONZER0;
  letimer->REP1 |= REP_REGS_NONZER0;
  while (letimer->SYNCBUSY);

  //assign letimer interrupt enable register
  letimer->IEN  |= app_letimer_struct->comp1_irq_enable * LETIMER_IEN_COMP1; //bool 1 * shift to bit2
  letimer->IEN  |= app_letimer_struct->comp0_irq_enable * LETIMER_IEN_COMP0; //bool 0 * shifted to bit1
  letimer->IEN  |= app_letimer_struct->uf_irq_enable * LETIMER_IEN_UF ;    //bool 1 * shifted to bit3

  NVIC_EnableIRQ(LETIMER0_IRQn);

  if(letimer->STATUS == LETIMER_STATUS_RUNNING){
      sleep_block_mode(EM4);
  }

}
/***************************************************************************//**
 * @brief
 *  letimer_start starts the LETIMER
 *
 * @details
 *  satisfies conditions of not running but enables and running but not enabled.
 *  LETIMER_Enable() - function from HAL that sets command register to start timer.
 *
 * @param
 *  pointer to LETIMER_TypeDef struct, boolean variable used to enable LETIMER.
 ******************************************************************************/
void letimer_start(LETIMER_TypeDef *letimer, bool enable)
{

   if(!((letimer->STATUS & LETIMER_STATUS_RUNNING) && enable))//not running but enabled
   {
     sleep_block_mode(EM4);
     LETIMER_Enable(letimer, enable);
     while(letimer->SYNCBUSY);

   }
   if((letimer->STATUS & LETIMER_STATUS_RUNNING) && !enable) //running but not enabled
   {
     LETIMER_Enable(letimer, enable);
     while(letimer->SYNCBUSY);
     sleep_unblock_mode(EM4);
   }
}

/***************************************************************************//**
 * @brief
 *  LETIMER0_IRQHandler adds scheduled event corresponding to letimer interrupts that
 *  fire for comp0(period), comp1(active period), and uf(underflow).
 *
 * @details
 *  interrupt for comp0 is not enabled in this code. Conditional statement is there
 *  if that happens to change
 *
 ******************************************************************************/
void LETIMER0_IRQHandler(void)
{
     uint32_t intflag = (LETIMER0->IF & LETIMER0->IEN);
     LETIMER0->IFC = intflag;
     if(intflag & LETIMER_IF_COMP0)
       {
         add_scheduled_event(scheduled_comp0_cb);
         EFM_ASSERT(!(LETIMER0->IF & LETIMER_IF_COMP0));
       }
     if(intflag & LETIMER_IF_COMP1)
       {
         add_scheduled_event(scheduled_comp1_cb);
         EFM_ASSERT(!(LETIMER0->IF & LETIMER_IF_COMP1));
       }
     if(intflag & LETIMER_IF_UF)
       {
         add_scheduled_event(scheduled_uf_cb);
         EFM_ASSERT(!(LETIMER0->IF & LETIMER_IF_UF));
       }
}
