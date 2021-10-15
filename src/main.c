/* ============================================================
* notify-desktop - sends desktop notifications
* Copyright (C) 2012-2015 David Rosca <nowrep@gmail.com>
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
#include <getopt.h>

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
           "  -r, --replaces-id=ID     Specifies the notifications ID that will be replaced\n"
           "  -R, --id-file=PATH       Specifies the path to a file for storing the id that will be replace\n"
           "  -u, --urgency=LEVEL      Specifies the urgency level (low, normal, critical)\n"
           "  -t, --expire-time=TIME   Specifies the timeout in ms to expire the notification\n"
           "  -a, --app-name=APP_NAME  Specifies the app name for the icon\n"
           "  -i, --icon=ICON          Specifies an icon filename or stock icon to display\n"
           "  -c, --category=TYPE      Specifies the notification category\n"
           "\n");
    printf("Application Output:\n"
           "   On success:             Prints ID of sent notification and returns 0\n"
           "   On failure:             Prints error and returns 1\n"
           "\n");
}

static void show_version(void)
{
    printf("notify-desktop 0.2.0\n");
}

int main(int argc, char **argv)
{
    struct NotifyData *data;
    int opt, urgency;

    static struct option options[] = {
        { "help", no_argument, 0, 'h' },
        { "version", no_argument, 0, 'v' },
        { "replaces-id", required_argument, 0, 'r' },
        { "urgency", required_argument, 0, 'u' },
        { "expire", required_argument, 0, 't' },
        { "app-name", required_argument, 0, 'a' },
        { "id-file", required_argument, 0, 'R' },
        { "icon", required_argument, 0, 'i' },
        { "category", required_argument, 0, 'c' },
        { 0, 0, 0, 0 }
    };

    if (argc < 1) {
        show_help();
        return 1;
    }

    data = notif_create_data();

    /* options */
    while ((opt = getopt_long(argc, argv,"hvr:R:u:t:a:i:c:", options, NULL )) != -1) {
        switch (opt) {
        case 'h' :
            show_help();
            notif_free_data(data);
            return 0;

        case 'v':
            show_version();
            notif_free_data(data);
            return 0;

        case 'r':
            if (notif_get_id_file(data) != stdout) {
                printf("-r and -R are incompatible options\n");
                goto error;
            }
            notif_set_replaces_id(data, atoi(optarg));
            break;

        case 'R': {
            if (notif_get_replaces_id(data) != 0) {
                printf("-r and -R are incompatible options\n");
                goto error;
            }
            FILE *file = fopen(optarg, "r+");
            if (file == NULL) {
                perror("Could not open the id_file");
                goto error;
            }
            notif_set_id_file(data, file);
            notif_set_replaces_id_from_file(data);
            break;
        }
        case 'u':
            urgency = parse_urgency(optarg);
            if (urgency == NOTIF_ERROR) {
                printf("Invalid urgency value!\n");
                goto error;
            }
            notif_set_urgency(data, urgency);
            break;

        case 't':
            notif_set_expire_time(data, atoi(optarg));
            break;

        case 'a':
            notif_set_app_name(data, optarg);
            break;

        case 'i':
            notif_set_icon(data, optarg);
            break;

        case 'c':
            notif_set_category(data, optarg);
            break;

        default:
            printf("Usage:\nnotify-desktop [OPTION...] <SUMMARY> [BODY] - create a notification\n");
            goto error;
        }
    }

    /* summary + body */
    for (; optind < argc; ++optind) {
        if (notif_get_summary(data) == NULL) {
            notif_set_summary(data, argv[optind]);
        }
        else if (notif_get_body(data) == NULL) {
            notif_set_body(data, argv[optind]);
        }
        else {
            printf("Too many arguments!\n");
            goto error;
        }
    }

    /* notif_print_data(data); */

    if (notif_validate_data(data)) {
        int id;
        id = notif_send_notification(data);

        if (id == -1) {
            printf("Error: %s\n", notif_get_error_message());
            notif_free_error_message();
            goto error;
        }
        else {
            fprintf(notif_get_id_file(data), "%i\n", id);
        }
    }

    notif_free_data(data);
    return 0;

error:
    notif_free_data(data);
    return 1;
}
