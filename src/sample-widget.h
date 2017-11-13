/* sample-widget.h
 *
 * Copyright (C) 2017 Ignacio Losiggio
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

#pragma once

#include <glib/gi18n.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SAMPLE_TYPE_WIDGET (sample_widget_get_type())
G_DECLARE_FINAL_TYPE (SampleWidget, sample_widget, SAMPLE, WIDGET, GtkBox)

glong sample_get_duration (const SampleWidget *self);
void  sample_set_duration (SampleWidget *self, glong duration);

glong sample_get_current_pos (const SampleWidget *self);
void  sample_set_current_pos (SampleWidget *self, glong current_pos);

const gchar* sample_get_sample (const SampleWidget *self);

void sample_play (GtkWidget *widget, gpointer data);

SampleWidget* sample_widget_new (const gchar *sample, glong audio_length);
G_END_DECLS
