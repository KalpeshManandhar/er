clang++ -O3 -IE:/libraries/stb -I./src/ ./src/math/*.cpp ./src/Renderer/2D/**.cpp ./src/Loader/*.cpp ./src/tests/demo.cpp ./src/tests/shaders/*.cpp -o demo.exe -luser32 -lgdi32 -D"demo"
