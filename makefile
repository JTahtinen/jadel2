CC=cl
CFLAGS=/I$(IDIR)
ODIR=obj
source:=$(wildcard *.cpp)
objects := $(patsubst %.cpp,%.obj,$(wildcard *.cpp))

.PHONY:
all: $(objects)
	$(CC) /Zi /O2 /LD $^ gdi32.lib user32.lib /Feexport\lib\jadel
	copy *.h export\include\ 
	
jadelmain: mainobjects
	lib -out:W:\jadel2\export\lib\jadelmain.lib main/*.obj

mainobjects:
	$(CC) /c /O2 /Zi /EHsc /std:c++latest main/jadelmain.cpp /Fomain/

%.obj: %.cpp
	$(CC) /Zi /O2 /c /DJADEL_EXPORT $^

.PHONY:	
clean:
	del *.obj export\lib\jadel.* *.lib