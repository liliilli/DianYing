#ifndef PHITOS_ENUMS_SWITCH_H
#define PHITOS_ENUMS_SWITCH_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Phitos/enums/switch.h
///
/// @brief Switch enum class replaces plain boolean type.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-15 Create file.
///

namespace phitos::enums {

///
/// @enum ESwitch
///
/// @brief
/// enum constant used everywhere instead of just using plain bool type.
///
enum class ESwitch : bool {
  Off = false,
  On = true
};

///
/// @brief Inversion operator for ESwitch.
/// @param[in] value ESwitch value.
/// @return Inverted ESwitch value.
///
inline ESwitch operator!(const ESwitch& value) {
  switch (value) {
  case ESwitch::Off:
    return ESwitch::On;
  case ESwitch::On:
    return ESwitch::Off;
  }

  return {};
}

} /// ::phitos::enums namespace

#endif /// PHITOS_ENUMS_SWITCH_H