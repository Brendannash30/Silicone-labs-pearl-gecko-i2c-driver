/**
 * @file
 *  sleep_routines.h
 * @author
 *  Brendan Nash
 * @date
 *  4/4/23
 *
 */

#ifndef SLEEP_ROUTINES_HG
#define SLEEP_ROUTINES_HG

//***********************************************************************************
// Include files
//***********************************************************************************
#include "em_emu.h"
#include "em_core.h"
#include "em_assert.h"

/**defined files*/
//***********************************************************************************
// defined files
//***********************************************************************************
#define EM0 0u
#define EM1 1u
#define EM2 2u
#define EM3 3u
#define EM4 4u
#define MAX_ENERGY_MODES 5u

/**global variable*/
//***********************************************************************************
// global variables
//***********************************************************************************


/**function prototypes*/
//***********************************************************************************
// function prototypes
//***********************************************************************************
void sleep_open(void);
void sleep_block_mode(uint32_t EM);
void sleep_unblock_mode(uint32_t EM);
void enter_sleep(void);
uint32_t current_block_energy_mode(void);

#endif
