cmake_minimum_required(VERSION 3.20)
project(Shopl VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
include(FetchContent)

option(SHOPL_BUILD_POSITION_BASED_DYNAMICS "Configure the pinned PositionBasedDynamics dependency" ON)

add_library(shopl_core
    src/shopl_core/SimulationState.cpp
)
target_include_directories(shopl_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(shopl_core PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(shopl_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(shopl_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(SHOPL_BUILD_POSITION_BASED_DYNAMICS)
    # Pin upstream to a known commit so builds do not silently change as upstream moves.
    set(PBD_LIBS_ONLY ON CACHE BOOL "Build only PositionBasedDynamics libraries" FORCE)
    set(USE_PYTHON_BINDINGS OFF CACHE BOOL "Do not build optional Python bindings in the initial CI target" FORCE)
    FetchContent_Declare(
        PositionBasedDynamics
        GIT_REPOSITORY https://github.com/InteractiveComputerGraphics/PositionBasedDynamics.git
        GIT_TAG 10a70bc146a97873dc3c8fef372f5217e010542e
        GIT_SHALLOW FALSE
    )
    FetchContent_MakeAvailable(PositionBasedDynamics)
endif()

if(BUILD_TESTING)
    add_executable(shopl_core_tests
        tests/test_simulation_state.cpp
    )
    target_link_libraries(shopl_core_tests PRIVATE shopl_core)
    target_compile_features(shopl_core_tests PRIVATE cxx_std_17)
    add_test(NAME shopl_core_tests COMMAND shopl_core_tests)
endif()
