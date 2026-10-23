cmake_minimum_required(VERSION 3.20)
project(Shopl VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

add_library(shopl_core
    src/shopl_core/SimulationState.cpp
    src/shopl_core/GravityIntegrator.cpp
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

if(BUILD_TESTING)
    add_executable(shopl_core_tests
        tests/test_simulation_state.cpp
    )
    target_link_libraries(shopl_core_tests PRIVATE shopl_core)
    target_compile_features(shopl_core_tests PRIVATE cxx_std_17)
    add_test(NAME shopl_core_tests COMMAND shopl_core_tests)

    add_executable(shopl_gravity_tests
        tests/test_gravity_integrator.cpp
    )
    target_link_libraries(shopl_gravity_tests PRIVATE shopl_core)
    target_compile_features(shopl_gravity_tests PRIVATE cxx_std_17)
    add_test(NAME shopl_gravity_tests COMMAND shopl_gravity_tests)
endif()
