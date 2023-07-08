CFLAGS=-Wall -Wextra -pedantic -std=c99 -O2 -g

all: bin/mkrom bin/mkrom.1

bin/mkrom:main.o
	mkdir -p bin/
	$(CC) $(CFLAGS) $^ -o $@

bin/mkrom.1:mkrom.1.scdoc
	scdoc < mkrom.1.scdoc > bin/mkrom.1
	
DESTDIR=/usr/local
BINDIR=$(DESTDIR)/bin/
MANDIR=$(DESTDIR)/share/man/

install: bin/mkrom
	mkdir -p $(BINDIR)
	cp bin/mkrom $(BINDIR)

install_man: bin/mkrom.1
	mkdir -p $(MANDIR)/man1/
	cp bin/mkrom.1 $(MANDIR)/man1/

uninstall:
	$(RM) $(BINDIR)/mkrom $(MANDIR)/man1/mkrom.1

clean:
	$(RM) bin *.o -rv

