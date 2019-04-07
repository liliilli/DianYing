#ifndef PHITOS_ENUMS_FOUND_H
#define PHITOS_ENUMS_FOUND_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Phitos/Enums/found.h
///
/// @log
/// 2018-06-09 Create file.
///

namespace phitos::enums {

///
/// @enum EFound
/// @brief
/// IsFound strong enum boolean type.
///
enum class EFound : bool {
  NotFound = false,
  Found = true
};

}

#endif /// PHITOS_ENUMS_FOUND_H