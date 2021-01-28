//----------------------------------------
// Tests for gtk lib
//----------------------------------------

#include <gtk/gtk.h>
#include <stdint.h>
#include "cgrom.h"


typedef struct {
    int R;
    int G;
    int B;
} rgb_st;

//-- Module Functions Declarations ---------------
void end_program (GtkWidget *, gpointer);
static void init_surface (void);
static void fill_surface (void);
static void unfill_surface (void);

static void draw_patch (GdkPixbuf * p, int x1, int x2, int y1, int y2, rgb_st *);

static void put_pixel (GdkPixbuf *pixbuf,
                       int x,
                       int y,
                       guchar red,
                       guchar green,
                       guchar blue,
                       guchar alpha);

static void draw_lcd_patch (GdkPixbuf * p, int x, int y, rgb_st *);
static void draw_mini_patch (GdkPixbuf * p, int x, int y, rgb_st *);
static void fill_with_lcd_patches (GdkPixbuf * p);
void get_lcd_patch_position (int row, int column, int * x, int * y);
static void draw_lcd_patch_from_cgrom (GdkPixbuf * p, int line_pos, int cursor_pos, int symbol_index);
    
static void clear_button_callback (void);


void Lcd_TransmitStr (char * line);
void Lcd_SetDDRAM (uint8_t ddram);

// Globals --------------------------------------------------------
GdkPixbuf * pix;
GtkWidget *imag1;
static cairo_surface_t *surface = NULL;


//-- Module Functions Definitions ---------------


int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "lcd4_layout.glade", NULL);

    //widgets from builder
    GtkWidget *win = (GtkWidget *) gtk_builder_get_object (builder, "window1");
    GtkWidget *btn1 = (GtkWidget *) gtk_builder_get_object (builder, "button1");
    GtkWidget *btn2 = (GtkWidget *) gtk_builder_get_object (builder, "button2");
    GtkWidget *btn3 = (GtkWidget *) gtk_builder_get_object (builder, "button3");
    imag1 = (GtkWidget *) gtk_builder_get_object (builder, "image1");    

    g_signal_connect(btn1, "clicked", G_CALLBACK(clear_button_callback), NULL);    
    g_signal_connect(btn2, "clicked", G_CALLBACK(fill_surface), NULL);
    g_signal_connect(btn3, "clicked", G_CALLBACK(end_program), NULL);    

    gtk_widget_show_all (win);
    init_surface();
    
    gtk_main();
    
    return 0;
}


void end_program (GtkWidget *wid, gpointer ptr)
{
    gtk_main_quit();
}


static void init_surface (void)
{
    gint width;
    gint height;    
    gdk_pixbuf_get_file_info ("download.jpeg", &width, &height);
    g_print("File is: %d width %d height\n", width, height);
                          
    GError *gerror = NULL;
    pix = gdk_pixbuf_new_from_file ("download.jpeg", &gerror);

    if (pix == NULL)
    {
        g_print("Error loading pix: %s\n", gerror->message);
        g_error_free(gerror);
    }
    else
    {
        g_print("pix loaded\n");
        unfill_surface();
        fill_with_lcd_patches(pix);
    }
}


static void draw_patch (GdkPixbuf * p, int x1, int x2, int y1, int y2, rgb_st * rgb)
{
    for (int j = y1; j < y2; j++)
    {
        for (int i = x1; i < x2; i++)
            put_pixel(p, i, j, rgb->R, rgb->G, rgb->B, 0);
    }
}


#define MINI_PATCH_WIDTH    3
#define MINI_PATCH_HEIGHT    3
static void draw_mini_patch (GdkPixbuf * p, int x, int y, rgb_st * rgb)
{
    for (int j = y; j < (MINI_PATCH_HEIGHT + y); j++)
    {
        for (int i = x; i < (MINI_PATCH_WIDTH + x); i++)
            put_pixel(p, i, j, rgb->R, rgb->G, rgb->B, 0);
    }
}


static void draw_lcd_patch (GdkPixbuf * p, int x, int y, rgb_st * rgb)
{
    int col = 0;
    int row = 0;

    //5 x 8 mini_patch
    for (int j = 0; j < 8; j++)
    {
        row = y + j * (3 + 1);
        for (int i = 0; i < 5; i++)
        {
            col = x + i * (3 + 1);
            draw_mini_patch(p, col, row, rgb);
        }
    }
}


#define PATCH_WIDTH    19
#define PATCH_HEIGHT    30
static void fill_with_patches (GdkPixbuf * p)
{
    rgb_st rgb;
    rgb.R = 255;
    rgb.G = 0;
    rgb.B = 0;
    
    //line 1
    int col = 0;
    for (int i = 0; i < 16; i++)
    {
        col = 73 + i * (PATCH_WIDTH + 1);
        draw_patch(p, col, col + PATCH_WIDTH, 75, 75 + PATCH_HEIGHT, &rgb);
    }

    //line 2
    col = 0;
    for (int i = 0; i < 16; i++)
    {
        col = 73 + i * (PATCH_WIDTH + 1);
        draw_patch(p, col, col + PATCH_WIDTH, 75 + PATCH_HEIGHT + 2, 75 + 2*PATCH_HEIGHT + 2, &rgb);
    }
}



