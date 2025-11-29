CC := gcc
CFLAGS := -Wall -O2
LDFLAGS := -lresolv

all: mini-dig-safe mini-dig-simple

mini-dig-safe: mini-dig.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

mini-dig-simple: mini-dig.c
	$(CC) $(CFLAGS) -DUSE_SIMPLE_TXT -o $@ $< $(LDFLAGS)

clean:
	rm -f mini-dig-safe mini-dig-simple
