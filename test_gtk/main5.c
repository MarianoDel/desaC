#include <gtk/gtk.h>

int main (int argc, char **argv)
{
    gtk_init (&argc, &argv);

        gint width;
    gint height;    
    gdk_pixbuf_get_file_info ("download.jpeg", &width, &height);
    g_print("File is: %d width %d height\n", width, height);
                          
    GError *gerror = NULL;
    GdkPixbuf * pix;
    pix = gdk_pixbuf_new_from_file ("download.jpeg", &gerror);
    // gdk_pixbuf_save(pix, "output.bmp", "bmp", &gerror, NULL);

    if (pix == NULL)
    {
        g_print("Error loading pix: %s\n", gerror->message);
        g_error_free(gerror);
    }
    else
        g_print("pix loaded\n");



    return 0;
}


