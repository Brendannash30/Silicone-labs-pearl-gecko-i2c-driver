/**
 * @file
 *  app.h
 * @author
 *  Brendan Nash
 * @date
 *  3/7/23
 *
 * @brief
 *  the only added function initialization is for run game. (to accomplish
 *  encapsulation)
 */
#ifndef APP_HG
#define APP_HG

//***********************************************************************************
// Include files
//***********************************************************************************
#include "em_cmu.h"
#include "cmu.h"
#include "gpio.h"
#include <stdint.h>
#include "letimer.h"
#include "brd_config.h"
#include "i2c.h"
#include "HW_delay.h"
#include "sleep_routines.h"
#include "scheduler.h"
#include "Si7021.h"


/**defined files*/
//***********************************************************************************
// defined files
//***********************************************************************************
#define PWM_PER 3.0 // PWM period (s)
#define PWM_ACT_PER 0.25 // PWM active period (s)
#define SET_IDLE_OUT_POL 1 //Idle value for output 0 and output 1.
#define REP_REGS_NONZER0 15  //used to set first four bits REP0 and REP1 register so nonzero
#define MILS_DELAY 80

/**enum to count state*/
typedef enum
{
  ONE,
  TWO,
  THREE
}tCountState;

/**state machine struct*/
typedef struct
{
  //Current state of state machine
  tCountState state;
  //Keeps track of the # of times a btn has been pressed in the current state
  uint32_t num_press;
  //encoded event handle to be serviced
  uint32_t cb;
  //For current state, records whether btn0 has been pressed
  bool btn0;
  //For current state, records whether btn1 has been pressed
  bool btn1;
}tAPP_STATE_MACHINE;

/**function prototypes*/
void app_letimer_pwm_open(float period, float act_period, uint32_t out0_route, uint32_t out1_route, uint32_t pin_0_en, uint32_t pin_1_en);
void intialize_SM(void);
void app_peripheral_setup(void);
void run_game(void);
void SM_sched_gpio_odd_irq_cb(void);
void SM_sched_gpio_even_irq_cb(void);
void state_machine(void);
void sched_letimer_comp0_cb(void);
void sched_letimer_comp1_cb(void);
void sched_letimer_uf_cb(void);
void i2c_read_scheduled_cb();
void app_i2c_open(I2C_TypeDef *i2c_peripheral);
void i2c_read_cb();
#endif
