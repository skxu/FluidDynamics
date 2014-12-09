rm ../outputs/sph-openmp-simd.txt
cd ../src/sph-openmp-simd
make clean
make
./solveFluid
cd ../../render-simple
make clean
make
./fluidsim -f ../outputs/sph-openmp-simd.txt
cd ../scripts