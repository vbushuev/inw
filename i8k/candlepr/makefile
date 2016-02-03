#=========================================================
#		*Commmon defines*
#=========================================================
TCROOT	= C:\TC
CC 	= $(TCROOT)\TCC.EXE
LINK 	= $(TCROOT)\TLINK.EXE
CCP = $(TCROOT)\CCP.EXE
ICPLIB	= C:\ICPDAS\LIB
TARGETDIR = dist
CCFLAGS = -ml -1 -V -vi-
CCFLAGS += -Di8000
CCFLAGS += -I$(TCROOT)\INCLUDE;$(ICPLIB);src
CCFLAGS += -L$(TCROOT)\LIB;$(ICPLIB)
OBJS = $(SOURCES:.c=obj/.obj)
LIBS =
#=========================================================
#		*Project Defines*
#=========================================================
NAME = candlepr
SOURCES = src/main.c
TARGET = $(TARGETDIR)/$(NAME)
LIBS+=
#=========================================================
#		*Rules*
#=========================================================
$(TARGET): $(OBJS) $(LIBS)
