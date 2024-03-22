/**
 * @file
 *  scheduler.h
 * @author
 *  Brendan Nash
 * @date
 *  3/8/23
 *
 */
#ifndef SCHEDULER_HG
#define SCHEDULER_HG



//***********************************************************************************
// Include files
//***********************************************************************************
#include <stdint.h>
#include "em_assert.h"
#include "em_core.h"
/**defined files*/
//***********************************************************************************
// defined files
//***********************************************************************************

/**global variable*/
//***********************************************************************************
// global variables
//***********************************************************************************

/**function prototypes*/
void scheduler_open(void);
uint32_t get_scheduled_events(void);
void add_scheduled_event(uint32_t event);
void remove_scheduled_event(uint32_t event);


#endif
