/* ============================================================
* notify-desktop - sends desktop notifications
* Copyright (C) 2012-2015 David Rosca <nowrep@gmail.com>
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

#define DBUS_API_SUBJECT_TO_CHANGE
#include "dbusimp.h"

#include <dbus/dbus.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *_notif_error = NULL;

static void create_error_message(char *mes)
{
    size_t size = strlen(mes) + 1;

    _notif_error = (char*) malloc(size);
    strncpy(_notif_error, mes, size);
}

int _notif_send_notification(struct NotifyData *data)
{
    /*
     * DBus code based on tutorial from http://www.matthew.ath.cx/misc/dbus
     * Thanks!
     */

    DBusMessage *msg;
    DBusMessageIter args, actions, hints, hint_1, hint_2, variant_1, variant_2;
    DBusConnection *conn;
    DBusError err;
    DBusPendingCall *pending;
    int ret, sent_id;
    char errorbuf[255];
    const char *tmp_string;
    unsigned int replaces_id;
    unsigned char urgency;
    int expire_time;

    sent_id = -1;

    /* initialise the errors */
    dbus_error_init(&err);

    /* connect to the system bus and check for errors */
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        sprintf(errorbuf, "Connection Error (%s)\n", err.message);
        dbus_error_free(&err);
        create_error_message(errorbuf);
        return sent_id;
    }
    if (NULL == conn) {
        create_error_message("Cannot create connection");
        return sent_id;
    }

    /* request our name on the bus */
    ret = dbus_bus_request_name(conn,
                                "test.method.caller",
                                DBUS_NAME_FLAG_REPLACE_EXISTING,
                                &err);

    if (dbus_error_is_set(&err)) {
        sprintf(errorbuf, "Name Error (%s)\n", err.message);
        dbus_error_free(&err);
        create_error_message(errorbuf);
        return sent_id;
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
        create_error_message("Cannot get name on the bus");
        return sent_id;
    }

    msg = dbus_message_new_method_call("org.freedesktop.Notifications",
                                       "/org/freedesktop/Notifications",
                                       "org.freedesktop.Notifications",
                                       "Notify");
    if (NULL == msg) {
        sprintf(errorbuf, "Message Null\n");
        create_error_message(errorbuf);
        return sent_id;
    }

    /* append arguments */
    dbus_message_iter_init_append(msg, &args);

    /* app_name STRING */
    tmp_string = notif_get_app_name(data);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &tmp_string))
        goto oom;

    /* replaces_id UINT32 */
    replaces_id = notif_get_replaces_id(data);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &replaces_id))
        goto oom;

    /* app_icon STRING */
    tmp_string = notif_get_icon(data);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &tmp_string))
        goto oom;

    /* summary STRING */
    tmp_string = notif_get_summary(data);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &tmp_string))
        goto oom;

    /* body STRING */
    tmp_string = notif_get_body(data);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &tmp_string))
        goto oom;

    /* actions ARRAY - appending empty array */
    if (!dbus_message_iter_open_container(&args,
                                          DBUS_TYPE_ARRAY,
                                          DBUS_TYPE_STRING_AS_STRING,
                                          &actions))
        goto oom;

    tmp_string = "";
    if (!dbus_message_iter_append_basic(&actions, DBUS_TYPE_STRING,
                                        &tmp_string))
        goto oom;

    if (!dbus_message_iter_close_container(&args, &actions))
        goto oom;

    /* hints DICT */
    if (!dbus_message_iter_open_container(&args,
                                          DBUS_TYPE_ARRAY,
                                          DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING
                                          DBUS_TYPE_STRING_AS_STRING
                                          DBUS_TYPE_VARIANT_AS_STRING
                                          DBUS_DICT_ENTRY_END_CHAR_AS_STRING,
                                          &hints))
        goto oom;

    /* hint_1 = category: string */
    if (!dbus_message_iter_open_container(&hints, DBUS_TYPE_DICT_ENTRY,
                                          NULL, &hint_1))
        goto oom;

    tmp_string = "category";
    if (!dbus_message_iter_append_basic(&hint_1, DBUS_TYPE_STRING,
                                        &tmp_string))
        goto oom;

    /* variant_1 = value: variant */
    if (!dbus_message_iter_open_container(&hint_1,
                                          DBUS_TYPE_VARIANT,
                                          DBUS_TYPE_STRING_AS_STRING,
                                          &variant_1))
        goto oom;

    tmp_string = notif_get_category(data);
    if (!dbus_message_iter_append_basic(&variant_1, DBUS_TYPE_STRING,
                                        &tmp_string))
        goto oom;

    if (!dbus_message_iter_close_container(&hint_1, &variant_1))
        goto oom;
    /* variant_1 end */

    if (!dbus_message_iter_close_container(&hints, &hint_1))
        goto oom;
    /* hint_1 end */

    /* hint_2 = urgency: byte */
    if (!dbus_message_iter_open_container(&hints, DBUS_TYPE_DICT_ENTRY,
                                          NULL, &hint_2))
        goto oom;

    tmp_string = "urgency";
    if (!dbus_message_iter_append_basic(&hint_2, DBUS_TYPE_STRING,
                                        &tmp_string))
        goto oom;

    /* variant_2 = value: byte */
    if (!dbus_message_iter_open_container(&hint_2,
                                          DBUS_TYPE_VARIANT,
                                          DBUS_TYPE_BYTE_AS_STRING,
                                          &variant_2))
        goto oom;

    urgency = notif_get_urgency(data);
    if (!dbus_message_iter_append_basic(&variant_2, DBUS_TYPE_BYTE, &urgency))
        goto oom;

    if (!dbus_message_iter_close_container(&hint_2, &variant_2))
        goto oom;
    /* variant_2 end */

    if (!dbus_message_iter_close_container(&hints, &hint_2))
        goto oom;
    /* hint_2 end */

    if (!dbus_message_iter_close_container(&args, &hints))
        goto oom;
    /* hints end */

    /* expire_timeout INT32 */
    expire_time = notif_get_expire_time(data);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &expire_time))
        goto oom;

    /* send message and get a handle for a reply (-1 default timeout) */
    if (!dbus_connection_send_with_reply (conn, msg, &pending, -1))
        goto oom;

    if (NULL == pending) {
        sprintf(errorbuf, "Pending Call Null\n");
        create_error_message(errorbuf);
        return sent_id;
    }
    dbus_connection_flush(conn);

    /* free message */
    dbus_message_unref(msg);

    /* block until we receive a reply */
    dbus_pending_call_block(pending);

    /* get the reply message */
    msg = dbus_pending_call_steal_reply(pending);
    if (NULL == msg) {
        sprintf(errorbuf, "Reply Null\n");
        create_error_message(errorbuf);
        return sent_id;
    }

    /* free the pending message handle */
    dbus_pending_call_unref(pending);

    if (dbus_message_iter_init(msg, &args) &&
        dbus_message_iter_get_arg_type(&args) == DBUS_TYPE_UINT32)
        dbus_message_iter_get_basic(&args, &sent_id);

    /* free reply and close connection */
    dbus_message_unref(msg);

    return sent_id;

oom:
    sprintf(errorbuf, "Out Of Memory!\n");
    create_error_message(errorbuf);
    return sent_id;
}

const char *_notif_get_error_message(void)
{
    return _notif_error;
}

void _notif_free_error_message(void)
{
    free(_notif_error);
}

