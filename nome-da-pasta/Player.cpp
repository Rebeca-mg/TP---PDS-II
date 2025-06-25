
/**
 * @file Player.cpp
 * @brief Implementação da classe Player
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#include "Player.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <stdexcept>

Player::Player(const std::string& name, int lives)
    : maxLives(std::max(1, lives)), currentLives(maxLives), score(0), level(1),
      totalCorrectSequences(0), totalWrongAttempts(0), bestStreak(0), currentStreak(0),
      averageReactionTime(0.0) {
    
    this->name = validateAndSetName(name);
    initializePlayerState();
}

std::string Player::validateAndSetName(const std::string& name) {
    if (name.empty()) {
        return "Anonymous";
    }
    
    // Sanitizar nome: remover caracteres especiais, limitar comprimento
    std::string sanitizedName;
    for (char c : name) {
        if (std::isalnum(c) || c == ' ' || c == '-' || c == '_') {
            sanitizedName += c;
        }
    }
    
    // Remover espaços extras
    sanitizedName.erase(0, sanitizedName.find_first_not_of(" \t"));
    sanitizedName.erase(sanitizedName.find_last_not_of(" \t") + 1);
    
    if (sanitizedName.empty()) {
        return "Anonymous";
    }
    
    // Limitar comprimento
    if (sanitizedName.length() > 20) {
        sanitizedName = sanitizedName.substr(0, 20);
    }
    
    return sanitizedName;
}

void Player::initializePlayerState() {
    gameHistory.clear();
    reactionTimes.clear();
    lastInputTime = std::chrono::steady_clock::now();
}

Player::Player(const Player& other)
    : name(other.name), maxLives(other.maxLives), currentLives(other.currentLives),
      score(other.score), level(other.level), currentInputSequence(other.currentInputSequence),
      totalCorrectSequences(other.totalCorrectSequences), totalWrongAttempts(other.totalWrongAttempts),
      gameStartTime(other.gameStartTime), gameEndTime(other.gameEndTime),
      bestStreak(other.bestStreak), currentStreak(other.currentStreak),
      averageReactionTime(other.averageReactionTime), reactionTimes(other.reactionTimes),
      lastInputTime(other.lastInputTime), gameHistory(other.gameHistory) {
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        name = other.name;
        maxLives = other.maxLives;
        currentLives = other.currentLives;
        score = other.score;
        level = other.level;
        currentInputSequence = other.currentInputSequence;
        totalCorrectSequences = other.totalCorrectSequences;
        totalWrongAttempts = other.totalWrongAttempts;
        gameStartTime = other.gameStartTime;
        gameEndTime = other.gameEndTime;
        bestStreak = other.bestStreak;
        currentStreak = other.currentStreak;
        averageReactionTime = other.averageReactionTime;
        reactionTimes = other.reactionTimes;
        lastInputTime = other.lastInputTime;
        gameHistory = other.gameHistory;
    }
    return *this;
}

std::string Player::getName() const {
    return name;
}

bool Player::setName(const std::string& newName) {
    std::string validatedName = validateAndSetName(newName);
    if (validatedName != "Anonymous" || newName == "Anonymous") {
        name = validatedName;
        return true;
    }
    return false;
}

int Player::getScore() const {
    return score;
}

void Player::addScore(int points, const std::string& reason) {
    if (points < 0) {
        return;
    }
    
    score += points;
    
    // Registrar evento de pontuação
    std::map<std::string, std::string> event;
    event["action"] = "score_added";
    event["points"] = std::to_string(points);
    event["reason"] = reason;
    event["totalScore"] = std::to_string(score);
    gameHistory.push_back(event);
}

int Player::getLevel() const {
    return level;
}

void Player::advanceLevel(int bonusPoints) {
    level++;
    
    if (bonusPoints > 0) {
        addScore(bonusPoints, "Level " + std::to_string(level - 1) + " completion bonus");
    }
    
    // Atualizar melhor sequência se a atual for melhor
    if (currentStreak > bestStreak) {
        bestStreak = currentStreak;
    }
    
    std::map<std::string, std::string> event;
    event["action"] = "level_advanced";
    event["level"] = std::to_string(level);
    event["bonusPoints"] = std::to_string(bonusPoints);
    gameHistory.push_back(event);
}

int Player::getLives() const {
    return currentLives;
}

int Player::getMaxLives() const {
    return maxLives;
}

bool Player::loseLife(const std::string& reason) {
    if (currentLives > 0) {
        currentLives--;
        totalWrongAttempts++;
        currentStreak = 0; // Resetar sequência em erro
        
        std::map<std::string, std::string> event;
        event["action"] = "life_lost";
        event["reason"] = reason;
        event["livesRemaining"] = std::to_string(currentLives);
        gameHistory.push_back(event);
    }
    
    return isAlive();
}

bool Player::isAlive() const {
    return currentLives > 0;
}

void Player::resetLives() {
    currentLives = maxLives;
}

void Player::startInputSequence() {
    currentInputSequence.clear();
    lastInputTime = std::chrono::steady_clock::now();
}

bool Player::addInput(const std::string& input) {
    if (input.empty()) {
        return false;
    }
    
    auto currentTime = std::chrono::steady_clock::now();
    
    // Calcular tempo de reação se não for a primeira entrada
    if (!currentInputSequence.empty()) {
        auto reactionTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - lastInputTime).count();
        reactionTimes.push_back(static_cast<double>(reactionTime));
    }
    
    currentInputSequence.push_back(input);
    lastInputTime = currentTime;
    
    std::map<std::string, std::string> event;
    event["action"] = "input_added";
    event["input"] = input;
    event["sequencePosition"] = std::to_string(currentInputSequence.size() - 1);
    gameHistory.push_back(event);
    
    return true;
}

std::vector<std::string> Player::getCurrentInput() const {
    return currentInputSequence;
}

void Player::clearCurrentInput() {
    currentInputSequence.clear();
    lastInputTime = std::chrono::steady_clock::now();
}

void Player::recordSuccessfulSequence(int sequenceLength) {
    totalCorrectSequences++;
    currentStreak++;
    
    // Calcular pontuação base com bônus
    int baseScore = sequenceLength * 10;
    int streakBonus = (currentStreak / 3) * 5; // Bônus a cada 3 sequências corretas
    int levelBonus = (level - 1) * 2; // Bônus crescente por nível
    
    int totalPoints = baseScore + streakBonus + levelBonus;
    addScore(totalPoints, "Sequence completed (Length: " + std::to_string(sequenceLength) + ")");
    
    std::map<std::string, std::string> event;
    event["action"] = "sequence_completed";
    event["sequenceLength"] = std::to_string(sequenceLength);
    event["streak"] = std::to_string(currentStreak);
    event["pointsEarned"] = std::to_string(totalPoints);
    gameHistory.push_back(event);
}

void Player::startGame() {
    gameStartTime = std::chrono::steady_clock::now();
    gameEndTime = std::chrono::steady_clock::time_point{};
    
    // Resetar estatísticas específicas do jogo
    score = 0;
    level = 1;
    currentStreak = 0;
    totalCorrectSequences = 0;
    totalWrongAttempts = 0;
    reactionTimes.clear();
    gameHistory.clear();
    resetLives();
    
    std::map<std::string, std::string> event;
    event["action"] = "game_started";
    event["playerName"] = name;
    gameHistory.push_back(event);
}

void Player::endGame() {
    gameEndTime = std::chrono::steady_clock::now();
    
    // Calcular tempo médio de reação
    if (!reactionTimes.empty()) {
        double sum = 0.0;
        for (double time : reactionTimes) {
            sum += time;
        }
        averageReactionTime = sum / reactionTimes.size();
    }
    
    std::map<std::string, std::string> event;
    event["action"] = "game_ended";
    event["finalScore"] = std::to_string(score);
    event["finalLevel"] = std::to_string(level);
    event["totalTime"] = std::to_string(getGameDuration());
    gameHistory.push_back(event);
}

long long Player::getGameDuration() const {
    if (gameStartTime == std::chrono::steady_clock::time_point{}) {
        return 0;
    }
    
    auto endTime = (gameEndTime == std::chrono::steady_clock::time_point{}) ? 
                   std::chrono::steady_clock::now() : gameEndTime;
    
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - gameStartTime).count();
}

std::string Player::getFormattedGameDuration() const {
    long long durationMs = getGameDuration();
    int seconds = static_cast<int>(durationMs / 1000);
    int minutes = seconds / 60;
    int remainingSeconds = seconds % 60;
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << remainingSeconds;
    
    return oss.str();
}

std::map<std::string, std::string> Player::getStatistics() const {
    std::map<std::string, std::string> stats;
    
    stats["name"] = name;
    stats["score"] = std::to_string(score);
    stats["level"] = std::to_string(level);
    stats["lives"] = std::to_string(currentLives);
    stats["maxLives"] = std::to_string(maxLives);
    stats["totalCorrectSequences"] = std::to_string(totalCorrectSequences);
    stats["totalWrongAttempts"] = std::to_string(totalWrongAttempts);
    stats["currentStreak"] = std::to_string(currentStreak);
    stats["bestStreak"] = std::to_string(bestStreak);
    stats["averageReactionTime"] = std::to_string(static_cast<int>(averageReactionTime));
    stats["gameDuration"] = std::to_string(getGameDuration());
    stats["formattedDuration"] = getFormattedGameDuration();
    
    int totalAttempts = getTotalAttempts();
    if (totalAttempts > 0) {
        double accuracy = (static_cast<double>(totalCorrectSequences) / totalAttempts) * 100.0;
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << accuracy;
        stats["accuracy"] = oss.str();
    } else {
        stats["accuracy"] = "0.0";
    }
    
    stats["isGameActive"] = (gameStartTime != std::chrono::steady_clock::time_point{} && 
                            gameEndTime == std::chrono::steady_clock::time_point{}) ? "true" : "false";
    stats["isAlive"] = isAlive() ? "true" : "false";
    
    return stats;
}

int Player::getTotalAttempts() const {
    return totalCorrectSequences + totalWrongAttempts;
}

std::vector<std::map<std::string, std::string>> Player::getGameHistory() const {
    return gameHistory;
}

void Player::reset() {
    score = 0;
    level = 1;
    currentLives = maxLives;
    currentInputSequence.clear();
    totalCorrectSequences = 0;
    totalWrongAttempts = 0;
    bestStreak = 0;
    currentStreak = 0;
    averageReactionTime = 0.0;
    reactionTimes.clear();
    gameStartTime = std::chrono::steady_clock::time_point{};
    gameEndTime = std::chrono::steady_clock::time_point{};
    lastInputTime = std::chrono::steady_clock::now();
    gameHistory.clear();
}

std::map<std::string, std::string> Player::toSaveData() const {
    std::map<std::string, std::string> saveData;
    
    saveData["name"] = name;
    saveData["score"] = std::to_string(score);
    saveData["level"] = std::to_string(level);
    saveData["bestStreak"] = std::to_string(bestStreak);
    saveData["totalCorrectSequences"] = std::to_string(totalCorrectSequences);
    saveData["averageReactionTime"] = std::to_string(averageReactionTime);
    saveData["gameDuration"] = std::to_string(getGameDuration());
    
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    saveData["gameEndTime"] = std::to_string(time_t);
    
    int totalAttempts = getTotalAttempts();
    if (totalAttempts > 0) {
        double accuracy = (static_cast<double>(totalCorrectSequences) / totalAttempts) * 100.0;
        saveData["accuracy"] = std::to_string(accuracy);
    } else {
        saveData["accuracy"] = "0.0";
    }
    
    return saveData;
}

bool Player::fromSaveData(const std::map<std::string, std::string>& saveData) {
    try {
        auto it = saveData.find("name");
        if (it != saveData.end()) {
            setName(it->second);
        }
        
        it = saveData.find("bestStreak");
        if (it != saveData.end()) {
            bestStreak = std::stoi(it->second);
        }
        
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

int Player::compareWith(const Player& other) const {
    // Comparação primária: pontuação
    if (score != other.score) {
        return other.score - score; // Pontuação maior fica primeiro
    }
    
    // Comparação secundária: nível
    if (level != other.level) {
        return other.level - level; // Nível maior fica primeiro
    }
    
    // Comparação terciária: melhor sequência
    if (bestStreak != other.bestStreak) {
        return other.bestStreak - bestStreak; // Sequência maior fica primeiro
    }
    
    // Comparação final: precisão
    int thisAttempts = getTotalAttempts();
    int otherAttempts = other.getTotalAttempts();
    
    double thisAccuracy = (thisAttempts > 0) ? 
        (static_cast<double>(totalCorrectSequences) / thisAttempts) : 0.0;
    double otherAccuracy = (otherAttempts > 0) ? 
        (static_cast<double>(other.totalCorrectSequences) / otherAttempts) : 0.0;
    
    if (thisAccuracy != otherAccuracy) {
        return (otherAccuracy > thisAccuracy) ? 1 : -1;
    }
    
    return 0; // Iguais
}

bool Player::operator==(const Player& other) const {
    return name == other.name && score == other.score && level == other.level;
}

bool Player::operator!=(const Player& other) const {
    return !(*this == other);
}

bool Player::operator<(const Player& other) const {
    return compareWith(other) > 0;
}

bool Player::operator>(const Player& other) const {
    return compareWith(other) < 0;
}

