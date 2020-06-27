#ifndef LIB_CORE_CONFIG_HPP_
#define LIB_CORE_CONFIG_HPP_

/** @file config.hpp
 *  @brief Config singleton class definition
 *
 * Project's configuration
 */

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <toml.hpp>

#include <libutil/util.hpp>

#include "common.hpp"

#include "allocation.hpp"

NAMESPACE_BEGIN

struct SpeedConfig;
struct MechanismSpeedProfile;
struct SpeedProfile;

namespace impl {
class ConfigImpl;
}

/** impl::ConfigImpl singleton class using StaticObj */
using Config = StaticObj<impl::ConfigImpl>;

struct SpeedConfig {
  SpeedConfig();
  DEBUG_ONLY(void print(std::ostream& os) const);

  double rpm;
  double acceleration;
  double deceleration;
};

struct MechanismSpeedProfile {
  MechanismSpeedProfile();
  DEBUG_ONLY(void print(std::ostream& os) const);

  SpeedConfig  x;
  SpeedConfig  y;
  SpeedConfig  z;
  unsigned int duty_cycle;
};

struct SpeedProfile {
  SpeedProfile();
  DEBUG_ONLY(void print(std::ostream& os) const);

  MechanismSpeedProfile slow;
  MechanismSpeedProfile normal;
  MechanismSpeedProfile fast;
};

template <class T>
auto operator<<(std::ostream& os, T const& t) -> decltype(t.print(os), os) {
  t.print(os);
  return os;
}

enum class Speed { slow, normal, fast };

