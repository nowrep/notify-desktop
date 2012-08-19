notify-desktop
----------------------------------------------------------------------------------------

Little application that lets you send desktop notifications with one command.
It's basically clone of notify-send from libnotify, but it supports reusing notifications
on screen by passing its ID. It also does not use any external dependencies (except from
libdbus of course).  
I wrote it because I needed to reuse notifications on screen from scripts.

Compiling
----------------------------------------------------------------------------------------

Build dependencies are only pkg-config and libdbus.

    $ mkdir bin && mkdir build && cd src
    $ make
    $ sudo make install
    
Usage
----------------------------------------------------------------------------------------

From notify-desktop --help:

    $ Usage: notify-desktop [OPTION...] <SUMMARY> [BODY]
    $
    $ Options:
    $   -r, --replaces-id=ID        Specified the notifications ID that will be replaced
    $   -u, --urgency=LEVEL         Specifies the urgency level (low, normal, critical)
    $   -t, --expire-time=TIME      Specifies the timeout in milliseconds to expire the notification
    $   -a, --app-name=APP_NAME     Specifies the app name for the icon
    $   -i, --icon=ICON             Specifies an icon filename or stock icon to display
    $   -c, --category=TYPE         Specifies the notification category

ID of sent notification is written into standard output.

Examples:
    
    $ notify-desktop "Minimal notification"
    $
    $ notify-desktop --icon=call-start "Incoming call"
    $
    $ notify-desktop -i down -u low "Low urgency" "Body of low urgency notification"
    