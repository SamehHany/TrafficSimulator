CC=g++
CFLAGS= -lGL -lGLU -lGLEW -lglut -Wall -g
#=========================================================

DEPS = common.h  \
	   Point.h  \
	   Color.h  \
	   Object2D.h \
	   Rectangle.h \
	   Circle.h \
	   HollowCircle.h \
	   Road.h \
	   UTurn.h \
	   Car.h \
	   CarList.h \
	   CollisionGrid.h

OBJ = Object2D.o \
	  Object2D.o  \
	  Rectangle.o  \
	  Circle.o \
	  HollowCircle.o \
	  Road.o \
	  UTurn.o \
	  Car.o \
	  CarList.o \
	  CollisionGrid.o \
	  main.o InitShader.o 

#=========================================================

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all : trafsimu

trafsimu : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *o trafsimu
