#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <chrono>

// Función optimizada
std::vector<int> scoreboard(const std::string &string_score) {
    static const std::unordered_map<std::string, int> number_map = {
        {"nil", 0}, {"zero", 0}, {"nothing", 0},
        {"one", 1}, {"two", 2}, {"three", 3},
        {"four", 4}, {"five", 5}, {"six", 6},
        {"seven", 7}, {"eight", 8}, {"nine", 9}
    };

    std::vector<int> result;
    result.reserve(2);

    std::stringstream ss(string_score);
    std::string word;

    while (ss >> word) {
        auto it = number_map.find(word);
        if (it != number_map.end()) {
            result.push_back(it->second);
            if (result.size() == 2) break;
        }
    }

    return result;
}

int main() {
    // Casos de prueba
    std::vector<std::string> test_cases = {
        "The score is four nil",
        "new score: two three",
        "Arsenal 0 Chelsea two",
        "nothing to nine",
        "five six"
    };

    std::cout << "--- PRUEBAS DE FUNCIONALIDAD ---\n";
    for (const auto &test : test_cases) {
        std::vector<int> res = scoreboard(test);
        std::cout << "\"" << test << "\" -> [" 
                  << (res.size() > 0 ? std::to_string(res[0]) : "N/A") << ", " 
                  << (res.size() > 1 ? std::to_string(res[1]) : "N/A") << "]\n";
    }

    // Benchmark rápido de rendimiento
    std::cout << "\n--- MEDIClÓN DE RENDIMIENTO ---\n";
    std::string sample = "Arsenal scored three and Chelsea scored five in the final match";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Ejecutamos 100,000 iteraciones para medir tiempo acumulado
    for (int i = 0; i < 100000; ++i) {
        volatile auto res = scoreboard(sample);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Tiempo total para 100,000 ejecuciones: " << duration << " us (" 
              << duration / 1000.0 << " ms)\n";

    return 0;
}
