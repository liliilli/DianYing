#ifndef NEU_PHITOS_DEBUG_WINMEMDUMP_H
#define NEU_PHITOS_DEBUG_WINMEMDUMP_H
#if defined(_WIN32)

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Phitos/Debug/winmemdump.h
///
/// @brief
///
///
/// @author Jongmin Yun
///
/// @log
/// 2018-05-27 Create file.
///

///
/// If you want to use Windows memory dump, remove this.
///
#define NEU_MEM_LEAK_DUMP

///
///
///
#if defined(NEU_MEM_LEAK_DUMP)

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define NEU_DUMP_MEMORY_LEAK() _CrtDumpMemoryLeaks()

#else

#define NEU_DUMP_MEMORY_LEAK() ((void*)0)

#endif /// defined(NEU_MEM_LEAK_DUMP)
#endif /// defined(_WIN32)
#endif /// NEU_PHITOS_DEBUG_WINMEMDUMP_H