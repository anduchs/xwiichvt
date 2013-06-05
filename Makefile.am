
EXTRA_DIST = README \
	         COPYING
#TODO: add sysVinit, systemd unit files

man1_MANS = doc/xwiichvt.man
EXTRA_DIST += $(man_MANS)

bin_PROGRAMS = xwiichvt

xwiichvt_SOURCES = src/xwiichvt.c
xwiichvt_CFLAGS = $(REQS_CFLAGS) -Wall -Wextra -Werror -O0
xwiichvt_LDADD = $(REQS_LIBS)

SUFFIXES: .man

%.man: %.ronn
	@RONN@ --roff --pipe \
	--manual=@PACKAGE@ \
	--organization=@AUTHOR@ \
	< $< > $@


