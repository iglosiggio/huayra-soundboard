/* main.c
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

#include "main.h"

static void
on_activate (GtkApplication *app)
{
	GtkWindow *window;

	/* It's good practice to check your parameters at the beginning of the
	 * function. It helps catch errors early and in development instead of
	 * by your users.
	 */
	g_assert (GTK_IS_APPLICATION (app));

	/* Get the current window or create one if necessary. */
	window = gtk_application_get_active_window (app);
	if (window == NULL)
		window = g_object_new (SOUNDBOARD_TYPE_WINDOW,
		                       "application", app,
		                       "default-width", 600,
		                       "default-height", 300,
		                       NULL);

	GtkBox *content = soundboard_get_content (SOUNDBOARD_WINDOW (window));
	//GtkWidget *label = gtk_label_new("TEST");
	//gtk_box_pack_start (content, label, TRUE, FALSE, 0);
	//gtk_widget_show(GTK_WIDGET(label));

	CategoryWidget *category = category_widget_new("Sonidos de prueba");
	SampleWidget *sample_urss = sample_widget_new ("https://upload.wikimedia.org/wikipedia/commons/d/db/Gimn_Sovetskogo_Soyuza_%281977_Vocal%29.oga",
						       "Himno de la URSS",
						       207000);
	SampleWidget *sample_tango = sample_widget_new ("https://upload.wikimedia.org/wikipedia/commons/7/7f/El_d%C3%ADa_que_me_quieras.ogg",
							"El dia que me quieras",
						        208000);
	SampleWidget *sample_meow = sample_widget_new ("https://upload.wikimedia.org/wikipedia/commons/5/53/Felis_silvestris_catus_meows.ogg",
						       "Gatito maullando",
						       10000);
	g_signal_connect (sample_urss,  "play", G_CALLBACK (soundboard_play_sample), NULL);
	g_signal_connect (sample_tango, "play", G_CALLBACK (soundboard_play_sample), NULL);
	g_signal_connect (sample_meow,  "play", G_CALLBACK (soundboard_play_sample), NULL);
	category_append_sample (category, sample_urss);
	category_append_sample (category, sample_tango);
	category_append_sample (category, sample_meow);
	gtk_box_pack_start (content, GTK_WIDGET(category), TRUE, FALSE, 0);
	gtk_widget_show_all(GTK_WIDGET (category));

	/* Load custom styles */
	GtkCssProvider *styles = gtk_css_provider_new ();
	gtk_css_provider_load_from_resource (styles, "/org/huayra/Soundboard/styles.css");
	gtk_style_context_add_provider_for_screen (gtk_widget_get_screen (GTK_WIDGET (window)),
						   GTK_STYLE_PROVIDER (styles),
						   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	/* Ask the window manager/compositor to present the window. */
	gtk_window_present (window);
}

int
main (int   argc,
      char *argv[])
{
	g_autoptr(GtkApplication) app = NULL;
	int ret;

	/* Set up gettext translations */
	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	/*
	 * Create a new GtkApplication. The application manages our main loop,
	 * application windows, integration with the window manager/compositor, and
	 * desktop features such as file opening and single-instance applications.
	 */
	app = gtk_application_new ("org.huayra.Soundboard", G_APPLICATION_FLAGS_NONE);

	/*
	 * We connect to the activate signal to create a window when the application
	 * has been lauched. Additionally, this signal notifies us when the user
	 * tries to launch a "second instance" of the application. When they try
	 * to do that, we'll just present any existing window.
	 *
	 * Because we can't pass a pointer to any function type, we have to cast
	 * our "on_activate" function to a GCallback.
	 */
	g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);

	player = gst_player_new (NULL, gst_player_g_main_context_signal_dispatcher_new (NULL));
	g_signal_connect (player, "media-info-updated", G_CALLBACK (soundboard_media_info_updated), NULL);
	g_signal_connect (player, "position-updated",   G_CALLBACK (soundboard_position_updated),   NULL);
	g_signal_connect (player, "state-changed",      G_CALLBACK (soundboard_state_changed),      NULL);

	/*
	 * Run the application. This function will block until the applicaiton
	 * exits. Upon return, we have our exit code to return to the shell. (This
	 * is the code you see when you do `echo $?` after running a command in a
	 * terminal.
	 *
	 * Since GtkApplication inherits from GApplication, we use the parent class
	 * method "run". But we need to cast, which is what the "G_APPLICATION()"
	 * macro does.
	 */
	ret = g_application_run (G_APPLICATION (app), argc, argv);

	return ret;
}

void soundboard_play_sample (SampleWidget *sample_widget) {
	const gchar* sample = sample_get_sample(sample_widget);

	gst_player_stop (player);

	/* TODO: this shouldn't be neccesary, we need to wait the for the stop to complete */
	if (playing_sample != NULL) {
		soundboard_position_updated (player, 0, NULL);
	}

	playing_sample = sample_widget;

	g_message ("PLAYING: %s", sample);

	gst_player_set_uri (player, sample);
	gst_player_play (player);
}

void soundboard_media_info_updated (GstPlayer          *player,
				    GstPlayerMediaInfo *info,
				    gpointer            data) {
	GstClockTime duration = gst_player_media_info_get_duration (info);
	glong seconds = GST_TIME_AS_SECONDS (duration);
	g_message("DURATION: %lumin %lusec", seconds / 60, seconds % 60);
}

void soundboard_position_updated (GstPlayer *player,
				  guint64    position,
				  gpointer   data) {
	if (playing_sample == NULL) return;
	sample_set_current_pos (playing_sample, GST_TIME_AS_MSECONDS (position));
}

void soundboard_state_changed (GstPlayer      *player,
			       GstPlayerState  state,
			       gpointer        data) {
	if (state == GST_PLAYER_STATE_STOPPED
	    && playing_sample != NULL) {
		soundboard_position_updated (player, 0, NULL);
		/* TODO: playing_sample must be equal to 0 after this */
	}
}
