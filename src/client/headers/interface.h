#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>

void set_builder(GtkBuilder *builder);
void setCfgPlayer(PlayerGameSettings cfg);
PlayerGameSettings getCfgPlayer();
void on_window_main_destroy();
void disable_check_button();
void on_connect_button_click();
void on_validate_button_click();
void on_leave_button_click(GtkWidget *widget);
void on_toggled_cooperate();
void on_toggled_betray();
void on_toggled_10();
void on_toggled_25();
void on_toggled_50();
void on_toggled_100();
#endif /* INTERFACE_H */