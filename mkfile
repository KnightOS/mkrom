</$objtype/mkfile

mkrom: main.c
	pcc -B -o $target $prereq

install: mkrom
	mkdir -p /amd64/bin/knightos/
	cp mkrom /amd64/bin/knightos/
