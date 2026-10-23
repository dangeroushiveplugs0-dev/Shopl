# Shopl bulge behavior requirements

Shopl must support at least two distinct causes of soft-tissue deformation. These are separate behavior models, not one generic inflate operation.

## 1. Internal contents and pressure

Example: liquid or another volume of contents inside a character's stomach.

Expected behavior:
- Internal contents occupy volume and exert pressure on a deformable enclosing tissue volume.
- Expansion follows pressure, available space, tissue stiffness, constraints, and contact with surrounding anatomy.
- The deformation should be continuous and spatially plausible, not a uniform scale of the whole mesh.
- Gravity and body movement can redistribute contents and affect the shape.
- Volume and pressure must be bounded by configurable material and safety limits; the solver must remain numerically stable.

Potential implementation: a deformable closed volume with volume-preservation constraints, a pressure/target-volume model, and collision constraints. The exact model should be selected after baseline tests.

## 2. Shape-aware localized contact or penetration

Example: an elongated, cylindrical, rectangular, or otherwise thin object presses into or passes through a soft-tissue volume.

Expected behavior:
- The affected region is driven by actual contact geometry, contact location, orientation, depth, and motion.
- An elongated object should produce an elongated response; a broad object should affect a broader region.
- Deformation propagates into nearby tissue according to the material model and constraint network, rather than creating a fixed spherical bulge.
- The surrounding surface remains continuous and collision handling prevents unrelated parts of the mesh from snapping or exploding.
- The simulation distinguishes external indentation, contact, and actual penetration through the body. Penetration is a separate collision/topology problem and must not be faked by simply moving vertices through the object.

Potential implementation: triangle-mesh or suitable narrow-phase collision queries, a deformable volume representation, local constraints, and a contact response model. A render mesh alone is not automatically a physically valid volume mesh.

## Shared requirements

- Treat the simulation volume and rendered surface as distinct representations when needed.
- Expose material stiffness, damping, pressure/target volume, contact response, and solver settings as parameters.
- Add repeatable tests for volume drift, local deformation footprint, collision response, finite coordinates, and recovery after the force is removed.
- Keep tests deterministic where possible and establish baseline measurements before optimizing.
- Do not claim anatomical realism from a visually plausible mesh alone.

## Scope

These are project requirements and planned behaviors. They are not implemented by the initial simulation-state scaffold.
