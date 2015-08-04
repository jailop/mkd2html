CC = gcc
LDLIBS = -lmarkdown
CFLAGS = -Wall -ansi -pedantic -g -O2 -pipe
# OBJCPY = objcopy \
		 --input binary \
		 --output elf64-x86-64 \
		 --binary-architecture i386 

# OBJCPY = objcopy \
		 --input binary \
		 --output elf32-i386 \
		 --binary-architecture i386 

OBJCPY = objcopy \
		--input binary \
		--output elf32-littlearm \
		--binary-architecture arm
OBJ = script.o style.o template.o nav.o math.o

all: mkd2web

mkd2web: mkd2web.c  $(OBJ) cstring.o

cstring.o: cstring.c cstring.h

script.o: script.js
	$(OBJCPY) script.js script.o

style.o: style.css
	$(OBJCPY) style.css style.o

template.o: template.html
	$(OBJCPY) template.html template.o

nav.o: nav.html
	$(OBJCPY) nav.html nav.o

math.o: math.js
	$(OBJCPY) $< $@

install: mkd2web
	cp mkd2web /usr/local/bin

clean:
	rm -f mkd2web 
	rm -f test
	rm -f *.o 
