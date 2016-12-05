# include <stdlib.h>
# include <gtk/gtk.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "main.h"
# include "IntToBin_Net.h"
#define UTF8(string) g_locale_to_utf8(string, -1, NULL, NULL, NULL)


void gtk_window_set_title(GtkWindow *window, const gchar *title);
void gtk_window_set_default_size(GtkWindow *window, gint width, gint height);
void gtk_window_set_position(GtkWindow *window, GtkWindowPosition position);

void gtk_container_add(GtkContainer *container, GtkWidget *widget);
void gtk_label_set_markup (GtkLabel *label, const gchar *str);

void AddBtn(GtkWidget *window);

struct text_gtk {
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	GtkTextMark *mark;
	GtkWidget *box;
	GtkWidget *entry;
};


void display_text_gtk(GtkButton *button, gpointer data) {
	if (button) {
		struct text_gtk *d = data;

		const char* path = malloc(256);
		path = gtk_entry_get_text(GTK_ENTRY(d->entry));

		if (*path != 0) {
			FILE *file = NULL;
			file = fopen("IRIS_text.txt", "w");
			SDL_Surface *img = load_image((char*)path);
			struct text *mat_img = cut(img);

			char *text = single_forward(mat_img);
			printf("%s\n", text);
			gtk_text_buffer_get_iter_at_mark(d->buffer, &(d->iter), d->mark);
			gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (d->box), GTK_WRAP_WORD);
			gtk_text_buffer_insert(d->buffer, &(d->iter), text, -1);
			if (file) {
				fputs(text, file);
				fclose(file);
			}
			gtk_text_buffer_insert(d->buffer, &(d->iter),
					"***\nFinished\n***\nYour text has been saved in the file
					'IRIS_text.txt'\n***\n", -1);
			free(text);
		}
		else {
			gtk_text_buffer_get_iter_at_mark(d->buffer, &(d->iter), d->mark);
			gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (d->box),
					GTK_WRAP_WORD);
			gtk_text_buffer_insert(d->buffer, &(d->iter),
					"ERROR: please enter a correct path\n", -1);
		}
	}
}

void load_image_gtk(GtkButton *button, gpointer data) {
	if (button) {
		GtkWidget *ImageWindow;
		GtkWidget *Image;

		struct text_gtk *d = data;

		ImageWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

		const char* path = malloc(256);
		path = gtk_entry_get_text(GTK_ENTRY(d->entry));
		Image = gtk_image_new_from_file(path);
		if (*path != 0) {
			ImageWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
			gtk_container_add(GTK_CONTAINER(ImageWindow), GTK_WIDGET(Image));
			gtk_window_set_title(GTK_WINDOW(ImageWindow), path);

			gtk_widget_show_all(ImageWindow);
		}
		else {
			gtk_text_buffer_get_iter_at_mark(d->buffer, &(d->iter), d->mark);
			gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (d->box), GTK_WRAP_WORD);
			gtk_text_buffer_insert(d->buffer, &(d->iter),
					"ERROR: please enter a correct path\n", -1);
		}
	}
}

int main(int argc, char **argv)
{
	init_sdl();
	GtkWidget *MainWindow = NULL;
	GtkWidget *Label = NULL;
	GtkWidget *Table;
	GtkWidget *Button[4];
	GtkWidget *entry;

	gtk_init(&argc, &argv);

	//Création de la fenetre/

	MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(MainWindow), "delete-event",
			G_CALLBACK(gtk_main_quit), NULL);

	gtk_window_set_default_size(GTK_WINDOW(MainWindow), 800, 600);
	gtk_window_set_title(GTK_WINDOW(MainWindow), "IRIS by Eye for an eye");
	gtk_window_set_position(GTK_WINDOW(MainWindow), GTK_WIN_POS_CENTER);

	//Création de la table/

	Table = gtk_table_new(10, 5, TRUE);
	gtk_container_add(GTK_CONTAINER(MainWindow), GTK_WIDGET(Table));

	//creat a second text box

	struct text_gtk *data = malloc(sizeof (struct text_gtk));

	data->buffer = gtk_text_buffer_new (NULL);
	data->box = gtk_text_view_new_with_buffer (data->buffer);
	data->mark = gtk_text_buffer_get_insert (data->buffer);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(data->box), FALSE);
	gtk_text_buffer_get_iter_at_mark (data->buffer, &(data->iter), data->mark);
	gtk_table_attach_defaults(GTK_TABLE(Table), data->box, 0, 5, 0, 8);

	//create a text box
	entry = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY (entry),0);
	gtk_table_attach_defaults(GTK_TABLE(Table), entry, 1, 5, 8, 9);
	data->entry = entry;
	//Création du Bouton
	//Bouton 1

	Button[0] = gtk_button_new_with_label("Quit");
	g_signal_connect(G_OBJECT(Button[0]), "clicked", G_CALLBACK(gtk_main_quit),
	 NULL);

	//Bouton 2

	Button[1] = gtk_button_new_with_label("Run");
	g_signal_connect(G_OBJECT(Button[1]), "clicked",
	G_CALLBACK(display_text_gtk), (gpointer)data);

	//Bouton 3

	Button[2] = gtk_button_new_with_label("Show");
	g_signal_connect(G_OBJECT(Button[2]), "clicked", G_CALLBACK(load_image_gtk),
	(gpointer)data);

	//gtk_box_pack_start(GTK_BOX(pVBox), Button, TRUE, FALSE, 0);
	gtk_table_attach(GTK_TABLE(Table), Button[0], 0, 1, 9, 10, GTK_FILL,
	GTK_EXPAND, 0, 0);
	gtk_table_attach(GTK_TABLE(Table), Button[1], 4, 5, 9, 10, GTK_FILL,
	GTK_EXPAND, 0, 0);
	gtk_table_attach(GTK_TABLE(Table), Button[2], 3, 4, 9, 10, GTK_FILL,
	GTK_EXPAND, 0, 0);
	Label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(Label), "<b><big> Image's path :</big></b>");
	gtk_table_attach(GTK_TABLE(Table), Label, 0, 1, 8, 9, GTK_EXPAND,
	GTK_EXPAND, 0, 0);

	//create a text box
	entry = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY (entry),0);
	gtk_table_attach_defaults(GTK_TABLE(Table), entry, 1, 5, 8, 9);

	//Ne rien mettre après ça !

	gtk_widget_show_all(MainWindow);
	gtk_main();

	return EXIT_SUCCESS;
}

