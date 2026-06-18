OBJ Half-Edge / Normals / Smoothing Test Pack
============================================

Constraints respected:
- triangles only, no n-gons
- no materials, no UVs
- closed 2-manifold geometry, except no intentional non-manifold cases
- OBJ winding is outward for tests 01-09
- test 10 is intentionally inverted as a negative test

Suggested usage order:
1. 01_cube_hard_no_normals.obj
   Baseline: importer calculates hard normals from triangles, s off.

2. 02_cube_hard_with_user_normals.obj
   Same cube, but with explicit vn face normals.

3. 03_cube_smooth_no_normals.obj
   All cube faces in smoothing group 1. If smoothing groups are applied, cube should look rounded.

4. 04_cube_smooth_with_user_normals.obj
   Explicit smooth vertex normals. Normal index equals vertex index.

5. 05_cone_closed_smooth_sides_no_normals.obj
   Important cone test. Side faces use s 1; base cap is s off. Normals must be computed.

6. 06_cone_closed_with_user_normals.obj
   Same cone idea, but with explicit vn. Side uses s 1; cap uses s off.

7. 07_cylinder_smooth_sides_no_normals.obj
   Side smooth, caps hard. Useful for checking smoothing group boundaries.

8. 08_cylinder_with_user_normals.obj
   Same cylinder idea, explicit vn: radial side normals, flat cap normals.

9. 09_two_components_same_smoothing_group_no_normals.obj
   Two disconnected tetrahedra both using s 1. Smoothing must not mix across components.

10. 10_NEGATIVE_cone_inverted_winding_no_normals.obj
   Intentionally inward winding. Use this only to test detection/correction of inverted components.

Notes:
- In OBJ, smoothing group IDs are face state, not a coefficient.
- For s off, treat each face as hard even if positions are shared.
- For s N, average only faces incident to the same geometric vertex and the same smoothing group / component.
- If explicit vn are present, the importer can use them directly, but preserving smoothing group metadata is still useful for recomputation/export.
