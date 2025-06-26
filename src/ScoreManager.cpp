/**
 * @file ScoreManager.cpp
 * @brief Implementação da classe ScoreManager
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#include "ScoreManager.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <set>
#include <cmath>

ScoreManager::ScoreManager(size_t maxScores, const std::string& filename)
    : maxScores(std::max(static_cast<size_t>(1), maxScores)), filename(filename) {
    
    fileAvailable = checkFileAvailability();
    loadScores();
    validateScoreFormat();
}

ScoreManager::~ScoreManager() {
    saveScores();
}

bool ScoreManager::checkFileAvailability() {
    std::ofstream testFile(filename, std::ios::app);
    if (testFile.is_open()) {
        testFile.close();
        return true;
    }
    return false;
}

void ScoreManager::validateScoreFormat() {
    auto it = std::remove_if(scores.begin(), scores.end(), [](const ScoreEntry& entry) {
        return entry.playerName.empty() || entry.score < 0;
    });
    scores.erase(it, scores.end());
    
    // Limitar nome dos jogadores
    for (auto& entry : scores) {
        if (entry.playerName.length() > 20) {
            entry.playerName = entry.playerName.substr(0, 20);
        }
        entry.score = std::max(0, entry.score);
        entry.level = std::max(1, entry.level);
        entry.accuracy = std::max(0.0, std::min(100.0, entry.accuracy));
    }
    
    sortScores();
    if (scores.size() > maxScores) {
        scores.resize(maxScores);
    }
}

bool ScoreManager::loadScores() {
    if (!fileAvailable) {
        scores.clear();
        return false;
    }
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        scores.clear();
        return false;
    }
    
    scores.clear();
    std::string line;
    
    // Formato: nome|pontuacao|nivel|data|precisao|duracao|sequencia
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Ignorar comentários
        
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(iss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 4) {
            ScoreEntry entry;
            entry.playerName = tokens[0];
            entry.score = std::stoi(tokens[1]);
            entry.level = (tokens.size() > 2) ? std::stoi(tokens[2]) : 1;
            entry.date = tokens[3];
            entry.accuracy = (tokens.size() > 4) ? std::stod(tokens[4]) : 0.0;
            entry.duration = (tokens.size() > 5) ? std::stoll(tokens[5]) : 0;
            entry.streak = (tokens.size() > 6) ? std::stoi(tokens[6]) : 0;
            
            scores.push_back(entry);
        }
    }
    
    file.close();
    return true;
}

bool ScoreManager::saveScores() {
    if (!fileAvailable) {
        return false;
    }
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Simon Game High Scores\n";
    file << "# Format: nome|pontuacao|nivel|data|precisao|duracao|sequencia\n";
    
    for (const auto& entry : scores) {
        file << entry.playerName << "|"
             << entry.score << "|"
             << entry.level << "|"
             << entry.date << "|"
             << std::fixed << std::setprecision(1) << entry.accuracy << "|"
             << entry.duration << "|"
             << entry.streak << "\n";
    }
    
    file.close();
    return true;
}

void ScoreManager::sortScores() {
    std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a > b; // Usa o operador > definido em ScoreEntry
    });
}

std::string ScoreManager::formatDate(const std::string& dateString) const {
    if (dateString.empty()) {
        return "Desconhecido";
    }
    
    // Se já estiver formatado, retornar como está
    if (dateString.find('/') != std::string::npos || dateString.find('-') != std::string::npos) {
        return dateString;
    }
    
    // Tentar converter timestamp
    try {
        std::time_t timestamp = std::stoll(dateString);
        std::tm* timeinfo = std::localtime(&timestamp);
        
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << timeinfo->tm_mday << "/"
            << std::setfill('0') << std::setw(2) << (timeinfo->tm_mon + 1) << "/"
            << (timeinfo->tm_year + 1900) << " "
            << std::setfill('0') << std::setw(2) << timeinfo->tm_hour << ":"
            << std::setfill('0') << std::setw(2) << timeinfo->tm_min;
        
        return oss.str();
    } catch (...) {
        return dateString;
    }
}

std::string ScoreManager::formatDuration(long long duration) const {
    if (duration <= 0) {
        return "00:00";
    }
    
    int seconds = static_cast<int>(duration / 1000);
    int minutes = seconds / 60;
    int hours = minutes / 60;
    
    std::ostringstream oss;
    if (hours > 0) {
        oss << hours << ":"
            << std::setfill('0') << std::setw(2) << (minutes % 60) << ":"
            << std::setfill('0') << std::setw(2) << (seconds % 60);
    } else {
        oss << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << (seconds % 60);
    }
    
    return oss.str();
}

std::string ScoreManager::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::tm* timeinfo = std::localtime(&time_t);
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << timeinfo->tm_mday << "/"
        << std::setfill('0') << std::setw(2) << (timeinfo->tm_mon + 1) << "/"
        << (timeinfo->tm_year + 1900) << " "
        << std::setfill('0') << std::setw(2) << timeinfo->tm_hour << ":"
        << std::setfill('0') << std::setw(2) << timeinfo->tm_min;
    
    return oss.str();
}

ScoreManager::ScoreManager(const ScoreManager& other)
    : scores(other.scores), maxScores(other.maxScores), filename(other.filename),
      fileAvailable(other.fileAvailable) {
}

ScoreManager& ScoreManager::operator=(const ScoreManager& other) {
    if (this != &other) {
        scores = other.scores;
        maxScores = other.maxScores;
        filename = other.filename;
        fileAvailable = other.fileAvailable;
    }
    return *this;
}

std::map<std::string, std::string> ScoreManager::addScore(const std::string& playerName, 
                                                         int score,
                                                         const std::map<std::string, std::string>& additionalData) {
    std::map<std::string, std::string> result;
    
    if (playerName.empty() || score < 0) {
        result["success"] = "false";
        result["error"] = "Nome do jogador deve ser não-vazio e pontuação não-negativa";
        result["rank"] = "0";
        result["isNewRecord"] = "false";
        return result;
    }
    
    // Verificar se é um novo recorde
    bool isNewRecord = scores.empty() || score > scores[0].score;
    
    // Criar nova entrada
    ScoreEntry newEntry;
    newEntry.playerName = playerName.substr(0, 20);
    newEntry.score = score;
    newEntry.date = getCurrentTimestamp();
    
    // Dados adicionais opcionais
    auto it = additionalData.find("level");
    newEntry.level = (it != additionalData.end()) ? std::stoi(it->second) : 1;
    
    it = additionalData.find("accuracy");
    newEntry.accuracy = (it != additionalData.end()) ? std::stod(it->second) : 0.0;
    
    it = additionalData.find("duration");
    newEntry.duration = (it != additionalData.end()) ? std::stoll(it->second) : 0;
    
    it = additionalData.find("streak");
    newEntry.streak = (it != additionalData.end()) ? std::stoi(it->second) : 0;
    
    // Adicionar à lista
    scores.push_back(newEntry);
    
    // Ordenar e limitar
    sortScores();
    if (scores.size() > maxScores) {
        scores.resize(maxScores);
    }
    
    // Encontrar posição da nova pontuação
    int rank = 0;
    for (size_t i = 0; i < scores.size(); ++i) {
        if (scores[i].playerName == newEntry.playerName &&
            scores[i].score == newEntry.score &&
            scores[i].date == newEntry.date) {
            rank = static_cast<int>(i + 1);
            break;
        }
    }
    
    bool saved = saveScores();
    
    result["success"] = "true";
    result["error"] = "";
    result["rank"] = std::to_string(rank);
    result["isNewRecord"] = isNewRecord ? "true" : "false";
    result["saved"] = saved ? "true" : "false";
    result["totalScores"] = std::to_string(scores.size());
    
    return result;
}

std::vector<ScoreEntry> ScoreManager::getScores(int limit) const {
    if (limit < 0 || static_cast<size_t>(limit) >= scores.size()) {
        return scores;
    }
    
    return std::vector<ScoreEntry>(scores.begin(), scores.begin() + limit);
}

const ScoreEntry* ScoreManager::getTopScore() const {
    if (scores.empty()) {
        return nullptr;
    }
    return &scores[0];
}

std::vector<ScoreEntry> ScoreManager::getPlayerScores(const std::string& playerName) const {
    std::vector<ScoreEntry> playerScores;
    
    std::string normalizedName = playerName;
    std::transform(normalizedName.begin(), normalizedName.end(), normalizedName.begin(), ::tolower);
    
    for (const auto& entry : scores) {
        std::string entryName = entry.playerName;
        std::transform(entryName.begin(), entryName.end(), entryName.begin(), ::tolower);
        
        if (entryName == normalizedName) {
            playerScores.push_back(entry);
        }
    }
    
    return playerScores;
}

const ScoreEntry* ScoreManager::getPlayerBestScore(const std::string& playerName) const {
    auto playerScores = getPlayerScores(playerName);
    if (playerScores.empty()) {
        return nullptr;
    }
    return &playerScores[0]; // Já está ordenado, primeiro é o melhor
}

bool ScoreManager::isQualifyingScore(int score) const {
    if (score < 0) {
        return false;
    }
    
    // Se temos menos que o máximo, qualquer pontuação válida se qualifica
    if (scores.size() < maxScores) {
        return true;
    }
    
    // Verificar se a pontuação é maior que a menor pontuação
    return score > scores.back().score;
}

std::map<std::string, std::string> ScoreManager::getStatistics() const {
    std::map<std::string, std::string> stats;
    
    if (scores.empty()) {
        stats["totalScores"] = "0";
        stats["averageScore"] = "0";
        stats["highestScore"] = "0";
        stats["lowestScore"] = "0";
        stats["averageLevel"] = "0";
        stats["averageAccuracy"] = "0";
        stats["totalPlayers"] = "0";
        stats["averageDuration"] = "0";
        return stats;
    }
    
    int totalScore = 0, totalLevel = 0;
    double totalAccuracy = 0.0;
    long long totalDuration = 0;
    std::set<std::string> uniquePlayers;
    
    for (const auto& entry : scores) {
        totalScore += entry.score;
        totalLevel += entry.level;
        totalAccuracy += entry.accuracy;
        totalDuration += entry.duration;
        
        std::string normalizedName = entry.playerName;
        std::transform(normalizedName.begin(), normalizedName.end(), normalizedName.begin(), ::tolower);
        uniquePlayers.insert(normalizedName);
    }
    
    size_t count = scores.size();
    stats["totalScores"] = std::to_string(count);
    stats["averageScore"] = std::to_string(totalScore / static_cast<int>(count));
    stats["highestScore"] = std::to_string(scores[0].score);
    stats["lowestScore"] = std::to_string(scores.back().score);
    stats["averageLevel"] = std::to_string(static_cast<int>(std::round(static_cast<double>(totalLevel) / count)));
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << (totalAccuracy / count);
    stats["averageAccuracy"] = oss.str();
    
    stats["totalPlayers"] = std::to_string(uniquePlayers.size());
    stats["averageDuration"] = formatDuration(totalDuration / static_cast<long long>(count));
    
    return stats;
}

bool ScoreManager::clearScores() {
    scores.clear();
    return saveScores();
}

bool ScoreManager::removeScore(size_t index) {
    if (index >= scores.size()) {
        return false;
    }
    
    scores.erase(scores.begin() + index);
    return saveScores();
}

int ScoreManager::removePlayerScores(const std::string& playerName) {
    std::string normalizedName = playerName;
    std::transform(normalizedName.begin(), normalizedName.end(), normalizedName.begin(), ::tolower);
    
    size_t initialSize = scores.size();
    
    auto it = std::remove_if(scores.begin(), scores.end(), [&normalizedName](const ScoreEntry& entry) {
        std::string entryName = entry.playerName;
        std::transform(entryName.begin(), entryName.end(), entryName.begin(), ::tolower);
        return entryName == normalizedName;
    });
    
    scores.erase(it, scores.end());
    
    int removedCount = static_cast<int>(initialSize - scores.size());
    if (removedCount > 0) {
        saveScores();
    }
    
    return removedCount;
}

std::string ScoreManager::exportScores() const {
    std::ostringstream oss;
    
    oss << "# Simon Game High Scores Export\n";
    oss << "# Data de exportação: " << getCurrentTimestamp() << "\n";
    oss << "# Versão: 1.0\n";
    oss << "# Total de pontuações: " << scores.size() << "\n";
    oss << "# Formato: nome|pontuacao|nivel|data|precisao|duracao|sequencia\n\n";
    
    for (const auto& entry : scores) {
        oss << entry.playerName << "|"
            << entry.score << "|"
            << entry.level << "|"
            << entry.date << "|"
            << std::fixed << std::setprecision(1) << entry.accuracy << "|"
            << entry.duration << "|"
            << entry.streak << "\n";
    }
    
    return oss.str();
}

std::map<std::string, std::string> ScoreManager::importScores(const std::string& data, bool merge) {
    std::map<std::string, std::string> result;
    
    if (!merge) {
        scores.clear();
    }
    
    std::istringstream iss(data);
    std::string line;
    int importedCount = 0;
    
    while (std::getline(iss, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream lineStream(line);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(lineStream, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 4) {
            try {
                ScoreEntry entry;
                entry.playerName = tokens[0];
                entry.score = std::stoi(tokens[1]);
                entry.level = (tokens.size() > 2) ? std::stoi(tokens[2]) : 1;
                entry.date = tokens[3];
                entry.accuracy = (tokens.size() > 4) ? std::stod(tokens[4]) : 0.0;
                entry.duration = (tokens.size() > 5) ? std::stoll(tokens[5]) : 0;
                entry.streak = (tokens.size() > 6) ? std::stoi(tokens[6]) : 0;
                
                if (!entry.playerName.empty() && entry.score >= 0) {
                    scores.push_back(entry);
                    importedCount++;
                }
            } catch (...) {
                continue; // Ignorar linhas inválidas
            }
        }
    }
    
    if (importedCount > 0) {
        sortScores();
        if (scores.size() > maxScores) {
            scores.resize(maxScores);
        }
        
        bool saved = saveScores();
        
        result["success"] = "true";
        result["error"] = "";
        result["imported"] = std::to_string(importedCount);
        result["total"] = std::to_string(scores.size());
        result["saved"] = saved ? "true" : "false";
    } else {
        result["success"] = "false";
        result["error"] = "Nenhuma pontuação válida encontrada nos dados de importação";
        result["imported"] = "0";
    }
    
    return result;
}

std::string ScoreManager::getFilename() const {
    return filename;
}

bool ScoreManager::isStorageWorking() const {
    return fileAvailable;
}

size_t ScoreManager::getMaxScores() const {
    return maxScores;
}

bool ScoreManager::setMaxScores(size_t newMax) {
    if (newMax < 1) {
        return false;
    }
    
    maxScores = newMax;
    
    if (scores.size() > maxScores) {
        scores.resize(maxScores);
        saveScores();
    }
    
    return true;
}

bool ScoreManager::forceSave() {
    return saveScores();
}

bool ScoreManager::reload() {
    return loadScores();
}

size_t ScoreManager::getTotalScores() const {
    return scores.size();
}