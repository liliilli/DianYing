#pragma once
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Builtin/Abstract/AGLAttachmentResource.h>
#include <Dy/Core/Reflection/RBuiltinResources.h>

namespace dy
{

/// @class FDyBtAttachmentEmissiveTempBlur
/// @brief Emissive information attachment for used to store blurred iamge.
/// This attachment supports HDR (RGB16F)
class FDyBtAttachmentEmissiveTempBlur final : public AGLAttachmentResource
{
  MDY_REGISTER_RESOURCE_ATTACHMENT(
    FDyBtAttachmentEmissiveTempBlur, 
    "dyBtEmissiveTempBlur")
};

} /// ::dy namespace
