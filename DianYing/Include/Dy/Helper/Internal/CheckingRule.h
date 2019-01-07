#ifndef GUARD_DY_HELPER_INTERNAL_CHECKINGRULE_H
#define GUARD_DY_HELPER_INTERNAL_CHECKINGRULE_H
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
/// @link https://philippegroarke.com/posts/2018/easy_defensive_programming/
///

namespace dy
{

#define MDY_TEST_DEFAULT_CONSTRUCTIBLE(__MAType__) \
	[]() { \
		static_assert(std::is_default_constructible_v<__MAType__>, #__MAType__ " : must be default constructible"); \
		return std::is_default_constructible_v<__MAType__>; \
	}

#define MDY_TEST_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(__MAType__) \
	[]() { \
		static_assert(std::is_trivially_default_constructible_v<__MAType__>, #__MAType__ " : must be trivially default constructible"); \
		return std::is_trivially_default_constructible_v<__MAType__>; \
	}

#define MDY_TEST_COPY_CONSTRUCTIBLE(__MAType__) \
	[]() { \
		static_assert(std::is_copy_constructible_v<__MAType__>, #__MAType__ " : must be copy constructible"); \
		return std::is_copy_constructible_v<__MAType__>; \
	}

#define MDY_TEST_NOT_COPY_CONSTRUCTIBLE(__MAType__) \
	[]() { \
		static_assert(!std::is_copy_constructible_v<__MAType__>, #__MAType__ " : must not be copy constructible"); \
		return !std::is_copy_constructible_v<__MAType__>; \
	}

#define MDY_TEST_TRIVIALLY_COPY_CONSTRUCTIBLE(__MAType__) \
	[]() { \
		static_assert(std::is_trivially_copy_constructible_v<__MAType__>, #__MAType__ " : must be trivially copy constructible"); \
		return std::is_trivially_copy_constructible_v<__MAType__>; \
	}

#define MDY_TEST_COPY_ASSIGNABLE(__MAType__) \
	[]() { \
		static_assert(std::is_copy_assignable_v<__MAType__>, #__MAType__ " : must be copy assignable"); \
		return std::is_copy_assignable_v<__MAType__>; \
	}

#define MDY_TEST_NOT_COPY_ASSIGNABLE(__MAType__) \
	[]() { \
		static_assert(!std::is_copy_assignable_v<__MAType__>, #__MAType__ " : must not be copy assignable"); \
		return !std::is_copy_assignable_v<__MAType__>; \
	}

#define MDY_TEST_TRIVIALLY_COPY_ASSIGNABLE(__MAType__) \
	[]() { \
		static_assert(std::is_trivially_copy_assignable_v<__MAType__>, #__MAType__ " : must be trivially copy assignable"); \
		return std::is_trivially_copy_assignable_v<__MAType__>; \
	}

#define MDY_TEST_MOVE_CONSTRUCTIBLE(__MAType__) \
	[]() { \
		static_assert(std::is_move_constructible_v<__MAType__>, #__MAType__ " : must be move constructible"); \
		return std::is_move_constructible_v<__MAType__>; \
	}

#define MDY_TEST_TRIVIALLY_MOVE_CONSTRUCTIBLE(__MAType__) \
	[]() { \
		static_assert(std::is_trivially_move_constructible_v<__MAType__>, #__MAType__ " : must be trivially move constructible"); \
		return std::is_trivially_move_constructible_v<__MAType__>; \
	}

#define MDY_TEST_NOTHROW_MOVE_CONSTRUCTIBLE(__MAType__) \
	[]() { \
		static_assert(std::is_nothrow_move_constructible_v<__MAType__>, #__MAType__ " : must be nothrow move constructible"); \
		return std::is_nothrow_move_constructible_v<__MAType__>; \
	}

#define MDY_TEST_MOVE_ASSIGNABLE(__MAType__) \
	[]() { \
		static_assert(std::is_move_assignable_v<__MAType__>, #__MAType__ " : must be move assignable"); \
		return std::is_move_assignable_v<__MAType__>; \
	}

#define MDY_TEST_TRIVIALLY_MOVE_ASSIGNABLE(__MAType__) \
	[]() { \
		static_assert(std::is_trivially_move_assignable_v<__MAType__>, #__MAType__ " : must be trivially move assignable"); \
		return std::is_trivially_move_assignable_v<__MAType__>; \
	}

#define MDY_TEST_DESTRUCTIBLE(__MAType__) \
	[]() { \
		static_assert(std::is_destructible_v<__MAType__>, #__MAType__ " : must be destructible"); \
		return std::is_destructible_v<__MAType__>; \
	}

#define MDY_TEST_TRIVIALLY_DESTRUCTIBLE(__MAType__) \
	[]() { \
		static_assert(std::is_trivially_destructible_v<__MAType__>, #__MAType__ " : must be trivially destructible"); \
		return std::is_trivially_destructible_v<__MAType__>; \
	}

#define MDY_TEST_TRIVIALLY_COPYABLE(__MAType__) \
	[]() { \
		static_assert(std::is_trivially_copyable_v<__MAType__>, #__MAType__ " : must be trivially copyable"); \
		return std::is_trivially_copyable_v<__MAType__>; \
	}

///
/// @macro MDY_TEST_FULFILLS_RULE_OF_5
/// @brief Check __MAType__ fulfills rule of 5.
///
#define MDY_TEST_FULFILLS_RULE_OF_5(__MAType__)                                                         \
	static_assert(MDY_TEST_DESTRUCTIBLE(__MAType__)() && MDY_TEST_COPY_CONSTRUCTIBLE(__MAType__)()        \
                && MDY_TEST_MOVE_CONSTRUCTIBLE(__MAType__)() && MDY_TEST_COPY_ASSIGNABLE(__MAType__)()  \
                && MDY_TEST_MOVE_ASSIGNABLE(__MAType__)(),                                              \
                #__MAType__ " : doesn't fulfill rule of 5")

///
/// @macro MDY_TEST_FULFILLS_RULE_OF_6
/// @brief Check __MAType__ fulfills rule of 6.
///
#define MDY_TEST_FULFILLS_RULE_OF_6(__MAType__)                                                         \
	static_assert(MDY_TEST_DESTRUCTIBLE(__MAType__)() && MDY_TEST_DEFAULT_CONSTRUCTIBLE(__MAType__)()     \
                && MDY_TEST_COPY_CONSTRUCTIBLE(__MAType__)()                                            \
                && MDY_TEST_MOVE_CONSTRUCTIBLE(__MAType__)() && MDY_TEST_COPY_ASSIGNABLE(__MAType__)()  \
                && MDY_TEST_MOVE_ASSIGNABLE(__MAType__)(),                                              \
                #__MAType__ " : doesn't fulfill rule of 6")

///
/// @macro MDY_TEST_FULFILLS_FAST_VECTOR
/// @brief Check __MAType__ fulfills fast vector traits.
/// @BUG is_trivially_copyable broken everywhere
///
#define MDY_TEST_FULFILLS_FAST_VECTOR(__MAType__)                                                                     \
	static_assert((std::is_trivially_copy_constructible_v<__MAType__> && std::is_trivially_destructible_v<__MAType__>)  \
					      || std::is_nothrow_move_constructible_v<__MAType__>,                                                  \
			          #__MAType__ " : doesn't fulfill fast vector (trivially copy constructible "                           \
			          "and trivially destructible, or nothrow move constructible)")

///
/// @macro MDY_TEST_FULFILLS_MOVE_ONLY
/// @brief Check __MAType__ is only moveable.
///
#define MDY_TEST_FULFILLS_MOVE_ONLY(__MAType__)                                                             \
	static_assert(MDY_TEST_NOT_COPY_CONSTRUCTIBLE(__MAType__)() && MDY_TEST_MOVE_CONSTRUCTIBLE(__MAType__)()  \
					      && MDY_TEST_NOT_COPY_ASSIGNABLE(__MAType__)() && MDY_TEST_MOVE_ASSIGNABLE(__MAType__)(),    \
			          #__MAType__ " : doesn't fulfill move only")

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_CHECKINGRULE_H