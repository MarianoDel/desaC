//----------------------------------------
// Tests for gtk lib
//----------------------------------------

#include <gtk/gtk.h>


//-- Module Functions Declarations ---------------
void end_program (GtkWidget *, gpointer);
static void clear_surface (void);
static void fill_surface (void);
static gboolean configure_surface (GtkWidget         *widget);

//-- Module Functions Definitions ---------------
/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;

int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "lcd_layout.glade", NULL);
    
    GtkWidget *win = (GtkWidget *) gtk_builder_get_object (builder, "window1");
    GtkWidget *btn1 = (GtkWidget *) gtk_builder_get_object (builder, "button1");
    GtkWidget *btn2 = (GtkWidget *) gtk_builder_get_object (builder, "button2");
    GtkWidget *btn3 = (GtkWidget *) gtk_builder_get_object (builder, "button3");    
    GtkWidget *imag = (GtkWidget *) gtk_builder_get_object (builder, "image1");

    g_signal_connect(btn1, "clicked", G_CALLBACK(clear_surface), NULL);    
    g_signal_connect(btn2, "clicked", G_CALLBACK(fill_surface), NULL);
    g_signal_connect(btn3, "clicked", G_CALLBACK(end_program), NULL);    
    
    gtk_widget_show_all (win);
    configure_surface(imag);
    gtk_main();
    
    return 0;
}


void end_program (GtkWidget *wid, gpointer ptr)
{
    if (surface)
        cairo_surface_destroy (surface);
    
    gtk_main_quit();
}


static void clear_surface (void)
{
    cairo_t *cr;

    cr = cairo_create (surface);

    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_paint (cr);

    cairo_destroy (cr);
}


static void fill_surface (void)
{
    cairo_t *cr;

    cr = cairo_create (surface);

    cairo_set_source_rgb (cr, 125, 0, 0);
    cairo_paint (cr);

    cairo_destroy (cr);
}


static gboolean configure_surface (GtkWidget         *widget)
{
    if (surface)
        cairo_surface_destroy (surface);

    surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                                 CAIRO_CONTENT_COLOR,
                                                 gtk_widget_get_allocated_width (widget),
                                                 gtk_widget_get_allocated_height (widget));

    /* Initialize the surface to white */
    clear_surface ();

    /* We've handled the configure event, no need for further processing. */
    return TRUE;
}


//--- end of file ---//
