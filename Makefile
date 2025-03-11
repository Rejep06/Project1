# Определение компилятора и флагов
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

# Определение исходных файлов и объекта
SRC = src/expression.cpp src/eval.cpp
OBJ = $(SRC:.cpp=.o)
BIN = bin/program

# Основная цель компиляции
all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $(BIN)

# Цель для компиляции .cpp в .o файлы
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Цель для запуска программы после компиляции
run: $(BIN)
	./$(BIN)

# Очищаем промежуточные файлы
clean:
	rm -f $(OBJ) $(BIN)
