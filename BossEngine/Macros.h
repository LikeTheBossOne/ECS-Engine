#pragma once

#include "SystemManager.h"
#include "ECS.h"

// PREPROCESSOR MACROS

// Concatenates tokens, even when the tokens are macros themselves.
#define PP_JOIN_HELPER_HELPER(_0, _1)       _0##_1
#define PP_JOIN_HELPER(_0, _1)              PP_JOIN_HELPER_HELPER(_0, _1)
#define PP_JOIN_IMPL(_0, _1)                PP_JOIN_HELPER(_0, _1)

#define PP_JOIN_2(_0, _1)                                                                   PP_JOIN_IMPL(_0, _1)
#define PP_JOIN_3(_0, _1, _2)                                                               PP_JOIN_2(PP_JOIN_2(_0, _1), _2)
#define PP_JOIN_4(_0, _1, _2, _3)                                                           PP_JOIN_2(PP_JOIN_3(_0, _1, _2), _3)
#define PP_JOIN_5(_0, _1, _2, _3, _4)                                                       PP_JOIN_2(PP_JOIN_4(_0, _1, _2, _3), _4)
#define PP_JOIN_6(_0, _1, _2, _3, _4, _5)                                                   PP_JOIN_2(PP_JOIN_5(_0, _1, _2, _3, _4), _5)
#define PP_JOIN_7(_0, _1, _2, _3, _4, _5, _6)                                               PP_JOIN_2(PP_JOIN_6(_0, _1, _2, _3, _4, _5), _6)
#define PP_JOIN_8(_0, _1, _2, _3, _4, _5, _6, _7)                                           PP_JOIN_2(PP_JOIN_7(_0, _1, _2, _3, _4, _5, _6), _7)
#define PP_JOIN_9(_0, _1, _2, _3, _4, _5, _6, _7, _8)                                       PP_JOIN_2(PP_JOIN_8(_0, _1, _2, _3, _4, _5, _6, _7), _8)
#define PP_JOIN_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9)                                  PP_JOIN_2(PP_JOIN_9(_0, _1, _2, _3, _4, _5, _6, _7, _8), _9)
#define PP_JOIN_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10)                             PP_JOIN_2(PP_JOIN_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9), _10)
#define PP_JOIN_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11)                        PP_JOIN_2(PP_JOIN_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10), _11)
#define PP_JOIN_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12)                   PP_JOIN_2(PP_JOIN_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11), _12)
#define PP_JOIN_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13)              PP_JOIN_2(PP_JOIN_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12), _13)
#define PP_JOIN_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14)         PP_JOIN_2(PP_JOIN_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13), _14)
#define PP_JOIN_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15)    PP_JOIN_2(PP_JOIN_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14), _15)

// Chooses a value based on a condition.
#define PP_IF_0(t, f)           f
#define PP_IF_1(t, f)           t
#define PP_IF(cond, t, f)       PP_JOIN_2(PP_IF_, PP_TO_BOOL(cond))(t, f)

// Converts a condition into a boolean 0 (=false) or 1 (=true).
#define PP_TO_BOOL_0 0
#define PP_TO_BOOL_1 1
#define PP_TO_BOOL_2 1
#define PP_TO_BOOL_3 1
#define PP_TO_BOOL_4 1
#define PP_TO_BOOL_5 1
#define PP_TO_BOOL_6 1
#define PP_TO_BOOL_7 1
#define PP_TO_BOOL_8 1
#define PP_TO_BOOL_9 1
#define PP_TO_BOOL_10 1
#define PP_TO_BOOL_11 1
#define PP_TO_BOOL_12 1
#define PP_TO_BOOL_13 1
#define PP_TO_BOOL_14 1
#define PP_TO_BOOL_15 1
#define PP_TO_BOOL_16 1

#define PP_TO_BOOL(x)       PP_JOIN_2(PP_TO_BOOL_, x)

