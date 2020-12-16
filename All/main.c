#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <err.h>
#include "PreTreatment.h"
#include "utility_SDL.h"
#include "Segmentation.h"

#define true 1
#define false 0


void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

GtkWidget *window;
GtkWidget *grid;
GtkBuilder* builder;
GtkWidget *image;
GtkWidget *res;
GError *error = NULL;
SDL_Surface *surface;
char *filename;

void open_image(GtkWidget *image, char* filename)
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale(filename, 800, 650, FALSE,&error);
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
}

void open_dialog(GtkWidget *window)
{
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new("Choose a file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, 
		"_Open", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
	gtk_widget_show_all(dialog);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_home_dir());
	gint answer = gtk_dialog_run(GTK_DIALOG(dialog));
	if(answer == GTK_RESPONSE_ACCEPT)
	{
		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		filename = gtk_file_chooser_get_filename(chooser);
		open_image(image, filename);
		open_image(res, filename);
		surface = load_image(filename);
	}
    gtk_widget_destroy(dialog);
}

void binarize_image()
{
    int h = surface->h;
    int w = surface->w;
    Uint8 binaryArray[h][w];
    AllTreatment(h, w, binaryArray, surface, "b");
    display_Array(h, w, binaryArray, surface);
    IMG_SavePNG(surface, "Save/res.png");
    open_image(res, "Save/res.png");
}

void init_window()
{
	GtkWidget *Open_file;
	GtkWidget *Binarize;

	window = GTK_WIDGET(gtk_builder_get_object(builder, "Main_window"));
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	Open_file = GTK_WIDGET(gtk_builder_get_object(builder, "Open"));
	g_signal_connect(GTK_BUTTON(Open_file), "activate", G_CALLBACK(open_dialog), NULL);

	image = GTK_WIDGET(gtk_builder_get_object(builder, "image"));
	res = GTK_WIDGET(gtk_builder_get_object(builder, "res"));

	Binarize = GTK_WIDGET(gtk_builder_get_object(builder, "Binarize"));
	g_signal_connect(GTK_BUTTON(Binarize), "activate", G_CALLBACK(binarize_image), NULL);
}

void freeS()
{
	SDL_FreeSurface(surface);
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	//ALL INIT OF GTK AND SDL
	gtk_init(&argc, &argv);
	builder = gtk_builder_new();

	GError* error = NULL;

	if (gtk_builder_add_from_file(builder, "main.glade", &error) == 0)
	{
		g_printerr("Error loading file: %s\n", error->message);
		g_clear_error(&error);
		return 1;
	}
	init_window();

	gtk_widget_show_all(window);
	gtk_main();

	freeS();

	printf("%s\n", "test");
	return 0;
}