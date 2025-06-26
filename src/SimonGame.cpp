/**
 * @file SimonGame.cpp
 * @brief Implementação da classe SimonGame
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#include "SimonGame.h"
#include <iostream>
#include <thread>
#include <limits>
#include <algorithm>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <cstdlib>
#endif

SimonGame::SimonGame() 
    : currentState(GameState::MENU), gameRunning(true), currentSequenceIndex(0),
      maxInputTime(5000), sequenceSpeed(1000), minSequenceSpeed(300), 
      speedDecrement(50), soundEnabled(true) {
    
    initializeComponents();
    loadGameConfig();
}

SimonGame::~SimonGame() {
    saveGameConfig();
}

void SimonGame::initializeComponents() {
    sequenceGenerator.reset(new SequenceGenerator(
        std::vector<std::string>{"A", "B", "C", "D"}, 1));
    player.reset(new Player("Jogador", 3));
    scoreManager.reset(new ScoreManager(10, "scores.dat"));
    
    // Inicializar estatísticas
    gameAnalytics["totalGamesPlayed"] = 0;
    gameAnalytics["totalSequencesCompleted"] = 0;
    gameAnalytics["bestLevel"] = 0;
    gameAnalytics["longestStreak"] = 0;
}

void SimonGame::loadGameConfig() {
    // Em uma implementação completa, carregaria configurações de arquivo
    // Por simplicidade, usando valores padrão
}

void SimonGame::saveGameConfig() {
    // Em uma implementação completa, salvaria configurações em arquivo
}

void SimonGame::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void SimonGame::displayGameHeader() {
    std::cout << "===============================================\n";
    std::cout << "           🎮 JOGO SIMON - MEMORIA 🎮\n";
    std::cout << "===============================================\n\n";
}

void SimonGame::displayPlayerStats() {
    auto stats = player->getStatistics();
    
    std::cout << "👤 Jogador: " << stats["name"] << "\n";
    std::cout << "🏆 Pontuacao: " << stats["score"] << "\n";
    std::cout << "📊 Nivel: " << stats["level"] << "\n";
    std::cout << "❤️  Vidas: " << stats["lives"] << "/" << stats["maxLives"] << "\n";
    std::cout << "🔥 Sequencia Atual: " << stats["currentStreak"] << "\n";
    std::cout << "⏱️  Tempo: " << stats["formattedDuration"] << "\n";
    std::cout << "🎯 Precisao: " << stats["accuracy"] << "%\n";
    std::cout << "-----------------------------------------------\n\n";
}

void SimonGame::displaySequence(const std::vector<std::string>& sequence) {
    std::cout << "🔍 MEMORIZE A SEQUENCIA:\n\n";
    std::cout << "⏳ Prepare-se... a sequencia vai comecar!\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    for (size_t i = 0; i < sequence.size(); ++i) {
        // Limpar linha anterior e mostrar elemento atual
        std::cout << "\r                                    "; // Limpar linha
        std::cout << "\r   Passo " << (i + 1) << "/" << sequence.size() << ": [" << sequence[i] << "]";
        std::cout.flush();
        
        if (soundEnabled) {
            playSound(sequence[i]);
        }
        
        // Pausa para memorizar
        std::this_thread::sleep_for(std::chrono::milliseconds(sequenceSpeed));
        
        // Limpar o elemento atual
        std::cout << "\r                                    ";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    std::cout << "\r✅ Sequencia completa!\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void SimonGame::playSound(const std::string& symbol) {
    if (!soundEnabled) return;
    
    // Simulação de som usando caracteres ASCII e frequências
    if (symbol == "A") {
        std::cout << " ♪(DO)";
    } else if (symbol == "B") {
        std::cout << " ♫(RE)";
    } else if (symbol == "C") {
        std::cout << " ♪(MI)";
    } else if (symbol == "D") {
        std::cout << " ♫(FA)";
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void SimonGame::waitForEnter() {
    std::cout << "\nPressione ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::string SimonGame::getValidInput(const std::string& prompt, 
                                   const std::vector<std::string>& validInputs) {
    std::string input;
    bool valid = false;
    
    while (!valid) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        // Converter para maiúscula
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        // Verificar se é entrada válida
        for (const auto& validInput : validInputs) {
            if (input == validInput) {
                valid = true;
                break;
            }
        }
        
        if (!valid) {
            std::cout << "❌ Entrada invalida! Opcoes validas: ";
            for (size_t i = 0; i < validInputs.size(); ++i) {
                std::cout << validInputs[i];
                if (i < validInputs.size() - 1) std::cout << ", ";
            }
            std::cout << "\n\n";
        }
    }
    
    return input;
}

void SimonGame::updateSequenceSpeed() {
    int newSpeed = sequenceSpeed - (player->getLevel() - 1) * speedDecrement;
    sequenceSpeed = std::max(minSequenceSpeed, newSpeed);
}

int SimonGame::run() {
    try {
        while (gameRunning) {
            switch (currentState) {
                case GameState::MENU:
                    showMainMenu();
                    break;
                case GameState::PLAYING:
                    // Lógica principal do jogo é tratada em startNewGame()
                    break;
                case GameState::GAME_OVER:
                    currentState = GameState::MENU;
                    break;
                default:
                    currentState = GameState::MENU;
                    break;
            }
        }
        
        std::cout << "\n🎮 Obrigado por jogar Simon! Até logo!\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Erro fatal: " << e.what() << std::endl;
        return 1;
    }
}

void SimonGame::showMainMenu() {
    clearScreen();
    displayGameHeader();
    
    std::cout << "📋 MENU PRINCIPAL:\n\n";
    std::cout << "1. 🎯 Jogar\n";
    std::cout << "2. 🏆 Ver Recordes\n";
    std::cout << "3. ⚙️  Configuracoes\n";
    std::cout << "4. ℹ️  Creditos\n";
    std::cout << "5. 🚪 Sair\n\n";
    
    std::string choice = getValidInput("👉 Escolha uma opcao (1-5): ", 
                                     {"1", "2", "3", "4", "5"});
    
    switch (choice[0]) {
        case '1': {
            std::cout << "\n👤 Digite seu nome (ou ENTER para 'Jogador'): ";
            std::string playerName;
            std::getline(std::cin, playerName);
            if (playerName.empty()) playerName = "Jogador";
            startNewGame(playerName);
            break;
        }
        case '2':
            showHighScores();
            break;
        case '3':
            showSettings();
            break;
        case '4':
            showCredits();
            break;
        case '5':
            gameRunning = false;
            break;
    }
}

bool SimonGame::startNewGame(const std::string& playerName) {
    try {
        currentState = GameState::PLAYING;
        
        // Configurar jogador
        player->setName(playerName);
        player->startGame();
        
        // Resetar componentes
        sequenceGenerator->reset();
        sequenceSpeed = 1000;
        currentSequenceIndex = 0;
        
        // Atualizar estatísticas
        gameAnalytics["totalGamesPlayed"]++;
        
        std::cout << "\n🚀 Iniciando novo jogo para " << playerName << "!\n";
        waitForEnter();
        
        // Loop principal do jogo
        while (currentState == GameState::PLAYING && player->isAlive()) {
            clearScreen();
            displayGameHeader();
            displayPlayerStats();
            
            // Gerar próximo elemento na sequência
            std::string newElement = sequenceGenerator->addRandomElement();
            if (newElement.empty()) {
                // Sequência máxima atingida - vitória!
                handleGameComplete();
                break;
            }
            
            
            // Mostrar sequência ao jogador
            showSequenceToPlayer();
            
            // Aguardar entrada do jogador
            waitForPlayerInput();
        }
        
        if (!player->isAlive()) {
            handleGameOver();
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Erro ao iniciar jogo: " << e.what() << std::endl;
        currentState = GameState::MENU;
        return false;
    }
}

void SimonGame::showSequenceToPlayer() {
    currentState = GameState::SHOWING_SEQUENCE;
    
    auto sequence = sequenceGenerator->getCurrentSequence();
    
    std::cout << "🎯 Nivel " << player->getLevel() << " - Sequencia de " << sequence.size() << " elementos\n\n";
    std::cout << "🧠 ATENCAO: Memorize bem a sequencia!\n";
    std::cout << "⚠️  Ela sera mostrada apenas UMA vez!\n\n";
    
    waitForEnter();
    
    displaySequence(sequence);
    
    // Limpar tela após mostrar sequência
    clearScreen();
    displayGameHeader();
    displayPlayerStats();
    std::cout << "🔥 Agora é sua vez! A sequencia foi mostrada.\n";
    std::cout << "🧠 Confie na sua memoria!\n\n";
}

void SimonGame::waitForPlayerInput() {
    currentState = GameState::WAITING_INPUT;
    currentSequenceIndex = 0;
    
    player->startInputSequence();
    auto targetSequence = sequenceGenerator->getCurrentSequence();
    
    std::cout << "🎯 SUA VEZ! Repita a sequencia de " << targetSequence.size() << " elementos:\n\n";
    std::cout << "📝 Opcoes disponiveis: A, B, C, D\n";
    std::cout << "🚪 Digite 'SAIR' para voltar ao menu\n";
    std::cout << "🔄 Digite 'REVELAR' se quiser ver a sequencia novamente (penalidade: -1 vida)\n\n";
    std::cout << "💡 Sua entrada ate agora: ";
    
    for (size_t i = 0; i < targetSequence.size(); ++i) {
        std::cout << "\n🎯 Elemento " << (i + 1) << "/" << targetSequence.size() << ": ";
        
        std::string input = getValidInput("", {"A", "B", "C", "D", "SAIR", "REVELAR"});
        
        if (input == "SAIR") {
            currentState = GameState::MENU;
            return;
        }
        
        if (input == "REVELAR") {
            std::cout << "\n⚠️  PENALIDADE: Voce perdeu uma vida por revelar a sequencia!\n";
            player->loseLife("Revelou a sequencia");
            
            std::cout << "🔍 Sequencia completa: ";
            for (size_t j = 0; j < targetSequence.size(); ++j) {
                std::cout << "[" << targetSequence[j] << "]";
                if (j < targetSequence.size() - 1) std::cout << " -> ";
            }
            std::cout << "\n\n";
            
            if (!player->isAlive()) {
                handleGameOver();
                return;
            }
            
            std::cout << "🔄 Continue de onde parou...\n";
            std::cout << "💡 Sua entrada ate agora: ";
            for (size_t j = 0; j < i; ++j) {
                std::cout << "[" << targetSequence[j] << "] ";
            }
            continue;
        }
        
        player->addInput(input);
        
        // Mostrar progresso sem revelar a sequência completa
        std::cout << "[" << input << "] ";
        
        // Verificar se a entrada está correta
        if (input == targetSequence[i]) {
            handleCorrectInput(input);
        } else {
            handleIncorrectInput(input, targetSequence[i]);
            return;
        }
    }
    
    // Se chegou aqui, completou a sequência
    handleSequenceComplete();
}

void SimonGame::handleCorrectInput(const std::string& input) {
    std::cout << "✅ Correto! [" << input << "]\n";
    if (soundEnabled) {
        playSound(input);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void SimonGame::handleIncorrectInput(const std::string& input, const std::string& expected) {
    std::cout << "❌ Incorreto! Voce digitou [" << input 
              << "] mas era [" << expected << "]\n";
    
    bool stillAlive = player->loseLife("Sequencia incorreta");
    
    if (stillAlive) {
        std::cout << "💔 Voce perdeu uma vida! Vidas restantes: " 
                  << player->getLives() << "\n";
        std::cout << "🔄 Tente novamente...\n";
        waitForEnter();
        
        // Mostrar sequência novamente
        showSequenceToPlayer();
        waitForPlayerInput();
    } else {
        currentState = GameState::GAME_OVER;
    }
}

void SimonGame::handleSequenceComplete() {
    int sequenceLength = static_cast<int>(sequenceGenerator->getSequenceLength());
    player->recordSuccessfulSequence(sequenceLength);
    player->advanceLevel();
    updateSequenceSpeed();
    
    gameAnalytics["totalSequencesCompleted"]++;
    
    if (player->getLevel() > gameAnalytics["bestLevel"]) {
        gameAnalytics["bestLevel"] = player->getLevel();
    }
    
    auto stats = player->getStatistics();
    int currentStreak = std::stoi(stats["currentStreak"]);
    if (currentStreak > gameAnalytics["longestStreak"]) {
        gameAnalytics["longestStreak"] = currentStreak;
    }
    
    std::cout << "\n🎉 SEQUENCIA COMPLETA!\n";
    std::cout << "📈 Pontos ganhos: " << (sequenceLength * 10) << "\n";
    std::cout << "🔥 Sequencia de acertos: " << currentStreak << "\n";
    
    // Verificar se atingiu nível máximo
    if (sequenceGenerator->isAtMaxLength()) {
        handleGameComplete();
        return;
    }
    
    waitForEnter();
}

void SimonGame::handleGameComplete() {
    currentState = GameState::GAME_OVER;
    player->endGame();
    
    clearScreen();
    displayGameHeader();
    
    std::cout << "🏆🎉 PARABENS! VOCE COMPLETOU TODOS OS NIVEIS! 🎉🏆\n\n";
    displayPlayerStats();
    
    std::cout << "🌟 Voce e um MESTRE do Simon!\n";
    std::cout << "🥇 Pontuacao maxima alcancada!\n\n";
    
    saveCurrentScore();
    waitForEnter();
}

void SimonGame::handleGameOver() {
    currentState = GameState::GAME_OVER;
    player->endGame();
    
    clearScreen();
    displayGameHeader();
    
    std::cout << "💀 GAME OVER!\n\n";
    displayPlayerStats();
    
    auto stats = player->getStatistics();
    std::cout << "📊 Estatisticas finais:\n";
    std::cout << "   • Sequencias corretas: " << stats["totalCorrectSequences"] << "\n";
    std::cout << "   • Melhor sequencia: " << stats["bestStreak"] << "\n";
    std::cout << "   • Precisao: " << stats["accuracy"] << "%\n";
    std::cout << "   • Tempo de reacao medio: " << stats["averageReactionTime"] << "ms\n\n";
    
    saveCurrentScore();
    waitForEnter();
}

void SimonGame::showHighScores() {
    clearScreen();
    displayGameHeader();
    
    std::cout << "🏆 RECORDES (TOP 10):\n\n";
    
    auto scores = scoreManager->getScores(10);
    
    if (scores.empty()) {
        std::cout << "📝 Nenhum recorde registrado ainda.\n";
        std::cout << "🎯 Seja o primeiro a jogar!\n\n";
    } else {
        std::cout << "Pos.  Nome                Score    Nivel  Data            Precisao\n";
        std::cout << "----------------------------------------------------------------\n";
        
        for (size_t i = 0; i < scores.size(); ++i) {
            const auto& entry = scores[i];
            std::cout << std::setw(3) << (i + 1) << ". "
                      << std::setw(15) << std::left << entry.playerName.substr(0, 15)
                      << std::setw(8) << std::right << entry.score
                      << std::setw(7) << entry.level
                      << "  " << std::setw(12) << std::left << entry.date.substr(0, 12)
                      << std::setw(6) << std::right << std::fixed << std::setprecision(1) 
                      << entry.accuracy << "%\n";
        }
    }
    
    std::cout << "\n";
    waitForEnter();
    currentState = GameState::MENU;
}

void SimonGame::showCredits() {
    clearScreen();
    displayGameHeader();
    
    std::cout << "ℹ️ CREDITOS:\n\n";
    std::cout << "🎮 Jogo Simon - Implementacao C++11\n";
    std::cout << "📚 Projeto Academico\n\n";
    
    std::cout << "🔧 Implementacao:\n";
    std::cout << "   • Arquitetura orientada a objetos\n";
    std::cout << "   • Padroes de design (MVC, Singleton, Strategy)\n";
    std::cout << "   • Gerenciamento de memoria inteligente\n";
    std::cout << "   • Persistencia de dados\n";
    std::cout << "   • Testes unitarios com doctest\n\n";
    
    std::cout << "⚡ Tecnologias:\n";
    std::cout << "   • C++11/14/17 Standard\n";
    std::cout << "   • STL (Standard Template Library)\n";
    std::cout << "   • Chrono para timing\n";
    std::cout << "   • Random para geracao de sequencias\n\n";
    
    std::cout << "🎯 Baseado no jogo classico Simon (1978)\n";
    std::cout << "   Criado por Ralph H. Baer e Howard J. Morrison\n\n";
    
    std::cout << "📅 Versao: " << getVersion() << "\n";
    std::cout << "🗓️ Data: 2025\n\n";
    
    waitForEnter();
    currentState = GameState::MENU;
}

void SimonGame::showSettings() {
    clearScreen();
    displayGameHeader();
    
    std::cout << "⚙️ CONFIGURACOES:\n\n";
    std::cout << "1. 🔊 Sons: " << (soundEnabled ? "Ligado" : "Desligado") << "\n";
    std::cout << "2. ⚡ Velocidade da sequencia: " << sequenceSpeed << "ms\n";
    std::cout << "3. ⏱️ Tempo limite de entrada: " << (maxInputTime / 1000) << "s\n";
    std::cout << "4. 📊 Ver estatisticas do jogo\n";
    std::cout << "5. 🔄 Limpar recordes\n";
    std::cout << "6. 🔙 Voltar ao menu\n\n";
    
    std::string choice = getValidInput("👉 Escolha uma opcao (1-6): ", 
                                     {"1", "2", "3", "4", "5", "6"});
    
    switch (choice[0]) {
        case '1':
            soundEnabled = !soundEnabled;
            std::cout << "\n🔊 Sons " << (soundEnabled ? "ligados" : "desligados") << "!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            showSettings();
            break;
            
        case '2': {
            std::cout << "\n⚡ Velocidade atual: " << sequenceSpeed << "ms\n";
            std::cout << "Digite nova velocidade (300-2000ms): ";
            int newSpeed;
            std::cin >> newSpeed;
            if (newSpeed >= 300 && newSpeed <= 2000) {
                sequenceSpeed = newSpeed;
                std::cout << "✅ Velocidade alterada para " << sequenceSpeed << "ms!\n";
            } else {
                std::cout << "❌ Valor invalido! Mantendo " << sequenceSpeed << "ms.\n";
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            showSettings();
            break;
        }
        
        case '3': {
            std::cout << "\n⏱️ Tempo limite atual: " << (maxInputTime / 1000) << "s\n";
            std::cout << "Digite novo tempo limite (3-30s): ";
            int newTime;
            std::cin >> newTime;
            if (newTime >= 3 && newTime <= 30) {
                maxInputTime = newTime * 1000;
                std::cout << "✅ Tempo limite alterado para " << newTime << "s!\n";
            } else {
                std::cout << "❌ Valor invalido! Mantendo " << (maxInputTime / 1000) << "s.\n";
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            showSettings();
            break;
        }
        
        case '4': {
            std::cout << "\n📊 ESTATISTICAS DO JOGO:\n\n";
            std::cout << "🎮 Total de jogos: " << gameAnalytics["totalGamesPlayed"] << "\n";
            std::cout << "✅ Sequencias completadas: " << gameAnalytics["totalSequencesCompleted"] << "\n";
            std::cout << "🏆 Melhor nivel: " << gameAnalytics["bestLevel"] << "\n";
            std::cout << "🔥 Maior sequencia: " << gameAnalytics["longestStreak"] << "\n\n";
            
            auto scoreStats = scoreManager->getStatistics();
            std::cout << "📈 Estatisticas dos recordes:\n";
            std::cout << "   • Total de recordes: " << scoreStats["totalScores"] << "\n";
            std::cout << "   • Pontuacao media: " << scoreStats["averageScore"] << "\n";
            std::cout << "   • Maior pontuacao: " << scoreStats["highestScore"] << "\n";
            std::cout << "   • Jogadores unicos: " << scoreStats["totalPlayers"] << "\n\n";
            
            waitForEnter();
            showSettings();
            break;
        }
        
        case '5': {
            std::cout << "\n⚠️ ATENCAO: Isso apagara TODOS os recordes!\n";
            std::string confirm = getValidInput("Tem certeza? (S/N): ", {"S", "N"});
            if (confirm == "S") {
                if (scoreManager->clearScores()) {
                    std::cout << "✅ Recordes limpos com sucesso!\n";
                } else {
                    std::cout << "❌ Erro ao limpar recordes!\n";
                }
            } else {
                std::cout << "❌ Operacao cancelada.\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            showSettings();
            break;
        }
        
        case '6':
            currentState = GameState::MENU;
            break;
    }
}

bool SimonGame::saveCurrentScore() {
    auto playerStats = player->getStatistics();
    int score = std::stoi(playerStats["score"]);
    
    if (score <= 0) {
        return false;
    }
    
    std::map<std::string, std::string> additionalData;
    additionalData["level"] = playerStats["level"];
    additionalData["accuracy"] = playerStats["accuracy"];
    additionalData["duration"] = playerStats["gameDuration"];
    additionalData["streak"] = playerStats["bestStreak"];
    
    auto result = scoreManager->addScore(player->getName(), score, additionalData);
    
    if (result["success"] == "true") {
        if (result["saved"] != "true") {
            scoreManager->forceSave();
        }
        std::cout << "💾 Pontuacao salva!\n";
        if (result["isNewRecord"] == "true") {
            std::cout << "🎉 NOVO RECORDE! Parabens!\n";
        }
        std::cout << "🏆 Posicao no ranking: #" << result["rank"] << "\n\n";
        return true;
    } else {
        std::cout << "❌ Erro ao salvar pontuacao: " << result["error"] << "\n\n";
        return false;
    }
}

GameState SimonGame::getCurrentState() const {
    return currentState;
}

bool SimonGame::isGameRunning() const {
    return gameRunning;
}

std::map<std::string, std::string> SimonGame::getCurrentPlayerStats() const {
    return player->getStatistics();
}

std::map<std::string, std::string> SimonGame::getCurrentSequenceInfo() const {
    std::map<std::string, std::string> info;
    
    auto sequence = sequenceGenerator->getCurrentSequence();
    info["length"] = std::to_string(sequence.size());
    info["sequence"] = sequenceGenerator->toString();
    info["isAtMaxLength"] = sequenceGenerator->isAtMaxLength() ? "true" : "false";
    
    auto stats = sequenceGenerator->getSequenceStats();
    info["uniqueColors"] = std::to_string(stats["uniqueColors"]);
    
    return info;
}

bool SimonGame::restartGame() {
    return startNewGame(player->getName());
}

bool SimonGame::togglePause() {
    if (currentState == GameState::PLAYING) {
        currentState = GameState::PAUSED;
        std::cout << "\n⏸️ Jogo pausado. Pressione ENTER para continuar...\n";
        waitForEnter();
        currentState = GameState::PLAYING;
        return true;
    }
    return false;
}

bool SimonGame::processPlayerInput(const std::string& input) {
    if (currentState != GameState::WAITING_INPUT) {
        return false;
    }
    
    auto targetSequence = sequenceGenerator->getCurrentSequence();
    if (static_cast<size_t>(currentSequenceIndex) >= targetSequence.size()) {
        return false;
    }
    
    bool correct = (input == targetSequence[currentSequenceIndex]);
    
    if (correct) {
        handleCorrectInput(input);
        currentSequenceIndex++;
        
        if (static_cast<size_t>(currentSequenceIndex) >= targetSequence.size()) {
            handleSequenceComplete();
        }
    } else {
        handleIncorrectInput(input, targetSequence[currentSequenceIndex]);
    }
    
    return correct;
}

void SimonGame::setSoundEnabled(bool enabled) {
    soundEnabled = enabled;
}

bool SimonGame::isSoundEnabled() const {
    return soundEnabled;
}

void SimonGame::setSequenceSpeed(int speed) {
    sequenceSpeed = std::max(minSequenceSpeed, std::min(2000, speed));
}

int SimonGame::getSequenceSpeed() const {
    return sequenceSpeed;
}

void SimonGame::setMaxInputTime(int timeMs) {
    maxInputTime = std::max(1000, std::min(30000, timeMs));
}

int SimonGame::getMaxInputTime() const {
    return maxInputTime;
}

std::string SimonGame::exportGameConfig() const {
    std::ostringstream oss;
    oss << "soundEnabled=" << (soundEnabled ? "true" : "false") << "\n";
    oss << "sequenceSpeed=" << sequenceSpeed << "\n";
    oss << "maxInputTime=" << maxInputTime << "\n";
    oss << "minSequenceSpeed=" << minSequenceSpeed << "\n";
    oss << "speedDecrement=" << speedDecrement << "\n";
    return oss.str();
}

bool SimonGame::importGameConfig(const std::string& config) {
    std::istringstream iss(config);
    std::string line;
    
    try {
        while (std::getline(iss, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                
                if (key == "soundEnabled") {
                    soundEnabled = (value == "true");
                } else if (key == "sequenceSpeed") {
                    sequenceSpeed = std::stoi(value);
                } else if (key == "maxInputTime") {
                    maxInputTime = std::stoi(value);
                } else if (key == "minSequenceSpeed") {
                    minSequenceSpeed = std::stoi(value);
                } else if (key == "speedDecrement") {
                    speedDecrement = std::stoi(value);
                }
            }
        }
        return true;
    } catch (...) {
        return false;
    }
}

std::string SimonGame::getVersion() {
    return "1.0.0";
}