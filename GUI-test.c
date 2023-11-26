#include <gtk/gtk.h>

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    float balance;
} User;

typedef struct {
    char category[MAX_CATEGORY_LENGTH];
    float amount;
    time_t date;
} Transaction;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *button;
    
    User currentUser;
    Transaction transactions[MAX_ENTRIES];
    int numTransactions = 0;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Budget Application");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("Welcome to the Budget Application!");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);

    button = gtk_button_new_with_label("Login");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);

    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button, "clicked", G_CALLBACK(login_button_clicked), NULL);

    gtk_main();

    return 0;
}

void login_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *login_window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *button;

    login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(login_window), "Login");
    gtk_container_set_border_width(GTK_CONTAINER(login_window), 10);
    gtk_window_set_default_size(GTK_WINDOW(login_window), 300, 200);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(login_window), grid);

    label = gtk_label_new("Enter your username and password:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);

    label = gtk_label_new("Username:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);

    // Add entry field for username

    label = gtk_label_new("Password:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

    // Add entry field for password

    button = gtk_button_new_with_label("Login");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 2, 1);

    gtk_widget_show_all(login_window);

    g_signal_connect(login_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button, "clicked", G_CALLBACK(login_button_clicked), NULL);
}

