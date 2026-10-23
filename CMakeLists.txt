cmake_minimum_required(VERSION 3.20)
project(Shopl VERSION 0.1.0 LANGUAGES CXX)
include(CTest)

add_library(shopl_core
    src/shopl_core/SimulationState.cpp
    src/shopl_core/GravityIntegrator.cpp)
target_include_directories(shopl_core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(shopl_core PUBLIC cxx_std_17)

add_library(shopl_constraints
    src/shopl_constraints/DistanceConstraint.cpp
    src/shopl_constraints/DistanceConstraintSolver.cpp)
target_link_libraries(shopl_constraints PUBLIC shopl_core)
target_include_directories(shopl_constraints PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(shopl_constraints PUBLIC cxx_std_17)

add_library(shopl_stepper src/shopl_stepper/FixedStepStepper.cpp)
target_link_libraries(shopl_stepper PUBLIC shopl_core shopl_constraints)
target_include_directories(shopl_stepper PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(shopl_stepper PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(shopl_core PRIVATE /W4 /permissive-)
    target_compile_options(shopl_constraints PRIVATE /W4 /permissive-)
    target_compile_options(shopl_stepper PRIVATE /W4 /permissive-)
else()
    target_compile_options(shopl_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(shopl_constraints PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(shopl_stepper PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_TESTING)
    add_executable(shopl_core_tests tests/test_simulation_state.cpp)
    target_link_libraries(shopl_core_tests PRIVATE shopl_core)
    target_compile_features(shopl_core_tests PRIVATE cxx_std_17)
    add_test(NAME shopl_core_tests COMMAND shopl_core_tests)

    add_executable(shopl_gravity_tests tests/test_gravity_integrator.cpp)
    target_link_libraries(shopl_gravity_tests PRIVATE shopl_core)
    target_compile_features(shopl_gravity_tests PRIVATE cxx_std_17)
    add_test(NAME shopl_gravity_tests COMMAND shopl_gravity_tests)

    add_executable(shopl_constraint_tests tests/test_distance_constraint.cpp)
    target_link_libraries(shopl_constraint_tests PRIVATE shopl_constraints)
    target_compile_features(shopl_constraint_tests PUBLIC cxx_std_17)
    add_test(NAME shopl_constraint_tests COMMAND shopl_constraint_tests)

    add_executable(shopl_constraint_solver_tests tests/test_distance_constraint_solver.cpp)
    target_link_libraries(shopl_constraint_solver_tests PRIVATE shopl_constraints)
    target_compile_features(shopl_constraint_solver_tests PRIVATE cxx_std_17)
    add_test(NAME shopl_constraint_solver_tests COMMAND shopl_constraint_solver_tests)

    add_executable(shopl_stepper_tests tests/test_fixed_step_stepper.cpp)
    target_link_libraries(shopl_stepper_tests PRIVATE shopl_stepper)
    target_compile_features(shopl_stepper_tests PRIVATE cxx_std_17)
    add_test(NAME shopl_stepper_tests COMMAND shopl_stepper_tests)

    add_executable(shopl_numerical_validation_tests tests/test_numerical_validation.cpp)
    target_link_libraries(shopl_numerical_validation_tests PRIVATE shopl_stepper)
    target_compile_features(shopl_numerical_validation_tests PRIVATE cxx_std_17)
    add_test(NAME shopl_numerical_validation_tests COMMAND shopl_numerical_validation_tests)
endif()
