rm ../outputs/$1
cd ../src
make clean
make
./solveFluid -i ../inputs/init.txt -o ../outputs/$1
cd ../render-simple
make clean
make
./fluidsim -f ../outputs/$1
cd ../scripts