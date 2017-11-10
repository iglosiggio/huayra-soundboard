/* sample-widget.c
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

#include "sample-widget.h"

struct _SampleWidget {
	GtkBox parent_instance;

	gchar *sample;
	glong audio_length;

	/* Template widgets */
	GtkProgressBar      *progress;
	GtkLabel            *duration;
};

enum {
	SIGNAL_PLAY = 1,
	LAST_SIGNAL
};

static gint signals[LAST_SIGNAL] = {0};

G_DEFINE_TYPE (SampleWidget, sample_widget, GTK_TYPE_BOX)

static void
sample_widget_class_init (SampleWidgetClass *klass) {
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	gtk_widget_class_set_template_from_resource (widget_class, "/org/huayra/Soundboard/sample-widget.ui");
	gtk_widget_class_bind_template_child (widget_class, SampleWidget, progress);
	gtk_widget_class_bind_template_child (widget_class, SampleWidget, duration);
	gtk_widget_class_bind_template_callback (widget_class, sample_play);

	signals [SIGNAL_PLAY] =
		g_signal_new ("play",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_LAST,
			      0,
			      NULL,
			      NULL,
			      g_cclosure_marshal_generic,
			      G_TYPE_NONE,
			      0);
}

gdouble sample_get_progress (const SampleWidget *self) {
	return gtk_progress_bar_get_fraction (self->progress);
}

void sample_set_progress (SampleWidget *self, gdouble value) {
	gtk_progress_bar_set_fraction (self->progress, value);
}

const gchar* sample_get_duration (const SampleWidget *self) {
	return gtk_label_get_label (self->duration);
}

void sample_set_duration (SampleWidget *self, const gchar *value) {
	gtk_label_set_label (self->duration, value);
}

const gchar* sample_get_sample (const SampleWidget *self) {
	return self->sample;
}

void sample_play (GtkWidget *widget, gpointer data) {
	SampleWidget *sample_widget = SAMPLE_WIDGET (data);
	g_signal_emit (sample_widget, signals[SIGNAL_PLAY], 0);
}

SampleWidget* sample_widget_new (const gchar *sample, glong audio_length) {
	SampleWidget* self = g_object_new (SAMPLE_TYPE_WIDGET, NULL);

	self->sample = g_strdup(sample);
	self->audio_length = audio_length;

	return self;
}

static void
sample_widget_init (SampleWidget *self) {
	gtk_widget_init_template (GTK_WIDGET (self));
}
