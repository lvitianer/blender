/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2008, Blender Foundation
 * This is a new part of Blender
 */

#pragma once

/** \file
 * \ingroup bke
 */

#ifdef __cplusplus
extern "C" {
#endif

struct Depsgraph;
struct Main;
struct Object;
struct RegionView3D;
struct Scene;
struct bGPDcurve;
struct bGPDframe;
struct bGPDspoint;
struct bGPDstroke;
struct bGPdata;

/* Object bound-box. */

/**
 * Get min/max bounds of all strokes in grease pencil data-block.
 * \param gpd: Grease pencil data-block
 * \param r_min: Result minimum coordinates
 * \param r_max: Result maximum coordinates
 * \return True if it was possible to calculate
 */
bool BKE_gpencil_data_minmax(const struct bGPdata *gpd, float r_min[3], float r_max[3]);
/**
 * Get min/max coordinate bounds for single stroke.
 * \param gps: Grease pencil stroke
 * \param use_select: Include only selected points
 * \param r_min: Result minimum coordinates
 * \param r_max: Result maximum coordinates
 * \return True if it was possible to calculate
 */
bool BKE_gpencil_stroke_minmax(const struct bGPDstroke *gps,
                               const bool use_select,
                               float r_min[3],
                               float r_max[3]);

/**
 * Get grease pencil object bounding box.
 * \param ob: Grease pencil object
 * \return Bounding box
 */
struct BoundBox *BKE_gpencil_boundbox_get(struct Object *ob);
/**
 * Compute center of bounding box.
 * \param gpd: Grease pencil data-block
 * \param r_centroid: Location of the center
 */
void BKE_gpencil_centroid_3d(struct bGPdata *gpd, float r_centroid[3]);
/**
 * Compute stroke bounding box.
 * \param gps: Grease pencil Stroke
 */
void BKE_gpencil_stroke_boundingbox_calc(struct bGPDstroke *gps);

/* Stroke geometry utilities. */

/**
 * Calculate stroke normals.
 * \param gps: Grease pencil stroke
 * \param r_normal: Return Normal vector normalized
 */
void BKE_gpencil_stroke_normal(const struct bGPDstroke *gps, float r_normal[3]);
/**
 * Reduce a series of points to a simplified version,
 * but maintains the general shape of the series.
 *
 * Ramer - Douglas - Peucker algorithm
 * by http://en.wikipedia.org/wiki/Ramer-Douglas-Peucker_algorithm
 * \param gpd: Grease pencil data-block
 * \param gps: Grease pencil stroke
 * \param epsilon: Epsilon value to define precision of the algorithm
 */
void BKE_gpencil_stroke_simplify_adaptive(struct bGPdata *gpd,
                                          struct bGPDstroke *gps,
                                          float epsilon);
/**
 * Simplify alternate vertex of stroke except extremes.
 * \param gpd: Grease pencil data-block
 * \param gps: Grease pencil stroke
 */
void BKE_gpencil_stroke_simplify_fixed(struct bGPdata *gpd, struct bGPDstroke *gps);
/**
 * Subdivide a stroke
 * \param gpd: Grease pencil data-block
 * \param gps: Stroke
 * \param level: Level of subdivision
 * \param type: Type of subdivision
 */
void BKE_gpencil_stroke_subdivide(struct bGPdata *gpd,
                                  struct bGPDstroke *gps,
                                  int level,
                                  int type);
/**
 * Trim stroke to the first intersection or loop.
 * \param gps: Stroke data
 */
bool BKE_gpencil_stroke_trim(struct bGPdata *gpd, struct bGPDstroke *gps);
/**
 * Reduce a series of points when the distance is below a threshold.
 * Special case for first and last points (both are kept) for other points,
 * the merge point always is at first point.
 *
 * \param gpd: Grease pencil data-block.
 * \param gpf: Grease Pencil frame.
 * \param gps: Grease Pencil stroke.
 * \param threshold: Distance between points.
 * \param use_unselected: Set to true to analyze all stroke and not only selected points.
 */
void BKE_gpencil_stroke_merge_distance(struct bGPdata *gpd,
                                       struct bGPDframe *gpf,
                                       struct bGPDstroke *gps,
                                       const float threshold,
                                       const bool use_unselected);

/**
 * Get points of stroke always flat to view not affected
 * by camera view or view position.
 * \param points: Array of grease pencil points (3D)
 * \param totpoints: Total of points
 * \param points2d: Result array of 2D points
 * \param r_direction: Return Concave (-1), Convex (1), or Auto-detect (0)
 */
void BKE_gpencil_stroke_2d_flat(const struct bGPDspoint *points,
                                int totpoints,
                                float (*points2d)[2],
                                int *r_direction);
/**
 * Get points of stroke always flat to view not affected by camera view or view position
 * using another stroke as reference.
 * \param ref_points: Array of reference points (3D)
 * \param ref_totpoints: Total reference points
 * \param points: Array of points to flat (3D)
 * \param totpoints: Total points
 * \param points2d: Result array of 2D points
 * \param scale: Scale factor
 * \param r_direction: Return Concave (-1), Convex (1), or Auto-detect (0)
 */
void BKE_gpencil_stroke_2d_flat_ref(const struct bGPDspoint *ref_points,
                                    int ref_totpoints,
                                    const struct bGPDspoint *points,
                                    int totpoints,
                                    float (*points2d)[2],
                                    const float scale,
                                    int *r_direction);
/**
 * Triangulate stroke to generate data for filling areas.
 * \param gps: Grease pencil stroke
 */
void BKE_gpencil_stroke_fill_triangulate(struct bGPDstroke *gps);
/**
 * Recalc all internal geometry data for the stroke
 * \param gpd: Grease pencil data-block
 * \param gps: Grease pencil stroke
 */
void BKE_gpencil_stroke_geometry_update(struct bGPdata *gpd, struct bGPDstroke *gps);
/**
 * Update Stroke UV data.
 * \param gps: Grease pencil stroke
 */
void BKE_gpencil_stroke_uv_update(struct bGPDstroke *gps);

/**
 * Apply grease pencil Transforms.
 * \param gpd: Grease pencil data-block
 * \param mat: Transformation matrix
 */
void BKE_gpencil_transform(struct bGPdata *gpd, const float mat[4][4]);

typedef struct GPencilPointCoordinates {
  /* This is used when doing "move only origin" in object_data_transform.c.
   * pressure is needs to be stored here as it is tied to object scale. */
  float co[3];
  float pressure;
} GPencilPointCoordinates;

/**
 * \note Used for "move only origins" in object_data_transform.c.
 */
int BKE_gpencil_stroke_point_count(const struct bGPdata *gpd);
/**
 * \note Used for "move only origins" in object_data_transform.c.
 */
void BKE_gpencil_point_coords_get(struct bGPdata *gpd, GPencilPointCoordinates *elem_data);
/**
 * \note Used for "move only origins" in object_data_transform.c.
 */
void BKE_gpencil_point_coords_apply(struct bGPdata *gpd, const GPencilPointCoordinates *elem_data);
/**
 * \note Used for "move only origins" in object_data_transform.c.
 */
void BKE_gpencil_point_coords_apply_with_mat4(struct bGPdata *gpd,
                                              const GPencilPointCoordinates *elem_data,
                                              const float mat[4][4]);

/**
 * Resample a stroke
 * \param gpd: Grease pencil data-block
 * \param gps: Stroke to sample
 * \param dist: Distance of one segment
 */
bool BKE_gpencil_stroke_sample(struct bGPdata *gpd,
                               struct bGPDstroke *gps,
                               const float dist,
                               const bool select);
/**
 * Apply smooth position to stroke point.
 * \param gps: Stroke to smooth
 * \param i: Point index
 * \param inf: Amount of smoothing to apply
 */
bool BKE_gpencil_stroke_smooth_point(struct bGPDstroke *gps, int i, float inf);
/**
 * Apply smooth strength to stroke point.
 * \param gps: Stroke to smooth
 * \param point_index: Point index
 * \param influence: Amount of smoothing to apply
 */
bool BKE_gpencil_stroke_smooth_strength(struct bGPDstroke *gps, int point_index, float influence);
/**
 * Apply smooth for thickness to stroke point (use pressure).
 * \param gps: Stroke to smooth
 * \param point_index: Point index
 * \param influence: Amount of smoothing to apply
 */
bool BKE_gpencil_stroke_smooth_thickness(struct bGPDstroke *gps, int point_index, float influence);
/**
 * Apply smooth for UV rotation to stroke point (use pressure).
 * \param gps: Stroke to smooth
 * \param point_index: Point index
 * \param influence: Amount of smoothing to apply
 */
bool BKE_gpencil_stroke_smooth_uv(struct bGPDstroke *gps, int point_index, float influence);
/**
 * Close grease pencil stroke.
 * \param gps: Stroke to close
 */
bool BKE_gpencil_stroke_close(struct bGPDstroke *gps);
/**
 * Dissolve points in stroke.
 * \param gpd: Grease pencil data-block
 * \param gpf: Grease pencil frame
 * \param gps: Grease pencil stroke
 * \param tag: Type of tag for point
 */
void BKE_gpencil_dissolve_points(struct bGPdata *gpd,
                                 struct bGPDframe *gpf,
                                 struct bGPDstroke *gps,
                                 const short tag);

/**
 * Backbone stretch similar to Freestyle.
 * \param gps: Stroke to sample.
 * \param dist: Length of the added section.
 * \param overshoot_fac: Relative length of the curve which is used to determine the extension.
 * \param mode: Affect to Start, End or Both extremes (0->Both, 1->Start, 2->End).
 * \param follow_curvature: True for approximating curvature of given overshoot.
 * \param extra_point_count: When follow_curvature is true, use this amount of extra points.
 */
bool BKE_gpencil_stroke_stretch(struct bGPDstroke *gps,
                                const float dist,
                                const float overshoot_fac,
                                const short mode,
                                const bool follow_curvature,
                                const int extra_point_count,
                                const float segment_influence,
                                const float max_angle,
                                const bool invert_curvature);
/**
 * Trim stroke to needed segments.
 * \param gps: Target stroke.
 * \param index_from: the index of the first point to be used in the trimmed result.
 * \param index_to: the index of the last point to be used in the trimmed result.
 */
bool BKE_gpencil_stroke_trim_points(struct bGPDstroke *gps,
                                    const int index_from,
                                    const int index_to);
/**
 * Split the given stroke into several new strokes, partitioning
 * it based on whether the stroke points have a particular flag
 * is set (e.g. #GP_SPOINT_SELECT in most cases, but not always).
 */
struct bGPDstroke *BKE_gpencil_stroke_delete_tagged_points(struct bGPdata *gpd,
                                                           struct bGPDframe *gpf,
                                                           struct bGPDstroke *gps,
                                                           struct bGPDstroke *next_stroke,
                                                           int tag_flags,
                                                           const bool select,
                                                           const bool flat_cap,
                                                           const int limit);
void BKE_gpencil_curve_delete_tagged_points(struct bGPdata *gpd,
                                            struct bGPDframe *gpf,
                                            struct bGPDstroke *gps,
                                            struct bGPDstroke *next_stroke,
                                            struct bGPDcurve *gpc,
                                            int tag_flags);

/**
 * Flip stroke.
 */
void BKE_gpencil_stroke_flip(struct bGPDstroke *gps);
/**
 * Split stroke.
 * \param gpd: Grease pencil data-block.
 * \param gpf: Grease pencil frame.
 * \param gps: Grease pencil original stroke.
 * \param before_index: Position of the point to split.
 * \param remaining_gps: Secondary stroke after split.
 * \return True if the split was done
 */
bool BKE_gpencil_stroke_split(struct bGPdata *gpd,
                              struct bGPDframe *gpf,
                              struct bGPDstroke *gps,
                              const int before_index,
                              struct bGPDstroke **remaining_gps);
/**
 * Shrink the stroke by length.
 * \param gps: Stroke to shrink
 * \param dist: delta length
 * \param mode: 1->Start, 2->End
 */
bool BKE_gpencil_stroke_shrink(struct bGPDstroke *gps, const float dist, const short mode);

/**
 * Calculate grease pencil stroke length.
 * \param gps: Grease pencil stroke.
 * \param use_3d: Set to true to use 3D points.
 * \return Length of the stroke.
 */
float BKE_gpencil_stroke_length(const struct bGPDstroke *gps, bool use_3d);
/** Calculate grease pencil stroke length between points. */
float BKE_gpencil_stroke_segment_length(const struct bGPDstroke *gps,
                                        const int start_index,
                                        const int end_index,
                                        bool use_3d);

/**
 * Set a random color to stroke using vertex color.
 * \param gps: Stroke
 */
void BKE_gpencil_stroke_set_random_color(struct bGPDstroke *gps);

/**
 * Join two strokes using the shortest distance (reorder stroke if necessary).
 */
void BKE_gpencil_stroke_join(struct bGPDstroke *gps_a,
                             struct bGPDstroke *gps_b,
                             const bool leave_gaps,
                             const bool fit_thickness,
                             const bool smooth);
/**
 * Copy the stroke of the frame to all frames selected (except current).
 */
void BKE_gpencil_stroke_copy_to_keyframes(struct bGPdata *gpd,
                                          struct bGPDlayer *gpl,
                                          struct bGPDframe *gpf,
                                          struct bGPDstroke *gps,
                                          const bool tail);

/**
 * Convert a mesh object to grease pencil stroke.
 *
 * \param bmain: Main thread pointer.
 * \param depsgraph: Original depsgraph.
 * \param scene: Original scene.
 * \param ob_gp: Grease pencil object to add strokes.
 * \param ob_mesh: Mesh to convert.
 * \param angle: Limit angle to consider a edge-loop ends.
 * \param thickness: Thickness of the strokes.
 * \param offset: Offset along the normals.
 * \param matrix: Transformation matrix.
 * \param frame_offset: Destination frame number offset.
 * \param use_seams: Only export seam edges.
 * \param use_faces: Export faces as filled strokes.
 */
bool BKE_gpencil_convert_mesh(struct Main *bmain,
                              struct Depsgraph *depsgraph,
                              struct Scene *scene,
                              struct Object *ob_gp,
                              struct Object *ob_mesh,
                              const float angle,
                              const int thickness,
                              const float offset,
                              const float matrix[4][4],
                              const int frame_offset,
                              const bool use_seams,
                              const bool use_faces,
                              const bool use_vgroups);

/**
 * Subdivide the grease pencil stroke so the number of points is target_number.
 * Does not change the shape of the stroke. The new points will be distributed as
 * uniformly as possible by repeatedly subdividing the current longest edge.
 *
 * \param gps: The stroke to be up-sampled.
 * \param target_number: The number of points the up-sampled stroke should have.
 * \param select: Select/Deselect the stroke.
 */
void BKE_gpencil_stroke_uniform_subdivide(struct bGPdata *gpd,
                                          struct bGPDstroke *gps,
                                          const uint32_t target_number,
                                          const bool select);

/**
 * Stroke to view space
 * Transforms a stroke to view space.
 * This allows for manipulations in 2D but also easy conversion back to 3D.
 * \note also takes care of parent space transform.
 */
void BKE_gpencil_stroke_to_view_space(struct RegionView3D *rv3d,
                                      struct bGPDstroke *gps,
                                      const float diff_mat[4][4]);
/**
 * Stroke from view space
 * Transforms a stroke from view space back to world space.
 * Inverse of #BKE_gpencil_stroke_to_view_space
 * \note also takes care of parent space transform.
 */
void BKE_gpencil_stroke_from_view_space(struct RegionView3D *rv3d,
                                        struct bGPDstroke *gps,
                                        const float diff_mat[4][4]);
/**
 * Calculates the perimeter of a stroke projected from the view and returns it as a new stroke.
 * \param subdivisions: Number of subdivisions for the start and end caps.
 * \return: bGPDstroke pointer to stroke perimeter.
 */
struct bGPDstroke *BKE_gpencil_stroke_perimeter_from_view(struct RegionView3D *rv3d,
                                                          struct bGPdata *gpd,
                                                          const struct bGPDlayer *gpl,
                                                          struct bGPDstroke *gps,
                                                          const int subdivisions,
                                                          const float diff_mat[4][4]);
/**
 * Get average pressure.
 */
float BKE_gpencil_stroke_average_pressure_get(struct bGPDstroke *gps);
/**
 * Check if the thickness of the stroke is constant.
 */
bool BKE_gpencil_stroke_is_pressure_constant(struct bGPDstroke *gps);
#ifdef __cplusplus
}
#endif
