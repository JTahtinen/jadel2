 
.PHONY:
all: wt
	cl /c /Zi /EHsc /std:c++latest *.o wt.lib

main: winAppTest.cpp
	cl /c /Zi /EHsc /std:c++latest $^ /Fewt.lib
	lib -out:wt.lib *.obj

wt: objects
	g++ *.cpp -c
	
objects: *.cpp
	g++ -c $^

%.o: %.cpp
	g++ -c $^

.PHONY:	
clean:
	del *.o *.exe wt.dll *.a *.lib