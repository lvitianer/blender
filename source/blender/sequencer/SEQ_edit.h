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
 * The Original Code is Copyright (C) 2004 Blender Foundation.
 * All rights reserved.
 */

#pragma once

/** \file
 * \ingroup sequencer
 */

#ifdef __cplusplus
extern "C" {
#endif

struct ListBase;
struct Main;
struct Scene;
struct Sequence;

int SEQ_edit_sequence_swap(struct Sequence *seq_a, struct Sequence *seq_b, const char **error_str);
/**
 * Move sequence to seqbase.
 *
 * \param scene: Scene containing the editing
 * \param dst_seqbase: seqbase where `seq` is located
 * \param seq: Sequence to move
 * \param dst_seqbase: Target seqbase
 */
bool SEQ_edit_move_strip_to_seqbase(struct Scene *scene,
                                    ListBase *seqbase,
                                    struct Sequence *src_seq,
                                    ListBase *dst_seqbase);
/**
 * Move sequence to meta sequence.
 *
 * \param scene: Scene containing the editing
 * \param src_seq: Sequence to move
 * \param dst_seqm: Target Meta sequence
 * \param error_str: Error message
 */
bool SEQ_edit_move_strip_to_meta(struct Scene *scene,
                                 struct Sequence *src_seq,
                                 struct Sequence *dst_seqm,
                                 const char **error_str);
bool SEQ_meta_separate(struct Scene *scene, struct Sequence *src_meta, const char **error_str);
/**
 * Flag seq and its users (effects) for removal.
 */
void SEQ_edit_flag_for_removal(struct Scene *scene,
                               struct ListBase *seqbase,
                               struct Sequence *seq);
/**
 * Remove all flagged sequences, return true if sequence is removed.
 */
void SEQ_edit_remove_flagged_sequences(struct Scene *scene, struct ListBase *seqbase);
void SEQ_edit_update_muting(struct Editing *ed);

typedef enum eSeqSplitMethod {
  SEQ_SPLIT_SOFT,
  SEQ_SPLIT_HARD,
} eSeqSplitMethod;

/**
 * Split Sequence at timeline_frame in two.
 *
 * \param bmain: Main in which Sequence is located
 * \param scene: Scene in which Sequence is located
 * \param seqbase: ListBase in which Sequence is located
 * \param seq: Sequence to be split
 * \param timeline_frame: frame at which seq is split.
 * \param method: affects type of offset to be applied to resize Sequence
 * \return The newly created sequence strip. This is always Sequence on right side.
 */
struct Sequence *SEQ_edit_strip_split(struct Main *bmain,
                                      struct Scene *scene,
                                      struct ListBase *seqbase,
                                      struct Sequence *seq,
                                      const int timeline_frame,
                                      const eSeqSplitMethod method,
                                      const char **r_error);
/**
 * Find gap after initial_frame and move strips on right side to close the gap
 *
 * \param scene: Scene in which strips are located
 * \param seqbase: ListBase in which strips are located
 * \param initial_frame: frame on timeline from where gaps are searched for
 * \param remove_all_gaps: remove all gaps instead of one gap
 * \return true if gap is removed, otherwise false
 */
bool SEQ_edit_remove_gaps(struct Scene *scene,
                          struct ListBase *seqbase,
                          const int initial_frame,
                          const bool remove_all_gaps);
void SEQ_edit_sequence_name_set(struct Scene *scene, struct Sequence *seq, const char *new_name);

#ifdef __cplusplus
}
#endif
