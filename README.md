# TP---PDS-II
Trabalho Prático - PDS II - TM1 -
  Maria Fernanda,
  Mariana,
  Kristyn,
  Rebeca

# Jogo Simon - Implementação C++11

Um jogo completo de memória Simon implementado em C++11, demonstrando princípios de programação orientada a objetos, arquitetura modular e mecânicas abrangentes de jogo.

## 📖 Visão Geral

Simon é um jogo de desafio de memória onde os jogadores devem reproduzir sequências cada vez mais complexas de cores e sons. O jogo testa memória, concentração e tempo de reação conforme as sequências ficam progressivamente mais longas e rápidas.

### Características do Jogo

- **Dificuldade Progressiva**: Sequências ficam mais longas e rápidas conforme você avança
- **Sistema de Vidas**: Três vidas por jogo com feedback visual
- **Sistema de Pontuação**: Pontos baseados em comprimento da sequência, nível e sequências de acertos
- **Rastreamento de Recordes**: Leaderboard local com armazenamento persistente
- **Feedback de Áudio**: Sons distintivos para cada botão de cor (simulados no terminal)
- **Suporte a Teclado**: Navegação e entrada completamente por teclado
- **Interface Responsiva**: Funciona em terminais de diferentes tamanhos
- **Configurações**: Sons, velocidade da sequência e tempo limite configuráveis

## 🏗️ Arquitetura

Esta implementação segue princípios de design orientado a objetos similares ao C++11, com clara separação de responsabilidades:

### Classes Principais

- **`SimonGame`**: Controlador principal do jogo orquestrando todos os componentes
- **`SequenceGenerator`**: Gerencia geração de sequências aleatórias e validação
- **`Player`**: Manipula dados do jogador, rastreamento de entrada e estatísticas
- **`ScoreManager`**: Gerencia pontuações altas e armazenamento persistente

### Padrões de Design Utilizados

- **Padrão MVC**: Clara separação entre modelo, visão e controlador
- **Padrão RAII**: Gerenciamento automático de recursos com ponteiros inteligentes
- **Padrão Strategy**: Regras de jogo e dificuldade configuráveis
- **Padrão Command**: Manipulação e validação de entrada do usuário

## 🚀 Começando

### Pré-requisitos

- Compilador C++11 ou superior (g++ 4.8+, clang++ 3.3+, MSVC 2013+)
- Sistema operacional Unix-like (Linux, macOS) ou Windows
- Make (para usar o Makefile fornecido)

### Ferramentas Opcionais

- **Doxygen**: Para gerar documentação (recomendado)
- **doctest**: Biblioteca de testes incluída no projeto
- **gcovr**: Para relatórios de cobertura de testes
- **Valgrind**: Para análise de memória (Linux)
- **cppcheck**: Para análise estática de código

### Compilação

#### Compilação Simples

```bash
# Clonar/baixar os arquivos do projeto
cd cpp_simon_game

# Compilar usando Make
make

# Executar o jogo
./bin/simon_game
```

#### Compilação Manual

```bash
# Criar diretórios
mkdir -p obj bin

# Compilar arquivos objeto
g++ -std=c++11 -Wall -Wextra -O2 -Iinclude -c src/*.cpp main.cpp
mv *.o obj/

# Linkar executável
g++ -std=c++11 -o bin/simon_game obj/*.o
```

### Executando Testes

```bash
# Compilar e executar testes unitários
make test

# Para relatório de cobertura (requer gcovr)
make coverage
```

### Gerando Documentação

```bash
# Gerar documentação com Doxygen
make docs

# Abrir documentação gerada
xdg-open docs/html/index.html  # Linux
open docs/html/index.html      # macOS
```

## 🎮 Como Jogar

### Menu Principal

Ao iniciar o jogo, você verá:

1. **Jogar** - Iniciar um novo jogo
2. **Ver Recordes** - Visualizar leaderboard local
3. **Configurações** - Ajustar som, velocidade e outras opções
4. **Créditos** - Informações sobre o jogo
5. **Sair** - Encerrar o programa

### Jogabilidade

1. **Observar**: O jogo mostrará uma sequência de letras (A, B, C, D)
2. **Memorizar**: Memorize a ordem e os símbolos mostrados
3. **Reproduzir**: Digite a mesma sequência usando as teclas correspondentes
4. **Avançar**: Cada nível adiciona um novo elemento à sequência
5. **Vidas**: Você tem 3 vidas - erros consomem uma vida
6. **Pontuação**: Ganhe pontos baseados no comprimento da sequência e nível

### Controles

- **A, B, C, D**: Entrada de cores/símbolos durante o jogo
- **1-5**: Navegação no menu principal
- **SAIR**: Voltar ao menu durante o jogo
- **ENTER**: Confirmar seleções e continuar

## 📁 Estrutura do Projeto

