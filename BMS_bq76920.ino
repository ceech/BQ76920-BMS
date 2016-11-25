
#include <Wire.h> 
#include <bq769x0.h>    // Library for Texas Instruments bq76920 battery management IC

#define BMS_ALERT_PIN 2     // attached to interrupt INT0
#define BMS_BOOT_PIN 7      // connected to TS1 input
#define BMS_I2C_ADDRESS 0x18

bq769x0 BMS(bq76920, BMS_I2C_ADDRESS);    // battery management system object
const int ledPin =  13;      // the number of the LED pin
// Variables will change :
int ledState = LOW;             // ledState used to set the LED
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated
// constants won't change :
const long interval = 250;  

void setup()
{
  Serial.begin(9600);
  int err = BMS.begin(BMS_ALERT_PIN, BMS_BOOT_PIN);

  BMS.setTemperatureLimits(-20, 45, 0, 45);
  BMS.setShuntResistorValue(20);
  BMS.setShortCircuitProtection(22000, 200);  // delay in us
  BMS.setOvercurrentChargeProtection(20000, 200);  // delay in ms
  BMS.setOvercurrentDischargeProtection(20000, 320); // delay in ms
  BMS.setCellUndervoltageProtection(2900, 2); // delay in s
  BMS.setCellOvervoltageProtection(4250, 2);  // delay in s

  BMS.setBalancingThresholds(0, 3650, 20);  // minIdleTime_min, minCellV_mV, maxVoltageDiff_mV
  BMS.setIdleCurrentThreshold(100);
  BMS.enableAutoBalancing();
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    BMS.update();  // should be called at least every 250 ms
    BMS.printRegisters();
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(ledPin, ledState);
    
  }
}
