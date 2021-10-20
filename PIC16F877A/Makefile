###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = PIC16F877A
COMPILER = "CCS for PIC"
TARGET = Debug
SHELL = C:\Windows\system32\cmd.exe

## Tools general options
CCFLAGS=-J +DF +ES -P -Z +O8 +CC +EXPORT +FM
LDFLAGS=-J +DF +ES -P -Z +O8 +CC +FM

## Processing Tools
CC=ccsc.exe
LD=ccsc.exe
#  C:/Users/LUCAS/Downloads/Projeto 11 (com projeto de inteface)/PIC16F877A/Debug.cof: TOOL=LD
#     C:/Users/LUCAS/Downloads/Projeto 11 (com projeto de inteface)/PIC16F877A/./main.o: TOOL=CC
#        C:/Users/LUCAS/Downloads/Projeto 11 (com projeto de inteface)/PIC16F877A/main.c

# Build tree:
all:	Debug

Debug:	Debug.cof

Debug.cof:	main.o 
	$(LD) $(LDFLAGS)  LINK="Debug.cof=main.o"

main.o:	main.c 
	$(CC) $(CCFLAGS)  "main.c"

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf ./Debug.esym
	rm -rf ./Debug.sym
	rm -rf ./Debug.lst
	rm -rf Debug.err
	rm -rf ./main.esym
	rm -rf ./main.osym
	rm -rf ./main.err

# cleanup - delete all generated files
clean:	tidy
	rm -rf Debug.cof
	rm -rf main.o

