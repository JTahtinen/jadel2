 
.PHONY:
all: *.obj
	cl /Zi /O2 /LD *.obj gdi32.lib user32.lib /Feexport\lib\jadel
	copy *.h export\include\ 
	
jadelmain: mainobjects
	lib -out:W:\jadel2\export\lib\jadelmain.lib main/*.obj

mainobjects:
	cl /c /O2 /Zi /EHsc /std:c++latest main/jadelmain.cpp /Fomain/

*.obj: *.cpp
	cl /Zi /O2 /c /DJADEL_EXPORT *.cpp 

.PHONY:	
clean:
	del *.obj export\lib\jadel.* *.lib