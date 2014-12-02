rm ../outputs/$1
cd ../src
make clean
make
./solveFluid
cd ../render-simple
make clean
make
./fluidsim -f ../outputs/run.txt
cd ../scripts