/**
 * @file test_SequenceGenerator.cpp
 * @brief Testes unitários para a classe SequenceGenerator
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#include "doctest.h"
#include "SequenceGenerator.h"
#include <stdexcept>

DOCTEST_TEST_CASE("SequenceGenerator - Construtor básico") {
    DOCTEST_SUBCASE("Construtor com parâmetros padrão") {
        SequenceGenerator generator;
        DOCTEST_CHECK_EQ(generator.getSequenceLength(), 1);
        DOCTEST_CHECK_EQ(generator.getAvailableColors().size(), 4);
    }

    DOCTEST_SUBCASE("Construtor com cores personalizadas") {
        std::vector<std::string> colors = {"X", "Y", "Z"};
        SequenceGenerator generator(colors, 2);
        DOCTEST_CHECK_EQ(generator.getSequenceLength(), 2);
        DOCTEST_CHECK_EQ(generator.getAvailableColors().size(), 3);
    }

    DOCTEST_SUBCASE("Construtor com parâmetros inválidos deve lançar exceção") {
        std::vector<std::string> emptyColors;
        DOCTEST_CHECK_THROWS(SequenceGenerator(emptyColors));
        
        std::vector<std::string> validColors = {"A", "B"};
        DOCTEST_CHECK_THROWS(SequenceGenerator(validColors, 0));
    }
}

DOCTEST_TEST_CASE("SequenceGenerator - Geração de elementos") {
    SequenceGenerator generator({"A", "B", "C"}, 1);

    DOCTEST_SUBCASE("Adicionar elemento aleatório") {
        std::string element = generator.addRandomElement();
        DOCTEST_CHECK_NE(element, "");
        DOCTEST_CHECK_EQ(generator.getSequenceLength(), 2);
        
        auto colors = generator.getAvailableColors();
        bool found = false;
        for (const auto& color : colors) {
            if (color == element) {
                found = true;
                break;
            }
        }
        DOCTEST_CHECK(found);
    }

    DOCTEST_SUBCASE("Obter elemento específico") {
        auto sequence = generator.getCurrentSequence();
        DOCTEST_CHECK_EQ(generator.getElementAt(0), sequence[0]);
        DOCTEST_CHECK_EQ(generator.getElementAt(999), ""); // Índice inválido
    }
}

DOCTEST_TEST_CASE("SequenceGenerator - Manipulação de sequência") {
    SequenceGenerator generator({"A", "B"}, 1);

    DOCTEST_SUBCASE("Reset da sequência") {
        generator.addRandomElement();
        generator.addRandomElement();
        DOCTEST_CHECK_GT(generator.getSequenceLength(), 1);
        
        generator.reset();
        DOCTEST_CHECK_EQ(generator.getSequenceLength(), 1);
    }

    DOCTEST_SUBCASE("Definir sequência personalizada") {
        std::vector<std::string> customSeq = {"A", "B", "A"};
        DOCTEST_CHECK(generator.setCustomSequence(customSeq));
        DOCTEST_CHECK_EQ(generator.getSequenceLength(), 3);
        DOCTEST_CHECK_EQ(generator.getElementAt(0), "A");
        DOCTEST_CHECK_EQ(generator.getElementAt(1), "B");
        DOCTEST_CHECK_EQ(generator.getElementAt(2), "A");
    }

    DOCTEST_SUBCASE("Sequência personalizada inválida") {
        std::vector<std::string> invalidSeq = {"X", "Y"}; // Cores não disponíveis
        DOCTEST_CHECK(!generator.setCustomSequence(invalidSeq));
    }
}

DOCTEST_TEST_CASE("SequenceGenerator - Validação de entrada") {
    SequenceGenerator generator({"A", "B", "C"}, 1);
    generator.setCustomSequence({"A", "B", "C"});

    DOCTEST_SUBCASE("Validação de sequência correta") {
        std::vector<std::string> userInput = {"A", "B", "C"};
        auto result = generator.validateUserSequence(userInput);
        DOCTEST_CHECK(result.first);
        DOCTEST_CHECK_EQ(result.second, 2);
    }

    DOCTEST_SUBCASE("Validação de sequência incorreta") {
        std::vector<std::string> userInput = {"A", "X", "C"};
        auto result = generator.validateUserSequence(userInput);
        DOCTEST_CHECK(!result.first);
        DOCTEST_CHECK_EQ(result.second, 1);
    }

    DOCTEST_SUBCASE("Validação parcial") {
        std::vector<std::string> userInput = {"A", "B"};
        auto result = generator.validateUserSequence(userInput, 1);
        DOCTEST_CHECK(result.first);
        DOCTEST_CHECK_EQ(result.second, 1);
    }
}

DOCTEST_TEST_CASE("SequenceGenerator - Atualização de cores") {
    SequenceGenerator generator({"A", "B"}, 1);

    DOCTEST_SUBCASE("Atualizar cores válidas") {
        std::vector<std::string> newColors = {"X", "Y", "Z"};
        DOCTEST_CHECK(generator.updateAvailableColors(newColors));
        DOCTEST_CHECK_EQ(generator.getAvailableColors().size(), 3);
        DOCTEST_CHECK_EQ(generator.getSequenceLength(), 1); // Reset após atualização
    }

    DOCTEST_SUBCASE("Atualizar com cores inválidas") {
        std::vector<std::string> emptyColors;
        DOCTEST_CHECK(!generator.updateAvailableColors(emptyColors));
        
        std::vector<std::string> duplicateColors = {"A", "A", "B"};
        DOCTEST_CHECK(!generator.updateAvailableColors(duplicateColors));
    }
}

DOCTEST_TEST_CASE("SequenceGenerator - Estatísticas") {
    SequenceGenerator generator({"A", "B"}, 1);
    generator.setCustomSequence({"A", "B", "A", "A"});

    DOCTEST_SUBCASE("Obter estatísticas da sequência") {
        auto stats = generator.getSequenceStats();
        DOCTEST_CHECK_EQ(stats["length"], 4);
        DOCTEST_CHECK_EQ(stats["uniqueColors"], 2);
        DOCTEST_CHECK_GE(stats["mostFrequentCount"], 1);
    }
}

DOCTEST_TEST_CASE("SequenceGenerator - Operadores") {
    SequenceGenerator gen1({"A", "B"}, 1);
    SequenceGenerator gen2({"A", "B"}, 1);
    SequenceGenerator gen3({"X", "Y"}, 1);

    DOCTEST_SUBCASE("Operador de igualdade") {
        gen1.setCustomSequence({"A", "B"});
        gen2.setCustomSequence({"A", "B"});
        gen3.setCustomSequence({"A", "B"});
        
        DOCTEST_CHECK(gen1 == gen2);
        DOCTEST_CHECK(gen1 != gen3); // Cores diferentes
    }
}

DOCTEST_TEST_CASE("SequenceGenerator - Funcionalidades auxiliares") {
    SequenceGenerator generator({"A", "B", "C"}, 1);

    DOCTEST_SUBCASE("Preview do próximo elemento") {
        std::string preview = generator.previewNextElement();
        auto colors = generator.getAvailableColors();
        bool found = false;
        for (const auto& color : colors) {
            if (color == preview) {
                found = true;
                break;
            }
        }
        DOCTEST_CHECK(found);
    }

    DOCTEST_SUBCASE("Verificar comprimento máximo") {
        DOCTEST_CHECK(!generator.isAtMaxLength());
        
        // Adicionar muitos elementos para testar limite
        for (int i = 0; i < 60; ++i) {
            generator.addRandomElement();
        }
        DOCTEST_CHECK(generator.isAtMaxLength());
    }

    DOCTEST_SUBCASE("Conversão para string") {
        generator.setCustomSequence({"A", "B", "C"});
        std::string str = generator.toString();
        DOCTEST_CHECK_EQ(str, "A, B, C");
        
        std::string strCustom = generator.toString(" - ");
        DOCTEST_CHECK_EQ(strCustom, "A - B - C");
    }
}

DOCTEST_TEST_CASE("SequenceGenerator - Construtor de cópia e atribuição") {
    SequenceGenerator original({"A", "B", "C"}, 2);
    original.addRandomElement();

    DOCTEST_SUBCASE("Construtor de cópia") {
        SequenceGenerator copy(original);
        DOCTEST_CHECK_EQ(copy.getSequenceLength(), original.getSequenceLength());
        DOCTEST_CHECK_EQ(copy.getAvailableColors().size(), original.getAvailableColors().size());
    }

    DOCTEST_SUBCASE("Operador de atribuição") {
        SequenceGenerator assigned({"X"}, 1);
        assigned = original;
        DOCTEST_CHECK_EQ(assigned.getSequenceLength(), original.getSequenceLength());
        DOCTEST_CHECK_EQ(assigned.getAvailableColors().size(), original.getAvailableColors().size());
    }
}
