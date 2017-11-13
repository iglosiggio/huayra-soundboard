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
	glong audio_length; /* mseconds */
	glong current_pos;  /* mseconds */

	/* Template widgets */
	GtkProgressBar      *progress;
	GtkLabel            *progress_label;
};

enum {
	SIGNAL_PLAY = 1,
	LAST_SIGNAL
};

static void sample_update_progress(SampleWidget *self);

static gint signals[LAST_SIGNAL] = {0};

G_DEFINE_TYPE (SampleWidget, sample_widget, GTK_TYPE_BOX)

static void
sample_widget_class_init (SampleWidgetClass *klass) {
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	gtk_widget_class_set_template_from_resource (widget_class, "/org/huayra/Soundboard/sample-widget.ui");
	gtk_widget_class_bind_template_child (widget_class, SampleWidget, progress);
	gtk_widget_class_bind_template_child (widget_class, SampleWidget, progress_label);
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

glong sample_get_duration (const SampleWidget *self) {
	return self->audio_length;
}

void sample_set_duration (SampleWidget *self,
			  glong         value) {
	self->audio_length = value;
	sample_update_progress (self);
}

glong sample_get_current_pos (const SampleWidget *self) {
	return self->current_pos;
}

void sample_set_current_pos (SampleWidget *self,
			     glong         value) {
	self->current_pos = value;
	sample_update_progress (self);
}

static void sample_update_progress (SampleWidget *self) {
	glong audio_length_minutes = self->audio_length / 1000 / 60,
	      audio_length_seconds = self->audio_length / 1000 % 60,
	      current_pos_minutes  = self->current_pos  / 1000 / 60,
	      current_pos_seconds  = self->current_pos  / 1000 % 60;
	gdouble progress = ((gdouble) self->current_pos) / ((gdouble) self->audio_length);

	gchar  *string_value = g_strdup_printf ("%.2lu:%.2lu/%.2lu:%.2lu",
						current_pos_minutes,
						current_pos_seconds,
						audio_length_minutes,
						audio_length_seconds);

	gtk_label_set_label (self->progress_label, string_value);

	gtk_progress_bar_set_fraction (self->progress, progress);

	g_free(string_value);
}

const gchar* sample_get_sample (const SampleWidget *self) {
	return self->sample;
}

void sample_play (GtkWidget *widget, gpointer data) {
	SampleWidget *sample_widget = SAMPLE_WIDGET (data);
	g_signal_emit (sample_widget, signals[SIGNAL_PLAY], 0);
}

SampleWidget* sample_widget_new (const gchar *sample,
				 glong        audio_length) {
	SampleWidget *self = g_object_new (SAMPLE_TYPE_WIDGET, NULL);

	self->sample = g_strdup(sample);

	sample_set_duration (self, audio_length);
	sample_set_current_pos (self, 0);

	return self;
}

static void
sample_widget_init (SampleWidget *self) {
	gtk_widget_init_template (GTK_WIDGET (self));
}
