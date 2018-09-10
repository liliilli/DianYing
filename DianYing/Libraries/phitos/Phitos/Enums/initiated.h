#ifndef PHITOS_ENUMS_INITIATED_H
#define PHITOS_ENUMS_INITIATED_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Phitos/Enums/initiated.h
///
/// @brief
/// Strong boolean enum aggregation file for runtime check of ::opgs16::manager
/// This file must not be together with the other flag.h file.
/// namespace is duplicated, might occur unexpcted behavior or error.
///
/// @log
/// 2018-05-20 Create file and make enum class for boolean flag.
/// 2018-06-06 Move OPGS16 to Phitos for useability.
///

namespace phitos::enums {

///
/// @enum EInitiated
/// @brief
/// This enum class is for checking and preventing duplicated
/// ::opgs16::manager::(namespace)::Initiate() call from somewhere.
///
enum class EInitiated : bool {
  NotInitiated = false,
  Initiated = true
};

}

#endif /// PHITOS_ENUMS_INITIATED_H