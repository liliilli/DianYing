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
/// This file is inspired by GSL pointer.h file.
/// @reference https://github.com/Microsoft/GSL/blob/master/include/gsl/pointers
///

#include <assimp/Importer.hpp>
#include <optional>

#include "HelperPointer.h"

/// @brief Read Model with `iFilePath`.
[[nodiscard]] std::optional<std::pair<std::shared_ptr<Assimp::Importer>, dy::NotNull<const aiScene*>>>
AssimpReadModel(const std::string& iFilePath);
   
