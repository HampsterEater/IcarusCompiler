CC=g++
CCFLAGS=-g -Wall
#-static-libgcc
AR=ar
ARFLAGS=cruv

LIBNAME=..\Base\lib\AppStub_MW.a

OBJS=AppStub.o Long.o IO.o Class.o Debug.o String.o Bool.o Float.o Int.o Double.o Exception.o Exception_asm.o

all: $(OBJS)
	$(AR) $(ARFLAGS) $(LIBNAME) $(OBJS)

AppStub.o: AppStub.cpp AppStub.h IO.h Long.h
	g++ -Wall -O3 -c AppStub.cpp

Debug.o: Debug.cpp Debug.h AppStub.h
	g++ -Wall -O3 -c Debug.cpp

IO.o: IO.cpp IO.h AppStub.h String.h
	g++ -Wall -O3 -c IO.cpp
   
String.o: String.cpp String.h AppStub.h Class.h
	g++ -Wall -O3 -c String.cpp

Class.o: Class.cpp Class.h AppStub.h String.h
	g++ -Wall -O3 -c Class.cpp

Long.o: Long.cpp Long.h AppStub.h String.h
	g++ -Wall -O3 -c Long.cpp

Bool.o: Bool.cpp Bool.h AppStub.h String.h
	g++ -Wall -O3 -c Bool.cpp

Float.o: Float.cpp Float.h AppStub.h String.h
	g++ -Wall -O3 -c Float.cpp

Int.o: Int.cpp Int.h AppStub.h String.h
	g++ -Wall -O3 -c Int.cpp
	
Double.o: Double.cpp Double.h AppStub.h String.h
	g++ -Wall -O3 -c Double.cpp

Exception.o: Exception.cpp Exception.h AppStub.h String.h
	g++ -Wall -O3 -c Exception.cpp

Exception_asm.o: Exception_asm.s
	g++ -Wall -O3 -c Exception_asm.s
			
clean: 
	del $(OBJS) $(LIBNAME)