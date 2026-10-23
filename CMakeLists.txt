cmake_minimum_required(VERSION 3.20)
project(Shopl VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
include(FetchContent)

option(SHOPL_BUILD_POSITION_BASED_DYNAMICS "Configure the pinned PositionBasedDynamics dependency" ON)

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

    # Upstream's top-level CMakeLists adds ${CMAKE_SOURCE_DIR}/CMake to the
    # module path. When embedded via FetchContent, CMAKE_SOURCE_DIR points at
    # Shopl instead of the dependency, so Utils cannot find
    # GetGitRevisionDescription.cmake. Populate first and add the upstream
    # project only after explicitly adding its own CMake module directory.
    FetchContent_GetProperties(PositionBasedDynamics)
    if(NOT positionbaseddynamics_POPULATED)
        FetchContent_Populate(PositionBasedDynamics)
    endif()
    list(APPEND CMAKE_MODULE_PATH "${positionbaseddynamics_SOURCE_DIR}/CMake")
    add_subdirectory(
        "${positionbaseddynamics_SOURCE_DIR}"
        "${positionbaseddynamics_BINARY_DIR}"
    )
endif()

if(SHOPL_BUILD_POSITION_BASED_DYNAMICS)
    add_library(shopl_pbd_adapter
        src/shopl_pbd_adapter/PbdModelAdapter.cpp
    )
    target_include_directories(shopl_pbd_adapter
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            # The upstream Simulation target omits the repository root from
            # its public includes, but its headers are included as Simulation/...
            $<BUILD_INTERFACE:${positionbaseddynamics_SOURCE_DIR}>
    )
    target_compile_features(shopl_pbd_adapter PUBLIC cxx_std_17)
    target_link_libraries(shopl_pbd_adapter PUBLIC shopl_core Simulation)

    if(MSVC)
        target_compile_options(shopl_pbd_adapter PRIVATE /W4 /permissive-)
    else()
        target_compile_options(shopl_pbd_adapter PRIVATE -Wall -Wextra -Wpedantic)
    endif()
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

    if(SHOPL_BUILD_POSITION_BASED_DYNAMICS)
        add_executable(shopl_pbd_adapter_tests
            tests/test_pbd_model_adapter.cpp
        )
        target_link_libraries(shopl_pbd_adapter_tests PRIVATE shopl_pbd_adapter)
        target_compile_features(shopl_pbd_adapter_tests PRIVATE cxx_std_17)
        add_test(NAME shopl_pbd_adapter_tests COMMAND shopl_pbd_adapter_tests)
    endif()
endif()
