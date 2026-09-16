#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

std::vector<int> scoreboard(const std::string &string_score) {
    // Mapa Hash estático: evita recrear la estructura en cada llamada a la función
    static const std::unordered_map<std::string, int> number_map = {
        {"nil", 0}, {"zero", 0}, {"nothing", 0},
        {"one", 1}, {"two", 2}, {"three", 3},
        {"four", 4}, {"five", 5}, {"six", 6},
        {"seven", 7}, {"eight", 8}, {"nine", 9}
    };

    std::vector<int> result;
    result.reserve(2); // Reserva espacio para los 2 elementos directamente (evita reasignaciones de memoria)

    std::stringstream ss(string_score);
    std::string word;

    while (ss >> word) {
        auto it = number_map.find(word);
        if (it != number_map.end()) {
            result.push_back(it->second);
            if (result.size() == 2) break; // Termina en cuanto encuentra ambos números
        }
    }

    return result;
}
