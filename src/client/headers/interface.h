#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>

extern ClientConfig cfgClient;
extern int sockfd;

void on_window_main_destroy();
void on_connect_button_click();
void on_validate_button_click();
void on_leave_button_click();

#endif /* INTERFACE_H */