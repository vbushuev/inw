TCROOT	= C:\TC
PROJECTDIR = Z:\inw\i8k\candlepr
CC 	= TCC.EXE
LINK 	= TLINK.EXE
CCP = $(TCROOT)\CCP.EXE
ICPLIB	= ..\lib
CCFLAGS = -ml -1 -V -vi-
CCFLAGS = $(CCFLAGS) -Di8000
CCFLAGS = $(CCFLAGS) -I$(TCROOT)\INCLUDE;$(ICPLIB)
#CCFLAGS = $(CCFLAGS) -L$(TCROOT)\LIB;$(ICPLIB)

LINKFLAGS = /x/c 
LINKFLAGS = $(LINKFLAGS) /L$(TCROOT)\LIB;$(ICPLIB)

NAME = candlepr
#LIB = $(ICPLIB)\8000e.lib $(ICPLIB)\8084w.lib
LIB = 8000e.lib 8084w.lib cl.lib
LIBOBJ = $(TCROOT)\LIB\c0l.obj

all: clean project
	dist\$(NAME).exe
clean:
	cmd /C del /Q /F dist\*,obj\main.obj
obj:
	$(CC) $(CCFLAGS) -c -oobj\main.obj src\main.c
#	$(CC) $(CCFLAGS) src\main.c
project: obj
	$(LINK) $(LINKFLAGS) $(LIBOBJ) obj\main.obj, dist\$(NAME).exe,,$(LIB)