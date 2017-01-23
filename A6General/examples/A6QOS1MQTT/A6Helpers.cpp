/*
 *   Following functions are implemented on behalf of the GPRSA6Device class
 *   None of them HAVE to be present but I would recommend that at least HWRESET is implemented
 *   else the modem may never get  going
 */
#include <Arduino.h>
#include "A6Modem.h"
#include "A6MQTT.h"

#define TRANSISTOR_CONTROL 7  // connect to base of transistor
#define A6_RESET_TIME 50 // ms


void GPRSA6Device::HWReset()
{
  /*
   * The A6 modem takes about 70mA throuh the reset pin which is too much for an Arduino GPIO pin
   * Instead connect an Arduino GPIO pin to the base of a transistor and toggle that.
   * Connect the emitter of the transistor to ground and the collector to the A6 reset pin
   * http://electronics.stackexchange.com/questions/82222/how-to-use-a-transistor-to-pull-a-pin-low
   */
  pinMode(TRANSISTOR_CONTROL,OUTPUT);
  digitalWrite(TRANSISTOR_CONTROL,LOW);
  digitalWrite(TRANSISTOR_CONTROL,HIGH);
  delay(A6_RESET_TIME);
  digitalWrite(TRANSISTOR_CONTROL,LOW);  
}

void GPRSA6Device::DebugWrite(uint16_t c)
{
  if (enableDebug)
    Serial.print(c);
}
void GPRSA6Device::DebugWrite(int c)
{
  if (enableDebug)
    Serial.print(c);
}
void GPRSA6Device::DebugWrite(char c)
{
  if (enableDebug)
    Serial.write(c);
}
void GPRSA6Device::DebugWrite(char *s)
{
  if (enableDebug)
    Serial.print(s);
}
void GPRSA6Device::DebugWrite(const __FlashStringHelper*s)
{
  if (enableDebug)
    Serial.print(s);  
}

