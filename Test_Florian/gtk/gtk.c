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
	GtkWidget *MainWindow = NULL;
	GtkWidget *Label = NULL;
    GtkWidget *Table;                   //GtkWidget *pVBox;
    GtkWidget *Button[4];
	gtk_init(&argc, &argv);

    /*Création de la fenetre*/

	MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(MainWindow), "delete-event", 
			G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_window_set_default_size(GTK_WINDOW(MainWindow), 800, 600);
	gtk_window_set_title(GTK_WINDOW(MainWindow), "IRIS by Eye for an eye");
	gtk_window_set_position(GTK_WINDOW(MainWindow), GTK_WIN_POS_CENTER);
    

    
    /*Création de la box

    pVBox = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(MainWindow), pVBox);
    
    */

    /*Création de la table*/

    Table = gtk_table_new(3, 2, TRUE);
    gtk_container_add(GTK_CONTAINER(MainWindow), GTK_WIDGET(Table));
    /*Création du Bouton*/
    //Bouton 1

    Button[0] = gtk_button_new_with_label("Quitter");
    g_signal_connect(G_OBJECT(Button[0]), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    
    //Bouton 2

    Button[1] = gtk_button_new_with_label("Bonjour");
    g_signal_connect(G_OBJECT(Button[1]), "clicked", G_CALLBACK(gtk_main_quit), NULL);

    //gtk_box_pack_start(GTK_BOX(pVBox), Button, TRUE, FALSE, 0);
    gtk_table_attach(GTK_TABLE(Table), Button[0], 0, 1, 2, 3, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(Table), Button[1], 1, 2, 2, 3, GTK_EXPAND, GTK_EXPAND, 0, 0);
	Label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(Label), "<b><big> Bonjour !</big></b>");
    gtk_table_attach(GTK_TABLE(Table), Label, 0, 2, 0, 2, GTK_EXPAND, GTK_EXPAND, 0, 0);

	/*AddBtn(MainWindow);

	g_signal_connect(G_OBJECT(MainWindow), "delete-event",
					G_CALLBACK(gtk_main_quit), NULL);
    */
	//Ne rien mettre après ça !
	
	gtk_widget_show_all(MainWindow);
	gtk_main();
	
	
	return EXIT_SUCCESS;
}


void add_label() {
        GtkWidget *Label;
        Label = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(Label), "<b><big> Bonjour ! Comment ca va ?</big></b>");
}

void AddBtn(GtkWidget *window) {
	GtkWidget *Button;
	Button  = gtk_button_new_with_label("Quitter");
	g_signal_connect(G_OBJECT(Button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
	gtk_container_add(GTK_CONTAINER(window), Button);
}
