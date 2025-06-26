#!/bin/bash
# Script de compilação e execução do Jogo Simon C++11

echo "================================================="
echo "    JOGO SIMON C++11 - PROJETO ACADÊMICO"
echo "================================================="

# Criar diretórios necessários
mkdir -p obj bin

# Compilar arquivos fonte
echo "Compilando arquivos fonte..."
g++ -std=c++11 -Wall -Wextra -Wpedantic -O2 -Iinclude -c src/SequenceGenerator.cpp -o obj/SequenceGenerator.o
g++ -std=c++11 -Wall -Wextra -Wpedantic -O2 -Iinclude -c src/Player.cpp -o obj/Player.o
g++ -std=c++11 -Wall -Wextra -Wpedantic -O2 -Iinclude -c src/ScoreManager.cpp -o obj/ScoreManager.o
g++ -std=c++11 -Wall -Wextra -Wpedantic -O2 -Iinclude -c src/SimonGame.cpp -o obj/SimonGame.o
g++ -std=c++11 -Wall -Wextra -Wpedantic -O2 -Iinclude -c main.cpp -o obj/main.o

# Linkar executável
echo "Linkando executável..."
g++ -std=c++11 -o bin/simon_game obj/*.o

if [ $? -eq 0 ]; then
    echo "✅ Compilação concluída com sucesso!"
    echo "📊 Estatísticas do projeto:"
    echo "   - Arquivos .h: $(find include -name '*.h' | wc -l)"
    echo "   - Arquivos .cpp: $(find src -name '*.cpp' | wc -l)"
    echo "   - Linhas de código: $(find include src main.cpp -name '*.cpp' -o -name '*.h' | xargs wc -l | tail -1)"
    echo "   - Testes: $(find tests -name 'test_*.cpp' | wc -l) arquivos"
    echo ""
    echo "🎮 Executável criado: bin/simon_game"
    echo "Para executar: ./bin/simon_game"
else
    echo "❌ Erro na compilação!"
    exit 1
fi