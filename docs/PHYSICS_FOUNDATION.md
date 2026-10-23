# Physics foundation and integration plan

## Selected upstream

- Project: [PositionBasedDynamics](https://github.com/InteractiveComputerGraphics/PositionBasedDynamics)
- Pinned revision: `10a70bc146a97873dc3c8fef372f5217e010542e`
- License: MIT (retain its copyright and license notice for copied or redistributed portions).
- Integration style: separate upstream dependency, with Shopl-specific code kept in Shopl-owned modules.
- Upstream source is not vendored or copied into this repository.

## Why this is the starting point

PositionBasedDynamics provides a C++ foundation for position-based and extended position-based dynamics, including deformable-body constraints and collision-related functionality. It is a useful base for learning and validating the desired behavior without claiming that it already implements Shopl's exact use case.

The upstream CMake project has build-time requirements and optional demo/Python-binding paths. Shopl pins a specific revision and validates the library build in CI rather than assuming the complete upstream demo suite is suitable for Shopl.

## Current Shopl modules

- `shopl_core`: solver-independent vector, particle state, simulation bookkeeping, and baseline gravity integration.
- `shopl_pbd_adapter`: currently maps Shopl vectors and particle operations to an upstream `PBD::SimulationModel`, and can add distance constraints. It does **not** step the upstream solver yet.
- `shopl_tests`: deterministic headless tests and regression fixtures.

## Planned modules

- `shopl_softbody`: Shopl-owned setup and behavior for deformable volumes, constraints, and later pressure-driven effects.
- `shopl_viewer` (later): visual inspection and interactive parameters.
- `tools/python` (later): repeatable scenarios and parameter analysis.
- `shaders/` (later): HLSL/GLSL visualization shaders.

Avoid exposing upstream implementation details throughout the project. Keep the adapter narrow so a future solver replacement does not require rewriting every higher-level system.

## Validation gates

1. Build a minimal C++ target in GitHub Actions.
2. Confirm the selected upstream revision and its required dependencies build in library-only mode.
3. Validate the Shopl-to-PBD particle/constraint adapter with headless tests.
4. Add a real PBD stepping bridge and deterministic deformable-volume fixture.
5. Test volume drift, finite positions, collision response, and repeatability across runs.
6. Only then add pressure-driven volume behavior and shape-aware local contact/penetration behavior.

## Future custom physics

A Shopl-owned solver is a possible later milestone, not a current requirement. Any replacement should be justified by measured limitations and validated against the same regression fixtures. Until then, keep the upstream engine identifiable as a dependency and preserve its license requirements.
