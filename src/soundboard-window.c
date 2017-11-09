/* soundboard-window.c
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

#include "soundboard-config.h"
#include "soundboard-window.h"

struct _SoundboardWindow
{
	GtkApplicationWindow  parent_instance;

	/* Template widgets */
	GtkHeaderBar        *header_bar;
	GtkBox              *content;
};

G_DEFINE_TYPE (SoundboardWindow, soundboard_window, GTK_TYPE_APPLICATION_WINDOW)

static void
soundboard_window_class_init (SoundboardWindowClass *klass) {
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	gtk_widget_class_set_template_from_resource (widget_class, "/org/huayra/Soundboard/soundboard-window.ui");
	gtk_widget_class_bind_template_child (widget_class, SoundboardWindow, header_bar);
	gtk_widget_class_bind_template_child (widget_class, SoundboardWindow, content);
}

GtkBox* soundboard_get_content(SoundboardWindow *board) {
	return board->content;
}

static void
soundboard_window_init (SoundboardWindow *self) {
	gtk_widget_init_template (GTK_WIDGET (self));
}
