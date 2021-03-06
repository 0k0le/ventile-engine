# Compile Engine - int
g++ -D_ENGINE_DLL Ventile/src/Ventile.cpp -c -Wall -Wextra -fpic -o bin-int/Debug-x64/Ventile/ventile.o
g++ -D_ENGINE_DLL Ventile/src/Application.cpp -lpthread -c -Wall -Wextra -fpic -o bin-int/Debug-x64/Ventile/application.o
g++ -D_ENGINE_DLL Ventile/src/Keyboard.cpp -lpthread -c -Wall -Wextra -fpic -o bin-int/Debug-x64/Ventile/keyboard.o
g++ -D_ENGINE_DLL Ventile/src/Mouse.cpp -lpthread -c -Wall -Wextra -fpic -o bin-int/Debug-x64/Ventile/mouse.o
g++ -D_ENGINE_DLL Ventile/src/Logging.cpp -c -Wall -Wextra -fpic -o bin-int/Debug-x64/Ventile/logging.o
g++ -D_ENGINE_DLL Ventile/src/Window.cpp $(pkg-config --static --libs glfw3) -lvulkan -c -Wall -Wextra -fpic -o bin-int/Debug-x64/Ventile/window.o
g++ -D_ENGINE_DLL Ventile/src/Renderer.cpp $(pkg-config --static --libs glfw3) -lvulkan -c -Wall -Wextra -fpic -o bin-int/Debug-x64/Ventile/renderer.o

# Compile engine - BIN
g++ -shared -o bin/Debug-x64/Ventile/libventile.so bin-int/Debug-x64/Ventile/ventile.o bin-int/Debug-x64/Ventile/application.o bin-int/Debug-x64/Ventile/keyboard.o bin-int/Debug-x64/Ventile/mouse.o bin-int/Debug-x64/Ventile/logging.o bin-int/Debug-x64/Ventile/window.o bin-int/Debug-x64/Ventile/renderer.o

sudo cp bin/Debug-x64/Ventile/libventile.so /usr/lib

# Compile Sample
g++ Sandbox/src/main.cpp -lventile -lvulkan -lglfw -lpthread -Wall -Wextra -O3 -o bin/Debug-x64/Sandbox/sandbox
