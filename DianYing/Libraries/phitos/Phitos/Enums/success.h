#ifndef PHITOS_ENUMS_SUCCESS_H
#define PHITOS_ENUMS_SUCCESS_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Phitos/Enums/success.h
///
/// @log
/// 2018-06-09 Create file.
///

namespace phitos::enums {

///
/// @enum ESucceed
/// @brief
/// IsSucceed strong boolean enum type.
///
enum class ESucceed : bool {
  Failed = false,
  Succeed = true
};

} /// ::phitos::enums namespace

#endif /// PHITOS_ENUMS_SUCCESS_H