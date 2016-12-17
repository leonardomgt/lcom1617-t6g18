# Makefile for lab4

COMPILER_TYPE= gnu

CC= gcc

PROG= quiz
SRCS= quiz.c timer.c keyboard.c video_gr.c utils.c statemachine.c

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys



LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
