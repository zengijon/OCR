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
#include "Call_Reseau.h"

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

int m_blur[3][3]={{1,1,1},{1,1,1},{1,1,1}};
int m_median[3][3]={{1,4,5},{3,9,6},{8,10,2}};
int m_smouth[3][3]={{1,2,1},{2,4,2},{1,2,1}};
GtkWidget *window;
GtkWidget *grid;
GtkBuilder* builder;
GtkWidget *image;
GtkWidget *res;
GError *error = NULL;
SDL_Surface *surface;
char *filename;
int h;
int w;
int is_auto;
int binarized = false;

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
		h = surface->h;
    	w = surface->w;
    	binarized = false;
	}
    gtk_widget_destroy(dialog);
}

void test(Uint8 binaryArray[h][w])
{
	InitFile(h, w, binaryArray, surface);
	display_Array(h, w, binaryArray, surface);
	IMG_SavePNG(surface, "Save/res.png");
	open_image(res, "Save/res.png");
	for (int i = 0; i < 1; ++i)
	{
		printf("%d\n", i);
		search_segmented(h, w, binaryArray);
	}
	printf("testounet\n");
}

void binarize_image()
{
	if (binarized == false)
	{
		binarized = true;
	    Uint8 binaryArray[h][w];
	    is_auto = Binarize(h, w, binaryArray, surface, is_auto);
	    display_Array(h, w, binaryArray, surface);
	    IMG_SavePNG(surface, "Save/res.png");
	    open_image(res, "Save/res.png");

	    test(binaryArray);
	}
}

void revert_image()
{
	Uint8 binaryArray[h][w];
	if (binarized == false)
	{
		binarize_image();
	}
	arrayd(h, w, surface, binaryArray);
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			if (binaryArray[i][j] == 255)
			{
				binaryArray[i][j] = 0;
			}
			else if (binaryArray[i][j] == 0)
			{
				binaryArray[i][j] = 255;
			}
		}
	}
	display_Array(h, w, binaryArray, surface);
    IMG_SavePNG(surface, "Save/res.png");
    open_image(res, "Save/res.png");
}

void edit_image(int matrix[3][3])
{
	if (binarized == false)
		binarize_image();
	Uint8 binaryArray[h][w];
	arrayd(h, w, surface, binaryArray);
	Convolution(h, w, matrix, binaryArray);
	display_Array(h, w, binaryArray, surface);
	IMG_SavePNG(surface, "Save/res.png");
    open_image(res, "Save/res.png");
    binarized = false;
}

void reload_image()
{
	open_image(image, filename);
	open_image(res, filename);
	surface = load_image(filename);
	h = surface->h;
	w = surface->w;
	binarized = false;
	is_auto = 0;
}

void button_clicked(GtkWidget *useless, gpointer data)
{
	int sauto = (int) strtol(gtk_entry_get_text(GTK_ENTRY(data)), (char **) NULL, 10);
	if (sauto > 255)
		sauto = 0;
	g_print("%d\n", sauto);
	gtk_editable_select_region(GTK_EDITABLE(data), 0, -1);
	gtk_editable_copy_clipboard(GTK_EDITABLE(data));
	if (binarized)
	{
		reload_image();
		binarized = false;
	}
	is_auto = sauto;
	binarize_image();
	useless = useless;
}

void manual_image()
{
	GtkWidget *Manual_window;
	GtkWidget *entry;
	GtkWidget *button_ok;
	GtkWidget *box;

	Manual_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(Manual_window), GTK_WIN_POS_CENTER);
	g_signal_connect(Manual_window, "delete-event", G_CALLBACK(gtk_widget_destroy), Manual_window);
	entry = gtk_entry_new();
	button_ok = gtk_button_new_with_mnemonic("_Write text");
	g_signal_connect(button_ok, "clicked", G_CALLBACK(button_clicked), entry);
	g_signal_connect(entry, "activate", G_CALLBACK(button_clicked), entry);

	Uint8 binaryArray[h][w];
	int seuil = Binarize(h, w, binaryArray, surface, 0);

	char s_seuil[3];
	sprintf(s_seuil, "%d", seuil);
	gtk_entry_set_text(GTK_ENTRY(entry), s_seuil);

	box = gtk_box_new(0, 0);
	gtk_box_pack_start(GTK_BOX(box), entry, 0, 0, 0);
	gtk_box_pack_start(GTK_BOX(box), button_ok, 0, 0, 0);

	gtk_container_add(GTK_CONTAINER(Manual_window), box);

	gtk_widget_show_all(Manual_window);

}

void init_window()
{
	GtkWidget *Open_file;
	GtkWidget *Auto;
	GtkWidget *Revert;
	GtkWidget *Blur;
	GtkWidget *Median;
	GtkWidget *Smouth;
	GtkWidget *Reload;
	GtkWidget *Manual;
	is_auto = 0;

	window = GTK_WIDGET(gtk_builder_get_object(builder, "Main_window"));
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	Open_file = GTK_WIDGET(gtk_builder_get_object(builder, "Open"));
	g_signal_connect(GTK_MENU_ITEM(Open_file), "activate", G_CALLBACK(open_dialog), NULL);

	image = GTK_WIDGET(gtk_builder_get_object(builder, "image"));
	res = GTK_WIDGET(gtk_builder_get_object(builder, "res"));

	Auto = GTK_WIDGET(gtk_builder_get_object(builder, "Auto"));
	g_signal_connect(GTK_MENU_ITEM(Auto), "activate", G_CALLBACK(binarize_image), NULL);

	Revert = GTK_WIDGET(gtk_builder_get_object(builder, "Revert"));
	g_signal_connect(GTK_MENU_ITEM(Revert), "activate", G_CALLBACK(revert_image), NULL);

	Blur = GTK_WIDGET(gtk_builder_get_object(builder, "Blur"));
	g_signal_connect(GTK_MENU_ITEM(Blur), "activate", G_CALLBACK(edit_image), m_blur);

	Median = GTK_WIDGET(gtk_builder_get_object(builder, "Median"));
	g_signal_connect(GTK_MENU_ITEM(Median), "activate", G_CALLBACK(edit_image), m_median);

	Smouth = GTK_WIDGET(gtk_builder_get_object(builder, "Smouth"));
	g_signal_connect(GTK_MENU_ITEM(Smouth), "activate", G_CALLBACK(edit_image), m_smouth);

	Reload = GTK_WIDGET(gtk_builder_get_object(builder, "Reload"));
	g_signal_connect(GTK_MENU_ITEM(Reload), "activate", G_CALLBACK(reload_image), NULL);

	Manual = GTK_WIDGET(gtk_builder_get_object(builder, "Manual"));
	g_signal_connect(GTK_MENU_ITEM(Manual), "activate", G_CALLBACK(manual_image), NULL);
	//g_signal_connect(GTK_MENU_ITEM(Manual), "activate", G_CALLBACK(binarize_image), NULL);

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