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

namespace dy::math
{
  
template <EValueCategory ECategory> struct TTypeListOf_T;
template <> struct TTypeListOf_T<EValueCategory::Character> final { using Type = TCharacterTypes; };
template <> struct TTypeListOf_T<EValueCategory::Signed> final    { using Type = TSignedTypes; };
template <> struct TTypeListOf_T<EValueCategory::Unsigned> final  { using Type = TUnsignedTypes; };
template <> struct TTypeListOf_T<EValueCategory::Real> final      { using Type = TRealTypes; };

} /// ::dy::math namespace