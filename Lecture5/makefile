CC = gcc
CFLAGS = -O0 -W -Wall
LDFLAGS = -lpthread -lm

run : all

all : pi_threaded HeatDiffusion

pi_threaded : pi_threaded.c
HeatDiffusion : HeatDiffusion.c

clean :	
	@rm -rf pi_threaded HeatDiffusion *~