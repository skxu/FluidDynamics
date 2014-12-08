rm ../outputs/sph-naive-no-grid.txt
cd ../src/sph-naive-no-grid
make clean
make
./solveFluid
cd ../../render-simple
make clean
make
./fluidsim -f ../outputs/sph-naive-no-grid.txt
cd ../scripts