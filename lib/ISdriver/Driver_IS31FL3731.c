#include "Driver_IS31FL3731.h"


bool initMatrix() {

  // active software pull up
  gpio_pull_up(SDA_pin);
  gpio_pull_up(SCL_pin);

  // Init i2c for it (400kHz)
  i2c_init(i2c1, 400 * 1000);
  gpio_set_function(SDA_pin, GPIO_FUNC_I2C);
  gpio_set_function(SCL_pin, GPIO_FUNC_I2C);

  // shutdown
  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x00);

  sleep_ms(10);

  // out of shutdown
  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x01);

  // picture mode
  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_CONFIG,
                 ISSI_REG_CONFIG_PICTUREMODE);

  // set to frame 0 by default
  displayFrame(0);

  // all LEDs on & 0 PWM
  clear(0); // set each led to 0 PWM

  for (uint8_t f = 0; f < 8; f++) {
    for (uint8_t i = 0; i <= 0x11; i++)
      writeRegister8(f, i, 0xff); // each 8 LEDs on
  }

  audioSync(false);

  return true;
}

/**************************************************************************/
/*!
    @brief Sets all LEDs on & 0 PWM for current frame.
*/
/**************************************************************************/
void clear(uint8_t frame) {
  selectBank(frame);
  uint8_t erasebuf[25];

  memset(erasebuf, 0, 25);

  for (uint8_t i = 0; i < 6; i++) {
    erasebuf[0] = 0x24 + i * 24;
    i2c_write_blocking(i2c1, ISSI_ADDR_DEFAULT, erasebuf, 25, true);
  }
}

/**************************************************************************/
/*!
    @brief Low level accesssor - sets a 8-bit PWM pixel value to a bank location
    does not handle rotation, x/y or any rearrangements!
    @param lednum The offset into the bank that corresponds to the LED
    @param bank The bank/frame we will set the data in
    @param pwm brightnes, from 0 (off) to 255 (max on)
*/
/**************************************************************************/
void setLEDPWM(uint8_t lednum, uint8_t pwm, uint8_t bank) {
  if (lednum >= 144)
    return;
  writeRegister8(bank, 0x24 + lednum, pwm);
}


/**************************************************************************/
/*!
    Sets a 8-bit PWM pixel value
    handles rotation and pixel arrangement, unlike setLEDPWM
    @param x The x position, starting with 0 for left-most side
    @param y The y position, starting with 0 for top-most side
    @param color Despite being a 16-bit value, takes 0 (off) to 255 (max on)
*/
/**************************************************************************/
void drawPixel(int16_t x, int16_t y, uint16_t color, uint8_t frame) {
  // check rotation, move pixel around if necessary
  // switch (getRotation()) {
  // case 1:
  //   _swap_int16_t(x, y);
  //   x = 16 - x - 1;
  //   break;
  // case 2:
  //   x = 16 - x - 1;
  //   y = 9 - y - 1;
  //   break;
  // case 3:
  //   _swap_int16_t(x, y);
  //   y = 9 - y - 1;
  //   break;
  // }

  if ((x < 0) || (x >= 16))
    return;
  if ((y < 0) || (y >= 9))
    return;
  if (color > 255)
    color = 255; // PWM 8bit max

  setLEDPWM(x + y * 16, color, frame);
  return;
}

/**************************************************************************/
/*!
    @brief Set's this object's frame tracker (does not talk to the chip)
    @param frame Ranges from 0 - 7 for the 8 frames
*/
/**************************************************************************/
// void setFrame(uint8_t frame) { frame_ = frame; }

/**************************************************************************/
/*!
    @brief Have the chip set the display to the contents of a frame
    @param frame Ranges from 0 - 7 for the 8 frames
*/
/**************************************************************************/
void displayFrame(uint8_t frame) {
  if (frame > 7)
    frame = 0;
  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_PICTUREFRAME, frame);
}

/**************************************************************************/
/*!
    @brief Switch to a given bank in the chip memory for future reads
    @param bank The IS31 bank to switch to
    @returns False if I2C command failed to be ack'd
*/
/**************************************************************************/
bool selectBank(uint8_t bank) {
  uint8_t cmd[2] = {ISSI_COMMANDREGISTER, bank};
  // return _i2c_dev->write(cmd, 2);
  return i2c_write_blocking(i2c1, ISSI_ADDR_DEFAULT, cmd, 2, true);
}

/**************************************************************************/
/*!
    @brief Enable the audio 'sync' for brightness pulsing (not really tested)
    @param sync True to enable, False to disable
*/
/**************************************************************************/
void audioSync(bool sync) {
  if (sync) {
    writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_AUDIOSYNC, 0x1);
  } else {
    writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_AUDIOSYNC, 0x0);
  }
}

/**************************************************************************/
/*!
    @brief Write one byte to a register located in a given bank
    @param bank The IS31 bank to write the register location
    @param reg the offset into the bank to write
    @param data The byte value
    @returns False if I2C command failed to be ack'd
*/
/**************************************************************************/
bool writeRegister8(uint8_t bank, uint8_t reg,
                                         uint8_t data) {
  selectBank(bank);

  uint8_t cmd[2] = {reg, data};
  return i2c_write_blocking(i2c1, ISSI_ADDR_DEFAULT, cmd, 2, true);
}

/**************************************************************************/
/*!
    @brief  Read one byte from a register located in a given bank
    @param   bank The IS31 bank to read the register location
    @param   reg the offset into the bank to read
    @return 1 byte value
*/
/**************************************************************************/
uint8_t readRegister8(uint8_t bank, uint8_t reg) {
  uint8_t val = 0xFF;

  selectBank(bank);
  i2c_write_blocking(i2c1, ISSI_ADDR_DEFAULT, &reg, 1, true);
  i2c_read_blocking(i2c1, ISSI_ADDR_DEFAULT, &val, 1, true);
  return val;
}



// /*!
//  *  @brief  Sends a single command byte over I2C
//  *  @param  reg
//  *          register address
//  *  @param  value
//  *          value to write
//  */
// void wireWriteRegister(uint8_t reg, uint16_t value) {
	
// 	uint8_t tmpi[3];
// 	tmpi[0] = reg;
// 	tmpi[1] = (value >> 8) & 0xFF;
// 	tmpi[2] = value & 0xFF;
	
// 	i2c_write_blocking(i2c1, ISSI_ADDR_DEFAULT, tmpi, 3, true); // true to keep master control of bus
// }

// /*!
//  *  @brief  Reads a single byte over I2C
//  *  @param  reg
//  *          register address
//  *  @param  *value
//  *          read value
//  */
// void wireReadRegister(uint8_t reg, uint8_t *value) {

// 	uint8_t tmpi[1];

// 	i2c_write_blocking(i2c1, ISSI_ADDR_DEFAULT, &reg, 1, true); // true to keep master control of bus
//   i2c_read_blocking(i2c1, ISSI_ADDR_DEFAULT, tmpi, 1, true);
// 	*value = tmpi[0];
// }