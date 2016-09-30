#include<stdlib.h>
#include<gtk/gtk.h>
#define UTF8(string) g_locale_to_utf8(string, -1, NULL, NULL, NULL)

void gtk_window_set_title(GtkWindow *window, const gchar *title);
void gtk_window_set_default_size(GtkWindow *window, gint width, gint height);
void gtk_window_set_position(GtkWindow *window, GtkWindowPosition position);

void gtk_container_add(GtkContainer *container, GtkWidget *widget);
void gtk_label_set_markup (GtkLabel *label, const gchar *str);

void AddBtn(GtkWidget *window);

int main(int argc, char **argv)
{
	GtkWidget* MainWindow = NULL;
	GtkWidget* Label = NULL;

	gtk_init(&argc, &argv);


	MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(MainWindow), "delete-event", 
			G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_window_set_default_size(GTK_WINDOW(MainWindow), 800, 600);
	gtk_window_set_title(GTK_WINDOW(MainWindow), "IRIS by Eye for an eye");
	gtk_window_set_position(GTK_WINDOW(MainWindow), GTK_WIN_POS_CENTER);
	
	Label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(Label), "<b><big> Ca a pas l'air mais\n ça a été grave casse couille de faire juste ça !</big></b>");

	AddBtn(MainWindow);

	gtk_container_add(GTK_CONTAINER(MainWindow), Label);
	

	g_signal_connect(G_OBJECT(MainWindow), "delete-event",
					G_CALLBACK(gtk_main_quit), NULL);

	//Ne rien mettre après ça !
	
	gtk_widget_show_all(MainWindow);
	gtk_main();
	
	
	return EXIT_SUCCESS;
}


void AddBtn(GtkWidget *window) {
	GtkWidget *Button;
	Button  = gtk_button_new_with_label("Quitter");
	g_signal_connect(G_OBJECT(Button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
	gtk_container_add(GTK_CONTAINER(window), Button);
}
