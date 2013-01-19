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

static int parse_urgency(char* string)
{
    if (strcmp(string, "low") == 0)
        return NOTIF_URGENCY_LOW;
    else if (strcmp(string, "normal") == 0)
        return NOTIF_URGENCY_NORMAL;
    else if (strcmp(string, "critical") == 0)
        return NOTIF_URGENCY_CRITICAL;

    return NOTIF_ERROR;
}

static char* crop_argument(char* source, int offset)
{
    size_t size = strlen(source) - offset;
    if (size <= 0)
        return NULL;

    char* string;
    string = (char*) malloc(sizeof(char) * (size + 1));

    strncpy(string, source + offset, size);
    string[size] = '\0';

    return string;
}

static void show_help(void)
{
    const char* help = ""
            "Usage:\n"
            "notify-desktop [OPTION...] <SUMMARY> [BODY] - create a notification\n"
            "\n"
            "Help Options:\n"
            "  -h, --help               Show help options\n"
            "  -v, --version            Version of the application\n"
            "\n"
            "Application Options:\n"
            "  -r, --replaces-id=ID     Specified the notifications ID that will be replaced\n"
            "  -u, --urgency=LEVEL      Specifies the urgency level (low, normal, critical)\n"
            "  -t, --expire-time=TIME   Specifies the timeout in ms to expire the notification\n"
            "  -a, --app-name=APP_NAME  Specifies the app name for the icon\n"
            "  -i, --icon=ICON          Specifies an icon filename or stock icon to display\n"
            "  -c, --category=TYPE      Specifies the notification category\n"
            "\n"
            "Application Output:\n"
            "   On success:             Prints ID of sent notification and returns 0\n"
            "   On failure:             Prints error and returns 1\n\n";

    printf(help);
}

static void show_version(void)
{
    printf("notify-desktop 0.1.0\n");
}

int main(int argc, char** argv)
{
    if (argc < 1) {
        show_help();
        return 1;
    }

    int return_code;
    return_code = 0;

    NotifyData* data;
    data = notif_create_data();

    int i;
    for (i = 1; i < argc; ++i) {
        char* line = argv[i];
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

            data->replaces_id = atoi(argv[++i]);
        }
        else if (strncmp(line, "--replaces-id=", 14) == 0) {
            size_t size = strlen(line) - 14;
            if (size <= 0)
                continue;

            char buf[size];
            strncpy(buf, line + 14, size);
            buf[size - 1] = '\0';

            data->replaces_id = atoi(buf);
        }
        else if (strcmp(line, "-u") == 0) {
            if (argc <= i)
                continue;

            int urgency = parse_urgency(argv[++i]);
            if (urgency != NOTIF_ERROR)
                data->urgency = urgency;
        }
        else if (strncmp(line, "--urgency=", 10) == 0) {
            size_t size = strlen(line) - 10;
            if (size <= 0)
                continue;

            char buf[size];
            strncpy(buf, line + 10, size);
            buf[size - 1] = '\0';

            int urgency = parse_urgency(buf);
            if (urgency != NOTIF_ERROR)
                data->urgency = urgency;
        }
        else if (strcmp(line, "-t") == 0) {
            if (argc <= i)
                continue;

            data->expire_time = atoi(argv[++i]);
        }
        else if (strncmp(line, "--expire-time=", 14) == 0) {
            size_t size = strlen(line) - 14;
            if (size <= 0)
                continue;

            char buf[size];
            strncpy(buf, line + 14, size);
            buf[size - 1] = '\0';

            data->expire_time = atoi(buf);
        }
        else if (strcmp(line, "-a") == 0) {
            if (argc <= i)
                continue;

            char* value = argv[++i];
            size_t size = strlen(value) + 1;
            data->app_name = (char*) malloc(sizeof(char) * size);

            strncpy(data->app_name, value, size);
        }
        else if (strncmp(line, "--app-name=", 11) == 0) {
            char* name = crop_argument(line, 11);
            if (name != NULL)
                data->app_name = name;
        }
        else if (strcmp(line, "-i") == 0) {
            if (argc <= i)
                continue;

            char* value = argv[++i];
            size_t size = strlen(value) + 1;
            data->icon = (char*) malloc(sizeof(char) * size);

            strncpy(data->icon, value, size);
        }
        else if (strncmp(line, "--icon=", 7) == 0) {
            char* value = crop_argument(line, 7);
            if (value != NULL)
                data->icon = value;
        }
        else if (strcmp(line, "-c") == 0) {
            if (argc <= i)
                continue;

            char* value = argv[++i];
            size_t size = strlen(value) + 1;
            data->category = (char*) malloc(sizeof(char) * size);

            strncpy(data->category, value, size);
        }
        else if (strncmp(line, "--category=", 11) == 0) {
            char* value = crop_argument(line, 11);
            if (value != NULL)
                data->category = value;
        }
        else if (strncmp(line, "-", 1) == 0) {
            printf("Unknown option \"%s\"", line);
            return_code = 1;
            break;
        }
        else if (data->summary == NULL) {
            size_t size = strlen(line) + 1;
            data->summary = (char*) malloc(sizeof(char) * size);

            strncpy(data->summary, line, size);
        }
        else if (data->body == NULL) {
            size_t size = strlen(line) + 1;
            data->body = (char*) malloc(sizeof(char) * size);

            strncpy(data->body, line, size);
        }
        else {
            printf("Too many arguments!\n");
            return_code = 1;
            break;
        }
    }

    //notif_print_data(data);

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
