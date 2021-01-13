CC = gcc
SRC = *.c
OUTPUT = table_str

all: main.c
	${CC} ${SRC} -g -o ${OUTPUT}

win: main.c
	gcc.exe ${SRC} -g -o table_str.exe
