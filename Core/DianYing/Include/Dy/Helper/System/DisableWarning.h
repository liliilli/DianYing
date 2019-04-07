#ifndef GUARD_DIANYING_HELPER_DISABLE_WARNING_H
#define GUARD_DIANYING_HELPER_DISABLE_WARNING_H
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

#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4100 4127 4201 5046 4324)
#endif

template <typename TType>
void MaybeNotUsed([[maybe_unused]] const TType&) {};

#endif ///  GUARD_DIANYING_HELPER_DISABLE_WARNING_H