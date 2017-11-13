/* category-widget.c
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

#include "category-widget.h"

struct _CategoryWidget
{
	GtkBox parent_instance;

	/* Template widgets */
	GtkLabel            *titulo;
	GtkListBox          *contenido;
};

G_DEFINE_TYPE (CategoryWidget, category_widget, GTK_TYPE_BOX)

static void
category_widget_class_init (CategoryWidgetClass *klass) {
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/huayra/Soundboard/category-widget.ui");
  gtk_widget_class_bind_template_child (widget_class, CategoryWidget, titulo);
  gtk_widget_class_bind_template_child (widget_class, CategoryWidget, contenido);
  gtk_widget_class_bind_template_callback (widget_class, category_widget_agregar_sample);
}

GtkLabel* category_get_titulo(CategoryWidget *self) {
	return self->titulo;
}

void category_widget_agregar_sample(GtkWidget *widget, gpointer data) {
	g_critical ("NO IMPLEMENTADO: category_widget_agregar_sample");
}

void category_append_sample(CategoryWidget *self, SampleWidget *sample) {
	gtk_list_box_insert (self->contenido, GTK_WIDGET(sample), -1);
}

CategoryWidget* category_widget_new(const gchar *name, Sample *samples, GCallback onplay) {
	CategoryWidget *self = g_object_new (CATEGORY_TYPE_WIDGET, NULL);

	gtk_label_set_text (self->titulo, name);

	for (Sample *s = samples; s->name != NULL; s++) {
		SampleWidget *sample_widget = sample_widget_new (s->uri, s->name, s->audio_length);
		category_append_sample (self, sample_widget);
		g_signal_connect (sample_widget, "play", onplay, NULL);
	}

	return self;
}

static void
category_widget_init (CategoryWidget *self) {
	gtk_widget_init_template (GTK_WIDGET (self));
}
