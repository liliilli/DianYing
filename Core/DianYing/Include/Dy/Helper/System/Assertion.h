#pragma once
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <AEngineBase.h>
#include <ADebugBase.h>
#include <Dy/Helper/Library/HelperString.h>

namespace dy::assert
{

#if defined(_DEBUG)

/// @def MDY_ASSERT_MSG(__MAExpr__, __MAMessage__)
/// @brief Do runtime check for __MAExpr__ (Macro Argument Expression) in debug mode,
/// If expression is false, Assert application with __MAMessage__.
/// https://stackoverflow.com/questions/3692954/add-custom-messages-in-assert
#define MDY_ASSERT_MSG(__MAExpr__, __MAMessage__) \
  MDY_ASSERT_MSG_FORCE(__MAExpr__, __MAMessage__)

/// @def MDY_ASSERT(__MAExpr__)
/// @brief Do runtime check for __MAExpr__ (Macro Argument Expression) in debug mode,
/// If expression is false, Assert application with __MAExpr__.
#define MDY_ASSERT(__MAExpr__) \
  MDY_ASSERT_FORCE(__MAExpr__)

#else

#define MDY_ASSERT_MSG(__MAExpr__, __MAMessage__) (void(0))
#define MDY_ASSERT(__MAExpr__) (void(0))

#endif /// defined(_DEBUG)
  
/// @def MDY_ASSERT_MSG_FORCE(__MAExpr__, __MAMessage__)
/// @brief Do runtime check for __MAExpr__ (Macro Argument Expression) in anymode,
/// If expression is false, Assert application with __MAMessage__.
#define MDY_ASSERT_MSG_FORCE(__MAExpr__, __MAMessage__) \
  { \
    if ((__MAExpr__) == false) \
    { \
      ::dy::gEngine->GetPlatformInfo().GetDebugManager().OnAssertionFailed( \
        ::dy::MakeStringU8("{}, {}", #__MAExpr__, __MAMessage__).c_str(), \
        __FUNCTION__, \
        __FILE__, \
        __LINE__ \
      ); \
    } \
  }
  
/// @def MDY_ASSERT_MSG(__MAExpr__)
/// @brief Do runtime check for __MAExpr__ (Macro Argument Expression) in anymode,
/// If expression is false, Assert application with __MAExpr__.
#define MDY_ASSERT_FORCE(__MAExpr__) \
  { \
    if ((__MAExpr__) == false) \
    { \
      ::dy::gEngine->GetPlatformInfo().GetDebugManager().OnAssertionFailed( \
        #__MAExpr__, \
        __FUNCTION__, \
        __FILE__, \
        __LINE__ \
      ); \
    } \
  }

/// @def MDY_NOT_IMPLEMENTED_ASSERT()
/// @brief Assert program when statement is called in runtime debug mode.
/// Programmer which uses this function must use this macro only in not implemen
/// ted function completely properly.
#define MDY_NOT_IMPLEMENTED_ASSERT() \
  { \
    ::dy::gEngine->GetPlatformInfo().GetDebugManager().OnAssertionFailed( \
      "This function is not implemented yet.", \
      __FUNCTION__, \
      __FILE__, \
      __LINE__ \
    ); \
  }

/// @def MDY_UNEXPECTED_BRANCH()
/// @brief
#define MDY_UNEXPECTED_BRANCH() \
  { \
    ::dy::gEngine->GetPlatformInfo().GetDebugManager().OnAssertionFailed( \
      "Unexpected branch not intended.", \
      __FUNCTION__, \
      __FILE__, \
      __LINE__ \
    ); \
  }

} /// ::dy::assert namespace
