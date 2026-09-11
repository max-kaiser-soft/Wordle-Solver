#include <gtk/gtk.h>
#include <string>

#include "backend.h"
#include "ui.h"


using namespace std;

static bool firstStart = false;

static GtkWidget *savedSubmit = NULL; //copy of the submit button to be used by backend


void manualButtonPress(){ //function for the backend to press the submit button
    if (savedSubmit != NULL) {
        g_signal_emit_by_name(savedSubmit, "clicked");
    }
}

static void on_file_chosen(GtkFileDialog *dialog, GAsyncResult *result, gpointer user_data)
{
    GFile *file = gtk_file_dialog_open_finish(dialog, result, NULL);
    if (!file)
        return;

    char *customFile = g_file_get_path(file);

    g_print("User selected: %s\n", customFile);

    g_free(customFile);
    g_object_unref(file);
}


static void file_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data){ //opens custom word list

 GtkFileDialog *dialog = gtk_file_dialog_new();

    gtk_file_dialog_open(dialog, GTK_WINDOW(user_data), NULL, (GAsyncReadyCallback)on_file_chosen, user_data);
}


static void help_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data){ //help/tutorial

    // GtkApplication *app = GTK_APPLICATION(user_data);
    // GtkWindow *parent = gtk_application_get_active_window(app);
    //
    // gtk_show_about_dialog(parent,
    //     "program-name", "Wordle Solver",
    //     "version", "1.0",
    //     "authors", (const char*[]) {"MAX KAISER", NULL},
    //     "comments", "A Worlde solving program.",
    //     NULL);



}

static void about_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data){ //about info

    GtkApplication *app = GTK_APPLICATION(user_data);
    GtkWindow *parent = gtk_application_get_active_window(app);

    //get icon png
    GFile *file = g_file_new_for_path("icon.png");
    GdkTexture *icon = gdk_texture_new_from_file(file, NULL);
    g_object_unref(file);


    gtk_show_about_dialog(parent,
        "program-name", "Wordle Solver",
        "version", "1.0",
        "authors", (const char*[]) {"MAX KAISER", NULL},
        "comments", "A Worlde solving program.",
        "logo", icon,
        "license-type", GTK_LICENSE_GPL_3_0,
        "website", "https://github.com/max-kaiser-soft/Wordle-Solver",
        "website-label", "GitHub Repo",
        NULL);

    //clean icon memory
    if (icon) {
        g_object_unref(icon);
    }
}


static void quit_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data){ //quits program
    g_application_quit(G_APPLICATION(user_data));
}

void setup_actions(GtkApplication *app){ //app.whatever actions
    const GActionEntry actions[] = {
        { "file", file_activated },
        //{ "help", help_activated },
        { "about", about_activated },
        { "quit", quit_activated }
    };

    g_action_map_add_action_entries(G_ACTION_MAP(app), actions, G_N_ELEMENTS(actions), app);
}

typedef struct { //struct to send input data to backend
    GtkWidget *finalLetter0;
    GtkWidget *finalLetter1;
    GtkWidget *finalLetter2;
    GtkWidget *finalLetter3;
    GtkWidget *finalLetter4;
    GtkWidget *unorderedLetters0;
    GtkWidget *unorderedLetters1;
    GtkWidget *unorderedLetters2;
    GtkWidget *unorderedLetters3;
    GtkWidget *unorderedLetters4;
    GtkWidget *missingLetters;
    GtkLabel *resultsLabel;
} inputData;


typedef enum { //used to identify type of letters being sent to backend
    FINALLETTER,
    UNORDEREDLETTER,
    MISSINGLETTER
} letterCallType;


void on_window_destroy(GtkWindow *window, gpointer user_data) {//destroy inputData struct
    inputData *input = (inputData*)user_data;
    g_free(input);
}

void sendToBackend(const char *input, letterCallType type, int index) {//checks letter inputs and sends inptus to backend
    if(strlen(input) == 0)
        return;

    switch (type) {
        case FINALLETTER:
            addOrderedLetter(input, index);
            break;
        case UNORDEREDLETTER:
            addUnorderedLetter(input, index);
            break;
        case MISSINGLETTER:
            removeLetters(input);
            break;
    }
}

