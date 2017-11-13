/* category-window.h
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

#include <gtk/gtk.h>
#include <glib.h>
#include "sample-widget.h"

G_BEGIN_DECLS

typedef struct {
	gchar *name;
	gchar *uri;
	glong audio_length;
} Sample;

#define CATEGORY_TYPE_WIDGET (category_widget_get_type())

G_DECLARE_FINAL_TYPE (CategoryWidget, category_widget, CATEGORY, WIDGET, GtkBox)
GtkLabel* category_get_titulo(CategoryWidget *category);

void category_append_sample(CategoryWidget *self, SampleWidget *sample);

void category_widget_agregar_sample(GtkWidget *widget, gpointer data);

CategoryWidget* category_widget_new(const gchar *name, Sample *samples, GCallback onplay);
G_END_DECLS
