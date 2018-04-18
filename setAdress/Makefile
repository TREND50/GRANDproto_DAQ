CC=gcc
CFLAGS=-c -Wall -g3 -O0
SOURCES=libGedek/rawSocket.c libGedek/libGedek.c
OBJECTS=$(SOURCES:.c=.o)
OUT=libGedek.a
EXECUTABLE=configureGedek
LDFLAGS=-static

SETCAP=/sbin/setcap cap_net_raw=ep

all: $(EXECUTABLE)

$(OUT): $(OBJECTS)
	ar rcs $(OUT) $(OBJECTS)

$(EXECUTABLE): configureGedek.c $(OUT)
	$(CC) -o $(EXECUTABLE) configureGedek.c -g3 $(OUT)
	
setcap:
	$(SETCAP) $(EXECUTABLE)
	
clean:
	rm -f $(OBJECTS) $(OUT) $(EXECUTABLE)
