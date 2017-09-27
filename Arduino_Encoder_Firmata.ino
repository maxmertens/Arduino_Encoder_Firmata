/*
 * Arduino Encoder Firmata
 */

#include <ConfigurableFirmata.h>

#include <DigitalInputFirmata.h>
DigitalInputFirmata digitalInput;

#include <DigitalOutputFirmata.h>
DigitalOutputFirmata digitalOutput;

#include <AnalogInputFirmata.h>
AnalogInputFirmata analogInput;

#include <AnalogOutputFirmata.h>
AnalogOutputFirmata analogOutput;

#include <Encoder.h>
#include <FirmataEncoder.h>
FirmataEncoder encoder;

#include <FirmataExt.h>
FirmataExt firmataExt;

#include <AnalogWrite.h>

#include <FirmataReporting.h>
FirmataReporting reporting;

void systemResetCallback()
{
  for (byte i = 0; i < TOTAL_PINS; i++) {
    if (IS_PIN_ANALOG(i)) {
      Firmata.setPinMode(i, ANALOG);
    } else if (IS_PIN_DIGITAL(i)) {
      Firmata.setPinMode(i, OUTPUT);
    }
  }
  firmataExt.reset();
}

void initTransport()
{
  // Uncomment to save a couple of seconds by disabling the startup blink sequence.
  Firmata.disableBlinkVersion();
  Firmata.begin(57600);
}

void initFirmata()
{
  Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);

  firmataExt.addFeature(digitalInput);
  firmataExt.addFeature(digitalOutput);
  firmataExt.addFeature(analogInput);
  firmataExt.addFeature(analogOutput);
  firmataExt.addFeature(encoder);
  firmataExt.addFeature(reporting);

  Firmata.attach(SYSTEM_RESET, systemResetCallback);
}

void setup()
{
  initFirmata();

  initTransport();

  systemResetCallback();
}

void loop()
{
  digitalInput.report();

  while(Firmata.available()) {
    Firmata.processInput();
  }

  if (reporting.elapsed()) {
    analogInput.report();
    encoder.report();
  }
}
