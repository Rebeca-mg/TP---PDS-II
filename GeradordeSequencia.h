/**
 * @file SequenceGenerator.h
 * @brief Declaração da classe SequenceGenerator para geração de sequências do jogo Simon
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#ifndef SEQUENCE_GENERATOR_H
#define SEQUENCE_GENERATOR_H

#include <vector>
#include <string>
#include <random>
#include <map>

/**
 * @class SequenceGenerator
 * @brief Responsável por gerar e gerenciar sequências aleatórias do jogo Simon
 * 
 * Esta classe encapsula a lógica de geração de sequências usando números aleatórios
 * criptograficamente seguros, validação de entrada do usuário e gerenciamento
 * da dificuldade progressiva do jogo.
 */
class SequenceGenerator {
private:
    std::vector<std::string> availableColors;  ///< Cores/símbolos disponíveis
    std::vector<std::string> currentSequence;  ///< Sequência atual do jogo
    size_t initialLength;                      ///< Comprimento inicial da sequência
    size_t maxSequenceLength;                  ///< Comprimento máximo permitido
    std::mt19937 randomGenerator;              ///< Gerador de números aleatórios
    std::uniform_int_distribution<int> distribution; ///< Distribuição uniforme

    /**
     * @brief Valida os parâmetros do construtor
     * @throws std::invalid_argument se os parâmetros forem inválidos
     */
    void validateConstructorParams();

    /**
     * @brief Inicializa a sequência com o comprimento inicial
     */
    void initializeSequence();

    /**
     * @brief Gera um índice aleatório criptograficamente seguro
     * @return Índice aleatório dentro do range de cores disponíveis
     */
    int generateRandomIndex();

public:
    /**
     * @brief Construtor da classe SequenceGenerator
     * @param colors Vetor de cores/símbolos disponíveis
     * @param initialLength Comprimento inicial da sequência (padrão: 1)
     * @throws std::invalid_argument se os parâmetros forem inválidos
     */
    explicit SequenceGenerator(const std::vector<std::string>& colors = {"A", "B", "C", "D"}, 
                              size_t initialLength = 1);

    /**
     * @brief Destrutor virtual padrão
     */
    virtual ~SequenceGenerator() = default;

    /**
     * @brief Construtor de cópia
     * @param other Objeto a ser copiado
     */
    SequenceGenerator(const SequenceGenerator& other);

    /**
     * @brief Operador de atribuição
     * @param other Objeto a ser atribuído
     * @return Referência para este objeto
     */
    SequenceGenerator& operator=(const SequenceGenerator& other);

    /**
     * @brief Adiciona um elemento aleatório à sequência atual
     * @return String com o elemento adicionado ou string vazia se falhar
     */
    std::string addRandomElement();

    /**
     * @brief Obtém a sequência atual
     * @return Cópia da sequência atual
     */
    std::vector<std::string> getCurrentSequence() const;

    /**
     * @brief Obtém o comprimento da sequência atual
     * @return Comprimento da sequência
     */
    size_t getSequenceLength() const;

    /**
     * @brief Obtém um elemento específico da sequência
     * @param index Índice do elemento desejado
     * @return Elemento no índice especificado ou string vazia se inválido
     */
    std::string getElementAt(size_t index) const;

    /**
     * @brief Reinicia a sequência para o estado inicial
     */
    void reset();

    /**
     * @brief Define uma sequência personalizada (útil para testes)
     * @param sequence Sequência personalizada
     * @return true se a sequência foi definida com sucesso
     */
    bool setCustomSequence(const std::vector<std::string>& sequence);

    /**
     * @brief Obtém as cores/símbolos disponíveis
     * @return Vetor com as cores disponíveis
     */
    std::vector<std::string> getAvailableColors() const;

    /**
     * @brief Atualiza as cores disponíveis (para diferentes níveis de dificuldade)
     * @param newColors Novo conjunto de cores
     * @return true se as cores foram atualizadas com sucesso
     */
    bool updateAvailableColors(const std::vector<std::string>& newColors);

    /**
     * @brief Gera uma prévia do próximo elemento (para dicas/ajuste de dificuldade)
     * @return Cor aleatória das cores disponíveis
     */
    std::string previewNextElement();

    /**
     * @brief Verifica se a sequência atingiu o comprimento máximo
     * @return true se estiver no comprimento máximo
     */
    bool isAtMaxLength() const;

    /**
     * @brief Obtém estatísticas da sequência para análise
     * @return Mapa com estatísticas da sequência atual
     */
    std::map<std::string, int> getSequenceStats() const;

    /**
     * @brief Converte a sequência para representação em string
     * @param separator Separador entre elementos (padrão: ", ")
     * @return String representando a sequência
     */
    std::string toString(const std::string& separator = ", ") const;

    /**
     * @brief Valida uma sequência de entrada do usuário
     * @param userSequence Sequência inserida pelo usuário
     * @param upToIndex Verifica apenas até este índice (para validação parcial)
     * @return Par com status de sucesso e posição do erro (se houver)
     */
    std::pair<bool, int> validateUserSequence(const std::vector<std::string>& userSequence, 
                                             int upToIndex = -1) const;

    /**
     * @brief Operador de igualdade
     * @param other Outro objeto para comparação
     * @return true se os objetos forem iguais
     */
    bool operator==(const SequenceGenerator& other) const;

    /**
     * @brief Operador de desigualdade
     * @param other Outro objeto para comparação
     * @return true se os objetos forem diferentes
     */
    bool operator!=(const SequenceGenerator& other) const;
};

#endif // SEQUENCE_GENERATOR_H
