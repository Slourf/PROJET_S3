#include<stdlib.h>
#include<gtk/gtk.h>


void gtk_window_set_title(GtkWindow *window, const gchar *title);
void gtk_window_set_default_size(GtkWindow *window, gint width, gint height);
void gtk_window_set_position(GtkWindow *window, GtkWindowPosition position);


int main(int argc, char **argv)
{
	GtkWidget * MainWindow = NULL;

	gtk_init(&argc, &argv);
	
	MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(MainWindow), "delete-event", 
			G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_window_set_default_size(GTK_WINDOW(MainWindow), 1920, 1080);
	gtk_window_set_title(GTK_WINDOW(MainWindow), "IRIS by Eye for an eye");
	gtk_window_set_position(GTK_WIN_POS_CENTER(MainWindow));
	
	//Ne rien mettre après ça !
	
	gtk_widget_show(MainWindow);
	gtk_main();
	
	
	return EXIT_SUCCESS;
}




