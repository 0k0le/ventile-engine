g++ Ventile/src/Ventile.cpp -c -Wall -Wextra -O3 -o Ventile/src/libventile.o
g++ -shared -o bin/Debug-x64/Ventile/libventile.so Ventile/src/libventile.o

sudo cp bin/Debug-x64/Ventile/libventile.so /usr/lib

g++ Sandbox/src/main.cpp -lventile -Wall -Wextra -O3 -o bin/Debug-x64/Sandbox/sandbox
