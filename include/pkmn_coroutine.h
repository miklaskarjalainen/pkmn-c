#ifndef PKMN_COROUTINE_H
#define PKMN_COROUTINE_H

typedef struct pkmn_coroutine_t {
    int index;
} pkmn_coroutine_t;

#define COROUTINE_START(co) do { switch(co.index) { default:
#define COROUTINE_CASE(co, name) case __LINE__: name: co.index = __LINE;
#define COROUTINE_EXIT(co) do { goto __co_end; } while (0)
#define COROUTINE_END(co) } co.index = 0; __co_end:; } while(0)
#define COROUTINE_YIELD(co) do { co.index = __LINE__; COROUTINE_EXIT(co); case __LINE__:; } while(0)
#define COROUTINE_YIELD_RET(co, ret) do { co.index = __LINE__; return ret; case __LINE__:; } while(0)

#endif
