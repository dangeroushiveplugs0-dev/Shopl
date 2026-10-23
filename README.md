# Shopl

Shopl is a long-term soft-body physics project, beginning with controllable, physically grounded soft-tissue deformation such as pressure-driven bulging and shape-aware local contact. It is intended to grow beyond a one-off demo.

## Physics strategy

Shopl is building a **Shopl-owned C++ physics core**. Established projects such as [PositionBasedDynamics](https://github.com/InteractiveComputerGraphics/PositionBasedDynamics) are references for studying algorithms and validating behavior, not runtime or build dependencies. The core should build independently and expose Shopl-owned types.

## Language responsibilities

- **C++** — simulation state, constraints, collision, deformation, and performance-sensitive core code.
- **Python** — repeatable test scenarios, parameter sweeps, and analysis tools.
- **HLSL / GLSL** — rendering and visualization shaders.
- Additional tooling languages are optional and should only be introduced when they provide a concrete benefit.

## Development principles

- Keep the physics core modular and avoid brute-force designs that become expensive as the project grows.
- Start with reproducible, testable simulation behavior before adding a polished viewer.
- Distinguish simulation volumes from rendered surfaces when needed.
- Test volume behavior, collision response, stability, and performance independently.
- Use external projects as references without inheriting unnecessary build complexity.
- Do not claim anatomical realism from a visually plausible mesh alone.

## Initial milestones

1. Establish a standalone C++ core and passing CI tests.
2. Add a Shopl-owned distance-constraint primitive and regression tests.
3. Implement fixed-step solver orchestration.
4. Add accelerated mesh collision queries and deformable-volume fixtures.
5. Add controlled pressure/volume behavior and shape-aware contact tests.
6. Add visualization after the solver tests are repeatable.

## Status

Early core-development stage. Gravity integration and basic simulation bookkeeping exist; a complete soft-body solver, mesh collision system, pressure-driven bulging, and visual demo are **not** yet implemented.
