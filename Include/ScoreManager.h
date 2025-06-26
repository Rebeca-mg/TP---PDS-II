/**
 * @file ScoreManager.h
 * @brief Declaração da classe ScoreManager para gerenciamento de pontuações
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <chrono>

/**
 * @struct ScoreEntry
 * @brief Estrutura para representar uma entrada de pontuação
 */
struct ScoreEntry {
    std::string playerName;     ///< Nome do jogador
    int score;                  ///< Pontuação
    int level;                  ///< Nível alcançado
    std::string date;           ///< Data da pontuação
    double accuracy;            ///< Precisão (%)
    long long duration;         ///< Duração em milissegundos
    int streak;                 ///< Sequência de acertos

    /**
     * @brief Construtor padrão
     */
    ScoreEntry() : score(0), level(1), accuracy(0.0), duration(0), streak(0) {}

    /**
     * @brief Construtor com parâmetros
     */
    ScoreEntry(const std::string& name, int sc, int lv, const std::string& dt, 
               double acc, long long dur, int str) 
        : playerName(name), score(sc), level(lv), date(dt), 
          accuracy(acc), duration(dur), streak(str) {}

    /**
     * @brief Operador de comparação para ordenação
     */
    bool operator>(const ScoreEntry& other) const {
        if (score != other.score) return score > other.score;
        if (level != other.level) return level > other.level;
        if (streak != other.streak) return streak > other.streak;
        return accuracy > other.accuracy;
    }
};

/**
 * @class ScoreManager
 * @brief Gerencia pontuações altas, persistência e funcionalidade de leaderboard
 * 
 * Esta classe é responsável por armazenar, recuperar e gerenciar pontuações
 * usando persistência em arquivo, ordenação de leaderboard e validação de dados.
 */
class ScoreManager {
private:
    std::vector<ScoreEntry> scores;     ///< Lista de pontuações
    size_t maxScores;                   ///< Número máximo de pontuações mantidas
    std::string filename;               ///< Nome do arquivo de persistência
    bool fileAvailable;                 ///< Indica se o arquivo está disponível

    /**
     * @brief Verifica se o arquivo de pontuações está disponível
     * @return true se o arquivo pode ser usado
     */
    bool checkFileAvailability();

    /**
     * @brief Valida e corrige formato de pontuação para dados carregados
     */
    void validateScoreFormat();

    /**
     * @brief Carrega pontuações do arquivo
     * @return true se carregou com sucesso
     */
    bool loadScores();

    /**
     * @brief Salva pontuações no arquivo
     * @return true se salvou com sucesso
     */
    bool saveScores();

    /**
     * @brief Ordena pontuações em ordem decrescente (maior primeiro)
     */
    void sortScores();

    /**
     * @brief Formata uma data para exibição
     * @param dateString String de data ISO
     * @return String de data formatada
     */
    std::string formatDate(const std::string& dateString) const;

    /**
     * @brief Formata duração em milissegundos para string legível
     * @param duration Duração em milissegundos
     * @return String de duração formatada
     */
    std::string formatDuration(long long duration) const;

    /**
     * @brief Obtém timestamp atual como string
     * @return String com timestamp atual
     */
    std::string getCurrentTimestamp() const;

public:
    /**
     * @brief Construtor da classe ScoreManager
     * @param maxScores Número máximo de pontuações a manter (padrão: 10)
     * @param filename Nome do arquivo para persistência (padrão: "scores.dat")
     */
    explicit ScoreManager(size_t maxScores = 10, const std::string& filename = "scores.dat");

    /**
     * @brief Destrutor - salva automaticamente ao destruir
     */
    ~ScoreManager();

    /**
     * @brief Construtor de cópia
     * @param other Objeto a ser copiado
     */
    ScoreManager(const ScoreManager& other);

    /**
     * @brief Operador de atribuição
     * @param other Objeto a ser atribuído
     * @return Referência para este objeto
     */
    ScoreManager& operator=(const ScoreManager& other);

