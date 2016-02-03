#=========================================================
#		*General Macro Define*
#=========================================================
TCROOT	= C:\TC
CC 	= $(TCROOT)\TCC.EXE
TLINK 	= $(TCROOT)\TLINK.EXE
ICPLIB	= C:\ICPDAS\LIB

#=========================================================
#		*Implicit  Rules*
#=========================================================
.c .obj:
	$(CC) -c {\obj\$< }


#=========================================================
#		*Target Macro Define*
#=========================================================
TARGET=dist
DEFINES=i8000
OBJS=obj\*.obj
INCS=C:\ICPDAS\LIB
LIBS=

CC=$(CC) +$(TARGET).CFG

#=========================================================
#		*Explicit Rules*
#=========================================================
$(TARGET).exe: $(TARGET).cfg $(OBJS) $(LIBS)
  $(TLINK) /x/c @&&|
$(TCROOT)\lib\c0l.obj+
$(OBJS)
$(TARGET),
{$MakeParams->ICPDAS_LIBS}
{$MakeParams->CC_LIBS}
|
#  -@del $(TARGET).cfg

