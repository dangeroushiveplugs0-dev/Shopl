# Physics foundation and integration plan

## Selected upstream

- Project: [PositionBasedDynamics](https://github.com/InteractiveComputerGraphics/PositionBasedDynamics)
- License: MIT (retain its copyright and license notice for copied or redistributed portions).
- Integration style: separate upstream dependency, with Shopl-specific code kept in Shopl-owned modules.
- Upstream source is not vendored or copied into this repository at this stage.

## Why this is the starting point

PositionBasedDynamics provides a C++ foundation for position-based and extended position-based dynamics, including deformable-body constraints and collision-related functionality. It is a useful base for learning and validating the desired behavior without claiming that it already implements Shopl's exact use case.

The upstream CMake project currently has build-time requirements and optional demo/Python-binding paths. Before wiring it into the default build, inspect the exact upstream revision and test a minimal library-only build in CI. Do not assume the complete upstream demo suite is suitable for Shopl's build.

## Planned Shopl modules

The names below are proposed boundaries, not existing implemented code.

- `shopl_core`: common simulation types, configuration, and stable public interfaces.
- `shopl_pbd_adapter`: narrow adapter around the selected PositionBasedDynamics APIs.
- `shopl_softbody`: Shopl-owned setup and behavior for deformable volumes, constraints, and later pressure-driven effects.
- `shopl_tests`: deterministic headless tests and regression fixtures.
- `shopl_viewer` (later): visual inspection and interactive parameters.
- `tools/python` (later): repeatable scenarios and parameter analysis.
- `shaders/` (later): HLSL/GLSL visualization shaders.

Avoid exposing upstream implementation details throughout the project. The adapter should make a future solver replacement possible without rewriting every higher-level system.

## First validation gates

1. Build a minimal C++ target in GitHub Actions.
2. Confirm the selected upstream revision and its required dependencies build in library-only mode.
3. Add a small deformable-volume fixture with fixed initial conditions.
4. Test volume drift, finite positions, collision response, and repeatability across runs.
5. Only then add stomach-specific pressure/shape behavior and rendering.

## Future custom physics

A Shopl-owned solver is a possible later milestone, not a current requirement. Any replacement should be justified by measured limitations and validated against the same regression fixtures. Until then, keep the upstream engine identifiable as a dependency and preserve its license requirements.
