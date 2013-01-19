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

struct NotifyData {
    unsigned int replaces_id;
    unsigned char urgency;
    int expire_time;

    char* app_name;
    char* icon;
    char* category;

    char* summary;
    char* body;
};

struct NotifyData *notif_create_data(void);
void notif_free_data(struct NotifyData *data);

bool notif_validate_data(struct NotifyData *data);
void notif_print_data(struct NotifyData *data);

int notif_send_notification(struct NotifyData *data);
char *notif_get_error_message(void);
void notif_free_error_message(void);

#endif /* NOTIF_H */
