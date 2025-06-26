/**
 * @file test_ScoreManager.cpp
 * @brief Testes unitários para a classe ScoreManager
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#include "doctest.h"
#include "ScoreManager.h"
#include <fstream>
#include <cstdio>

DOCTEST_TEST_CASE("ScoreManager - Construtor básico") {
    DOCTEST_SUBCASE("Construtor com parâmetros padrão") {
        ScoreManager manager;
        DOCTEST_CHECK_EQ(manager.getMaxScores(), 10);
        DOCTEST_CHECK_EQ(manager.getFilename(), "scores.dat");
        DOCTEST_CHECK_EQ(manager.getTotalScores(), 0);
    }

    DOCTEST_SUBCASE("Construtor com parâmetros personalizados") {
        ScoreManager manager(5, "test_scores.dat");
        DOCTEST_CHECK_EQ(manager.getMaxScores(), 5);
        DOCTEST_CHECK_EQ(manager.getFilename(), "test_scores.dat");
    }

    DOCTEST_SUBCASE("Construtor com parâmetros inválidos") {
        ScoreManager manager(0, "test.dat");
        DOCTEST_CHECK_GE(manager.getMaxScores(), 1);
    }
}

DOCTEST_TEST_CASE("ScoreManager - Adição de pontuações") {
    ScoreManager manager(5, "test_add_scores.dat");
    
    DOCTEST_SUBCASE("Adicionar pontuação válida") {
        std::map<std::string, std::string> extraData;
        extraData["level"] = "5";
        extraData["accuracy"] = "95.5";
        extraData["duration"] = "12000";
        extraData["streak"] = "10";
        
        auto result = manager.addScore("TestPlayer", 1000, extraData);
        DOCTEST_CHECK_EQ(result["success"], "true");
        DOCTEST_CHECK_EQ(result["rank"], "1");
        DOCTEST_CHECK_EQ(result["isNewRecord"], "true");
        DOCTEST_CHECK_EQ(manager.getTotalScores(), 1);
    }

    DOCTEST_SUBCASE("Adicionar múltiplas pontuações") {
        manager.addScore("Player1", 1000);
        manager.addScore("Player2", 1500);
        manager.addScore("Player3", 800);
        
        DOCTEST_CHECK_EQ(manager.getTotalScores(), 3);
        
        auto scores = manager.getScores();
        DOCTEST_CHECK_EQ(scores[0].score, 1500); // Maior pontuação primeiro
        DOCTEST_CHECK_EQ(scores[1].score, 1000);
        DOCTEST_CHECK_EQ(scores[2].score, 800);
    }

    DOCTEST_SUBCASE("Adicionar pontuação inválida") {
        auto result1 = manager.addScore("", 100);
        DOCTEST_CHECK_EQ(result1["success"], "false");
        
        auto result2 = manager.addScore("Player", -50);
        DOCTEST_CHECK_EQ(result2["success"], "false");
    }

    DOCTEST_SUBCASE("Limite máximo de pontuações") {
        ScoreManager smallManager(2, "test_limit.dat");
        smallManager.addScore("P1", 100);
        smallManager.addScore("P2", 200);
        smallManager.addScore("P3", 300);
        
        DOCTEST_CHECK_EQ(smallManager.getTotalScores(), 2);
        auto scores = smallManager.getScores();
        DOCTEST_CHECK_EQ(scores[0].score, 300);
        DOCTEST_CHECK_EQ(scores[1].score, 200);
    }
    
    // Cleanup
    std::remove("test_add_scores.dat");
    std::remove("test_limit.dat");
}

DOCTEST_TEST_CASE("ScoreManager - Recuperação de pontuações") {
    ScoreManager manager(10, "test_retrieve.dat");
    
    // Adicionar dados de teste
    manager.addScore("Alice", 1000);
    manager.addScore("Bob", 1500);
    manager.addScore("Alice", 800);
    manager.addScore("Charlie", 1200);

    DOCTEST_SUBCASE("Obter todas as pontuações") {
        auto scores = manager.getScores();
        DOCTEST_CHECK_EQ(scores.size(), 4);
        DOCTEST_CHECK_EQ(scores[0].playerName, "Bob");
        DOCTEST_CHECK_EQ(scores[0].score, 1500);
    }

    DOCTEST_SUBCASE("Obter pontuações com limite") {
        auto scores = manager.getScores(2);
        DOCTEST_CHECK_EQ(scores.size(), 2);
        DOCTEST_CHECK_EQ(scores[0].score, 1500);
        DOCTEST_CHECK_EQ(scores[1].score, 1200);
    }

    DOCTEST_SUBCASE("Obter melhor pontuação") {
        auto topScore = manager.getTopScore();
        DOCTEST_CHECK_NE(topScore, nullptr);
        DOCTEST_CHECK_EQ(topScore->score, 1500);
        DOCTEST_CHECK_EQ(topScore->playerName, "Bob");
    }

    DOCTEST_SUBCASE("Obter pontuações de jogador específico") {
        auto aliceScores = manager.getPlayerScores("Alice");
        DOCTEST_CHECK_EQ(aliceScores.size(), 2);
        DOCTEST_CHECK_EQ(aliceScores[0].score, 1000); // Melhor pontuação primeiro
    }

    DOCTEST_SUBCASE("Obter melhor pontuação de jogador") {
        auto aliceBest = manager.getPlayerBestScore("Alice");
        DOCTEST_CHECK_NE(aliceBest, nullptr);
        DOCTEST_CHECK_EQ(aliceBest->score, 1000);
        
        auto inexistentBest = manager.getPlayerBestScore("Inexistente");
        DOCTEST_CHECK_EQ(inexistentBest, nullptr);
    }
    
    std::remove("test_retrieve.dat");
}

DOCTEST_TEST_CASE("ScoreManager - Qualificação de pontuações") {
    ScoreManager manager(3, "test_qualify.dat");

    DOCTEST_SUBCASE("Pontuação qualificante com espaço") {
        manager.addScore("P1", 100);
        manager.addScore("P2", 200);
        
        DOCTEST_CHECK(manager.isQualifyingScore(150));
        DOCTEST_CHECK(manager.isQualifyingScore(50));
    }

    DOCTEST_SUBCASE("Pontuação qualificante quando lista cheia") {
        manager.addScore("P1", 100);
        manager.addScore("P2", 200);
        manager.addScore("P3", 300);
        
        DOCTEST_CHECK(manager.isQualifyingScore(150));
        DOCTEST_CHECK(!manager.isQualifyingScore(50));
    }

    DOCTEST_SUBCASE("Pontuação inválida") {
        DOCTEST_CHECK(!manager.isQualifyingScore(-10));
    }
    
    std::remove("test_qualify.dat");
}

DOCTEST_TEST_CASE("ScoreManager - Estatísticas") {
    ScoreManager manager(10, "test_stats.dat");

    DOCTEST_SUBCASE("Estatísticas sem pontuações") {
        auto stats = manager.getStatistics();
        DOCTEST_CHECK_EQ(stats["totalScores"], "0");
        DOCTEST_CHECK_EQ(stats["averageScore"], "0");
        DOCTEST_CHECK_EQ(stats["totalPlayers"], "0");
    }

    DOCTEST_SUBCASE("Estatísticas com pontuações") {
        std::map<std::string, std::string> extraData;
        extraData["level"] = "5";
        extraData["accuracy"] = "90.0";
        
        manager.addScore("Alice", 1000, extraData);
        manager.addScore("Bob", 1200, extraData);
        manager.addScore("Alice", 800, extraData);
        
        auto stats = manager.getStatistics();
        DOCTEST_CHECK_EQ(stats["totalScores"], "3");
        DOCTEST_CHECK_EQ(stats["highestScore"], "1200");
        DOCTEST_CHECK_EQ(stats["lowestScore"], "800");
        DOCTEST_CHECK_EQ(stats["totalPlayers"], "2");
        DOCTEST_CHECK_EQ(stats["averageScore"], "1000");
    }
    
    std::remove("test_stats.dat");
}

DOCTEST_TEST_CASE("ScoreManager - Remoção de pontuações") {
    ScoreManager manager(10, "test_remove.dat");
    
    manager.addScore("P1", 100);
    manager.addScore("P2", 200);
    manager.addScore("P3", 300);
    manager.addScore("P1", 150);

    DOCTEST_SUBCASE("Remover pontuação por índice") {
        DOCTEST_CHECK(manager.removeScore(0));
        DOCTEST_CHECK_EQ(manager.getTotalScores(), 3);
        
        auto scores = manager.getScores();
        DOCTEST_CHECK_EQ(scores[0].score, 200);
    }

    DOCTEST_SUBCASE("Remover índice inválido") {
        DOCTEST_CHECK(!manager.removeScore(999));
    }

    DOCTEST_SUBCASE("Remover pontuações de jogador") {
        int removed = manager.removePlayerScores("P1");
        DOCTEST_CHECK_EQ(removed, 2);
        DOCTEST_CHECK_EQ(manager.getTotalScores(), 2);
    }

    DOCTEST_SUBCASE("Limpar todas as pontuações") {
        DOCTEST_CHECK(manager.clearScores());
        DOCTEST_CHECK_EQ(manager.getTotalScores(), 0);
    }
    
    std::remove("test_remove.dat");
}

DOCTEST_TEST_CASE("ScoreManager - Importação e exportação") {
    ScoreManager manager(10, "test_import_export.dat");
    
    manager.addScore("Alice", 1000);
    manager.addScore("Bob", 1500);

    DOCTEST_SUBCASE("Exportar pontuações") {
        std::string exported = manager.exportScores();
        DOCTEST_CHECK_GT(exported.length(), 0);
        DOCTEST_CHECK_NE(exported.find("Alice"), std::string::npos);
        DOCTEST_CHECK_NE(exported.find("Bob"), std::string::npos);
    }

    DOCTEST_SUBCASE("Importar pontuações (substituir)") {
        std::string importData = "Charlie|2000|10|01/01/2025 10:00|100.0|15000|5\n";
        importData += "David|1800|8|01/01/2025 11:00|95.0|12000|3\n";
        
        auto result = manager.importScores(importData, false);
        DOCTEST_CHECK_EQ(result["success"], "true");
        DOCTEST_CHECK_EQ(result["imported"], "2");
        DOCTEST_CHECK_EQ(manager.getTotalScores(), 2);
        
        auto scores = manager.getScores();
        DOCTEST_CHECK_EQ(scores[0].playerName, "Charlie");
    }

    DOCTEST_SUBCASE("Importar pontuações (mesclar)") {
        ScoreManager mergeManager(10, "test_merge.dat");
        mergeManager.addScore("Existing", 500);
        
        std::string importData = "New|1000|5|01/01/2025 10:00|90.0|10000|2\n";
        auto result = mergeManager.importScores(importData, true);
        
        DOCTEST_CHECK_EQ(result["success"], "true");
        DOCTEST_CHECK_EQ(mergeManager.getTotalScores(), 2);
    }

    DOCTEST_SUBCASE("Importar dados inválidos") {
        std::string invalidData = "invalid data format\n";
        auto result = manager.importScores(invalidData, false);
        DOCTEST_CHECK_EQ(result["success"], "false");
    }
    
    std::remove("test_import_export.dat");
    std::remove("test_merge.dat");
}

DOCTEST_TEST_CASE("ScoreManager - Configurações") {
    ScoreManager manager(5, "test_config.dat");

    DOCTEST_SUBCASE("Alterar máximo de pontuações") {
        DOCTEST_CHECK(manager.setMaxScores(3));
        DOCTEST_CHECK_EQ(manager.getMaxScores(), 3);
        
        DOCTEST_CHECK(!manager.setMaxScores(0));
    }

    DOCTEST_SUBCASE("Reduzir máximo com pontuações existentes") {
        manager.addScore("P1", 100);
        manager.addScore("P2", 200);
        manager.addScore("P3", 300);
        manager.addScore("P4", 400);
        
        DOCTEST_CHECK(manager.setMaxScores(2));
        DOCTEST_CHECK_EQ(manager.getTotalScores(), 2);
        
        auto scores = manager.getScores();
        DOCTEST_CHECK_EQ(scores[0].score, 400);
        DOCTEST_CHECK_EQ(scores[1].score, 300);
    }

    DOCTEST_SUBCASE("Forçar salvamento") {
        manager.addScore("Test", 100);
        DOCTEST_CHECK(manager.forceSave());
    }

    DOCTEST_SUBCASE("Recarregar dados") {
        manager.addScore("Original", 100);
        DOCTEST_CHECK(manager.forceSave());
        
        manager.addScore("Memory", 200);
        DOCTEST_CHECK_EQ(manager.getTotalScores(), 2);
        
        DOCTEST_CHECK(manager.reload());
        DOCTEST_CHECK_EQ(manager.getTotalScores(), 1);
        
        auto scores = manager.getScores();
        DOCTEST_CHECK_EQ(scores[0].playerName, "Original");
    }
    
    std::remove("test_config.dat");
}

DOCTEST_TEST_CASE("ScoreManager - Persistência de arquivo") {
    const std::string testFile = "test_persistence.dat";

    DOCTEST_SUBCASE("Salvar e carregar automaticamente") {
        {
            ScoreManager manager1(5, testFile);
            manager1.addScore("Persistent", 999);
            DOCTEST_CHECK_EQ(manager1.getTotalScores(), 1);
        } // Destrutor deve salvar automaticamente
        
        {
            ScoreManager manager2(5, testFile);
            DOCTEST_CHECK_EQ(manager2.getTotalScores(), 1);
            
            auto scores = manager2.getScores();
            DOCTEST_CHECK_EQ(scores[0].playerName, "Persistent");
            DOCTEST_CHECK_EQ(scores[0].score, 999);
        }
    }

    DOCTEST_SUBCASE("Verificar disponibilidade de armazenamento") {
        ScoreManager manager(5, testFile);
        DOCTEST_CHECK(manager.isStorageWorking());
    }
    
    std::remove(testFile.c_str());
}

DOCTEST_TEST_CASE("ScoreManager - Construtor de cópia e atribuição") {
    ScoreManager original(5, "test_copy.dat");
    original.addScore("Original", 500);

    DOCTEST_SUBCASE("Construtor de cópia") {
        ScoreManager copy(original);
        DOCTEST_CHECK_EQ(copy.getTotalScores(), original.getTotalScores());
        DOCTEST_CHECK_EQ(copy.getMaxScores(), original.getMaxScores());
        DOCTEST_CHECK_EQ(copy.getFilename(), original.getFilename());
    }

    DOCTEST_SUBCASE("Operador de atribuição") {
        ScoreManager assigned(10, "different.dat");
        assigned = original;
        DOCTEST_CHECK_EQ(assigned.getTotalScores(), original.getTotalScores());
        DOCTEST_CHECK_EQ(assigned.getMaxScores(), original.getMaxScores());
    }
    
    std::remove("test_copy.dat");
}