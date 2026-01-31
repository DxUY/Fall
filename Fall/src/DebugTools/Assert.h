#ifdef FALL_ENABLE_ASSERTS

#define FALL_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
        do { \
            if (!(check)) { \
                FALL##type##ERROR(msg, __VA_ARGS__); \
                FALL_DEBUGBREAK(); \
            } \
        } while (0)

#define FALL_INTERNAL_ASSERT_WITH_MSG(type, check, ...) \
        FALL_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)

#define FALL_INTERNAL_ASSERT_NO_MSG(type, check) \
        FALL_INTERNAL_ASSERT_IMPL(type, check, \
            "Assertion '{0}' failed at {1}:{2}", #check, __FILE__, __LINE__)

#define FALL_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define FALL_INTERNAL_ASSERT_GET_MACRO(...) \
        FALL_EXPAND_MACRO( \
            FALL_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, \
                FALL_INTERNAL_ASSERT_WITH_MSG, \
                FALL_INTERNAL_ASSERT_NO_MSG))

#define FALL_ASSERT(...) \
        FALL_EXPAND_MACRO( FALL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )

#define FALL_CORE_ASSERT(...) \
        FALL_EXPAND_MACRO( FALL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )

#else
#define FALL_ASSERT(...)
#define FALL_CORE_ASSERT(...)
#endif