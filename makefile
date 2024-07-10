CC=cl
CFLAGS=/I$(IDIR)
ODIR=obj
IDIR=include
source:=$(wildcard *.cpp)
_OBJ=*.obj
DEBUGDEFS=/DDEBUG /DJADEL_EXPORT
RELEASEDEFS=/DRELEASE /DJADEL_EXPORT /O2
objects := $(patsubst %.cpp,%.obj,$(wildcard *.cpp))
OBJ=$($(wildcard patsubst %,$(ODIR)/%,$(_OBJ)))
debugobjects:=$(patsubst %.cpp,obj/debug/%.obj,$(wildcard *.cpp))
releaseobjects:=$(patsubst %.cpp,obj/release/%.obj,$(wildcard *.cpp))

.PHONY:
all: release

obj/debug/%.obj: %.cpp
	$(CC) /Zi /I$(IDIR) $(DEBUGDEFS) /c $^ /Foobj/debug/

obj/release/%.obj: %.cpp
	$(CC) /Zi /I$(IDIR) $(RELEASEDEFS) /c $^ /Foobj\release/

.PHONY:
release: $(releaseobjects)
	$(CC) /Zi /O2 /LD $^ gdi32.lib user32.lib /Feexport\lib\jadel
	copy *.h export\include\ 
	
.PHONY:
debug: $(debugobjects)
	$(CC) /Zi /LD $^ gdi32.lib user32.lib /Feexport\lib\jadelDEBUG
	copy *.h export\include\ 

.PHONY:
jadelmain: mainobjects
	lib -out:W:\jadel\export\lib\jadelmain.lib main/*.obj Shell32.lib 

.PHONY:
mainobjects:
	$(CC) /c /O2 /Zi /EHsc /std:c++latest main/jadelmain.cpp /Fomain/

.PHONY:
cleandebug:
	del obj\debug\*.obj export\lib\jadelDEBUG*

.PHONY:
cleanrelease:
	del obj\release\*.obj export\lib\jadel.*

.PHONY:
cleanmain: 
	del export\lib\jadelmain.lib

.PHONY:	
clean: cleandebug cleanrelease