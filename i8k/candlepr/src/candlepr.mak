#
# Borland C++ IDE generated makefile
# Generated 01.01.2002 at 1:30:15 
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCCDOS  = Bcc +BccDos.cfg 
TLINK   = TLink
TLIB    = TLib
TASM    = Tasm
#
# IDE macros
#


#
# Options
#
IDE_LinkFLAGSDOS =  -LC:\BC5\LIB
IDE_BFLAGS = 
LinkerLocalOptsAtDOS_ZcbI8KbCANDLEPRbDISTbcandleprdexe =  -c -Tde -LC:\BC5\LIB;Z:\I8K\LIB
ResLocalOptsAtDOS_ZcbI8KbCANDLEPRbDISTbcandleprdexe = 
BLocalOptsAtDOS_ZcbI8KbCANDLEPRbDISTbcandleprdexe = 
CompInheritOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe = -IC:\BC5\INCLUDE;Z:\I8K\LIB\ -Di8000
LinkerInheritOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe = -x
LinkerOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe = $(LinkerLocalOptsAtDOS_ZcbI8KbCANDLEPRbDISTbcandleprdexe)
ResOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe = $(ResLocalOptsAtDOS_ZcbI8KbCANDLEPRbDISTbcandleprdexe)
BOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe = $(BLocalOptsAtDOS_ZcbI8KbCANDLEPRbDISTbcandleprdexe)

#
# Dependency List
#
Dep_candlepr = \
   Z:\I8K\CANDLEPR\DIST\candlepr.exe

candlepr : BccDos.cfg $(Dep_candlepr)
  echo MakeNode

Dep_ZcbI8KbCANDLEPRbDISTbcandleprdexe = \
   ..\..\lib\8080l.lib\
   ..\..\lib\8000e.lib\
   candle.obj\
   funcs.obj\
   io.obj\
   main.obj\
   modbus.obj\
   common.obj

Z:\I8K\CANDLEPR\DIST\candlepr.exe : $(Dep_ZcbI8KbCANDLEPRbDISTbcandleprdexe)
  $(TLINK)   @&&|
 /v $(IDE_LinkFLAGSDOS) $(LinkerOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) $(LinkerInheritOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) +
C:\BC5\LIB\c0l.obj+
candle.obj+
funcs.obj+
io.obj+
main.obj+
modbus.obj+
common.obj
$<,$*
..\..\lib\8080l.lib+
..\..\lib\8000e.lib+
C:\BC5\LIB\emu.lib+
C:\BC5\LIB\mathl.lib+
C:\BC5\LIB\cl.lib



|
candle.obj :  candle.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) $(CompInheritOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) -o$@ candle.c
|

funcs.obj :  funcs.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) $(CompInheritOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) -o$@ funcs.c
|

io.obj :  io.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) $(CompInheritOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) -o$@ io.c
|

main.obj :  main.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) $(CompInheritOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) -o$@ main.c
|

modbus.obj :  modbus.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) $(CompInheritOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) -o$@ modbus.c
|

common.obj :  common.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) $(CompInheritOptsAt_ZcbI8KbCANDLEPRbDISTbcandleprdexe) -o$@ common.c
|

# Compiler configuration file
BccDos.cfg : 
   Copy &&|
-W-
-w
-R
-v
-vi
-H
-H=candlepr.csm
-ml
-f
-1
-v-
-R-
-k-
| $@


