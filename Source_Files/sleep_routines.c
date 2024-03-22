/**
 * @file
 *  sleep_routines.c
 * @author
 *  Brendan Nash
 * @date
 *  4/4/23
 *
 */

//***********************************************************************************
// Include files
//***********************************************************************************
#include "sleep_routines.h"


//***********************************************************************************
// private variables
//***********************************************************************************
static uint32_t lowest_energy_mode[MAX_ENERGY_MODES];
//***********************************************************************************
// functions
//***********************************************************************************
/**functions*/
/***************************************************************************//**
 * @brief
 *  Initializes default sleep modes in array
 *
 *
 * @details
 *  sets all elements of lowest_energy_mode[] array to 0.
 *
 *
 ******************************************************************************/
void sleep_open(void)
{
  for(int i = 0; i<5; i++)
    {
      lowest_energy_mode[i] = 0;
    }
 // sleep_block_mode(EM4);
}
/***************************************************************************//**
 * @brief
 * blocks energy mode that peripherals are allowed to go into.
 *
 * @details
 *  increments element of array associated with the argument (energy mode)
 * @param
 *  takes the energy state that will be blocked
 *
 ******************************************************************************/
void sleep_block_mode(uint32_t EM)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  lowest_energy_mode[EM]++;
  EFM_ASSERT(lowest_energy_mode[EM] < 5);
  CORE_EXIT_CRITICAL();

}
/***************************************************************************//**
 * @brief
 *  unblocks energy mode that peripherals are allowed to go into.
 *
 * @details
 *  decrements element of array associated with the argument (energy mode)
 *
 *  @param
 *    takes the energy state that will be unblocked
 *
 ******************************************************************************/
void sleep_unblock_mode(uint32_t EM)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  lowest_energy_mode[EM]--;
  EFM_ASSERT(lowest_energy_mode[EM] >= 0);
  CORE_EXIT_CRITICAL();
}
/***************************************************************************//**
 * @brief
 *  enter_sleep enters mode
 *
 *
 * @details
 *  enters sleep mode under proper conditions
 ******************************************************************************/
void enter_sleep(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  if(lowest_energy_mode[EM0]>0)
  {
      //do nothing
  }
  else if(lowest_energy_mode[EM1]>0)
  {
     //do nothing
  }
  else if(lowest_energy_mode[EM2]>0)
  {
      EMU_EnterEM1();
  }
  else if(lowest_energy_mode[EM3]>0)
   {
      EMU_EnterEM2(true);
   }
  else
    {
      EMU_EnterEM3(true);
    }
  CORE_EXIT_CRITICAL();
  return;
}
/***************************************************************************//**
 * @brief
 *  current_block_energy_mode is a getter for blocked energy modes
 *
 * @details
 *  returns integer of blocked energy mode index
 ******************************************************************************/
uint32_t current_block_energy_mode(void)
{
  for(int i = 0; i < MAX_ENERGY_MODES; i++)
    {
      if(lowest_energy_mode[i] > 0)
        {
          return i;
        }
    }
          return MAX_ENERGY_MODES;


}
