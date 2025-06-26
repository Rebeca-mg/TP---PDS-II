/**
 * @file test_Player.cpp
 * @brief Testes unitários para a classe Player
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#include "doctest.h"
#include "Player.h"
#include <thread>
#include <chrono>

DOCTEST_TEST_CASE("Player - Construtor básico") {
    DOCTEST_SUBCASE("Construtor com parâmetros padrão") {
        Player player;
        DOCTEST_CHECK_EQ(player.getName(), "Anonymous");
        DOCTEST_CHECK_EQ(player.getLives(), 3);
        DOCTEST_CHECK_EQ(player.getMaxLives(), 3);
        DOCTEST_CHECK_EQ(player.getScore(), 0);
        DOCTEST_CHECK_EQ(player.getLevel(), 1);
        DOCTEST_CHECK(player.isAlive());
    }

    DOCTEST_SUBCASE("Construtor com nome personalizado") {
        Player player("TestPlayer", 5);
        DOCTEST_CHECK_EQ(player.getName(), "TestPlayer");
        DOCTEST_CHECK_EQ(player.getLives(), 5);
        DOCTEST_CHECK_EQ(player.getMaxLives(), 5);
    }

    DOCTEST_SUBCASE("Construtor com nome inválido") {
        Player player("", 3);
        DOCTEST_CHECK_EQ(player.getName(), "Anonymous");
        
        Player player2("!!!@@@###", 3);
        DOCTEST_CHECK_EQ(player2.getName(), "Anonymous");
    }

    DOCTEST_SUBCASE("Construtor com vidas inválidas") {
        Player player("Test", 0);
        DOCTEST_CHECK_GE(player.getMaxLives(), 1);
        
        Player player2("Test", -5);
        DOCTEST_CHECK_GE(player2.getMaxLives(), 1);
    }
}

DOCTEST_TEST_CASE("Player - Gerenciamento de nome") {
    Player player;

    DOCTEST_SUBCASE("Definir nome válido") {
        DOCTEST_CHECK(player.setName("NovoJogador"));
        DOCTEST_CHECK_EQ(player.getName(), "NovoJogador");
    }

    DOCTEST_SUBCASE("Definir nome com caracteres especiais") {
        DOCTEST_CHECK(player.setName("Jogador_123-Test"));
        DOCTEST_CHECK_EQ(player.getName(), "Jogador_123-Test");
    }

    DOCTEST_SUBCASE("Definir nome muito longo") {
        std::string longName(30, 'A');
        DOCTEST_CHECK(player.setName(longName));
        DOCTEST_CHECK_LE(player.getName().length(), 20);
    }

    DOCTEST_SUBCASE("Definir nome vazio") {
        DOCTEST_CHECK(!player.setName(""));
        DOCTEST_CHECK_EQ(player.getName(), "Anonymous");
    }
}

DOCTEST_TEST_CASE("Player - Sistema de pontuação") {
    Player player("TestPlayer");

    DOCTEST_SUBCASE("Adicionar pontos válidos") {
        player.addScore(100, "Teste");
        DOCTEST_CHECK_EQ(player.getScore(), 100);
        
        player.addScore(50, "Teste2");
        DOCTEST_CHECK_EQ(player.getScore(), 150);
    }

    DOCTEST_SUBCASE("Adicionar pontos inválidos") {
        int initialScore = player.getScore();
        player.addScore(-10, "Invalido");
        DOCTEST_CHECK_EQ(player.getScore(), initialScore);
    }

    DOCTEST_SUBCASE("Histórico de pontuação") {
        player.addScore(25, "Primeira pontuação");
        auto history = player.getGameHistory();
        DOCTEST_CHECK_GT(history.size(), 0);
        
        bool found = false;
        for (const auto& event : history) {
            auto it = event.find("action");
            if (it != event.end() && it->second == "score_added") {
                found = true;
                break;
            }
        }
        DOCTEST_CHECK(found);
    }
}

DOCTEST_TEST_CASE("Player - Sistema de níveis") {
    Player player("TestPlayer");

    DOCTEST_SUBCASE("Avançar nível sem bônus") {
        DOCTEST_CHECK_EQ(player.getLevel(), 1);
        player.advanceLevel();
        DOCTEST_CHECK_EQ(player.getLevel(), 2);
    }

    DOCTEST_SUBCASE("Avançar nível com bônus") {
        int initialScore = player.getScore();
        player.advanceLevel(100);
        DOCTEST_CHECK_EQ(player.getLevel(), 2);
        DOCTEST_CHECK_GT(player.getScore(), initialScore);
    }

    DOCTEST_SUBCASE("Múltiplos avanços de nível") {
        for (int i = 0; i < 5; ++i) {
            player.advanceLevel();
        }
        DOCTEST_CHECK_EQ(player.getLevel(), 6);
    }
}

DOCTEST_TEST_CASE("Player - Sistema de vidas") {
    Player player("TestPlayer", 3);

    DOCTEST_SUBCASE("Perder vida e verificar status") {
        DOCTEST_CHECK(player.isAlive());
        DOCTEST_CHECK_EQ(player.getLives(), 3);
        
        bool stillAlive = player.loseLife("Teste");
        DOCTEST_CHECK(stillAlive);
        DOCTEST_CHECK_EQ(player.getLives(), 2);
    }

    DOCTEST_SUBCASE("Perder todas as vidas") {
        player.loseLife("Erro 1");
        player.loseLife("Erro 2");
        bool stillAlive = player.loseLife("Erro 3");
        
        DOCTEST_CHECK(!stillAlive);
        DOCTEST_CHECK(!player.isAlive());
        DOCTEST_CHECK_EQ(player.getLives(), 0);
    }

    DOCTEST_SUBCASE("Reset de vidas") {
        player.loseLife("Teste");
        player.loseLife("Teste");
        DOCTEST_CHECK_LT(player.getLives(), player.getMaxLives());
        
        player.resetLives();
        DOCTEST_CHECK_EQ(player.getLives(), player.getMaxLives());
        DOCTEST_CHECK(player.isAlive());
    }
}

DOCTEST_TEST_CASE("Player - Gerenciamento de entrada") {
    Player player("TestPlayer");

    DOCTEST_SUBCASE("Iniciar sequência de entrada") {
        player.startInputSequence();
        auto input = player.getCurrentInput();
        DOCTEST_CHECK_EQ(input.size(), 0);
    }

    DOCTEST_SUBCASE("Adicionar entradas válidas") {
        player.startInputSequence();
        DOCTEST_CHECK(player.addInput("A"));
        DOCTEST_CHECK(player.addInput("B"));
        DOCTEST_CHECK(player.addInput("C"));
        
        auto input = player.getCurrentInput();
        DOCTEST_CHECK_EQ(input.size(), 3);
        DOCTEST_CHECK_EQ(input[0], "A");
        DOCTEST_CHECK_EQ(input[1], "B");
        DOCTEST_CHECK_EQ(input[2], "C");
    }

    DOCTEST_SUBCASE("Adicionar entrada inválida") {
        player.startInputSequence();
        DOCTEST_CHECK(!player.addInput(""));
    }

    DOCTEST_SUBCASE("Limpar entrada") {
        player.startInputSequence();
        player.addInput("A");
        player.addInput("B");
        DOCTEST_CHECK_GT(player.getCurrentInput().size(), 0);
        
        player.clearCurrentInput();
        DOCTEST_CHECK_EQ(player.getCurrentInput().size(), 0);
    }
}

DOCTEST_TEST_CASE("Player - Registro de sequências") {
    Player player("TestPlayer");

    DOCTEST_SUBCASE("Registrar sequência bem-sucedida") {
        int initialScore = player.getScore();
        player.recordSuccessfulSequence(5);
        
        DOCTEST_CHECK_GT(player.getScore(), initialScore);
        auto stats = player.getStatistics();
        DOCTEST_CHECK_EQ(std::stoi(stats["totalCorrectSequences"]), 1);
        DOCTEST_CHECK_EQ(std::stoi(stats["currentStreak"]), 1);
    }

    DOCTEST_SUBCASE("Múltiplas sequências bem-sucedidas") {
        player.recordSuccessfulSequence(3);
        player.recordSuccessfulSequence(4);
        player.recordSuccessfulSequence(5);
        
        auto stats = player.getStatistics();
        DOCTEST_CHECK_EQ(std::stoi(stats["totalCorrectSequences"]), 3);
        DOCTEST_CHECK_EQ(std::stoi(stats["currentStreak"]), 3);
    }

    DOCTEST_SUBCASE("Sequência quebrada por erro") {
        player.recordSuccessfulSequence(3);
        player.recordSuccessfulSequence(4);
        DOCTEST_CHECK_EQ(std::stoi(player.getStatistics()["currentStreak"]), 2);
        
        player.loseLife("Erro");
        DOCTEST_CHECK_EQ(std::stoi(player.getStatistics()["currentStreak"]), 0);
    }
}

DOCTEST_TEST_CASE("Player - Cronometragem do jogo") {
    Player player("TestPlayer");

    DOCTEST_SUBCASE("Iniciar e terminar jogo") {
        player.startGame();
        DOCTEST_CHECK_EQ(player.getScore(), 0); // Reset no início
        DOCTEST_CHECK_EQ(player.getLevel(), 1);
        
        // Simular algum tempo de jogo
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        player.endGame();
        DOCTEST_CHECK_GT(player.getGameDuration(), 0);
    }

    DOCTEST_SUBCASE("Duração sem jogo iniciado") {
        Player newPlayer("Test");
        DOCTEST_CHECK_EQ(newPlayer.getGameDuration(), 0);
    }

    DOCTEST_SUBCASE("Formatação de duração") {
        player.startGame();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        player.endGame();
        
        std::string formatted = player.getFormattedGameDuration();
        DOCTEST_CHECK_GT(formatted.length(), 0);
        DOCTEST_CHECK_NE(formatted.find(":"), std::string::npos);
    }
}

DOCTEST_TEST_CASE("Player - Estatísticas") {
    Player player("TestPlayer");
    
    DOCTEST_SUBCASE("Estatísticas básicas") {
        auto stats = player.getStatistics();
        DOCTEST_CHECK_EQ(stats["name"], "TestPlayer");
        DOCTEST_CHECK_EQ(stats["score"], "0");
        DOCTEST_CHECK_EQ(stats["level"], "1");
        DOCTEST_CHECK_EQ(stats["lives"], "3");
        DOCTEST_CHECK_EQ(stats["isAlive"], "true");
    }

    DOCTEST_SUBCASE("Cálculo de precisão") {
        player.recordSuccessfulSequence(3);
        player.recordSuccessfulSequence(4);
        player.loseLife("Erro");
        
        auto stats = player.getStatistics();
        double accuracy = std::stod(stats["accuracy"]);
        DOCTEST_CHECK_GT(accuracy, 0.0);
        DOCTEST_CHECK_LE(accuracy, 100.0);
    }

    DOCTEST_SUBCASE("Total de tentativas") {
        player.recordSuccessfulSequence(3);
        player.loseLife("Erro");
        player.recordSuccessfulSequence(4);
        
        DOCTEST_CHECK_EQ(player.getTotalAttempts(), 3);
    }
}

DOCTEST_TEST_CASE("Player - Dados de salvamento") {
    Player player("TestPlayer");
    player.addScore(1000);
    player.advanceLevel(50);
    player.recordSuccessfulSequence(5);

    DOCTEST_SUBCASE("Exportar dados de salvamento") {
        auto saveData = player.toSaveData();
        DOCTEST_CHECK_EQ(saveData["name"], "TestPlayer");
        DOCTEST_CHECK_EQ(saveData["score"], "1050"); // 1000 + 50 bonus
        DOCTEST_CHECK_EQ(saveData["level"], "2");
    }

    DOCTEST_SUBCASE("Importar dados de salvamento") {
        std::map<std::string, std::string> loadData;
        loadData["name"] = "LoadedPlayer";
        loadData["bestStreak"] = "10";
        
        Player newPlayer;
        DOCTEST_CHECK(newPlayer.fromSaveData(loadData));
        DOCTEST_CHECK_EQ(newPlayer.getName(), "LoadedPlayer");
    }
}

DOCTEST_TEST_CASE("Player - Operadores de comparação") {
    Player player1("Player1");
    Player player2("Player2");
    Player player3("Player3");
    
    player1.addScore(1000);
    player2.addScore(500);
    player3.addScore(1000);
    player3.advanceLevel(); // Mesmo score, mas nível maior

    DOCTEST_SUBCASE("Comparação por pontuação") {
        DOCTEST_CHECK_GT(player1.compareWith(player2), 0); // player2 tem score maior
        DOCTEST_CHECK_LT(player2.compareWith(player1), 0); // player1 tem score maior
    }

    DOCTEST_SUBCASE("Comparação por nível (mesmo score)") {
        DOCTEST_CHECK_GT(player1.compareWith(player3), 0); // player3 tem nível maior
    }

    DOCTEST_SUBCASE("Operadores booleanos") {
        DOCTEST_CHECK(player1 != player2);
        DOCTEST_CHECK(player2 < player1);
        DOCTEST_CHECK(player1 > player2);
    }
}

DOCTEST_TEST_CASE("Player - Reset e construtor de cópia") {
    Player original("Original");
    original.addScore(500);
    original.advanceLevel();
    original.recordSuccessfulSequence(3);

    DOCTEST_SUBCASE("Reset completo") {
        original.reset();
        DOCTEST_CHECK_EQ(original.getScore(), 0);
        DOCTEST_CHECK_EQ(original.getLevel(), 1);
        DOCTEST_CHECK_EQ(original.getLives(), original.getMaxLives());
        DOCTEST_CHECK_EQ(original.getCurrentInput().size(), 0);
    }

    DOCTEST_SUBCASE("Construtor de cópia") {
        Player copy(original);
        DOCTEST_CHECK_EQ(copy.getName(), original.getName());
        DOCTEST_CHECK_EQ(copy.getScore(), original.getScore());
        DOCTEST_CHECK_EQ(copy.getLevel(), original.getLevel());
    }

    DOCTEST_SUBCASE("Operador de atribuição") {
        Player assigned("Different");
        assigned = original;
        DOCTEST_CHECK_EQ(assigned.getName(), original.getName());
        DOCTEST_CHECK_EQ(assigned.getScore(), original.getScore());
    }
}