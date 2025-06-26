/**
 * @file main.cpp
 * @brief Ponto de entrada principal do Jogo Simon
 * @author Projeto Acadêmico C++11
 * @date 2025
 * @version 1.0
 */

#include "SimonGame.h"
#include <iostream>
#include <exception>

/**
 * @brief Função principal do programa
 * 
 * Inicializa o jogo Simon e executa o loop principal.
 * Trata exceções globais e retorna código de saída apropriado.
 * 
 * @return int Código de saída (0 = sucesso, != 0 = erro)
 */
int main() {
    try {
        // Exibir informações de inicialização
        std::cout << "=================================================\n";
        std::cout << "    🎮 JOGO SIMON - PROJETO ACADÊMICO C++11\n";
        std::cout << "=================================================\n";
        std::cout << "Versão: " << SimonGame::getVersion() << "\n";
        std::cout << "Autor: Projeto Acadêmico\n";
        std::cout << "Data: 2025\n\n";
        
        std::cout << "Inicializando componentes do jogo...\n";
        
        // Criar instância do jogo principal
        SimonGame game;
        
        std::cout << "✅ Jogo inicializado com sucesso!\n";
        std::cout << "🚀 Iniciando Simon Game...\n\n";
        
        // Executar o jogo
        int exitCode = game.run();
        
        if (exitCode == 0) {
            std::cout << "\n✅ Jogo encerrado normalmente.\n";
        } else {
            std::cout << "\n⚠️ Jogo encerrado com código: " << exitCode << "\n";
        }
        
        return exitCode;
        
    } catch (const std::bad_alloc& e) {
        std::cerr << "\n❌ ERRO CRÍTICO: Memória insuficiente!\n";
        std::cerr << "Detalhes: " << e.what() << std::endl;
        return 2;
        
    } catch (const std::runtime_error& e) {
        std::cerr << "\n❌ ERRO DE EXECUÇÃO: " << e.what() << std::endl;
        std::cerr << "O jogo não pode continuar.\n";
        return 3;
        
    } catch (const std::logic_error& e) {
        std::cerr << "\n❌ ERRO LÓGICO: " << e.what() << std::endl;
        std::cerr << "Erro interno do programa.\n";
        return 4;
        
    } catch (const std::exception& e) {
        std::cerr << "\n❌ ERRO INESPERADO: " << e.what() << std::endl;
        std::cerr << "Tipo: " << typeid(e).name() << std::endl;
        return 5;
        
    } catch (...) {
        std::cerr << "\n❌ ERRO DESCONHECIDO!\n";
        std::cerr << "Ocorreu uma exceção não tratada.\n";
        return 6;
    }
}
