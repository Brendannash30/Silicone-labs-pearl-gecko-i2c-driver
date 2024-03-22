/**
 * @file cmu.c
 *
 * @author
 *  Brendan Nash
 * @date
 * 3/7/23
 *
 */
//***********************************************************************************
// Include files
//***********************************************************************************
#include "cmu.h"

/***************************************************************************//**
 * @brief
 *  cmu_open enables clock manager unit.
 *
 * @details
 *  selects clock
 *  enables clock
 *  enables ultra low frequency Oscillator
 *  routes/enables low frequency clock
 *
 ******************************************************************************/
void cmu_open(void){

		//CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);

		CMU_ClockEnable(cmuClock_HFPER, true);

		// By default, LFRCO is enabled
		CMU_OscillatorEnable(cmuOsc_LFRCO, false, false);	// using LFXO or ULFRCO

		// Route LF clock to the LF clock tree
		//CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);
		CMU_OscillatorEnable(cmuOsc_LFXO, false, false);		// Disable LFXO
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);	// routing clock to LFA


		//CMU_ClockEnable(cmuClock_LFA, true);
		CMU_ClockEnable(cmuClock_CORELE, true);

}

