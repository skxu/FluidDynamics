rm ../outputs/sph-naive-grid.txt
cd ../src/sph-naive-grid
make clean
make
./solveFluid
cd ../../render-simple
make clean
make
./fluidsim -f ../outputs/sph-naive-grid.txt
cd ../scripts