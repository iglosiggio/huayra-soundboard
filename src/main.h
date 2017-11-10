/* main.h
 *
 * Copyright (C) 2017 Ignacio Losiggio <iglosiggio@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib/gi18n.h>
#include <gst/player/gstplayer.h>
#include "soundboard-config.h"
#include "soundboard-window.h"
#include "category-widget.h"
#include "sample-widget.h"

void soundboard_play_sample(const SampleWidget *sample_widget);
void soundboard_media_info_updated (GstPlayer          *player,
                                    GstPlayerMediaInfo *info,
                                    gpointer            data);
void soundboard_position_updated (GstPlayer *player,
                                 guint64    position,
                                 gpointer   data);

GstPlayer *player = NULL;
SampleWidget *playing_sample = NULL;
