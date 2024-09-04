cmake_minimum_required(VERSION 3.28)
project(pkmn-c)

option(BUILD_TESTS "Build tests" on)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS off)
set(
    SOURCE_FILES
    include/pkmn_ai.h src/pkmn_ai.c
    include/pkmn_coroutine.h
    include/pkmn_battle.h src/pkmn_battle.c
    include/pkmn_battle_cli.h src/pkmn_battle_cli.c
    include/pkmn_battler.h src/pkmn_battler.c
    include/pkmn_config.h
    include/pkmn_math.h src/pkmn_math.c
    include/pkmn_move.h src/pkmn_move.c
    include/pkmn_rand.h src/pkmn_rand.c
    include/pkmn_species.h
    include/pkmn_stats.h src/pkmn_stats.c
    include/pkmn_type.h src/pkmn_type.c

    include/data/pkmn_move_data.h
    include/data/pkmn_pokemon_data.h
)

set(
    INCLUDE_DIRS
    include/
)


if (BUILD_TESTS)
    set(CMAKE_CXX_STANDARD 11)

    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2
        GIT_TAG 216713a4066b79d9803d374f261ccb30c0fb451f # v1.15.2
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(
        tests ${SOURCE_FILES}
        tests/test_main.cpp
        tests/test_pkmn_battler.cpp
        tests/test_pkmn_battle.cpp
        tests/test_pkmn_type.cpp
    )

    target_link_libraries(tests PRIVATE Catch2::Catch2)
    target_include_directories(tests PRIVATE ${INCLUDE_DIRS} Catch2::Catch2)

    enable_testing()
    add_test(NAME Tests COMMAND tests)
endif()

add_executable(${PROJECT_NAME} src/main.c ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
add_library(${PROJECT_NAME}-lib ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME}-lib PRIVATE ${INCLUDE_DIRS})
