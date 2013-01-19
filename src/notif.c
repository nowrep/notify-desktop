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

#include "notif.h"
#include "dbusimp.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct NotifyData *notif_create_data(void)
{
    struct NotifyData *data;
    data = (struct NotifyData*) malloc(sizeof(struct NotifyData));

    data->replaces_id = 0;
    data->urgency = NOTIF_URGENCY_NORMAL;
    data->expire_time = -1;
    data->app_name = NULL;
    data->icon = NULL;
    data->category = NULL;
    data->summary = NULL;
    data->body = NULL;

    return data;
}

void notif_free_data(struct NotifyData *data)
{
    if (data == NULL)
        return;

    free(data->app_name);
    free(data->icon);
    free(data->category);
    free(data->summary);
    free(data->body);

    free(data);
}

bool notif_validate_data(struct NotifyData *data)
{
    if (data == NULL)
        return false;

    if (data->summary == NULL)
        return false;

    /* allocate all empty strings with null string */
    if (data->app_name == NULL) {
        data->app_name = (char*) malloc(sizeof(char));
        data->app_name[0] = '\0';
    }

    if (data->icon == NULL) {
        data->icon = (char*) malloc(sizeof(char));
        data->icon[0] = '\0';
    }

    if (data->category == NULL) {
        data->category = (char*) malloc(sizeof(char));
        data->category[0] = '\0';
    }

    if (data->body == NULL) {
        data->body = (char*) malloc(sizeof(char));
        data->body[0] = '\0';
    }

    return true;
}

void notif_print_data(struct NotifyData *data)
{
    printf("> NotifyData:\n"
           "  replaces_id: \t%i\n"
           "  urgency: \t%i\n"
           "  expire_time: \t%i\n"
           "  app_name: \t\"%s\"\n"
           "  icon: \t\"%s\"\n"
           "  category: \t\"%s\"\n"
           "  summary: \t\"%s\"\n"
           "  body: \t\"%s\"\n",
           data->replaces_id,
           data->urgency,
           data->expire_time,
           data->app_name,
           data->icon,
           data->category,
           data->summary,
           data->body);
}

int notif_send_notification(struct NotifyData *data)
{
    return _notif_send_notification(data);
}

char *notif_get_error_message(void)
{
    return _notif_get_error_message();
}

void notif_free_error_message(void)
{
    char *message = _notif_get_error_message();

    if (message != NULL)
        free(message);
}

