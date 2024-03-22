/**
 * @file brd_config.h
 *
 * @author
 *  Brendan Nash
 * @date
 * 3/7/23
 * @brief
 *  defines for LED's, buttons, and drive strength of LEDS.
 */
#ifndef BRD_CONFIG_HG
#define BRD_CONFIG_HG

//***********************************************************************************
// Include files
//***********************************************************************************
#include "em_gpio.h"
#include "em_cmu.h"
//scheduled event call backs



//***********************************************************************************
// defined files
//***********************************************************************************
/**defined files*/
// Application scheduled events
//I2C GPIO pins
#define SI7021_SCL_PORT gpioPortC
#define SI7021_SCL_PIN 11u
#define SI7021_SDA_PORT gpioPortC
#define SI7021_SDA_PIN 10u
#define SI7021_I2C_GPIOMODE gpioModeWiredAnd
#define SI7021_SCL_DEFAULT 1
#define SI7021_SDA_DEFAULT 1
#define SI7021_ADRESS 0x40

#define SI7021_SENSOR_EN_PORT gpioPortB
#define SI7021_SENSOR_EN_PIN 10u
#define SI7021_EN_DRIVE_STRENGTH gpioDriveStrengthWeakAlternateWeak
#define SI7021_EN_GPIOMODE gpioModePushPull
#define SI7021_EN_DEFAULT 1


//call back values LETIMER interrupts
#define LETIMER0_COMP0_CB 0x04
#define LETIMER0_COMP1_CB 0x08
#define LETIMER0_UF_CB    0x10
//call back values GPIO interrupts
#define I2C_CB 0x20


//LED 0 pin is
#define	LED0_PORT     gpioPortF
#define LED0_PIN	    4u
#define LED0_DEFAULT  0 	// Default false (0) = off, true (1) = on
#define LED0_GPIOMODE	gpioModePushPull

//LED 1 pin is
#define LED1_PORT	        gpioPortF
#define LED1_PIN				  5u
#define LED1_DEFAULT			0	// Default false (0) = off, true (1) = on
#define LED1_GPIOMODE			gpioModePushPull

#define MCU_HFXO_FREQ			cmuHFRCOFreq_26M0Hz//cmuHFRCOFreq_38M0Hz

// GPIO pin setup
#define STRONG_DRIVE

#ifdef STRONG_DRIVE
	#define LED0_DRIVE_STRENGTH		gpioDriveStrengthStrongAlternateStrong
	#define LED1_DRIVE_STRENGTH		gpioDriveStrengthStrongAlternateStrong
#else
	#define LED0_DRIVE_STRENGTH		gpioDriveStrengthWeakAlternateWeak
	#define LED1_DRIVE_STRENGTH		gpioDriveStrengthWeakAlternateWeak
#endif


#endif
