rm ../outputs/run.txt
cd ../src
make clean
make
./solveFluid
cd ../render-simple
make clean
make
./fluidsim -f ../outputs/run.txt
cd ../scripts