    /**
     * @brief Adiciona uma nova pontuação ao leaderboard
     * @param playerName Nome do jogador
     * @param score Pontuação do jogador
     * @param additionalData Dados adicionais da pontuação
     * @return Mapa com resultado da operação
     */
    std::map<std::string, std::string> addScore(const std::string& playerName, 
                                               int score, 
                                               const std::map<std::string, std::string>& additionalData = {});

    /**
     * @brief Obtém todas as pontuações no leaderboard
     * @param limit Número máximo de pontuações a retornar (-1 para todas)
     * @return Vetor de entradas de pontuação
     */
    std::vector<ScoreEntry> getScores(int limit = -1) const;

    /**
     * @brief Obtém a pontuação mais alta
     * @return Ponteiro para a melhor pontuação ou nullptr se não houver pontuações
     */
    const ScoreEntry* getTopScore() const;

    /**
     * @brief Obtém pontuações para um jogador específico
     * @param playerName Nome do jogador
     * @return Vetor de pontuações para o jogador
     */
    std::vector<ScoreEntry> getPlayerScores(const std::string& playerName) const;

    /**
     * @brief Obtém a melhor pontuação do jogador
     * @param playerName Nome do jogador
     * @return Ponteiro para a melhor pontuação do jogador ou nullptr
     */
    const ScoreEntry* getPlayerBestScore(const std::string& playerName) const;

    /**
     * @brief Verifica se uma pontuação se qualifica para o leaderboard
     * @param score Pontuação a verificar
     * @return true se a pontuação se qualifica para o leaderboard
     */
    bool isQualifyingScore(int score) const;

    /**
     * @brief Obtém estatísticas do leaderboard
     * @return Mapa com estatísticas sobre o leaderboard
     */
    std::map<std::string, std::string> getStatistics() const;

    /**
     * @brief Limpa todas as pontuações do leaderboard
     * @return true se as pontuações foram limpas com sucesso
     */
    bool clearScores();

    /**
     * @brief Remove uma pontuação específica por índice
     * @param index Índice da pontuação a remover
     * @return true se a pontuação foi removida com sucesso
     */
    bool removeScore(size_t index);

    /**
     * @brief Remove todas as pontuações para um jogador específico
     * @param playerName Nome do jogador cujas pontuações devem ser removidas
     * @return Número de pontuações removidas
     */
    int removePlayerScores(const std::string& playerName);

    /**
     * @brief Exporta pontuações para uma string formatada
     * @return String com representação das pontuações
     */
    std::string exportScores() const;

    /**
     * @brief Importa pontuações de uma string formatada
     * @param data String contendo pontuações
     * @param merge Se deve mesclar com pontuações existentes ou substituir
     * @return Mapa com resultado da importação
     */
    std::map<std::string, std::string> importScores(const std::string& data, bool merge = false);

    /**
     * @brief Obtém o nome do arquivo sendo usado
     * @return Nome do arquivo
     */
    std::string getFilename() const;

    /**
     * @brief Verifica se o armazenamento está funcionando
     * @return true se o armazenamento está disponível
     */
    bool isStorageWorking() const;

    /**
     * @brief Obtém o número máximo de pontuações mantidas
     * @return Número máximo de pontuações
     */
    size_t getMaxScores() const;

    /**
     * @brief Define um novo número máximo de pontuações
     * @param newMax Novo número máximo de pontuações
     * @return true se o máximo foi atualizado com sucesso
     */
    bool setMaxScores(size_t newMax);

    /**
     * @brief Força salvamento das pontuações
     * @return true se salvou com sucesso
     */
    bool forceSave();

    /**
     * @brief Recarrega pontuações do arquivo
     * @return true se recarregou com sucesso
     */
    bool reload();

    /**
     * @brief Obtém número total de pontuações armazenadas
     * @return Número de pontuações
     */
    size_t getTotalScores() const;
};

#endif // SCORE_MANAGER_H