$STB_DIR = "E:/libraries/stb"

clang++ -O3 -IE:/libraries/stb -I./src/ ./src/math/*.cpp ./src/Renderer/2D/**.cpp ./src/Loader/*.cpp ./src/tests/*.cpp ./src/tests/shaders/*.cpp -o test.exe -luser32 -lgdi32 