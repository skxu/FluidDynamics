rm ../outputs/sph-openmp-v1.txt
cd ../src/sph-openmp-v1
make clean
make
./solveFluid
cd ../../render-simple
make clean
make
./fluidsim -f ../outputs/sph-openmp-v1.txt
cd ../scripts