// Returns 1 if the arguments to the variadic macro are separated by a comma, 0 otherwise.
#define PP_HAS_COMMA(...)                           PP_HAS_COMMA_EVAL(PP_HAS_COMMA_ARGS(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))
#define PP_HAS_COMMA_EVAL(...)                      __VA_ARGS__
#define PP_HAS_COMMA_ARGS(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16

// Returns 1 if the argument list to the variadic macro is empty, 0 otherwise.
#define PP_IS_EMPTY(...)                                                        \
    PP_HAS_COMMA                                                                \
    (                                                                           \
        PP_JOIN_5                                                           \
        (                                                                       \
            PP_IS_EMPTY_CASE_,                                              \
            PP_HAS_COMMA(__VA_ARGS__),                                      \
            PP_HAS_COMMA(PP_IS_EMPTY_BRACKET_TEST __VA_ARGS__),         \
            PP_HAS_COMMA(__VA_ARGS__ (~)),                                  \
            PP_HAS_COMMA(PP_IS_EMPTY_BRACKET_TEST __VA_ARGS__ (~))      \
        )                                                                       \
    )

#define PP_IS_EMPTY_CASE_0001           ,
#define PP_IS_EMPTY_BRACKET_TEST(...)   ,

// Retrieve the number of arguments handed to a variable-argument macro.
#define PP_VA_NUM_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...)    N
#define PP_VA_NUM_ARGS(...) PP_VA_NUM_ARGS_HELPER(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

// Correctly handles the case of 0 arguments.
#define PP_NUM_ARGS(...)        PP_IF(PP_IS_EMPTY(__VA_ARGS__), 0, PP_VA_NUM_ARGS(__VA_ARGS__))

// Pass each variable in a VA_ARGS list to a macro.
#define PP_FE_0(action, X)
#define PP_FE_1(action, X) action(X)
#define PP_FE_2(action, X, ...) action(X)PP_FE_1(action, __VA_ARGS__)
#define PP_FE_3(action, X, ...) action(X)PP_FE_2(action, __VA_ARGS__)
#define PP_FE_4(action, X, ...) action(X)PP_FE_3(action, __VA_ARGS__)
#define PP_FE_5(action, X, ...) action(X)PP_FE_4(action, __VA_ARGS__)
#define PP_FE_6(action, X, ...) action(X)PP_FE_5(action, __VA_ARGS__)
#define PP_FE_7(action, X, ...) action(X)PP_FE_6(action, __VA_ARGS__)
#define PP_FE_8(action, X, ...) action(X)PP_FE_7(action, __VA_ARGS__)
#define PP_FE_9(action, X, ...) action(X)PP_FE_8(action, __VA_ARGS__)
#define PP_FE_10(action, X, ...) action(X)PP_FE_9(action, __VA_ARGS__)
#define PP_FE_11(action, X, ...) action(X)PP_FE_10(action, __VA_ARGS__)
#define PP_FE_12(action, X, ...) action(X)PP_FE_11(action, __VA_ARGS__)
#define PP_FE_13(action, X, ...) action(X)PP_FE_12(action, __VA_ARGS__)
#define PP_FE_14(action, X, ...) action(X)PP_FE_13(action, __VA_ARGS__)
#define PP_FE_15(action, X, ...) action(X)PP_FE_14(action, __VA_ARGS__)
#define PP_FE_16(action, X, ...) action(X)PP_FE_15(action, __VA_ARGS__)

#define PP_FOR_EACH(action, ...) PP_JOIN_2(PP_FE_, PP_NUM_ARGS(__VA_ARGS__))(action, __VA_ARGS__)

#define ECS_SYSTEM(comp)    signature.set(ECS::GetComponentType<comp>());
#define USYSTEM(sys, ...)                                               \
	public:                                                             \
		void __INIT_SYSTEM()                                            \
		{                                                               \
			Signature signature;                                        \
			PP_FOR_EACH(ECS_SYSTEM, __VA_ARGS__)                        \
			ECS::SetSystemSignature<sys>(signature);                    \
			Init();                                                     \
		}
