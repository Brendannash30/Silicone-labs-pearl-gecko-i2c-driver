/**
 * @file
 *  Si7021.c
 * @author
 *  Brendan Nash
 * @date
 *  4/25/23
 *
 */

#include "Si7021.h"

 /***************************************************************************//**
  * @brief
  *   Si7021_open() sets all members of I2C_OPEN_STRUCT to enable Si7021 IC.
  *
  * @details
  *   enacts proper delay as per SI7021 data sheet.
  *   enables and sets all pins for Si7021 and calls i2c_open to enable communication
  *   for i2c.
  ******************************************************************************/
void si7021_i2c_open(I2C_TypeDef *i2c_peripheral)
{
   timer_delay(MILS_DELAY);

   I2C_OPEN_STRUCT i2c_open_struct;

   i2c_open_struct.enable             = true;
   i2c_open_struct.master             = true;
   i2c_open_struct.refFreq            = 0;
   i2c_open_struct.freq               = I2C_FREQ_FAST_MAX;
   i2c_open_struct.c1hr               = i2cClockHLRAsymetric;
   i2c_open_struct.out_pin_route_SCL  = I2C_ROUTELOC0_SCLLOC_LOC15; //out 0 route to gpio port/pin
   i2c_open_struct.out_pin_route_SDA  = I2C_ROUTELOC0_SDALOC_LOC15;//out 1 route to gpio port/pin
   i2c_open_struct.out_pin_SCL_en     = I2C_ROUTEPEN_SCLPEN;   //enable out 0 route
   i2c_open_struct.out_pin_SDA_en     = I2C_ROUTEPEN_SDAPEN;
   //enable out 1 route
   i2c_open(i2c_peripheral, &i2c_open_struct);


}
/***************************************************************************//**
 * @brief
 *  si7021_read initializes values for si7021 temp sensor
 *
 * @details
 *  sets parameters for si7021 and passes them to i2c_start
 * @param
 *  I2C_STATE_MACHINE
 ******************************************************************************/
void si7021_read(I2C_TypeDef *i2c_peripheral)
{

  uint32_t device_adress   = SI7021_ADRESS;
  uint32_t RW_command      = WRITE_BIT;
  uint32_t i2c_busy        = 1;
  uint32_t curr_bytes      = 2;
  uint32_t num_bytes       = 2;
  uint32_t register_adress = MT_no_hold_mast;
  uint32_t i2c_data        = 0;
  uint32_t i2c_cb          = I2C_CB;

  i2c_start(device_adress, RW_command, i2c_busy, curr_bytes , num_bytes, register_adress , i2c_data, i2c_cb, i2c_peripheral);
}



