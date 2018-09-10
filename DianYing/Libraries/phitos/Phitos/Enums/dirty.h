#ifndef PHITOS_ENUMS_DIRTY_H
#define PHITOS_ENUMS_DIRTY_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Phitos/Enums/dirty.h
///
/// @log
/// 2018-06-14 Create file.
///

namespace phitos::enums {

///
/// @enum EDirty
///
/// @brief
///
///
enum class EDirty : bool {
  Clean = false,
  Dirty = true
};

} /// ::phitos::enums namespace

#endif /// PHITOS_ENUMS_DIRTY_H