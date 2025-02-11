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
 */

#pragma once

/** \file
 * \ingroup bmesh
 */

#include "BLI_compiler_attrs.h"

/**
 * Parameters used to determine which kinds of data needs to be generated.
 */
typedef struct BMPartialUpdate_Params {
  bool do_normals;
  bool do_tessellate;
} BMPartialUpdate_Params;

/**
 * Cached data to speed up partial updates.
 *
 * Hints:
 *
 * - Avoid creating this data for single updates,
 *   it should be created and reused across multiple updates to gain a significant benefit
 *   (while transforming geometry for example).
 *
 * - Partial normal updates use face & loop indices,
 *   setting them to dirty values between updates will slow down normal recalculation.
 */
typedef struct BMPartialUpdate {
  BMVert **verts;
  BMFace **faces;
  int verts_len, verts_len_alloc;
  int faces_len, faces_len_alloc;

  /** Store the parameters used in creation so invalid use can be asserted. */
  BMPartialUpdate_Params params;
} BMPartialUpdate;

/**
 * All Tagged & Connected, see: #BM_mesh_partial_create_from_verts
 * Operate on everything that's tagged as well as connected geometry.
 */
BMPartialUpdate *BM_mesh_partial_create_from_verts(BMesh *bm,
                                                   const BMPartialUpdate_Params *params,
                                                   const unsigned int *verts_mask,
                                                   const int verts_mask_count)
    ATTR_NONNULL(1, 2, 3) ATTR_WARN_UNUSED_RESULT;

/**
 * All Connected, operate on all faces that have both tagged and un-tagged vertices.
 *
 * Reduces computations when transforming isolated regions.
 */
BMPartialUpdate *BM_mesh_partial_create_from_verts_group_single(
    BMesh *bm,
    const BMPartialUpdate_Params *params,
    const unsigned int *verts_mask,
    const int verts_mask_count) ATTR_NONNULL(1, 2, 3) ATTR_WARN_UNUSED_RESULT;

/**
 * All Connected, operate on all faces that have vertices in the same group.
 *
 * Reduces computations when transforming isolated regions.
 *
 * This is a version of #BM_mesh_partial_create_from_verts_group_single
 * that handles multiple groups instead of a bitmap mask.
 *
 * This is needed for example when transform has mirror enabled,
 * since one side needs to have a different group to the other since a face that has vertices
 * attached to both won't have an affine transformation.
 *
 * \param verts_groups: Vertex aligned array of groups.
 * Values are used as follows:
 * - >0: Each face is grouped with other faces of the same group.
 * -  0: Not in a group (don't handle these).
 * - -1: Don't use grouping logic (include any face that contains a vertex with this group).
 * \param verts_groups_count: The number of non-zero values in `verts_groups`.
 */
BMPartialUpdate *BM_mesh_partial_create_from_verts_group_multi(
    BMesh *bm,
    const BMPartialUpdate_Params *params,
    const int *verts_group,
    const int verts_group_count) ATTR_NONNULL(1, 2, 3) ATTR_WARN_UNUSED_RESULT;

void BM_mesh_partial_destroy(BMPartialUpdate *bmpinfo) ATTR_NONNULL(1);
