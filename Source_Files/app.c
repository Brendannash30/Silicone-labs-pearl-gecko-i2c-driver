/**
 * @file
 *  app.c
 * @author
 *  Brendan Nash
 * @date
 *  4/25/23
 *
 */

//***********************************************************************************
// Include files
//***********************************************************************************
#include "app.h"
/***************************************************************************//**
 * @brief
 *  sched_letimer_comp0_cb is called when letimer interrupt is fired for comp0
 *
 * @details
 *  to be determined
 ******************************************************************************/
void sched_letimer_comp0_cb(void)
{
  //to be filled in
}

/***************************************************************************//**
 * @brief
 *  sched_letimer_comp1_cb is called when letimer interrupt is fired for comp1
 *
 * @details
 *  to be determined
 ******************************************************************************/
void sched_letimer_comp1_cb(void)
{
  //to be filled in
}
/***************************************************************************//**
 * @brief
 *  sched_letimer_uf_cb is called when letimer interrupt is fired for comp1
 *
 * @details
 *  calls si7021_read and
 ******************************************************************************/
 void sched_letimer_uf_cb(void)
{
  si7021_read(I2C0);
}
 /***************************************************************************//**
  * @brief
  *  i2c_read_c is called to read scheduled callback
  *
  * @details
  *  calls get data.
  *  turns on led 0 if temperature is greater than 20 degrees celsius
  *  turns on led 0 off temperature is less than 20 degrees celsius
  ******************************************************************************/
 void i2c_read_cb()
 {
   float temperature = get_data();
   if(temperature > 20)
     {
       GPIO_PinOutSet(LED0_PORT, LED0_PIN);
     }
   else
     {
       GPIO_PinOutClear(LED0_PORT, LED0_PIN);
     }

 }
/***************************************************************************//**
 * @brief
 *  app_letimer_pwm_open sets all members of APP_LETIMER_PWM_TypeDef struct.
 *
 * @details
 *  members of APP_LETIMER_PWM_TypeDef are used to set LETIMER LETIMER_TypeDef registers
 *  in letimer_pwm_open.
 *  calling letimer_pwm_open begins this process.
 *
 *  note:out_pin_0_en and out_pin_1_en have been changed to uint32_t data types and
 *       the bitmasks have been passed in as arguments.
 * @param
 *  period, active period, led route location 0, led route location 1, led bit 0, led bit 1
 *
 ******************************************************************************/
void app_letimer_pwm_open(float period, float act_period, uint32_t out0_route, uint32_t out1_route, uint32_t pin_0_en, uint32_t pin_1_en)
{
  APP_LETIMER_PWM_TypeDef app_letimer_struct;
  app_letimer_struct.debugRun = false; // True, keep LETIMER running while halted
  app_letimer_struct.enable = false; // enable LETIMER upon completion of open()
  app_letimer_struct.period = period; // total period in seconds
  app_letimer_struct.active_period = act_period; // part of period that is LLH in seconds
  app_letimer_struct.comp0_irq_enable = false; // enable interrupt on comp0 interrupt
  app_letimer_struct.comp0_cb = LETIMER0_COMP0_CB; // comp0 callback (unique for scheduler)
  app_letimer_struct.comp1_irq_enable = true; // enable interrupt on comp1 interrupt
  app_letimer_struct.comp1_cb = LETIMER0_COMP1_CB; // comp1 callback (unique for scheduler)
  app_letimer_struct.uf_irq_enable = true; // enable interrupt on underflow interrupt
  app_letimer_struct.uf_cb = LETIMER0_UF_CB; // underflow cb (unique for scheduler)

  app_letimer_struct.out_pin_route0 = out0_route; // out 0 route to gpio port/pin
  app_letimer_struct.out_pin_route1 = out1_route; // out 1 route to gpio port/pin
  app_letimer_struct.out_pin_0_en = pin_0_en; // enable out 0 route
  app_letimer_struct.out_pin_1_en = pin_1_en; // enable out 1 route

  letimer_pwm_open(LETIMER0, &app_letimer_struct);

}
/***************************************************************************//**
 * @brief
 *  app_peripheral_setup calls all relevant functions so only one function call
 *  will be needed in main.c.
 *
 * @details
 *  scheduler_open initializes event_scheduled to 0.
 *  game_gpio initializes "game struct" and calls gpio_open to initialize interrupts
 *  to the buttons. gpio_open calls start_game to enable gpio interrupts.
 *  app_letimer_pwm_open() initializes members of APP_LETIMER_PWM_TypeDef struct
 *  letimer_start begins letimer
 *
 ******************************************************************************/
void app_peripheral_setup(void)
{
  cmu_open();
	scheduler_open();
//	game_gpio_open();
	gpio_open();
	sleep_open();
	app_letimer_pwm_open(PWM_PER, PWM_ACT_PER, LETIMER_ROUTELOC0_OUT0LOC_LOC29, LETIMER_ROUTELOC0_OUT1LOC_LOC28, LETIMER_ROUTEPEN_OUT0PEN, LETIMER_ROUTEPEN_OUT1PEN);
	letimer_start(LETIMER0, true);
  si7021_i2c_open(I2C0);
}

