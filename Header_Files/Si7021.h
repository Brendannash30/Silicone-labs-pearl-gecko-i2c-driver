/**
 * @file
 *  Si7021.h
 * @author
 *  Brendan Nash
 * @date
 *  4/25/23
 *
 */


#ifndef SI7021_HG
#define SI7021_HG

#include "em_assert.h"
#include "em_cmu.h"
#include "em_i2c.h"
#include "scheduler.h"
#include "sleep_routines.h"
#include <stdbool.h>
#include "HW_delay.h"
#include "i2c.h"
#include "app.h"

typedef enum
{
  MRH_hold_mast = 0xE5,
  MRH_no_hold_mast = 0xF5,
  MT_hold_mast = 0xE3,
  MT_no_hold_mast = 0xF3,
  RT_from_prev_RH = 0xE0,
  RESET = 0xFE
}SI7021_MODES;

void si7021_i2c_open(I2C_TypeDef *i2c_peripheral);
void si7021_read(I2C_TypeDef *i2c_peripheral);

#endif
