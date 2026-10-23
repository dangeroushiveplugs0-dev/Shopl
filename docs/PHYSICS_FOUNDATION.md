# Physics foundation and reference strategy

## Direction

Shopl is building a Shopl-owned C++ physics core. PositionBasedDynamics (PBD) is a **reference implementation for studying algorithms and comparing behavior**, not a build or runtime dependency.

We will not fetch, compile, link, or expose upstream PBD types in Shopl's public API. Useful techniques may inform independently implemented modules. If code is copied or redistributed, its license and copyright obligations must be reviewed and retained; the preferred path is independent implementation with references documented.

## Current Shopl modules

- `shopl_core`: vector, particle state (including inverse mass for pinned particles), simulation bookkeeping, and semi-implicit Euler gravity integration.
- `shopl_constraints`: distance constraints plus a deterministic ordered batch solver. Batch solving uses a working copy so invalid constraints do not partially mutate caller state.
- `shopl_stepper`: fixed-duration multi-step orchestration. After constraint projection, dynamic velocities are reconstructed from corrected displacement to avoid stale pre-projection velocity.
- Headless tests cover regression cases, discrete constant-acceleration behavior, invalid inputs, and same-build repeatability.
- Numerical-validation tests compare fixed-step gravity to the known discrete solution over several time steps and run a constrained particle for 2,400 steps under gravity.

## Planned modules

- `shopl_collision`: spatial acceleration structures and mesh-aware narrow-phase queries; avoid unbounded brute-force pair checks.
- `shopl_softbody`: deformable volume, constraint networks, volume preservation, and pressure-driven effects.
- `shopl_viewer` (later): visual inspection and interactive parameters.
- `tools/python` (later): repeatable scenarios, parameter sweeps, and analysis tools.
- Android APK test harness and a GitHub Pages-based visual report/scenario viewer (later), built through GitHub Actions once the headless API and test scenarios are stable.

## Validation gates

A green CI run means only that the configured build and automated tests passed. It does not establish physical realism.

1. Keep standalone core and regression tests green.
2. Validate distance constraints: rest length, stiffness bounds, pinned particles, invalid indices/parameters, and degenerate pairs.
3. Validate multi-constraint convergence, deterministic ordering, iteration limits, and no partial mutation on failure.
4. Validate fixed-step orchestration against known discrete results for the selected integrator, and check repeatability and failure atomicity.
5. Run numerical-stability sweeps across time steps, masses, stiffnesses, constraint counts, and long durations; record error and finite-state failures, not just pass/fail.
6. Compare measured behavior with analytical or well-established reference cases, including free fall, pendulum/oscillator cases when implemented, and energy or momentum behavior where applicable, with documented tolerance bands.
7. Use visual scenarios and Android APKs for physical/interactive inspection; keep visual inspection complementary to numerical tests.
8. Add spatial acceleration and mesh collision tests before large-scale collision work.
9. Measure deformable-volume drift, finite positions, and repeatability.
10. Add pressure-driven volume behavior only after the baseline is stable.
11. Keep contact, indentation, and actual penetration/topology as separate concerns.

Every technique must earn its place through tests and measurable benefits. PBD and other reputable sources are references for algorithms and expected properties, not drop-in dependencies. Internal-pressure bulging and shape-aware contact remain planned behaviors documented in `BULGE_BEHAVIOR.md`.
