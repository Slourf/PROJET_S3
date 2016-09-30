#include<stdlib.h>  
#include<gtk/gtk.h> 


int main(int argc, char **argv)
{
	GtkWidget* MainWindow= gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget* Picture=gtk_image_new_from_file("chemin_vers_l'image");
	
	gtk_image_set_from_file(Picture,"chemin vers une nouvelle image")"Cette fonction remplace l'image par une nouvelle image"
	gtk_widget_show_all(pWindow);
	return EXIT_SUCCESS;
}
