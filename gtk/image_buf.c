#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#define CAMERA_WIDTH 128
#define CAMERA_HEIGHT 128

unsigned char *rgbImage;
GtkWidget *image;

int loadImage(unsigned char *data)
{
printf("Got image!\n");
GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(data, GDK_COLORSPACE_RGB,
                                         FALSE, 8, CAMERA_WIDTH, CAMERA_HEIGHT,
                                         CAMERA_WIDTH  * 3, NULL, NULL);
gtk_image_set_from_pixbuf((GtkImage*) image, pixbuf);
gtk_widget_queue_draw(image);
printf("Loaded\n");

return 0;
}


int main( int argc, char *argv[])
{
    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Image2");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    gtk_container_set_border_width(GTK_CONTAINER(window), 2);

    image = gtk_image_new();

    gtk_container_add(GTK_CONTAINER(window), image);

    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);

    gtk_widget_show_all(window);

loadImage(rgbImage);

    gtk_main();

    return 0;
}
