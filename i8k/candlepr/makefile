TCROOT = C:\TC
DIR = src
CC = TCC.EXE
LINK = TLINK.EXE
CCP = $(TCROOT)\CCP.EXE
ICPLIB	= ..\lib
CCFLAGS = -ml -1 -V -vi
CCFLAGS = $(CCFLAGS) -Di8000
CCFLAGS = $(CCFLAGS) -I$(TCROOT)\INCLUDE;$(ICPLIB);$(DIR)
#CCFLAGS = $(CCFLAGS) -L$(TCROOT)\LIB;$(ICPLIB)

LINKFLAGS = /x/c
LINKFLAGS = $(LINKFLAGS) /L$(TCROOT)\LIB;$(ICPLIB)

NAME = candlepr
LIB = mathl.lib emu.lib cl.lib 8000e.lib 8080l.lib
LIBOBJ = $(TCROOT)\LIB\c0l.obj
OBJ = obj\common.obj obj\io.obj obj\modbus.obj obj\main.obj obj\funcs.obj

all: project
	#echo $(NAME).exe >> dist\autoexec.bat

obj\common.obj: $(DIR)\common.c
		$(CC) $(CCFLAGS) -c -o$@ $?

obj\io.obj: $(DIR)\io.c
			$(CC) $(CCFLAGS) -c -o$@ $?

obj\main.obj: $(DIR)\main.c
	$(CC) $(CCFLAGS) -c -o$@ $?

obj\modbus.obj: $(DIR)\modbus.c
	$(CC) $(CCFLAGS) -c -o$@ $?

obj\funcs.obj: $(DIR)\funcs.c
	$(CC) $(CCFLAGS) -c -o$@ $?

project: $(OBJ)
	$(LINK) $(LINKFLAGS) @&&|
$(LIBOBJ)+
$(OBJ),+
dist\$(NAME).exe,,+
$(LIB)
|
#%.obj:%.c
#	$(CC) $(CCFLAGS) -c -oobj\$@ src\$(@,.obj=.c)
#	$(CC) $(CCFLAGS) -c $(.SOURCE)
#	$(CC) $(CCFLAGS) -c -oobj\$(.SOURCE) $(.SOURCE)
#	$(CC) $(CCFLAGS) src\main.c
