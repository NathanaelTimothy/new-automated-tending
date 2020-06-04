#include "init.hpp"

#include "gpio.hpp"

#include "analog/PCF8591.hpp"
#include "analog/analog.hpp"

#include "digital/digital.hpp"

NAMESPACE_BEGIN

using namespace device;

static ATM_STATUS initialize_analog_devices() {
  return AnalogDeviceRegistry::get()->create<analog::PCF8591Device>("PCF8591");
}

static ATM_STATUS initialize_input_digital_devices() {
  return DigitalInputDeviceRegistry::get()->create<DigitalInputDevice>("INPUT1",
                                                                       0);
}

static ATM_STATUS initialize_output_digital_devices() {
  ATM_STATUS status = ATM_OK;
  status = DigitalOutputDeviceRegistry::get()->create("OUTPUT1", 1);

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = DigitalOutputDeviceRegistry::get()->create("OUTPUT2", 2);

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return status;
}

ATM_STATUS initialize_device() {
  if (gpioInitialise() < 0)
    return ATM_ERR;

  ATM_STATUS status = ATM_OK;

  status = AnalogDeviceRegistry::create();

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = DigitalInputDeviceRegistry::create();

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = DigitalOutputDeviceRegistry::create();

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = initialize_analog_devices();

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = initialize_input_digital_devices();

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = initialize_output_digital_devices();

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return ATM_OK;
}

void destroy_device() {
  gpioTerminate();
}

NAMESPACE_END