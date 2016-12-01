# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h -I../h

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:gamedemo.elf

#additional rules for files
gamedemo.elf: ${COMMON_OBJECTS} gamedemo.o wdt_handler.o sound.o
	${CC} -mmcu=${CPU}   -o $@ $^ -L../lib -lTimer -lLcd -lShape -lCircle -lp2sw

load: gamedemo.elf
	mspdebug rf2500 "prog $^"

sound.c: sound.h

clean:
	rm -f *.o *.elf
