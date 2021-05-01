CFLAGS=-Wall -Wextra -pedantic -std=c99 -O2 -g

bin/mkrom:main.o
	mkdir -p bin/
	$(CC) $(CFLAGS) $^ -o $@

DESTDIR=/usr
BINDIR=$(DESTDIR)/bin/

install: bin/mkrom
	mkdir -p $(BINDIR)
	cp bin/mkrom $(BINDIR)

uninstall:
	$(RM) $(BINDIR)/mkrom
