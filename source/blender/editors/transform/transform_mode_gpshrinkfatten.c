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
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 */

/** \file
 * \ingroup edtransform
 */

#include <stdlib.h>

#include "BLI_math.h"
#include "BLI_string.h"

#include "BKE_context.h"
#include "BKE_unit.h"

#include "DNA_gpencil_types.h"

#include "ED_screen.h"

#include "UI_interface.h"

#include "BLT_translation.h"

#include "transform.h"
#include "transform_convert.h"
#include "transform_snap.h"

#include "transform_mode.h"

/* -------------------------------------------------------------------- */
/** \name Transform (GPencil Strokes Shrink/Fatten)
 * \{ */

static void applyGPShrinkFatten(TransInfo *t, const int UNUSED(mval[2]))
{
  float ratio;
  int i;
  char str[UI_MAX_DRAW_STR];

  ratio = t->values[0] + t->values_modal_offset[0];

  transform_snap_increment(t, &ratio);

  applyNumInput(&t->num, &ratio);

  t->values_final[0] = ratio;

  /* header print for NumInput */
  if (hasNumInput(&t->num)) {
    char c[NUM_STR_REP_LEN];

    outputNumInput(&(t->num), c, &t->scene->unit);
    BLI_snprintf(str, sizeof(str), TIP_("Shrink/Fatten: %s"), c);
  }
  else {
    BLI_snprintf(str, sizeof(str), TIP_("Shrink/Fatten: %3f"), ratio);
  }

  bool recalc = false;
  FOREACH_TRANS_DATA_CONTAINER (t, tc) {
    TransData *td = tc->data;
    bGPdata *gpd = td->ob->data;
    const bool is_curve_edit = (bool)GPENCIL_CURVE_EDIT_SESSIONS_ON(gpd);
    /* Only recalculate data when in curve edit mode. */
    if (is_curve_edit) {
      recalc = true;
    }

    for (i = 0; i < tc->data_len; i++, td++) {
      if (td->flag & TD_SKIP) {
        continue;
      }

      if (td->val) {
        *td->val = td->ival * ratio;
        /* apply PET */
        *td->val = (*td->val * td->factor) + ((1.0f - td->factor) * td->ival);
        if (*td->val <= 0.0f) {
          *td->val = 0.001f;
        }
      }
    }
  }

  if (recalc) {
    recalcData(t);
  }

  ED_area_status_text(t->area, str);
}

void initGPShrinkFatten(TransInfo *t)
{
  t->mode = TFM_GPENCIL_SHRINKFATTEN;
  t->transform = applyGPShrinkFatten;

  initMouseInputMode(t, &t->mouse, INPUT_SPRING);

  t->idx_max = 0;
  t->num.idx_max = 0;
  t->snap[0] = 0.1f;
  t->snap[1] = t->snap[0] * 0.1f;

  copy_v3_fl(t->num.val_inc, t->snap[0]);
  t->num.unit_sys = t->scene->unit.system;
  t->num.unit_type[0] = B_UNIT_NONE;

#ifdef USE_NUM_NO_ZERO
  t->num.val_flag[0] |= NUM_NO_ZERO;
#endif

  t->flag |= T_NO_CONSTRAINT;
}
/** \} */
