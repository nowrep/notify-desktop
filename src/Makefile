#############################################################################
# Makefile for building: notify-desktop
#############################################################################

TARGET = notify-desktop
OBJ = main.o notif.o dbusimp.o
OBJECTSDIR = ../build
TARGETDIR = ../bin

DEL_FILE = rm -f
INSTALL_PROGRAM = install -m 755 -p
MKDIR = mkdir -p
CC = cc
CFLAGS	+= -Wall -Wextra -pedantic $(shell pkg-config --cflags dbus-1)
LIBS 	+= $(shell pkg-config --libs dbus-1)

ifeq ($(BUILD),debug)
CFLAGS += -O0 -g
else
CFLAGS += -O2 -s -DNDEBUG
endif

OBJECTS = $(patsubst %,$(OBJECTSDIR)/%,$(OBJ))

$(OBJECTSDIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(CFLAGS) $(LIBS)

debug:
	make "BUILD=debug"

clean:
	rm -f $(OBJECTSDIR)/*.o *~ core

install:
	$(INSTALL_PROGRAM) $(TARGETDIR)/$(TARGET) /usr/bin/$(TARGET)

uninstall:
	$(DEL_FILE) /usr/bin/$(TARGET)