namespace impl {
/**
 * @brief Config implementation.
 *        This is a class wrapper that should not be instantiated and accessed
 * publicly.
 *
 * Machine's configuration that contains all the information the machine needed
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class ConfigImpl : public StackObj {
  template <class ConfigImpl>
  template <typename... Args>
  friend ATM_STATUS StaticObj<ConfigImpl>::create(Args&&... args);

 public:
  typedef std::pair<double, double> coordinate;
  typedef std::vector<coordinate>   path_container;
  /**
   * Get name of app from config
   *
   * It should be in key "general.app"
   *
   * @return application name
   */
  std::string name() const;
  /**
   * Get debug status of logging message
   *
   * It should be in key "general.debug"
   *
   * @return debug status
   */
  bool debug() const;
  /**
   * Get speed Profile of Fault mechanism
   *
   * @tparam Speed type of speed
   *
   * @return fault speed profile
   */
  template <Speed speed = Speed::normal>
  inline const MechanismSpeedProfile& fault_speed_profile() const {
    if (speed == Speed::slow) {
      return fault_speed_profile_.slow;
    } else if (speed == Speed::normal) {
      return fault_speed_profile_.normal;
    } else {
      return fault_speed_profile_.fast;
    }
  }
  /**
   * Get speed Profile of Homing mechanism
   *
   * @tparam Speed type of speed
   *
   * @return homing speed profile
   */
  template <Speed speed = Speed::normal>
  inline const MechanismSpeedProfile& homing_speed_profile() const {
    if (speed == Speed::slow) {
      return homing_speed_profile_.slow;
    } else if (speed == Speed::normal) {
      return homing_speed_profile_.normal;
    } else {
      return homing_speed_profile_.fast;
    }
  }
  /**
   * Get speed Profile of Spraying mechanism
   *
   * @tparam Speed type of speed
   *
   * @return spraying speed profile
   */
  template <Speed speed = Speed::normal>
  inline const MechanismSpeedProfile& spraying_speed_profile() const {
    if (speed == Speed::slow) {
      return spraying_speed_profile_.slow;
    } else if (speed == Speed::normal) {
      return spraying_speed_profile_.normal;
    } else {
      return spraying_speed_profile_.fast;
    }
  }
  /**
   * Get speed Profile of Tending mechanism
   *
   * @tparam Speed type of speed
   *
   * @return spraying speed profile
   */
  template <Speed speed = Speed::normal>
  inline const MechanismSpeedProfile& tending_speed_profile() const {
    if (speed == Speed::slow) {
      return tending_speed_profile_.slow;
    } else if (speed == Speed::normal) {
      return tending_speed_profile_.normal;
    } else {
      return tending_speed_profile_.fast;
    }
  }
  /**
   * Get speed Profile of Cleaning mechanism
   *
   * @tparam Speed type of speed
   *
   * @return cleaning speed profile
   */
  template <Speed speed = Speed::normal>
  inline const MechanismSpeedProfile& cleaning_speed_profile() const {
    if (speed == Speed::slow) {
      return cleaning_speed_profile_.slow;
    } else if (speed == Speed::normal) {
      return cleaning_speed_profile_.normal;
    } else {
      return cleaning_speed_profile_.fast;
    }
  }
  /**
   * Get stepper x-axis device info
   *
   * It should be in key "devices.stepper.x"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return stepper x-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T stepper_x(Keys&&... keys) const {
    return find<T>("devices", "stepper", "x", std::forward<Keys>(keys)...);
  }
  /**
   * Get stepper y-axis device info
   *
   * It should be in key "devices.stepper.y"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return stepper y-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T stepper_y(Keys&&... keys) const {
    return find<T>("devices", "stepper", "y", std::forward<Keys>(keys)...);
  }
  /**
   * Get stepper z-axis device info
   *
   * It should be in key "devices.stepper.z"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return stepper z-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T stepper_z(Keys&&... keys) const {
    return find<T>("devices", "stepper", "z", std::forward<Keys>(keys)...);
  }
  /**
   * Get limit switch x-axis device info
   *
   * It should be in key "devices.limit-switch.x"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return limit switch x-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T limit_switch_x(Keys&&... keys) const {
    return find<T>("devices", "limit-switch", "x", std::forward<Keys>(keys)...);
  }
  /**
   * Get limit switch y-axis device info
   *
   * It should be in key "devices.limit-switch.y"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return limit switch y-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T limit_switch_y(Keys&&... keys) const {
    return find<T>("devices", "limit-switch", "y", std::forward<Keys>(keys)...);
  }
  /**
   * Get limit switch upper bound z-axis device info
   *
   * It should be in key "devices.limit-switch.z1"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return limit switch upper bound z-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T limit_switch_z1(Keys&&... keys) const {
    return find<T>("devices", "limit-switch", "z1",
                   std::forward<Keys>(keys)...);
  }
  /**
   * Get limit switch lower bound z-axis device info
   *
   * It should be in key "devices.limit-switch.z2"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return limit switch lower bound z-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T limit_switch_z2(Keys&&... keys) const {
    return find<T>("devices", "limit-switch", "z2",
                   std::forward<Keys>(keys)...);
  }
  /**
   * Get spray device info
   *
   * It should be in key "devices.spray"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return spray device info with type T
   */
  template <typename T, typename... Keys>
  inline T spray(Keys&&... keys) const {
    return find<T>("devices", "spray", std::forward<Keys>(keys)...);
  }
  /**
   * Get anomaly device info
   *
   * It should be in key "devices.anomaly"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return anomaly device info with type T
   */
  template <typename T, typename... Keys>
  inline T anomaly(Keys&&... keys) const {
    return find<T>("devices", "anomaly", std::forward<Keys>(keys)...);
  }
  /**
   * Get finger device info
   *
   * It should be in key "devices.finger"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return finger device info with type T
   */
  template <typename T, typename... Keys>
  inline T finger(Keys&&... keys) const {
    return find<T>("devices", "finger", std::forward<Keys>(keys)...);
  }
  /**
   * Get analog device info
   *
   * It should be in key "devices.analog"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return stepper x-axis info with type T
   */
  template <typename T, typename... Keys>
  inline T analog(Keys&&... keys) const {
    return find<T>("devices", "analog", std::forward<Keys>(keys)...);
  }
  /**
   * Get spraying position
   *
   * It should be in key "mechanisms.spraying.position"
   *
   * @return spraying position
   */
  const coordinate& spraying_position();
  /**
   * Get spraying movement path
   *
   * It should be in key "mechanisms.spraying.path"
   *
   * @return spraying movement path
   */
  const path_container& spraying_path();
  /**
   * Get spraying movement path coordinate at specified index
   *
   * @param idx index to get
   *
   * @return spraying movement path at specified index
   */
  const coordinate& spraying_path(size_t idx);
  /**
   * Get tending position
   *
   * It should be in key "mechanisms.tending.position"
   *
   * @return tending position
   */
  const coordinate& tending_position();
  /**
   * Get tending edge movement path
   *
   * It should be in key "mechanisms.tending.path.edge"
   *
   * @return tending edge movement path
   */
  const path_container& tending_path_edge();
  /**
   * Get tending edge movement path coordinate at specified index
   *
   * @param idx index to get
   *
   * @return tending edge movement path at specified index
   */
  const coordinate& tending_path_edge(size_t idx);
  /**
   * Get tending zigzag movement path
   *
   * It should be in key "mechanisms.tending.path.zigzag"
   *
   * @return tending zigzag movement path
   */
  const path_container& tending_path_zigzag();
  /**
   * Get tending zigzag movement path coordinate at specified index
   *
   * @param idx index to get
   *
   * @return tending movement path at specified index
   */
  const coordinate& tending_path_zigzag(size_t idx);
  /**
   * Get mechanisms fault manual mode movement
   *
   * It should be in key "mechanisms.fault.manual.movement"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return manual mode config
   */
  template <typename T, typename... Keys>
  inline T fault_manual_movement(Keys&&... keys) const {
    return find<T>("mechanisms", "fault", "manual", "movement",
                   std::forward<Keys>(keys)...);
  }
  /**
   * Get shift register device configuration
   *
   * It should be in key "devices.shift-register"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return shift register device configuration
   */
  template <typename T, typename... Keys>
  inline T shift_register(Keys&&... keys) const {
    return find<T>("devices", "shift-register", std::forward<Keys>(keys)...);
  }
  /**
   * Get communication device from PLC to RaspberryPI
   *
   * It should be in key "devices.plc-to-pi"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return communication device info from PLC to RaspberryPI with type T
   */
  template <typename T, typename... Keys>
  inline T plc_to_pi(Keys&&... keys) const {
    return find<T>("devices", "plc-to-pi", std::forward<Keys>(keys)...);
  }
  /**
   * Get communication device from RaspberryPI to PLC
   *
   * It should be in key "devices.shift-register.pi-to-plc"
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return communication device info from RaspberryPI to PLC with type T
   */
  template <typename T, typename... Keys>
  inline T pi_to_plc(Keys&&... keys) const {
    return shift_register<T>("pi-to-plc", std::forward<Keys>(keys)...);
  }

