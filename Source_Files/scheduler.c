/**
 * @file
 *  scheduler.c
 * @author
 *  Brendan Nash
 * @date
 *  4/25/23
 *
 */

#include "scheduler.h"

/**include files*/

/**defined files*/

/**private variables*/
static uint32_t event_scheduled;

/**functions*/
/***************************************************************************//**
 * @brief
 *  scheduler_open initializes event_scheduled to 0.
 *
 * @details
 *atomic function
 ******************************************************************************/
void scheduler_open(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  event_scheduled = 0x0;
  CORE_EXIT_CRITICAL();
}
/***************************************************************************//**
 * @brief
 *  get_scheduled_events returns event_scheduled.
 *
 * @details
 *  thats all it does.
 ******************************************************************************/
uint32_t get_scheduled_events(void)
{
  return event_scheduled;
 }
/***************************************************************************//**
 * @brief
 *  add_scheduled_event schedules event.
 *
 * @details
 *  atomic function.
 *  sets bit in event_scheduled to be first or second bit.
 * @param
 *  takes either a one or two to set event_scheduled bits.
 ******************************************************************************/
void add_scheduled_event(uint32_t event)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  event_scheduled |= event;
  CORE_EXIT_CRITICAL();
}
/******************************************************************add_scheduled_event*********//**
 * @brief
 *  remove_scheduled_event sets the bit in event_secheduled that is set by
 *  add_scheduled_event to 0.
 *
 * @details
 *  atomic function.
 *  removes bit in event_scheduled (first or second bit).
 * @param
 *  takes either a one or two to remove event_scheduled bits.
 ******************************************************************************/
void remove_scheduled_event(uint32_t event)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  event_scheduled &= ~event;
  CORE_EXIT_CRITICAL();
}
