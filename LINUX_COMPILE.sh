# Compile Engine - int
g++ -D_ENGINE_DLL Ventile/src/Ventile.cpp -lpthread -c -Wall -Wextra -fpic -o bin-int/Debug-x64/Ventile/ventile.o
g++ -D_ENGINE_DLL Ventile/src/Application.cpp -lpthread -c -Wall -Wextra -fpic -o bin-int/Debug-x64/Ventile/application.o
g++ -D_ENGINE_DLL Ventile/src/Keyboard.cpp -lpthread -c -Wall -Wextra -fpic -o bin-int/Debug-x64/Ventile/keyboard.o

# Compile engine - BIN
g++ -shared -o bin/Debug-x64/Ventile/libventile.so bin-int/Debug-x64/Ventile/ventile.o bin-int/Debug-x64/Ventile/application.o bin-int/Debug-x64/Ventile/keyboard.o

sudo cp bin/Debug-x64/Ventile/libventile.so /usr/lib

# Compile Sample
g++ Sandbox/src/main.cpp -lpthread -lventile -Wall -Wextra -O3 -o bin/Debug-x64/Sandbox/sandbox
