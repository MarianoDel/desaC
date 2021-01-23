//----------------------------------------
// Tests for gtk lib
//----------------------------------------

#include <gtk/gtk.h>


//-- Module Functions Declarations ---------------
void end_program (GtkWidget *, gpointer);
static void clear_surface (void);
static void fill_surface (void);
static gboolean configure_surface (GtkWidget         *widget);
static void draw_brush (GtkWidget *widget,
                        gdouble    x,
                        gdouble    y);
static gboolean
draw_cb (GtkWidget *widget,
         cairo_t   *cr,
         gpointer   data);


//-- Module Functions Definitions ---------------
/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;
GtkWidget *drawing;

int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "lcd2_layout.glade", NULL);
    
    GtkWidget *win = (GtkWidget *) gtk_builder_get_object (builder, "window1");
    GtkWidget *btn1 = (GtkWidget *) gtk_builder_get_object (builder, "button1");
    GtkWidget *btn2 = (GtkWidget *) gtk_builder_get_object (builder, "button2");
    GtkWidget *btn3 = (GtkWidget *) gtk_builder_get_object (builder, "button3");    
    drawing = (GtkWidget *) gtk_builder_get_object (builder, "drawingarea1");

    g_signal_connect (drawing, "draw", G_CALLBACK (draw_cb), NULL);
    g_signal_connect(btn1, "clicked", G_CALLBACK(clear_surface), NULL);    
    g_signal_connect(btn2, "clicked", G_CALLBACK(fill_surface), NULL);
    g_signal_connect(btn3, "clicked", G_CALLBACK(end_program), NULL);    
    
    gtk_widget_show_all (win);
    configure_surface(drawing);
    gtk_main();
    
    return 0;
}


void end_program (GtkWidget *wid, gpointer ptr)
{
    if (surface)
        cairo_surface_destroy (surface);
    
    gtk_main_quit();
}


/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean
draw_cb (GtkWidget *widget,
         cairo_t   *cr,
         gpointer   data)
{
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);

  return FALSE;
}


static void clear_surface (void)
{
    cairo_t *cr;

    cr = cairo_create (surface);

    cairo_set_source_rgb (cr, 0.1, 0.1, 0.1);
    cairo_paint (cr);

    cairo_destroy (cr);
    g_print("Clearing surface\n");
}


static void fill_surface (void)
{
    // cairo_t *cr;

    // cr = cairo_create (surface);

    // cairo_set_source_rgb (cr, 125, 0, 0);
    // cairo_paint (cr);

    // cairo_destroy (cr);
    // g_print("Filling surface\n");

    //draw brush down here
    draw_brush(drawing, 10, 10);
    g_print("Draw Brush\n");
}


/* Draw a rectangle on the surface at the given position */
static void draw_brush (GtkWidget *widget,
                        gdouble    x,
                        gdouble    y)
{
    cairo_t *cr;

    /* Paint to the surface, where we store our state */
    cr = cairo_create (surface);

    cairo_rectangle (cr, x - 3, y - 3, 6, 6);
    cairo_fill (cr);

    cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area. */
    gtk_widget_queue_draw_area (widget, x - 3, y - 3, 6, 6);
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
