/**
 * @file
 *  gpio.c
 * @author
 *  Brendan Nash
 * @date
 *  3/7/23
 *
 */
//***********************************************************************************
// Include files
//***********************************************************************************
#include "gpio.h"

//***********************************************************************************
// functions
//***********************************************************************************
/***************************************************************************//**
 * @brief
 *  gpio_open() enables gpio pins for SI7021 temp sensor
 *
 * @details
 *   enables peripheral clock,
 *   sets gpio pins for temp sensor on i2c
 ******************************************************************************/

void gpio_open(){
  // Peripheral clock enabled
  CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_DriveStrengthSet(SI7021_SENSOR_EN_PORT, SI7021_EN_DRIVE_STRENGTH);
	GPIO_PinModeSet(SI7021_SENSOR_EN_PORT, SI7021_SENSOR_EN_PIN, SI7021_EN_GPIOMODE, SI7021_EN_DEFAULT);

	GPIO_PinModeSet(SI7021_SCL_PORT, SI7021_SCL_PIN, SI7021_I2C_GPIOMODE, SI7021_SCL_DEFAULT);
	GPIO_PinModeSet(SI7021_SDA_PORT, SI7021_SDA_PIN, SI7021_I2C_GPIOMODE, SI7021_SDA_DEFAULT);

}