void submitButton(GtkButton *button, gpointer user_data) {//button press actions, backend to do logic

    firstStart = true;

    restart();

    inputData *input = (inputData*)user_data;

    const char *finalLetter0 = gtk_editable_get_text(GTK_EDITABLE(input->finalLetter0));
    const char *finalLetter1 = gtk_editable_get_text(GTK_EDITABLE(input->finalLetter1));
    const char *finalLetter2 = gtk_editable_get_text(GTK_EDITABLE(input->finalLetter2));
    const char *finalLetter3 = gtk_editable_get_text(GTK_EDITABLE(input->finalLetter3));
    const char *finalLetter4 = gtk_editable_get_text(GTK_EDITABLE(input->finalLetter4));

    const char *unorderedLetters0 = gtk_editable_get_text(GTK_EDITABLE(input->unorderedLetters0));
    const char *unorderedLetters1 = gtk_editable_get_text(GTK_EDITABLE(input->unorderedLetters1));
    const char *unorderedLetters2 = gtk_editable_get_text(GTK_EDITABLE(input->unorderedLetters2));
    const char *unorderedLetters3 = gtk_editable_get_text(GTK_EDITABLE(input->unorderedLetters3));
    const char *unorderedLetters4 = gtk_editable_get_text(GTK_EDITABLE(input->unorderedLetters4));

    const char *missingLetters = gtk_editable_get_text(GTK_EDITABLE(input->missingLetters));


    sendToBackend(finalLetter0, FINALLETTER, 0);
    sendToBackend(finalLetter1, FINALLETTER, 1);
    sendToBackend(finalLetter2, FINALLETTER, 2);
    sendToBackend(finalLetter3, FINALLETTER, 3);
    sendToBackend(finalLetter4, FINALLETTER, 4);

    sendToBackend(unorderedLetters0, UNORDEREDLETTER, 0);
    sendToBackend(unorderedLetters1, UNORDEREDLETTER, 1);
    sendToBackend(unorderedLetters2, UNORDEREDLETTER, 2);
    sendToBackend(unorderedLetters3, UNORDEREDLETTER, 3);
    sendToBackend(unorderedLetters4, UNORDEREDLETTER, 4);

    sendToBackend(missingLetters, MISSINGLETTER, -1);


    recalculate();


    char* s = getTopTen_c(); //get top ten words from backend


    gtk_label_set_text(input->resultsLabel, s);
}

void clearButton(GtkButton *button, gpointer user_data) {//clears all input boxes

    firstStart = false;

    restart();
    resetFileName();

    inputData *input = (inputData*)user_data;

    gtk_editable_set_text(GTK_EDITABLE(input->finalLetter0), "");
    gtk_editable_set_text(GTK_EDITABLE(input->finalLetter1), "");
    gtk_editable_set_text(GTK_EDITABLE(input->finalLetter2), "");
    gtk_editable_set_text(GTK_EDITABLE(input->finalLetter3), "");
    gtk_editable_set_text(GTK_EDITABLE(input->finalLetter4), "");

    gtk_editable_set_text(GTK_EDITABLE(input->unorderedLetters0), "");
    gtk_editable_set_text(GTK_EDITABLE(input->unorderedLetters1), "");
    gtk_editable_set_text(GTK_EDITABLE(input->unorderedLetters2), "");
    gtk_editable_set_text(GTK_EDITABLE(input->unorderedLetters3), "");
    gtk_editable_set_text(GTK_EDITABLE(input->unorderedLetters4), "");

    gtk_editable_set_text(GTK_EDITABLE(input->missingLetters), "");


    gtk_label_set_text(input->resultsLabel, "Press \n'Get Words'\n to generate words.");

    //call submit again
}

static void on_dropdown_changed (GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data){ //adjusts which word list program uses
    GObject *selected_item = G_OBJECT (gtk_drop_down_get_selected_item (dropdown));
    const char *wordListName = gtk_string_object_get_string (GTK_STRING_OBJECT (selected_item)); //name of word list

    setWordList(wordListName);

    if(firstStart)
        submitButton (NULL, user_data); //clicks the new list button
}


