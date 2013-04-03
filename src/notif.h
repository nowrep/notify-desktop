/* ============================================================
* notify-desktop - sends desktop notifications
* Copyright (C) 2012  David Rosca <nowrep@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */

#ifndef NOTIF_H
#define NOTIF_H

#include <stdbool.h>

#define NOTIF_URGENCY_LOW 0
#define NOTIF_URGENCY_NORMAL 1
#define NOTIF_URGENCY_CRITICAL 2

#define NOTIF_ERROR -1

typedef void NotifyData;

struct NotifyData *notif_create_data(void);
void notif_free_data(struct NotifyData *data);

void notif_set_replaces_id(struct NotifyData *data, unsigned int id);
void notif_set_urgency(struct NotifyData *data, unsigned char urgency);
void notif_set_expire_time(struct NotifyData *data, int time);
void notif_set_app_name(struct NotifyData *data, char *name);
void notif_set_icon(struct NotifyData *data, char *icon);
void notif_set_category(struct NotifyData *data, char *category);
void notif_set_summary(struct NotifyData *data, char *summary);
void notif_set_body(struct NotifyData *data, char *body);

unsigned int notif_get_replaces_id(struct NotifyData *data);
unsigned char notif_get_urgency(struct NotifyData *data);
int notif_get_expire_time(struct NotifyData *data);
char *notif_get_app_name(struct NotifyData *data);
char *notif_get_icon(struct NotifyData *data);
char *notif_get_category(struct NotifyData *data);
char *notif_get_summary(struct NotifyData *data);
char *notif_get_body(struct NotifyData *data);

bool notif_validate_data(struct NotifyData *data);
void notif_print_data(struct NotifyData *data);

int notif_send_notification(struct NotifyData *data);
char *notif_get_error_message(void);
void notif_free_error_message(void);

#endif /* NOTIF_H */
