/**
 * @file
 *  main.c
 * @author
 *  Brendan Nash
 * @date
 *  4/4/23
 *
 */

#include "main.h"
#include "app.h"

int
main (void)
{
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;

  /* Chip errata */
  CHIP_Init ();

  CMU_HFRCOBandSet (MCU_HFXO_FREQ);				// Sets main CPU oscillator frequency
  EMU_EM23Init_TypeDef em23Init = EMU_EM23INIT_DEFAULT;
  EMU_DCDCInit (&dcdcInit);
  em23Init.vScaleEM23Voltage = emuVScaleEM23_LowPower;
  EMU_EM23Init (&em23Init);
  CMU_HFXOInit (&hfxoInit);

  /* Switch HFCLK to HFRCO and disable HFRCO */
  CMU_OscillatorEnable (cmuOsc_HFRCO, true, true);
  CMU_ClockSelectSet (cmuClock_HF, cmuSelect_HFRCO);
  CMU_OscillatorEnable (cmuOsc_HFXO, false, false);

  /* initialize all required peripheral */
  app_peripheral_setup ();

  while (1)
    {
      CORE_DECLARE_IRQ_STATE;
      CORE_ENTER_CRITICAL();
      if (!get_scheduled_events())
        {

          enter_sleep ();
        }
      CORE_EXIT_CRITICAL();

//      if (get_scheduled_events () & LETIMER0_COMP0_CB)
//        {
//          remove_scheduled_event (LETIMER0_COMP0_CB);
//          sched_letimer_comp0_cb ();
//        }
//      if (get_scheduled_events () & LETIMER0_COMP1_CB)
//        {
//          remove_scheduled_event (LETIMER0_COMP1_CB);
//          sched_letimer_comp1_cb ();
//        }
      if (get_scheduled_events () & LETIMER0_UF_CB)
        {
          remove_scheduled_event (LETIMER0_UF_CB);
          sched_letimer_uf_cb ();
        }
      if (get_scheduled_events () & I2C_CB)
        {
          remove_scheduled_event (I2C_CB);
          i2c_read_cb();
        }

    }
}

