//----------------------------------------
// Tests for gtk lib
//----------------------------------------

#include <gtk/gtk.h>


//-- Module Functions Declarations ---------------
void end_program (GtkWidget *, gpointer);

//-- Module Functions Definitions ---------------
int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);
    GtkWidget *win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *btn = gtk_button_new_with_label ("Close Window");
    g_signal_connect(btn, "clicked", G_CALLBACK(end_program), NULL);
    gtk_container_add (GTK_CONTAINER (win), btn);
    
    gtk_widget_show_all (win);
    gtk_main();
    
    return 0;
}


void end_program (GtkWidget *wid, gpointer ptr)
{
    gtk_main_quit();
}

//--- end of file ---//
