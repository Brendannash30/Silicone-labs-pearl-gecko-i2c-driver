/**
 * @file
 *  letimer.h
 * @author
 *  Brendan Nash
 * @date
 *  4/18/23
 *
 */
#ifndef LETIMER_HG
#define LETIMER_HG

/**include files*/
//***********************************************************************************
// Include files
//***********************************************************************************
#include <stdint.h>
#include <stdbool.h>
/* Silicon Labs include statements */
#include "em_cmu.h"
#include "em_assert.h"
#include "em_letimer.h"
#include "em_gpio.h"
#include "sleep_routines.h"

/* The developer's include statements */
#include "scheduler.h"
#include "app.h"
/**defined files*/
//***********************************************************************************
// defined files
//***********************************************************************************


/**global variable*/
//***********************************************************************************
// global variables
//***********************************************************************************
typedef struct {
bool debugRun; // True, keep LETIMER running while halted
bool enable; // enable LETIMER upon completion of open()
uint32_t out_pin_route0; // out 0 route to gpio port/pin
uint32_t out_pin_route1; // out 1 route to gpio port/pin
uint32_t out_pin_0_en; // enable out 0 route
uint32_t out_pin_1_en; // enable out 1 route
float period; // total period in seconds
float active_period; // part of period that is LLH in seconds
bool comp0_irq_enable; // enable interrupt on comp0 interrupt
uint32_t comp0_cb; // comp0 callback (unique for scheduler)
bool comp1_irq_enable; // enable interrupt on comp1 interrupt
uint32_t comp1_cb; // comp1 callback (unique for scheduler)
bool uf_irq_enable; // enable interrupt on underflow interrupt
uint32_t uf_cb; // underflow cb (unique for scheduler)
}APP_LETIMER_PWM_TypeDef;

/**function prototypes*/
//***********************************************************************************
// function prototypes
//***********************************************************************************
void letimer_pwm_open(LETIMER_TypeDef *letimer, APP_LETIMER_PWM_TypeDef *app_letimer_struct);
void letimer_start(LETIMER_TypeDef *letimer, bool enable);


#endif
