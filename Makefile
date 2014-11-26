CC = gcc
CFLAGS = -g
CFILESSEARCH = indexer.c format.c inverted-list.c main.c
CFILESINDEX = indexer.c format.c inverted-list.c main1.c
HFILES = headers.h

all: indexer search

indexer : $(CFILESINDEX)
	$(CC) $(CFLAGS) -o indexer $(CFILESINDEX)

search : $(CFILESSEARCH)
	$(CC) $(CFLAGES) -o search $(CFILESSEARCH)

archive: main
	ar -r libsl.a $(HFILES)

clean:
	rm -rf *.o
