# Shopl

Shopl is a long-term soft-body physics project, beginning with controllable, physically grounded soft-tissue deformation such as stomach bulging. It is intended to grow beyond a one-off demo.

## Current foundation

Shopl uses [PositionBasedDynamics](https://github.com/InteractiveComputerGraphics/PositionBasedDynamics) as its initial physics foundation. We will integrate it as a separate upstream dependency rather than copying its implementation into Shopl. Its MIT license and copyright notice must be preserved wherever required.

The long-term possibility of building a Shopl-owned physics core remains open. That is a future decision informed by testing, profiling, and what we learn from the existing solver—not a reason to duplicate the engine prematurely.

## Language responsibilities

- **C++** — physics integration, simulation data, deformation, and performance-sensitive core code.
- **C#** — project-facing controls and tooling where a C# layer is useful.
- **Python** — repeatable test scenarios, parameter sweeps, and analysis tools.
- **HLSL / GLSL** — rendering and visualization shaders.

These are intended roles, not a requirement to force every language into every build target.

## Development principles

- Keep the physics core modular and avoid brute-force designs that will become expensive as the project grows.
- Start with a reproducible, testable simulation before adding a polished viewer.
- Distinguish the simulation volume from its render surface; visual smoothness must not hide unstable or invalid physics.
- Test volume behavior, collision response, stability, and performance independently.
- Track upstream dependency versions and licenses explicitly.

## Initial milestones

1. Record the upstream dependency and integration approach.
2. Build a minimal C++ integration target in GitHub Actions.
3. Create a deterministic soft-body test fixture and baseline measurements.
4. Add controlled pressure/volume behavior and collision tests.
5. Add a visualization layer after the solver tests are repeatable.

## Status

Planning and dependency-integration stage. A complete stomach-bulging solver or visual demo is **not** claimed to exist yet.
