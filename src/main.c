/* ============================================================
* notify-desktop - sends desktop notifications
* Copyright (C) 2012  David Rosca <nowrep@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int parse_urgency(char *string)
{
    if (strcmp(string, "low") == 0)
        return NOTIF_URGENCY_LOW;
    else if (strcmp(string, "normal") == 0)
        return NOTIF_URGENCY_NORMAL;
    else if (strcmp(string, "critical") == 0)
        return NOTIF_URGENCY_CRITICAL;

    return NOTIF_ERROR;
}

static char *crop_argument(char *source, int offset)
{
    char *string;
    size_t size = strlen(source) - offset;

    if (size <= 0)
        return NULL;

    string = (char*) malloc(sizeof(char) * (size + 1));

    strncpy(string, source + offset, size);
    string[size] = '\0';

    return string;
}

static void show_help(void)
{
    printf("Usage:\n"
           "notify-desktop [OPTION...] <SUMMARY> [BODY] - create a notification\n"
           "\n"
           "Help Options:\n"
           "  -h, --help               Show help options\n"
           "  -v, --version            Version of the application\n"
           "\n");
    printf("Application Options:\n"
           "  -r, --replaces-id=ID     Specified the notifications ID that will be replaced\n"
           "  -u, --urgency=LEVEL      Specifies the urgency level (low, normal, critical)\n"
           "  -t, --expire-time=TIME   Specifies the timeout in ms to expire the notification\n"
           "  -a, --app-name=APP_NAME  Specifies the app name for the icon\n"
           "  -i, --icon=ICON          Specifies an icon filename or stock icon to display\n"
           "  -c, --category=TYPE      Specifies the notification category\n"
           "\n");
    printf("Application Output:\n"
           "   On success:             Prints ID of sent notification and returns 0\n"
           "   On failure:             Prints error and returns 1\n\n");
}

static void show_version(void)
{
    printf("notify-desktop 0.1.0\n");
}

int main(int argc, char **argv)
{
    struct NotifyData *data;
    int i, return_code = 0;

    if (argc < 1) {
        show_help();
        return 1;
    }

    data = notif_create_data();

    for (i = 1; i < argc; ++i) {
        char *line = argv[i];
        if (strcmp(line, "-h") == 0 || strcmp(line, "--help") == 0) {
            show_help();
            notif_free_data(data);
            return 0;
        }
        else if (strcmp(line, "-v") == 0 || strcmp(line, "--version") == 0) {
            show_version();
            notif_free_data(data);
            return 0;
        }
        else if (strcmp(line, "-r") == 0) {
            if (argc <= i)
                continue;

            notif_set_replaces_id(data, atoi(argv[++i]));
        }
        else if (strncmp(line, "--replaces-id=", 14) == 0) {
            size_t size = strlen(line) - 14;
            char *buff;

            if (size <= 0)
                continue;

            buff = (char*) malloc(size + 1);
            strncpy(buff, line + 14, size);
            buff[size - 1] = '\0';

            notif_set_replaces_id(data, atoi(buff));
            free(buff);
        }
        else if (strcmp(line, "-u") == 0) {
            int urgency;
            if (argc <= i)
                continue;

            urgency = parse_urgency(argv[++i]);
            if (urgency != NOTIF_ERROR)
                notif_set_urgency(data, urgency);
        }
        else if (strncmp(line, "--urgency=", 10) == 0) {
            size_t size = strlen(line) - 10;
            char *buf;
            int urgency;

            if (size <= 0)
                continue;

            buf = (char*) malloc(size + 1);
            strncpy(buf, line + 10, size);
            buf[size - 1] = '\0';

            urgency = parse_urgency(buf);
            if (urgency != NOTIF_ERROR)
                notif_set_urgency(data, urgency);

            free(buf);
        }
        else if (strcmp(line, "-t") == 0) {
            if (argc <= i)
                continue;

            notif_set_expire_time(data, atoi(argv[++i]));
        }
        else if (strncmp(line, "--expire-time=", 14) == 0) {
            size_t size = strlen(line) - 14;
            char *buff;

            if (size <= 0)
                continue;

            buff = (char*) malloc(size + 1);
            strncpy(buff, line + 14, size);
            buff[size - 1] = '\0';

            notif_set_expire_time(data, atoi(buff));
            free(buff);
        }
        else if (strcmp(line, "-a") == 0) {
            char *value, *buff;
            size_t size;

            if (argc <= i)
                continue;

            value = argv[++i];
            size = strlen(value) + 1;
            buff = (char*) malloc(sizeof(char) * size);

            strncpy(buff, value, size);
            notif_set_app_name(data, buff);
        }
        else if (strncmp(line, "--app-name=", 11) == 0) {
            char *name = crop_argument(line, 11);
            if (name != NULL)
                notif_set_app_name(data, name);
        }
        else if (strcmp(line, "-i") == 0) {
            char *value, *buff;
            size_t size;

            if (argc <= i)
                continue;

            value = argv[++i];
            size = strlen(value) + 1;
            buff = (char*) malloc(sizeof(char) * size);

            strncpy(buff, value, size);
            notif_set_icon(data, buff);
        }
        else if (strncmp(line, "--icon=", 7) == 0) {
            char *icon = crop_argument(line, 7);

            if (icon != NULL)
                notif_set_icon(data, icon);
        }
        else if (strcmp(line, "-c") == 0) {
            char *value, *buff;
            size_t size;

            if (argc <= i)
                continue;

            value = argv[++i];
            size = strlen(value) + 1;
            buff = (char*) malloc(sizeof(char) * size);

            strncpy(buff, value, size);
            notif_set_category(data, buff);
        }
        else if (strncmp(line, "--category=", 11) == 0) {
            char* value = crop_argument(line, 11);

            if (value != NULL)
                notif_set_category(data, value);
        }
        else if (strncmp(line, "-", 1) == 0) {
            printf("Unknown option \"%s\"", line);
            return_code = 1;
            break;
        }
        else if (notif_get_summary(data) == NULL) {
            char *buff;
            size_t size = strlen(line) + 1;

            buff = (char*) malloc(sizeof(char) * size);
            strncpy(buff, line, size);
            notif_set_summary(data, buff);
        }
        else if (notif_get_body(data) == NULL) {
            char *buff;
            size_t size = strlen(line) + 1;

            buff = (char*) malloc(sizeof(char) * size);
            strncpy(buff, line, size);
            notif_set_body(data, buff);
        }
        else {
            printf("Too many arguments!\n");
            return_code = 1;
            break;
        }
    }

    /* notif_print_data(data); */

    if (return_code == 0 && notif_validate_data(data)) {
        int id;
        id = notif_send_notification(data);

        if (id == -1) {
            printf("Error: %s\n", notif_get_error_message());
            notif_free_error_message();
            return_code = 1;
        }
        else {
            printf("%i\n", id);
        }
    }
    else {
        printf("Usage:\n"
               "notify-desktop [OPTION...] <SUMMARY> [BODY] - create a notification\n");
    }

    notif_free_data(data);
    return return_code;
}
