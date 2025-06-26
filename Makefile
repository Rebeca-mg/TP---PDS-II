# Makefile para o Projeto Simon Game C++11
# Projeto Acadêmico - 2025

# Configurações do compilador
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -O2
DEBUG_FLAGS = -std=c++11 -Wall -Wextra -Wpedantic -g -DDEBUG
TEST_FLAGS = -std=c++11 -Wall -Wextra -Wpedantic -g

# Diretórios
SRCDIR = src
INCDIR = include
TESTDIR = tests
OBJDIR = obj
BINDIR = bin
DOCDIR = docs

# Arquivos fonte
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
MAIN_OBJ = $(OBJDIR)/main.o
GAME_OBJECTS = $(filter-out $(MAIN_OBJ), $(OBJECTS))

# Testes
TEST_SOURCES = $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.cpp=$(OBJDIR)/%.o)

# Executáveis
EXECUTABLE = $(BINDIR)/simon_game
TEST_EXECUTABLE = $(BINDIR)/test_runner

# Alvos principais
.PHONY: all clean debug test docs install help

all: $(EXECUTABLE)

# Compilação do executável principal
$(EXECUTABLE): $(OBJECTS) | $(BINDIR)
	@echo "🔗 Linkando executável principal..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "✅ Executável criado: $@"

# Compilação dos objetos principais
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "🔨 Compilando: $<"
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Compilação do main.cpp
$(OBJDIR)/main.o: main.cpp | $(OBJDIR)
	@echo "🔨 Compilando: $<"
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Executável de testes
$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(GAME_OBJECTS) | $(BINDIR)
	@echo "🔗 Linkando executável de testes..."
	$(CXX) $(TEST_FLAGS) -o $@ $^
	@echo "✅ Executável de testes criado: $@"

# Compilação dos testes
$(OBJDIR)/%.o: $(TESTDIR)/%.cpp | $(OBJDIR)
	@echo "🔨 Compilando teste: $<"
	$(CXX) $(TEST_FLAGS) -I$(INCDIR) -I$(TESTDIR) -c $< -o $@

# Execução dos testes
test: $(TEST_EXECUTABLE)
	@echo "🧪 Executando testes unitários..."
	./$(TEST_EXECUTABLE)

# Criação de diretórios
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

# Limpeza
clean:
	@echo "🧹 Limpando arquivos temporários..."
	rm -rf $(OBJDIR) $(BINDIR) $(DOCDIR)/html $(DOCDIR)/latex
	rm -f *.gcov *.gcda *.gcno
	@echo "✅ Limpeza concluída!"

# Execução do jogo
run: $(EXECUTABLE)
	@echo "🎮 Executando Simon Game..."
	./$(EXECUTABLE)

# Ajuda
help:
	@echo "📖 Makefile do Projeto Simon Game C++11"
	@echo ""
	@echo "Alvos disponíveis:"
	@echo "  all            - Compila o projeto completo (padrão)"
	@echo "  clean          - Remove arquivos temporários"
	@echo "  test           - Compila e executa testes unitários"
	@echo "  run            - Executa o jogo"
	@echo "  help           - Mostra esta ajuda"
