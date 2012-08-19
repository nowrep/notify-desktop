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

install:
	$(INSTALL_PROGRAM) $(TARGETDIR)/$(TARGET) /usr/bin/$(TARGET)
    
uninstall:
	$(DEL_FILE) /usr/bin/$(TARGET)
    
