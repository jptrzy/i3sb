.POSIX:

# i3sb version
VERSION = 3.0.0

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man
BLOCKS = Blocks


all: options i3sb

options:
	@echo i3sb build options:

i3sb: clean
	go build 
	strip $@
	
clean:
	rm -f i3sb

install: all install_blocks
#	mkdir -p ${HOME}/.config/i3sb
#	cp config.json ${HOME}/.config/i3sb/config.json
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f i3sb ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/i3sb
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < i3sb.1 > ${DESTDIR}${MANPREFIX}/man1/i3sb.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/i3sb.1
#	./blocksInstall ${DESTDIR} ${PREFIX}

install_blocks: $(BLOCKS)/*
	for file in $^ ; do \
		[[ -f $${file} ]] && \
		cp $(BLOCKS)/$${file#*/} ${DESTDIR}${PREFIX}/bin/i3sb-$${file#*/} && \
		chmod 755 ${DESTDIR}${PREFIX}/bin/i3sb-$${file#*/} && \
		echo "Install" $${file#*/} "block." ; \
	done

uninstall:
	rm "${DESTDIR}${PREFIX}/bin/i3sb" & \
	rm "${DESTDIR}${PREFIX}/bin/i3sb-"* & \
	rm "${DESTDIR}${MANPREFIX}/man1/i3sb.1"
