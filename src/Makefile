OBJS = main.o Grid.o Initializer.o Update.o Variables.o clhelp.o

UNAME_S := $(shell uname -s)

#check if linux
ifeq ($(UNAME_S), Linux)
OCL_INC=/usr/local/cuda-4.2/include
OCL_LIB=/usr/local/cuda-4.2/lib64
%.o: %.cpp clhelp.h
	g++ -fopenmp -O2 -msse3 -msse2 -msse -c $< -I$(OCL_INC)

all: $(OBJS)
	g++ -fopenmp -O3 -msse3 -msse2 -msse *.o -o solveFluid -L$(OCL_LIB) -lOpenCL
endif

#check if os x
ifeq ($(UNAME_S), Darwin)
%.o: %.cpp clhelp.h
	g++ -fopenmp -O2 -msse3 -msse2 -msse -c $<

all: $(OBJS)
	g++ -fopenmp -O3 -msse3 -msse2 -msse *.o -o solveFluid -framework OpenCL
endif

clean:
	rm -rf $(OBJS) solveFluid
