CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DOSX
INCFLAGS = -I./glm-0.9.2.7 -I/usr/X11/include -I./include/
LDFLAGS = -framework GLUT -framework OpenGL \
		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-lGL -lGLU -lm -lstdc++
else
CFLAGS = -g -DGL_GLEXT_PROTOTYPES 
INCFLAGS = -I./glm-0.9.2.7 -I./include/ -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lglut -lGLU -lGL -lX11 -lfreeimage
endif

RM = /bin/rm -f 
all: fluidsim
fluidsim: main.o shaders.o Transform.o scene.o table.o entity.o container.o
	$(CC) $(CFLAGS) -o fluidsim shaders.o main.o Transform.o scene.o table.o container.o entity.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp shaders.h Transform.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
shaders.o: shaders.cpp shaders.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
scene.o: scene.cpp scene.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c scene.cpp
Transform.o: Transform.cpp Transform.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp 
entity.o: entity.cpp entity.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c entity.cpp 
table.o: table.cpp table.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c table.cpp
container.o: container.cpp container.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c container.cpp
clean: 
	$(RM) *.o fluidsim


 
