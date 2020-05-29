#include "device.hpp"

#include "PCF8591.hpp"

NAMESPACE_BEGIN

namespace device {
namespace analog {
PCF8591Device::PCF8591Device() : AnalogDevice(0x48, 1, 0) {
  DEBUG_ONLY(obj_name_ = "PCF8591Device");
}

ATM_STATUS PCF8591Device::write(unsigned char pin, unsigned int val) {
  unsigned char command[2];
  command[0] = 0x40 | (pin & 0x03);
  command[1] = val;
  PI_RES res = write_device(reinterpret_cast<char*>(&command), 2);

  if (res == ATM_OK) {
    return ATM_OK;
  }

  LOG_DEBUG("[FAILED] PCF8591::write to pin {} with val {}, result = {}", pin,
            val, res);
  return ATM_ERR;
}

int PCF8591Device::read(unsigned char pin) {
  unsigned char command[1];
  command[0] = 0x40 | (pin & 0x03);
  PI_RES res = write_device(reinterpret_cast<char*>(&command), 1);

  if (res == ATM_OK) {
    res = read_byte();

    if (res == ATM_ERR) {
      LOG_DEBUG("[FAILED] PCF8591::read (i2cReadByte) to pin {}", pin);
      return ATM_ERR;
    }

    return res;
  }

  LOG_DEBUG("[FAILED] PCF8591::read (writeDevice) to pin {}", pin);
  return ATM_ERR;
}
}  // namespace analog
}  // namespace analog

NAMESPACE_END
