# Physics foundation and reference strategy

## Direction

Shopl is building a Shopl-owned C++ physics core. PositionBasedDynamics (PBD) is a **reference implementation for studying algorithms and comparing behavior**, not a build or runtime dependency.

We will not fetch, compile, link, or expose upstream PBD types in Shopl's public API. Useful techniques may inform independently implemented modules. When code is copied or redistributed, its license and copyright obligations must be reviewed and retained; the preferred path is to implement algorithms independently and document relevant references.

## Current Shopl modules

- `shopl_core`: solver-independent vector, particle state, simulation bookkeeping, and baseline gravity integration.
- `shopl_tests`: deterministic headless tests and regression fixtures.

## Planned Shopl-owned modules

- `shopl_constraints`: distance and other local constraints with bounded, validated parameters.
- `shopl_collision`: broad-phase acceleration structures and mesh-aware narrow-phase queries; avoid unbounded brute-force pair checks.
- `shopl_softbody`: deformable volume setup, constraint networks, volume preservation, and pressure-driven effects.
- `shopl_stepper`: fixed-step simulation orchestration, damping, stability limits, and deterministic stepping.
- `shopl_viewer` (later): visual inspection and interactive parameters.
- `tools/python` (later): repeatable scenarios, parameter sweeps, and analysis tools.

Keep modules small and replaceable. Public APIs should use Shopl-owned types rather than upstream implementation details.

## Validation gates

1. Keep the standalone core building and its regression tests passing in CI.
2. Implement a Shopl-owned distance-constraint primitive with tests for rest length, stiffness bounds, invalid indices, and degenerate cases.
3. Add a fixed-step solver loop and deterministic multi-step tests.
4. Add spatial acceleration and mesh collision tests before large-scale collision work.
5. Build a deformable-volume fixture and measure volume drift, finite positions, and repeatability.
6. Add pressure-driven volume behavior only after the baseline is stable.
7. Add shape-aware local contact response and keep contact, indentation, and actual penetration/topology as separate concerns.

## Reference-based development

Use PBD and other reputable sources to understand algorithms, compare expected properties, and design tests. Do not treat an upstream implementation as a drop-in solution or copy code without reviewing license obligations. Each adopted technique must earn its place through tests and measurable benefits for Shopl's target behaviors.

## Scope

The current scaffold is not a complete soft-body solver. Internal-pressure bulging and shape-aware contact remain planned behaviors, documented in `BULGE_BEHAVIOR.md`.
