#############################################################################
# Makefile for building: notify-desktop
#############################################################################

TARGET = notify-desktop
OBJ = main.o notif.o dbusimp.o
OBJECTSDIR = ../build
TARGETDIR = ../bin

MKDIR = mkdir -p
CC = c99
CFLAGS	+= -Wall -pedantic -g $(shell pkg-config --cflags dbus-1)
LDFLAGS	+= -g
LIBS 	+= $(shell pkg-config --libs dbus-1)

OBJECTS = $(patsubst %,$(OBJECTSDIR)/%,$(OBJ))

$(OBJECTSDIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTSDIR)/*.o *~ core