```
cpp_simon_game/
├── include/               # Arquivos de cabeçalho (.h)
│   ├── SequenceGenerator.h
│   ├── Player.h
│   ├── ScoreManager.h
│   └── SimonGame.h
├── src/                   # Arquivos de implementação (.cpp)
│   ├── SequenceGenerator.cpp
│   ├── Player.cpp
│   ├── ScoreManager.cpp
│   └── SimonGame.cpp
├── tests/                 # Testes unitários
│   ├── doctest.h
│   ├── test_main.cpp
│   ├── test_SequenceGenerator.cpp
│   ├── test_Player.cpp
│   └── test_ScoreManager.cpp
├── docs/                  # Documentação gerada
├── main.cpp              # Ponto de entrada principal
├── Makefile              # Script de build
├── Doxyfile              # Configuração do Doxygen
└── README.md             # Este arquivo
```

## 🔧 Makefile - Comandos Disponíveis

```bash
make               # Compilar projeto (padrão)
make clean         # Limpar arquivos temporários
make debug         # Compilar versão debug
make test          # Executar testes unitários
make coverage      # Gerar relatório de cobertura
make docs          # Gerar documentação
make install       # Instalar no sistema
make run           # Executar o jogo
make style-check   # Verificar estilo de código
make static-analysis # Análise estática
make memcheck      # Análise de memória
make package       # Criar pacote do código fonte
make help          # Mostrar todos os comandos
```

## 🧪 Testes

O projeto inclui testes unitários abrangentes usando a biblioteca doctest:

- **Cobertura Mínima**: 60% (conforme especificação do projeto)
- **Classes Testadas**: Todas as classes principais
- **Cenários**: Casos normais, extremos e de erro
- **Validação**: Entrada, saída e estado interno

### Executar Testes Específicos

```bash
# Compilar apenas os testes
make test

# Executar com informações detalhadas
./bin/test_runner -v

# Executar testes específicos
./bin/test_runner -tc="Player*"
```

## 📚 Documentação

A documentação completa é gerada usando Doxygen e inclui:

- **Referência de API**: Documentação de todas as classes e métodos
- **Guias de Uso**: Como usar cada componente
- **Exemplos**: Código de exemplo para funcionalidades principais
- **Diagramas**: Diagramas de classe e dependência (se configurado)

## ⚙️ Configuração

### Arquivo de Configuração

O jogo salva configurações automaticamente. Configurações disponíveis:

- **Sons**: Habilitado/Desabilitado
- **Velocidade da Sequência**: 300-2000ms
- **Tempo Limite**: 3-30 segundos
- **Arquivo de Pontuações**: Localização do arquivo de recordes

### Persistência

- **Recordes**: Salvos em `scores.dat`
- **Configurações**: Integradas no código (expandível)
- **Formato**: Texto simples para facilitar backup/edição

## 🔍 Análise de Código

### Verificação de Qualidade

```bash
# Análise estática
make static-analysis

# Verificação de estilo
make style-check

# Análise de memória
make memcheck
```

### Métricas do Código

- **Linhas de Código**: 2000+ (conforme especificação)
- **Classes**: 4 principais + estruturas auxiliares
- **Métodos**: 80+ métodos documentados
- **Arquivos**: 12 arquivos de código fonte
- **Testes**: 50+ casos de teste

## 🚀 Instalação no Sistema

```bash
# Instalar globalmente (requer sudo)
make install

# Executar de qualquer lugar
simon_game
```

## 🐛 Solução de Problemas

### Problemas Comuns

1. **Erro de Compilação**: Verifique se o compilador suporta C++11
2. **Arquivo não encontrado**: Certifique-se de estar no diretório correto
3. **Permissões**: Use `chmod +x` se necessário
4. **Testes falhando**: Verifique se todos os arquivos foram compilados

### Plataformas Suportadas

- **Linux**: Ubuntu 16.04+, CentOS 7+, Debian 9+
- **macOS**: 10.12+ com Xcode 8+
- **Windows**: MSYS2, MinGW-w64, Visual Studio 2015+

## 📈 Melhorias Futuras

### Funcionalidades Planejadas

- Interface gráfica usando SFML ou SDL
- Suporte a múltiplos jogadores
- Diferentes modos de jogo
- Integração com banco de dados
- Sistema de conquistas
- Sons reais em vez de simulados

### Contribuições

Este é um projeto acadêmico, mas sugestões são bem-vindas:

1. Relatórios de bugs via issues
2. Sugestões de melhorias
3. Documentação adicional
4. Otimizações de performance

## 📜 Licença

Projeto acadêmico para fins educacionais. Baseado no jogo clássico Simon (1978) criado por Ralph H. Baer e Howard J. Morrison.

## 🙏 Agradecimentos

- **Ralph H. Baer e Howard J. Morrison**: Criadores do jogo Simon original
- **Comunidade C++**: Pela documentação e recursos
- **doctest**: Pela excelente biblioteca de testes
- **Doxygen**: Pela ferramenta de documentação

--
