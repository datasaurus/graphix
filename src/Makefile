#- Makefile --
#-	This is the make file for applications.
#-	
# Copyright (c) 2011, Gordon D. Carrie. All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
#     * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Please send feedback to dev0@trekix.net
#
# Where to install things
#PREFIX=/usr/local
PREFIX=${HOME}/local
BINDIR=${PREFIX}/bin
MAN1DIR=${PREFIX}/share/man/man1
MAN3DIR=${PREFIX}/share/man/man3

# Options
CC = cc
CFLAGS = -std=c99 -g -Wall -Wmissing-prototypes
EXECS = hsv color_legend bighi biglo biglrg bigsml
SCRIPTS = raster_clrs
# EFENCE_LIBS = -L/opt/local/lib -lefence
LIBS = ${EFENCE_LIBS} -lm

all : ${EXECS}

install : ${EXECS}
	mkdir -p ${BINDIR}
	cp -f -p ${EXECS} ${SCRIPTS} ${BINDIR}
	mkdir -p ${MAN1DIR}
	cp -f -p ../man/man1/*.1 ${MAN1DIR}

OBJ = hsv_app.o hsv_lib.o
hsv : ${OBJ}
	${CC} ${CFLAGS} -o hsv ${OBJ} -lm

hsv_app.o : hsv_app.c hsv_lib.c hsv_lib.h
	${CC} ${CFLAGS} -c hsv_app.c

hsv_lib.o : hsv_lib.c hsv_lib.h
	${CC} ${CFLAGS} -c hsv_lib.c

color_legend : color_legend.c
	${CC} ${CFLAGS} -o color_legend color_legend.c ${LIBS}

bighi : bighi.c
	${CC} ${CFLAGS} -o bighi bighi.c -lm

biglo : biglo.c
	${CC} ${CFLAGS} -o biglo biglo.c -lm

biglrg : biglrg.c
	${CC} ${CFLAGS} -o biglrg biglrg.c -lm

bigsml : bigsml.c
	${CC} ${CFLAGS} -o bigsml bigsml.c -lm

get_colors.o : get_colors.c
	${CC} ${CFLAGS} -c get_colors.c

clean :
	rm -fr *core* *.o ${EXECS} *.dSYM
