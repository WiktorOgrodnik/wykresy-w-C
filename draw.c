#include "draw.h"

static void put_pixel (GdkPixbuf* pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha);

void draw_chart (GtkWidget *widget, eqData* data) 
{
    const gchar* wejscie, *interval, *scale_;
    gchar* eptr;
    gdouble l, p, delta, scale;
    gint r;

    wejscie = gtk_entry_get_text(GTK_ENTRY(data->equation));
    interval = gtk_entry_get_text(GTK_ENTRY(data->interval));
    scale_ = gtk_entry_get_text(GTK_ENTRY(data->res));

    if (strcmp(interval, "") == 0) l = 10.0;
    else l = strtod(interval, &eptr); 

    p = l;
    l *= -1;

    r = (gint)800;
    delta = (p - l) / (gdouble)r;

    gdouble wyniki[r];
    gdouble max = -800.0;

    data->chartData = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, 800, 600);
    put_lines_to_chart(data->chartData, l, p);

    for (gint i = 0; i < r; i++)
    {
        double wynik = calc(wejscie, strlen(wejscie), l);

        wyniki[i] = wynik;

        if (!isnan(wynik) && wynik > max) max = wynik;
        l+=delta;
    }

    max += max * 0.4;
    if (max > 300) max = 300;

    if (strcmp(scale_, "") == 0) scale = 300 / max;
    else scale = strtod(scale_, &eptr);

    makeLegend(data->chartLegendLeft, (300/scale) / 5);
    makeLegend(data->chartLegendBottom, p / 5);

    /*for(gint i = 0; i < 10; i++)
    {
        GtkRequisition r;

        gtk_widget_get_preferred_size(data->chartLegendBottom[i], &r, NULL);

        gint width = 80 - r.width;
        gtk_widget_set_margin_end(GTK_WIDGET(data->chartLegendBottom[i]), width);
    }*/

    for (gint i = 0; i < r; i++) 
    {
        wyniki[i] = (gint)(300 - (wyniki[i]) * scale);

        if (!isnan(wyniki[i]) && wyniki[i] >=0 && wyniki[i] <= 600) put_pixel(data->chartData, i, wyniki[i], 0, 0, 255, 255); 

        if (i && !isnan(wyniki[i]))
        {
            gint start = wyniki[i - 1] < wyniki[i] ? (gint)wyniki[i - 1] : (gint)wyniki[i];
            gint end = wyniki[i - 1] < wyniki[i] ? (gint)wyniki[i] : (gint)wyniki[i - 1];

            if (end - start >= 2 && end - start < 400)
            {
                for (gint j = start; j < end; j++)
                {
                    put_pixel(data->chartData, i, j, 0, 0, 255, 255);
                }
            }
        }
    }
    
    gtk_image_set_from_pixbuf(GTK_IMAGE(data->chart), data->chartData);
}

static void put_pixel (GdkPixbuf* pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha)
{
    int width, height, rowstride, n_channels;
    guchar *pixels, *p; 

    n_channels = gdk_pixbuf_get_n_channels (pixbuf);

    g_assert (gdk_pixbuf_get_colorspace(pixbuf) == GDK_COLORSPACE_RGB);
    g_assert (gdk_pixbuf_get_bits_per_sample(pixbuf) == 8);
    g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
    g_assert (n_channels == 4);

    width = gdk_pixbuf_get_width (pixbuf);
    height = gdk_pixbuf_get_height (pixbuf);

    if (x < 0 || x >= width) return;
    if (y < 0 || y >= height) return;

    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels(pixbuf);

    p = pixels + y * rowstride + x * n_channels;
    p[0] = red;
    p[1] = green;
    p[2] = blue;
    p[3] = alpha;
}

void put_lines_to_chart(GdkPixbuf* pixbuf, gdouble l, gdouble p)
{
    for (gint i = 0; i < 800; i++)
    {
        for (gint j = 0; j < 600; j++)
        {
            if (j % 60 == 0 && j) put_pixel(pixbuf, i, j, 0, 0, 0, 127);
            else if (j % 12 == 0 && j) put_pixel(pixbuf, i, j, 0, 0, 0, 63);
        }
    }

    for (gint i = 0; i < 600; i++)
    {
        for (gint j = 0; j < 800; j++)
        {
            if (j % 80 == 0 && j) put_pixel(pixbuf, j, i, 0, 0, 0, 127);
            else if (j % 16 == 0 && j) put_pixel(pixbuf, j, i, 0, 0, 0, 63);
        }
    }

    for (gint i = 0; i < 800; i++)
    {
        put_pixel(pixbuf, i, 299, 0, 0, 0, 255);
        put_pixel(pixbuf, i, 300, 0, 0, 0, 255);
    }

    for (gint i = 0; i < 600; i++)
    {
        put_pixel(pixbuf, 399, i, 0, 0, 0, 255);
        put_pixel(pixbuf, 400, i, 0, 0, 0, 255);
    }
}

void makeLegend(GtkWidget* chartLeft[], gdouble delta)
{
    for (gint i = 1; i <= 11; i++)
    {
        if (i < 6)
        {
            gchar str[20]; double_to_char((gdouble)((6 - i) * delta), 2, str);
            gtk_label_set_text(GTK_LABEL(chartLeft[i - 1]), str);
        }
        else if (i == 6)
        {
            gtk_label_set_text(GTK_LABEL(chartLeft[i - 1]), "0");
        }
        else
        {   
            gchar str[20]; double_to_char((gdouble)((i - 6) * delta), 2, str);
            gtk_label_set_text(GTK_LABEL(chartLeft[i - 1]), str);
        }
    }
}