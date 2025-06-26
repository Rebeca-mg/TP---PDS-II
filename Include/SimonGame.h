/**
 * @file SimonGame.h
 * @brief Declaração da classe SimonGame - controlador principal do jogo
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#ifndef SIMON_GAME_H
#define SIMON_GAME_H

#include "SequenceGenerator.h"
#include "Player.h"
#include "ScoreManager.h"
#include <memory>
#include <chrono>
#include <iostream>

/**
 * @enum GameState
 * @brief Estados possíveis do jogo
 */
enum class GameState {
    MENU,           ///< No menu principal
    PLAYING,        ///< Jogo em andamento
    SHOWING_SEQUENCE, ///< Mostrando sequência ao jogador
    WAITING_INPUT,  ///< Aguardando entrada do jogador
    GAME_OVER,      ///< Jogo terminado
    PAUSED          ///< Jogo pausado
};

/**
 * @class SimonGame
 * @brief Controlador principal do jogo que orquestra todos os componentes
 * 
 * Esta classe coordena a geração de sequências, entrada do jogador, pontuação
 * e interface do usuário, implementando toda a lógica de fluxo do jogo Simon.
 */
class SimonGame {
private:
    std::unique_ptr<SequenceGenerator> sequenceGenerator; ///< Gerador de sequências
    std::unique_ptr<Player> player;                       ///< Jogador atual
    std::unique_ptr<ScoreManager> scoreManager;           ///< Gerenciador de pontuações
    
    GameState currentState;                               ///< Estado atual do jogo
    bool gameRunning;                                     ///< Indica se o jogo está rodando
    int currentSequenceIndex;                             ///< Índice atual na sequência
    std::chrono::steady_clock::time_point inputStartTime; ///< Tempo de início da entrada
    
    // Configurações do jogo
    int maxInputTime;                                     ///< Tempo máximo para entrada (ms)
    int sequenceSpeed;                                    ///< Velocidade da sequência (ms)
    int minSequenceSpeed;                                 ///< Velocidade mínima (dificuldade máxima)
    int speedDecrement;                                   ///< Decremento de velocidade por nível
    bool soundEnabled;                                    ///< Sons habilitados
    
    // Estatísticas do jogo
    std::map<std::string, int> gameAnalytics;            ///< Análises do jogo

    /**
     * @brief Inicializa os componentes do jogo
     */
    void initializeComponents();

    /**
     * @brief Carrega configurações do jogo
     */
    void loadGameConfig();

    /**
     * @brief Salva configurações do jogo
     */
    void saveGameConfig();

    /**
     * @brief Atualiza a velocidade da sequência baseada no nível
     */
    void updateSequenceSpeed();

    /**
     * @brief Mostra a sequência atual ao jogador
     */
    void showSequenceToPlayer();

    /**
     * @brief Aguarda entrada do jogador
     */
    void waitForPlayerInput();

    /**
     * @brief Processa entrada correta do jogador
     * @param input Entrada do jogador
     */
    void handleCorrectInput(const std::string& input);

    /**
     * @brief Processa entrada incorreta do jogador
     * @param input Entrada incorreta
     * @param expected Entrada esperada
     */
    void handleIncorrectInput(const std::string& input, const std::string& expected);

    /**
     * @brief Manipula conclusão de uma sequência
     */
    void handleSequenceComplete();

    /**
     * @brief Manipula fim de jogo (game over)
     */
    void handleGameOver();

    /**
     * @brief Manipula conclusão completa do jogo (vitória)
     */
    void handleGameComplete();

    /**
     * @brief Limpa a tela do terminal
     */
    void clearScreen();

    /**
     * @brief Exibe cabeçalho do jogo
     */
    void displayGameHeader();

    /**
     * @brief Exibe estatísticas do jogador
     */
    void displayPlayerStats();

    /**
     * @brief Exibe a sequência usando símbolos no terminal
     * @param sequence Sequência a ser exibida
     */
    void displaySequence(const std::vector<std::string>& sequence);

    /**
     * @brief Simula som para um símbolo (usando caracteres ASCII)
     * @param symbol Símbolo que foi pressionado
     */
    void playSound(const std::string& symbol);