 private:
  /**
   * ConfigImpl Constructor
   *
   * Initialize YAML-CPP and load yaml config file for this project
   *
   * @param config_path   config file path
   */
  explicit ConfigImpl(const std::string& config_path);
  /**
   * ConfigImpl Destructor
   *
   * Noop
   *
   */
  ~ConfigImpl() = default;
  /**
   * Get TOML Config
   *
   * @return config tree
   */
  inline const toml::value& config() const { return config_; }
  /**
   * Find key in the TOML config
   *
   * @tparam T     type of config value
   * @tparam Keys  variadic args for keys (should be string)
   *
   * @return config value with type T
   */
  template <typename T, typename... Keys>
  inline T find(Keys&&... keys) const {
    return toml::find<T>(config(), std::forward<Keys>(keys)...);
  }
  /**
   * Load speed profile for all mechanisms
   */
  void load_speed_profiles();

 private:
  /**
   * TOML config data
   */
  const toml::value config_;
  /**
   * Config file
   */
  const std::string config_path_;
  /**
   * Spraying movement path
   */
  path_container spraying_path_;
  /**
   * Spraying position
   */
  coordinate spraying_position_;
  /**
   * Tending movement path for edge pattern
   */
  path_container tending_path_edge_;
  /**
   * Tending movement path for zigzag pattern
   */
  path_container tending_path_zigzag_;
  /**
   * Tending position
   */
  coordinate tending_position_;
  /**
   * Fault speed profile
   */
  SpeedProfile fault_speed_profile_;
  /**
   * Homing speed profile
   */
  SpeedProfile homing_speed_profile_;
  /**
   * Tending speed profile
   */
  SpeedProfile tending_speed_profile_;
  /**
   * Spraying speed profile
   */
  SpeedProfile spraying_speed_profile_;
  /**
   * Cleaning speed profile
   */
  SpeedProfile cleaning_speed_profile_;
};
}  // namespace impl

NAMESPACE_END

#endif
