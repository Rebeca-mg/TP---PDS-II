/**
 * @file SequenceGenerator.cpp
 * @brief Implementação da classe SequenceGenerator
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#include "SequenceGenerator.h"
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <map>
#include <climits>

SequenceGenerator::SequenceGenerator(const std::vector<std::string>& colors, size_t initialLength)
    : availableColors(colors), initialLength(initialLength), maxSequenceLength(50),
      randomGenerator(std::random_device{}()) {
    
    validateConstructorParams();
    distribution = std::uniform_int_distribution<int>(0, static_cast<int>(availableColors.size() - 1));
    initializeSequence();
}

void SequenceGenerator::validateConstructorParams() {
    if (availableColors.empty()) {
        throw std::invalid_argument("SequenceGenerator: availableColors não pode estar vazio");
    }
    
    if (initialLength < 1) {
        throw std::invalid_argument("SequenceGenerator: initialLength deve ser um número positivo");
    }
    
    // Verificar se não há cores duplicadas
    std::vector<std::string> sortedColors = availableColors;
    std::sort(sortedColors.begin(), sortedColors.end());
    auto it = std::unique(sortedColors.begin(), sortedColors.end());
    if (it != sortedColors.end()) {
        throw std::invalid_argument("SequenceGenerator: cores duplicadas não são permitidas");
    }
}

void SequenceGenerator::initializeSequence() {
    currentSequence.clear();
    for (size_t i = 0; i < initialLength; ++i) {
        addRandomElement();
    }
}

int SequenceGenerator::generateRandomIndex() {
    return distribution(randomGenerator);
}

SequenceGenerator::SequenceGenerator(const SequenceGenerator& other)
    : availableColors(other.availableColors),
      currentSequence(other.currentSequence),
      initialLength(other.initialLength),
      maxSequenceLength(other.maxSequenceLength),
      randomGenerator(std::random_device{}()),
      distribution(other.distribution) {
}

SequenceGenerator& SequenceGenerator::operator=(const SequenceGenerator& other) {
    if (this != &other) {
        availableColors = other.availableColors;
        currentSequence = other.currentSequence;
        initialLength = other.initialLength;
        maxSequenceLength = other.maxSequenceLength;
        randomGenerator.seed(std::random_device{}());
        distribution = other.distribution;
    }
    return *this;
}

std::string SequenceGenerator::addRandomElement() {
    if (currentSequence.size() >= maxSequenceLength) {
        return "";
    }
    
    int randomIndex = generateRandomIndex();
    std::string newElement = availableColors[randomIndex];
    currentSequence.push_back(newElement);
    
    return newElement;
}

std::vector<std::string> SequenceGenerator::getCurrentSequence() const {
    return currentSequence;
}

size_t SequenceGenerator::getSequenceLength() const {
    return currentSequence.size();
}

std::string SequenceGenerator::getElementAt(size_t index) const {
    if (index >= currentSequence.size()) {
        return "";
    }
    return currentSequence[index];
}

void SequenceGenerator::reset() {
    initializeSequence();
}

bool SequenceGenerator::setCustomSequence(const std::vector<std::string>& sequence) {
    // Validar que todos os elementos são cores válidas
    for (const auto& element : sequence) {
        if (std::find(availableColors.begin(), availableColors.end(), element) == availableColors.end()) {
            return false;
        }
    }
    
    if (sequence.size() > maxSequenceLength) {
        return false;
    }
    
    currentSequence = sequence;
    return true;
}

std::vector<std::string> SequenceGenerator::getAvailableColors() const {
    return availableColors;
}

bool SequenceGenerator::updateAvailableColors(const std::vector<std::string>& newColors) {
    if (newColors.empty()) {
        return false;
    }
    
    // Verificar duplicatas
    std::vector<std::string> sortedColors = newColors;
    std::sort(sortedColors.begin(), sortedColors.end());
    auto it = std::unique(sortedColors.begin(), sortedColors.end());
    if (it != sortedColors.end()) {
        return false;
    }
    
    availableColors = newColors;
    distribution = std::uniform_int_distribution<int>(0, static_cast<int>(availableColors.size() - 1));
    
    // Reiniciar sequência para usar novas cores
    reset();
    return true;
}

std::string SequenceGenerator::previewNextElement() {
    int randomIndex = generateRandomIndex();
    return availableColors[randomIndex];
}

bool SequenceGenerator::isAtMaxLength() const {
    return currentSequence.size() >= maxSequenceLength;
}

std::map<std::string, int> SequenceGenerator::getSequenceStats() const {
    std::map<std::string, int> stats;
    
    stats["length"] = static_cast<int>(currentSequence.size());
    stats["maxLength"] = static_cast<int>(maxSequenceLength);
    stats["uniqueColors"] = 0;
    
    // Contar ocorrências de cada cor
    std::map<std::string, int> colorCounts;
    for (const auto& color : availableColors) {
        colorCounts[color] = 0;
    }
    
    for (const auto& element : currentSequence) {
        colorCounts[element]++;
    }
    
    // Encontrar cor mais frequente e menos frequente
    std::string mostFrequent, leastFrequent;
    int maxCount = -1, minCount = INT_MAX;
    
    for (const auto& pair : colorCounts) {
        if (pair.second > 0) {
            stats["uniqueColors"]++;
        }
        
        if (pair.second > maxCount) {
            maxCount = pair.second;
            mostFrequent = pair.first;
        }
        
        if (pair.second < minCount && pair.second > 0) {
            minCount = pair.second;
            leastFrequent = pair.first;
        }
    }
    
    stats["mostFrequentCount"] = maxCount;
    stats["leastFrequentCount"] = minCount;
    
    return stats;
}

std::string SequenceGenerator::toString(const std::string& separator) const {
    if (currentSequence.empty()) {
        return "";
    }
    
    std::ostringstream oss;
    for (size_t i = 0; i < currentSequence.size(); ++i) {
        if (i > 0) {
            oss << separator;
        }
        oss << currentSequence[i];
    }
    
    return oss.str();
}

std::pair<bool, int> SequenceGenerator::validateUserSequence(const std::vector<std::string>& userSequence, 
                                                            int upToIndex) const {
    size_t checkLength = userSequence.size();
    if (upToIndex >= 0) {
        checkLength = std::min(checkLength, static_cast<size_t>(upToIndex + 1));
    }
    
    for (size_t i = 0; i < checkLength; ++i) {
        if (i >= currentSequence.size()) {
            return std::make_pair(false, static_cast<int>(i));
        }
        
        if (userSequence[i] != currentSequence[i]) {
            return std::make_pair(false, static_cast<int>(i));
        }
    }
    
    return std::make_pair(true, static_cast<int>(checkLength - 1));
}

bool SequenceGenerator::operator==(const SequenceGenerator& other) const {
    return availableColors == other.availableColors &&
           currentSequence == other.currentSequence &&
           initialLength == other.initialLength &&
           maxSequenceLength == other.maxSequenceLength;
}

bool SequenceGenerator::operator!=(const SequenceGenerator& other) const {
    return !(*this == other);
}
