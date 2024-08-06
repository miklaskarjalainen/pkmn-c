#ifndef PKMN_CONFIG_H
#define PKMN_CONFIG_H

#define PKMN_MAX_NAME_LENGTH 16
#define PKMN_ARRAY_SIZE(array) (size_t)(sizeof(array) / sizeof(*array))

// DEV stuff
#if !defined(STDOUT_COLORS)
    #define STDOUT_COLORS
    #define STDOUT_RESET   "\033[0;0m"
    #define STDOUT_BLACK   "\033[30m"
    #define STDOUT_WHITE   "\033[37m"
    #define STDOUT_RED     "\033[31m"
    #define STDOUT_YELLOW  "\033[33m"
    #define STDOUT_CYAN    "\033[36m"
    #define STDOUT_GREEN   "\033[32m"
    #define STDOUT_MAGENTA "\033[35m"
    #define STDOUT_BLUE    "\033[34m"
    #define STDOUT_BOLD    "\033[1m"

    #define STDOUT_UNDERLINE "\033[4m"
    #define STDOUT_UNDERLINE_OFF "\033[24m"
    
    #define STDOUT_WARNING STDOUT_YELLOW STDOUT_BOLD
    #define STDOUT_ERROR  STDOUT_RED STDOUT_BOLD
#endif

#define PKMN_FORCE_EXIT() *(volatile char*)NULL = 0 

#define PKMN_INFO(...)                  \
    do {                                \
        printf(STDOUT_CYAN STDOUT_BOLD);\
        printf("[INFO] " __VA_ARGS__);  \
        printf(STDOUT_RESET "\n");      \
    } while(0)

#define PKMN_RUNTIME_ASSERT(expr, ...) \
    do {                                                                                                    \
        if (!(expr)) {                                                                                      \
            printf(STDOUT_ERROR "Error: " STDOUT_RESET "assertion failed at " __FILE__ ":%i '", __LINE__);  \
            printf(__VA_ARGS__);                                                                            \
            printf("'\n");                                                                                  \
            PKMN_FORCE_EXIT();                                                                              \
        }                                                                                                   \
    } while(0)

#define PKMN_PANIC(...) \
    do {                                                                                                \
        printf(STDOUT_ERROR "PANICKED: " STDOUT_RESET "at " __FILE__ ":%i '", __LINE__);                \
        printf(__VA_ARGS__);                                                                            \
        printf("'\n");                                                                                  \
        PKMN_FORCE_EXIT();                                                                              \
    } while(0)

#define UNUSED(var) (void)(var)
#define TODO(msg) PANIC("TODO: " msg)

#if __OPTIMIZE__
    #define PKMN_DEBUG_ASSERT(expr, ...)
#else
    #define PKMN_DEBUG_ASSERT(expr, ...) PKMN_RUNTIME_ASSERT(expr, __VA_ARGS__)
    
    // This is strategic, not allowing these to go into release builds.
    #define UNIMPLEMENTED(msg) PANIC("Unimplemented: " msg)
#endif

#endif