    /**
     * @brief Aguarda pressionar Enter para continuar
     */
    void waitForEnter();

    /**
     * @brief Obtém entrada do usuário com validação
     * @param prompt Mensagem para o usuário
     * @param validInputs Entradas válidas aceitas
     * @return Entrada válida do usuário
     */
    std::string getValidInput(const std::string& prompt, 
                             const std::vector<std::string>& validInputs);

public:
    /**
     * @brief Construtor da classe SimonGame
     */
    SimonGame();

    /**
     * @brief Destrutor virtual
     */
    virtual ~SimonGame();

    /**
     * @brief Construtor de cópia (deletado - uso de unique_ptr)
     */
    SimonGame(const SimonGame&) = delete;

    /**
     * @brief Operador de atribuição (deletado - uso de unique_ptr)
     */
    SimonGame& operator=(const SimonGame&) = delete;

    /**
     * @brief Inicia o programa principal
     * @return Código de saída (0 = sucesso)
     */
    int run();

    /**
     * @brief Exibe o menu principal
     */
    void showMainMenu();

    /**
     * @brief Inicia um novo jogo
     * @param playerName Nome do jogador (opcional)
     * @return true se o jogo foi iniciado com sucesso
     */
    bool startNewGame(const std::string& playerName = "");

    /**
     * @brief Reinicia o jogo atual
     * @return true se o jogo foi reiniciado com sucesso
     */
    bool restartGame();

    /**
     * @brief Pausa ou retoma o jogo
     * @return true se a operação foi bem-sucedida
     */
    bool togglePause();

    /**
     * @brief Processa entrada do jogador durante o jogo
     * @param input Entrada do jogador
     * @return true se a entrada foi processada com sucesso
     */
    bool processPlayerInput(const std::string& input);

    /**
     * @brief Exibe recordes (high scores)
     */
    void showHighScores();

    /**
     * @brief Exibe créditos do jogo
     */
    void showCredits();

    /**
     * @brief Exibe configurações do jogo
     */
    void showSettings();

    /**
     * @brief Salva pontuação atual
     * @return true se a pontuação foi salva com sucesso
     */
    bool saveCurrentScore();

    /**
     * @brief Obtém estado atual do jogo
     * @return Estado atual
     */
    GameState getCurrentState() const;

    /**
     * @brief Verifica se o jogo está rodando
     * @return true se o jogo está rodando
     */
    bool isGameRunning() const;

    /**
     * @brief Obtém estatísticas do jogador atual
     * @return Mapa com estatísticas
     */
    std::map<std::string, std::string> getCurrentPlayerStats() const;

    /**
     * @brief Obtém informações da sequência atual
     * @return Mapa com informações da sequência
     */
    std::map<std::string, std::string> getCurrentSequenceInfo() const;

    /**
     * @brief Define se os sons estão habilitados
     * @param enabled true para habilitar sons
     */
    void setSoundEnabled(bool enabled);

    /**
     * @brief Verifica se os sons estão habilitados
     * @return true se os sons estão habilitados
     */
    bool isSoundEnabled() const;

    /**
     * @brief Define velocidade da sequência
     * @param speed Velocidade em milissegundos
     */
    void setSequenceSpeed(int speed);

    /**
     * @brief Obtém velocidade atual da sequência
     * @return Velocidade em milissegundos
     */
    int getSequenceSpeed() const;

    /**
     * @brief Define tempo máximo para entrada
     * @param timeMs Tempo em milissegundos
     */
    void setMaxInputTime(int timeMs);

    /**
     * @brief Obtém tempo máximo para entrada
     * @return Tempo em milissegundos
     */
    int getMaxInputTime() const;

    /**
     * @brief Exporta configurações do jogo
     * @return String com configurações
     */
    std::string exportGameConfig() const;

    /**
     * @brief Importa configurações do jogo
     * @param config String com configurações
     * @return true se importou com sucesso
     */
    bool importGameConfig(const std::string& config);

    /**
     * @brief Obtém versão do jogo
     * @return String com versão
     */
    static std::string getVersion();
};

#endif // SIMON_GAME_H
