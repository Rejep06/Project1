# Компилятор
CXX = g++

# Флаги компиляции
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror

# Флаги линковки
LDFLAGS =

# Если включена отладка
ifeq ($(DEBUG),1)
    CXXFLAGS += -g
else
    CXXFLAGS += -flto -DNDEBUG
    LDFLAGS  += -flto
endif

# Пути к заголовочным файлам
CXXFLAGS += -I $(abspath include)

# Поиск всех исходников
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(SOURCES:src/%.cpp=build/%.o)

# Итоговые исполняемые файлы
EXECUTABLE = build/get_pi
TEST_EXECUTABLE = build/test

# Главная цель
default: $(EXECUTABLE)

# Линковка для eval.cpp
$(EXECUTABLE): $(filter-out build/test.o, $(OBJECTS)) | build
	@printf "Linking executable $(EXECUTABLE)\n"
	$(CXX) $(LDFLAGS) $(filter-out build/test.o, $(OBJECTS)) -o $@

# Линковка для тестов (включаются все исходники кроме eval.cpp)
$(TEST_EXECUTABLE): $(filter-out src/get_pi.cpp, $(SOURCES)) | build
	@printf "Building test executable $(TEST_EXECUTABLE)\n"
	$(CXX) $(CXXFLAGS) $(filter-out src/get_pi.cpp, $(SOURCES)) -o $@

# Компиляция всех .cpp в .o
build/%.o: src/%.cpp | build
	@printf "Building object file $@\n"
	$(CXX) -c $< $(CXXFLAGS) -o $@

# Создание папки build
build:
	mkdir -p build

# Запуск eval (не линковать test.cpp)
run: $(EXECUTABLE)
	@printf "Running executable\n"
	@mkdir -p res
	@./$(EXECUTABLE) $(PRECISION)

# Запуск тестов
test: $(TEST_EXECUTABLE)
	@printf "Running tests\n"
	@./$(TEST_EXECUTABLE) 

# Очистка
clean:
	@printf "Cleaning build and resource directories\n"
	rm -rf res build

.PHONY: default run test clean
