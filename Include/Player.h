/**
 * @file Player.h
 * @brief Declaração da classe Player para gerenciamento de dados do jogador
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <chrono>
#include <map>

/**
 * @class Player
 * @brief Gerencia informações do jogador, entrada e estado do jogo
 * 
 * Esta classe encapsula todos os dados relacionados ao jogador, incluindo
 * pontuação, vidas, sequências de entrada, estatísticas de desempenho
 * e histórico do jogo para análise posterior.
 */
class Player {
private:
    std::string name;                           ///< Nome do jogador
    int maxLives;                              ///< Número máximo de vidas
    int currentLives;                          ///< Vidas restantes
    int score;                                 ///< Pontuação atual
    int level;                                 ///< Nível atual
    std::vector<std::string> currentInputSequence; ///< Sequência de entrada atual
    int totalCorrectSequences;                 ///< Total de sequências corretas
    int totalWrongAttempts;                    ///< Total de tentativas incorretas
    std::chrono::steady_clock::time_point gameStartTime; ///< Tempo de início do jogo
    std::chrono::steady_clock::time_point gameEndTime;   ///< Tempo de fim do jogo
    int bestStreak;                            ///< Melhor sequência de acertos
    int currentStreak;                         ///< Sequência atual de acertos
    double averageReactionTime;                ///< Tempo médio de reação
    std::vector<double> reactionTimes;         ///< Tempos de reação individuais
    std::chrono::steady_clock::time_point lastInputTime; ///< Tempo da última entrada
    std::vector<std::map<std::string, std::string>> gameHistory; ///< Histórico do jogo

    /**
     * @brief Valida e sanitiza o nome do jogador
     * @param name Nome a ser validado
     * @return Nome validado
     */
    std::string validateAndSetName(const std::string& name);

    /**
     * @brief Inicializa o estado do jogador para um novo jogo
     */
    void initializePlayerState();

public:
    /**
     * @brief Construtor da classe Player
     * @param name Nome do jogador (padrão: "Anonymous")
     * @param lives Número de vidas permitidas (padrão: 3)
     */
    explicit Player(const std::string& name = "Anonymous", int lives = 3);

    /**
     * @brief Destrutor virtual padrão
     */
    virtual ~Player() = default;

    /**
     * @brief Construtor de cópia
     * @param other Objeto a ser copiado
     */
    Player(const Player& other);

    /**
     * @brief Operador de atribuição
     * @param other Objeto a ser atribuído
     * @return Referência para este objeto
     */
    Player& operator=(const Player& other);

    /**
     * @brief Obtém o nome do jogador
     * @return Nome do jogador
     */
    std::string getName() const;

    /**
     * @brief Define um novo nome para o jogador
     * @param newName Novo nome a ser definido
     * @return true se o nome foi definido com sucesso
     */
    bool setName(const std::string& newName);

    /**
     * @brief Obtém a pontuação atual
     * @return Pontuação atual
     */
    int getScore() const;

    /**
     * @brief Adiciona pontos à pontuação do jogador
     * @param points Pontos a serem adicionados
     * @param reason Motivo da pontuação (para histórico do jogo)
     */
    void addScore(int points, const std::string& reason = "Correct sequence");

    /**
     * @brief Obtém o nível atual
     * @return Nível atual
     */
    int getLevel() const;

    /**
     * @brief Avança para o próximo nível
     * @param bonusPoints Pontos bônus pela conclusão do nível
     */
    void advanceLevel(int bonusPoints = 0);

    /**
     * @brief Obtém as vidas restantes
     * @return Vidas restantes
     */
    int getLives() const;

    /**
     * @brief Obtém o número máximo de vidas
     * @return Número máximo de vidas para este jogo
     */
    int getMaxLives() const;

    /**
     * @brief Remove uma vida (chamado quando o jogador comete um erro)
     * @param reason Motivo da perda de vida
     * @return true se o jogador ainda tem vidas restantes
     */
    bool loseLife(const std::string& reason = "Wrong sequence");

    /**
     * @brief Verifica se o jogador ainda está vivo
     * @return true se o jogador tem vidas restantes
     */
    bool isAlive() const;

    /**
     * @brief Reinicia as vidas para o máximo (para novo jogo)
     */
    void resetLives();

    /**
     * @brief Inicia uma nova sequência de entrada
     */
    void startInputSequence();

    /**
     * @brief Adiciona uma entrada à sequência atual
     * @param input Entrada de cor/símbolo
     * @return true se a entrada foi adicionada com sucesso
     */
    bool addInput(const std::string& input);

    /**
     * @brief Obtém a sequência de entrada atual
     * @return Cópia da sequência de entrada atual
     */
    std::vector<std::string> getCurrentInput() const;

    /**
     * @brief Limpa a sequência de entrada atual
     */
    void clearCurrentInput();

    /**
     * @brief Registra uma conclusão bem-sucedida de sequência
     * @param sequenceLength Comprimento da sequência concluída
     */
    void recordSuccessfulSequence(int sequenceLength);

    /**
     * @brief Inicia a cronometragem do jogo
     */
    void startGame();

    /**
     * @brief Termina a cronometragem do jogo e calcula estatísticas finais
     */
    void endGame();

    /**
     * @brief Obtém a duração do jogo atual/último em milissegundos
     * @return Duração do jogo em milissegundos, ou 0 se o jogo não foi iniciado
     */
    long long getGameDuration() const;

    /**
     * @brief Obtém string formatada da duração do jogo
     * @return Duração formatada (MM:SS)
     */
    std::string getFormattedGameDuration() const;

    /**
     * @brief Obtém estatísticas abrangentes do jogador
     * @return Mapa com estatísticas do jogador
     */
    std::map<std::string, std::string> getStatistics() const;

    /**
     * @brief Obtém total de tentativas (corretas + erradas)
     * @return Total de tentativas feitas
     */
    int getTotalAttempts() const;

    /**
     * @brief Obtém histórico do jogo para análise
     * @return Vetor com eventos do jogo
     */
    std::vector<std::map<std::string, std::string>> getGameHistory() const;

    /**
     * @brief Reinicia todos os dados do jogador para um novo começo
     */
    void reset();

    /**
     * @brief Cria um objeto pronto para salvamento dos dados do jogador
     * @return Mapa com dados serializáveis do jogador
     */
    std::map<std::string, std::string> toSaveData() const;

    /**
     * @brief Carrega dados do jogador de um objeto de salvamento
     * @param saveData Dados do jogador previamente salvos
     * @return true se os dados foram carregados com sucesso
     */
    bool fromSaveData(const std::map<std::string, std::string>& saveData);

    /**
     * @brief Compara este jogador com outro para ranking
     * @param other Jogador para comparar
     * @return Negativo se este jogador tem rank superior, positivo se inferior, 0 se igual
     */
    int compareWith(const Player& other) const;

    /**
     * @brief Operador de igualdade
     * @param other Outro objeto para comparação
     * @return true se os objetos forem iguais
     */
    bool operator==(const Player& other) const;

    /**
     * @brief Operador de desigualdade
     * @param other Outro objeto para comparação
     * @return true se os objetos forem diferentes
     */
    bool operator!=(const Player& other) const;

    /**
     * @brief Operador menor que (para ordenação)
     * @param other Outro objeto para comparação
     * @return true se este jogador tem rank inferior
     */
    bool operator<(const Player& other) const;

    /**
     * @brief Operador maior que (para ordenação)
     * @param other Outro objeto para comparação
     * @return true se este jogador tem rank superior
     */
    bool operator>(const Player& other) const;
};

#endif // PLAYER_H