static void activate(GtkApplication *app, gpointer user_data) { //activate GUI

    setup_actions(app); //app.whatever actions

    GtkWidget *window = gtk_application_window_new(app);

    gtk_window_set_default_icon_name("com.kaiser.wordlesolver");

    gtk_window_set_title(GTK_WINDOW(window), "Worlde Solver");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 520);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);


    //header
    GtkWidget *header = gtk_header_bar_new();

    //menu
    GtkWidget *menu_button = gtk_menu_button_new();

    GMenu *menu = g_menu_new();

    //g_menu_append(menu, "Open Custom Word List", "app.file");
    //g_menu_append(menu, "Help", "app.help");
    g_menu_append(menu, "About", "app.about");
    g_menu_append(menu, "Quit", "app.quit");

    GtkWidget *popover = gtk_popover_menu_new_from_model(G_MENU_MODEL(menu));
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(menu_button), popover);

    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), menu_button);

    //menu icon
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menu_button), "open-menu-symbolic");


    //attach header to window
    gtk_window_set_titlebar(GTK_WINDOW(window), header);



    //grid creation
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 15);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

    gtk_window_set_child(GTK_WINDOW(window), grid);

    //margins for grid
    gtk_widget_set_margin_start(GTK_WIDGET(grid), 15);
    gtk_widget_set_margin_end(GTK_WIDGET(grid), 15);
    gtk_widget_set_margin_top(GTK_WIDGET(grid), 0);
    gtk_widget_set_margin_bottom(GTK_WIDGET(grid), 15);



    //column, row, width, height////////////////////////////////////////


    //logo
    GdkTexture *texture = gdk_texture_new_from_filename ("logo.png", NULL); //load to memory

    GtkWidget *logo = gtk_image_new_from_paintable (GDK_PAINTABLE (texture)); //makes image

    gtk_image_set_pixel_size (GTK_IMAGE (logo), 100); //selects width of image
    gtk_widget_set_size_request (logo, 100, 100); //size of box for image (WxL)

    gtk_widget_set_hexpand (logo, FALSE); //image cannot grow
    gtk_widget_set_vexpand (logo, FALSE);
  //gtk_widget_set_halign (logo, GTK_ALIGN_START); //image alignment
    gtk_widget_set_valign (logo, GTK_ALIGN_START);

    gtk_grid_attach (GTK_GRID (grid), logo, 0, 0, 1, 1);



    //title
    GtkWidget *programTitle = gtk_label_new(NULL);

    gtk_label_set_markup(GTK_LABEL(programTitle),
    "<span font='Arial Bold 24'>Wordle Solver</span>");

    gtk_grid_attach(GTK_GRID(grid), programTitle, 1, 0, 2, 1);
    gtk_widget_set_halign (programTitle, GTK_ALIGN_START);
    //gtk_widget_set_valign (programTitle, GTK_ALIGN_END);



    //final letters label
    GtkWidget *finalLettersLabel = gtk_label_new("Final letters (green letters):");
    gtk_grid_attach(GTK_GRID(grid), finalLettersLabel, 0, 1, 5, 1);
    gtk_widget_set_halign (finalLettersLabel, GTK_ALIGN_START);


    //final letters (green); limits to 1 character
    GtkWidget *finalLetter0 = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(finalLetter0), 1);
    gtk_grid_attach(GTK_GRID(grid), finalLetter0, 0, 2, 1, 1);
    GtkWidget *finalLetter1 = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(finalLetter1), 1);
    gtk_grid_attach(GTK_GRID(grid), finalLetter1, 1, 2, 1, 1);
    GtkWidget *finalLetter2 = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(finalLetter2), 1);
    gtk_grid_attach(GTK_GRID(grid), finalLetter2, 2, 2, 1, 1);
    GtkWidget *finalLetter3 = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(finalLetter3), 1);
    gtk_grid_attach(GTK_GRID(grid), finalLetter3, 3, 2, 1, 1);
    GtkWidget *finalLetter4 = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(finalLetter4), 1);
    gtk_grid_attach(GTK_GRID(grid), finalLetter4, 4, 2, 1, 1);


    //unordered letters label
    GtkWidget *unorderedLettersLabel = gtk_label_new("Unordered letters (yellow letters):");
    gtk_grid_attach(GTK_GRID(grid), unorderedLettersLabel, 0, 3, 5, 1);
    gtk_widget_set_halign (unorderedLettersLabel, GTK_ALIGN_START);

    //unordered letters (yellow)
    GtkWidget *unorderedLetters0 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), unorderedLetters0, 0, 4, 1, 1);
    GtkWidget *unorderedLetters1 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), unorderedLetters1, 1, 4, 1, 1);
    GtkWidget *unorderedLetters2 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), unorderedLetters2, 2, 4, 1, 1);
    GtkWidget *unorderedLetters3 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), unorderedLetters3, 3, 4, 1, 1);
    GtkWidget *unorderedLetters4 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), unorderedLetters4, 4, 4, 1, 1);


    //wrong letters label
    GtkWidget *wrongLettersLabel = gtk_label_new("Wrong letters (gray letters):");
    gtk_grid_attach(GTK_GRID(grid), wrongLettersLabel, 0, 5, 5, 1);
    gtk_widget_set_halign (wrongLettersLabel, GTK_ALIGN_START);


    //wrong letters (gray); all one box
    GtkWidget *missingLetters = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), missingLetters, 0, 6, 2, 1);

    //results box
    GtkWidget *resultsFrame = gtk_frame_new(NULL);
    gtk_widget_set_margin_top(resultsFrame, 15); //add margin at top
    gtk_widget_set_margin_bottom(resultsFrame, 15); //add margin at bottom

    gtk_widget_set_halign(resultsFrame, GTK_ALIGN_CENTER); //don't stretch
    gtk_widget_set_valign(resultsFrame, GTK_ALIGN_FILL);
    gtk_widget_set_size_request(resultsFrame, 100, -1); //width, height

    GtkWidget *resultsLabel = gtk_label_new("Press \n'Get Words'\n to generate words."); //adds label

    gtk_label_set_wrap(GTK_LABEL(resultsLabel), TRUE); //text wrapping
    // gtk_label_set_wrap_mode(GTK_LABEL(resultsLabel), PANGO_WRAP_WORD); //splits at spaces (as opposed to at character)

    gtk_label_set_width_chars(GTK_LABEL(resultsLabel), 10); //set width to X characters
    gtk_label_set_max_width_chars(GTK_LABEL(resultsLabel), 10);

    gtk_label_set_justify(GTK_LABEL(resultsLabel), GTK_JUSTIFY_CENTER); //center lines of text
    gtk_widget_set_halign(resultsLabel, GTK_ALIGN_CENTER); //center label horizontal
    gtk_widget_set_valign(resultsLabel, GTK_ALIGN_CENTER); //center label vertical



    gtk_frame_set_child(GTK_FRAME(resultsFrame), resultsLabel);
    gtk_grid_attach(GTK_GRID(grid), resultsFrame, 5, 1, 2, 8);



    //struct for data
    inputData *input = g_new(inputData, 1);
    input->finalLetter0 = finalLetter0;
    input->finalLetter1 = finalLetter1;
    input->finalLetter2 = finalLetter2;
    input->finalLetter3 = finalLetter3;
    input->finalLetter4 = finalLetter4;
    input->unorderedLetters0 = unorderedLetters0;
    input->unorderedLetters1 = unorderedLetters1;
    input->unorderedLetters2 = unorderedLetters2;
    input->unorderedLetters3 = unorderedLetters3;
    input->unorderedLetters4 = unorderedLetters4;
    input->missingLetters = missingLetters;
    input->resultsLabel = GTK_LABEL(resultsLabel);



    //activate button
    GtkWidget *submit = gtk_button_new_with_label("Get Words");
    savedSubmit = submit;
    gtk_grid_attach(GTK_GRID(grid), submit, 3, 6, 2, 1);

    g_signal_connect(submit, "clicked", G_CALLBACK(submitButton), input);


    //clear all button
    GtkWidget *clearAll = gtk_button_new_with_label("Restart");
    gtk_grid_attach(GTK_GRID(grid), clearAll, 2, 6, 1, 1);

    g_signal_connect(clearAll, "clicked", G_CALLBACK(clearButton), input);



    //word list choice
    GtkWidget *wordListLabel = gtk_label_new("Word List:");
    gtk_widget_set_halign (wordListLabel, GTK_ALIGN_START);

    gtk_grid_attach(GTK_GRID(grid), wordListLabel, 0, 7, 1, 1);

    const char *wordListChoices[] = {"Automatic (recommended)", "Core List", "Expanded List", NULL};
    GtkWidget *dropdown = gtk_drop_down_new_from_strings (wordListChoices);
    gtk_widget_set_halign (dropdown, GTK_ALIGN_END);
    gtk_widget_set_size_request (dropdown, 260, -1); //width

    gtk_grid_attach(GTK_GRID(grid), dropdown, 0, 7, 2, 1);

    g_signal_connect (dropdown, "notify::selected", G_CALLBACK (on_dropdown_changed), input);


    //destroy handler for inputData struct
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), input);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) { //main
    int status;

    GtkApplication *app = gtk_application_new("com.kaiser.wordlesolver", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
