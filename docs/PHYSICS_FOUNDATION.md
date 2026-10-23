# Physics foundation and reference strategy

## Direction

Shopl is building a Shopl-owned C++ physics core. PositionBasedDynamics (PBD) is a **reference implementation for studying algorithms and comparing behavior**, not a build or runtime dependency.

We will not fetch, compile, link, or expose upstream PBD types in Shopl's public API. Useful techniques may inform independently implemented modules. If code is copied or redistributed, its license and copyright obligations must be reviewed and retained; the preferred path is independent implementation with references documented.

## Current Shopl modules

- `shopl_core`: vector, particle state (including inverse mass for pinned particles), simulation bookkeeping, and gravity integration.
- `shopl_constraints`: distance constraints plus a deterministic ordered batch solver. Batch solving uses a working copy so invalid constraints do not partially mutate caller state.
- Headless tests cover regression cases, invalid inputs, and repeatability.

## Planned modules

- `shopl_stepper`: fixed-step orchestration, damping, stability limits, and deterministic stepping.
- `shopl_collision`: spatial acceleration structures and mesh-aware narrow-phase queries; avoid unbounded brute-force pair checks.
- `shopl_softbody`: deformable volume, constraint networks, volume preservation, and pressure-driven effects.
- `shopl_viewer` (later): visual inspection and interactive parameters.
- `tools/python` (later): repeatable scenarios, parameter sweeps, and analysis tools.

Keep modules small and replaceable. Public APIs use Shopl-owned types.

## Validation gates

1. Keep standalone core and regression tests green.
2. Validate distance constraints: rest length, stiffness bounds, pinned particles, invalid indices/parameters, and degenerate pairs.
3. Validate multi-constraint convergence, deterministic ordering, iteration limits, and no partial mutation on failure.
4. Add fixed-step orchestration and deterministic multi-step tests.
5. Add spatial acceleration and mesh collision tests before large-scale collision work.
6. Measure deformable-volume drift, finite positions, and repeatability.
7. Add pressure-driven volume behavior only after the baseline is stable.
8. Keep contact, indentation, and actual penetration/topology as separate concerns.

PBD and other reputable sources are references for algorithms and expected properties, not drop-in dependencies. Every technique must earn its place through tests and measurable benefits. Internal-pressure bulging and shape-aware contact remain planned behaviors documented in `BULGE_BEHAVIOR.md`.
