$STB_DIR = "E:/libraries/stb"

clang++ -O3 -IE:/libraries/stb -I./src/ ./src/math/*.cpp ./src/Renderer/2D/**.cpp ./src/tests/*.cpp -o test.exe -luser32 -lgdi32 