#define COLUMN_START    70
#define ROW_START    74
static void fill_with_lcd_patches (GdkPixbuf * p)
{
    rgb_st rgb;
    rgb.R = 123;
    rgb.G = 160;
    rgb.B = 47;

    //line 1
    int col = 0;
    for (int i = 0; i < 16; i++)
    {
        col = COLUMN_START + i * (PATCH_WIDTH + 2);
        draw_lcd_patch(p, col, ROW_START, &rgb);
    }

    //line 2
    col = 0;
    for (int i = 0; i < 16; i++)
    {
        col = COLUMN_START + i * (PATCH_WIDTH + 2);
        draw_lcd_patch(p, col, ROW_START + PATCH_HEIGHT + 4, &rgb);
    }
}


#define COLUMN_START    70
#define ROW_START    74
static void unfill_surface (void)
{
    rgb_st rgb;
    rgb.R = 135;
    rgb.G = 175;
    rgb.B = 53;
    
    int x1 = COLUMN_START;
    int x2 = COLUMN_START + 16 * (PATCH_WIDTH + 2);
    int y1 = ROW_START;
    int y2 = ROW_START + 2 * PATCH_HEIGHT + 8;
    
    draw_patch(pix, x1, x2, y1, y2, &rgb);
    
    gtk_image_set_from_pixbuf (GTK_IMAGE(imag1), pix);
}

static void fill_surface (void)
{
    // fill_with_patches(pix);
    fill_with_lcd_patches(pix);    
    gtk_image_set_from_pixbuf (GTK_IMAGE(imag1), pix);    
}

static void put_pixel (GdkPixbuf *pixbuf,
                       int x,
                       int y,
                       guchar red,
                       guchar green,
                       guchar blue,
                       guchar alpha)
{
    int width, height, rowstride, n_channels;
    guchar *pixels, *p;

    n_channels = gdk_pixbuf_get_n_channels (pixbuf);
    // g_print("n_channels: %d\n", n_channels);

    g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
    g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
    // g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
    // g_assert (n_channels == 4);

    width = gdk_pixbuf_get_width (pixbuf);
    height = gdk_pixbuf_get_height (pixbuf);

    g_assert (x >= 0 && x < width);
    g_assert (y >= 0 && y < height);

    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    p = pixels + y * rowstride + x * n_channels;
    p[0] = red;
    p[1] = green;
    p[2] = blue;
    // p[3] = alpha;
}



static void draw_lcd_patch_from_cgrom (GdkPixbuf * p, int line_pos, int cursor_pos, int symbol_index)
{
    rgb_st rgb_blank;
    rgb_blank.R = 123;
    rgb_blank.G = 160;
    rgb_blank.B = 47;

    rgb_st rgb_fill;
    // rgb_fill.R = 255;
    // rgb_fill.G = 0;
    // rgb_fill.B = 0;
    rgb_fill.R = 10;
    rgb_fill.G = 10;
    rgb_fill.B = 10;

    // sanity checks
    if (symbol_index > LAST_SYMBOL_IN_CGROM)
        return;
    
    if ((line_pos > 1) || (cursor_pos > 15))
        return;

    int x = 0;
    int y = 0;
    get_lcd_patch_position (line_pos, cursor_pos, &x, &y);    

    int col = 0;
    int row = 0;

    //5 x 8 mini_patch
    for (int j = 0; j < 8; j++)
    {
        row = y + j * (3 + 1);
        uint8_t * vline = cgrom[symbol_index];
        uint8_t line = *(vline + j);
        // uint8_t line = char_a[j];        
        uint8_t mask = 0;
        for (int i = 0; i < 5; i++)
        {
            mask = 0x10;
            mask >>= i;
            
            col = x + i * (3 + 1);

            if (line & mask)
                draw_mini_patch(p, col, row, &rgb_fill);
            else
                draw_mini_patch(p, col, row, &rgb_blank);
        }
    }
}


void get_lcd_patch_position (int row, int column, int * x, int * y)
{
    if (row > 1)
        return;

    if (column > 15)
        return;
    
    *x = COLUMN_START + column * (PATCH_WIDTH + 2);
    if (row)
        *y = ROW_START + (PATCH_HEIGHT * row) + 4;
    else
        *y = ROW_START;
}


static void clear_button_callback (void)
{
    // for (int i = 0; i < 16; i++)
    // {
    //     draw_lcd_patch_from_cgrom (pix, 0, i, i);
    // }

    // for (int i = 16; i < 32; i++)
    // {
    //     draw_lcd_patch_from_cgrom (pix, 1, i - 16, i);
    // }

    // for (int i = 32; i < 48; i++)
    // {
    //     draw_lcd_patch_from_cgrom (pix, 0, i - 32, i);
    // }

    Lcd_SetDDRAM(0x00);
    Lcd_TransmitStr("     Hola Mundo!");

    Lcd_SetDDRAM(0x40);
    Lcd_TransmitStr("Chau Mundo!");
    
    
    gtk_image_set_from_pixbuf (GTK_IMAGE(imag1), pix);    
}


int lcd_line = 0;
int lcd_cursor = 0;
void Lcd_TransmitStr (char * line)
{
    while (*line != '\0')
    {
        //if we have the char in the list send it
        if ((*line >= 32) && (*line < (LAST_SYMBOL_IN_CGROM + 32)))
        {
            draw_lcd_patch_from_cgrom (pix, lcd_line, lcd_cursor, (*line - 32));
        }

        //update the cursor
        lcd_cursor++;
        line++;
    }
}


void Lcd_SetDDRAM (uint8_t ddram)
{
    if (ddram < 16)
    {
        lcd_line = 0;
        lcd_cursor = ddram;
    }

    if ((ddram < (0x40 + 16)) && (ddram >= 0x40))
    {
        lcd_line = 1;
        lcd_cursor = ddram - 0x40;
    }
}

//--- end of file ---//
