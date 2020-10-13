
VERSION="1.0"

PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

CXX = g++

all: options i3sb

options:
	@echo i3sb build options:
	@echo "CXX       = ${CXX}"

i3sb: clean
	${CXX} i3sb.cpp -pthread -o i3sb
	strip i3sb
	
clean:
	rm -f i3sb
	
test: 
	@echo "PREFIX       = ${PREFIX}"
	@echo "MANPREFIX    = ${MANPREFIX}"
	
install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f i3sb ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/i3sb
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < i3sb.1 > ${DESTDIR}${MANPREFIX}/man1/i3sb.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/i3sb.1
	./blocksInstall ${DESTDIR} ${PREFIX}

uninstall:
	rm "${DESTDIR}${PREFIX}/bin/i3sb"*
	rm "${DESTDIR}${MANPREFIX}/man1/i3sb.1"

	



