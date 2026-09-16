#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <chrono>
#include <iomanip>
#include <string_view>

using namespace std;

// 1. TU CÓDIGO ORIGINAL (BEFORE)
vector<int> scoreboard_before(const string &exam) {
    vector<int> scoreboard;
    size_t save = 0;
    while (scoreboard.size() != 2) {
        string cadena = "";
        for (size_t i = save; i < exam.size(); ++i) {
            if (exam[i] == ' ') {
                save = i;
                break;
            }
            cadena += exam[i];
        }

        for (size_t j = save; j < exam.size(); ++j) {
            if (exam[j] != ' ') {
                save = j;
                break;
            }
        }

        if (cadena == "nothing" or cadena == "zero" or cadena == "nil") scoreboard.push_back(0);
        if (cadena == "one") scoreboard.push_back(1);
        if (cadena == "two") scoreboard.push_back(2);
        if (cadena == "three") scoreboard.push_back(3);
        if (cadena == "four") scoreboard.push_back(4);
        if (cadena == "five") scoreboard.push_back(5);
        if (cadena == "six") scoreboard.push_back(6);
        if (cadena == "seven") scoreboard.push_back(7);
        if (cadena == "eight") scoreboard.push_back(8);
        if (cadena == "nine") scoreboard.push_back(9);

        if (save >= exam.size() - 1) break; // Protección contra bucle infinito
    }
    return scoreboard;
}





vector<int> scoreboard_after(const string &exam) {
    vector<int> result;
    result.reserve(2); // Evita reasignaciones de memoria dinámica

    const char* str = exam.c_str();
    size_t len = exam.length();
    size_t i = 0;

    while (i < len && result.size() < 2) {
        // 1. Omitir espacios iniciales
        while (i < len && str[i] == ' ') ++i;
        if (i >= len) break;

        size_t start = i;
        while (i < len && str[i] != ' ') ++i;
        size_t word_len = i - start;

        const char* w = &str[start];

        // 2. Filtrar primero por longitud de palabra para minimizar comparaciones
        if (word_len == 3) {
            if (w[0] == 'o' && w[1] == 'n' && w[2] == 'e') result.push_back(1);
            else if (w[0] == 't' && w[1] == 'w' && w[2] == 'o') result.push_back(2);
            else if (w[0] == 's' && w[1] == 'i' && w[2] == 'x') result.push_back(6);
            else if (w[0] == 'n' && w[1] == 'i' && w[2] == 'l') result.push_back(0);
        } else if (word_len == 4) {
            if (w[0] == 'f' && w[1] == 'o' && w[2] == 'u' && w[3] == 'r') result.push_back(4);
            else if (w[0] == 'f' && w[1] == 'i' && w[2] == 'v' && w[3] == 'e') result.push_back(5);
            else if (w[0] == 'n' && w[1] == 'i' && w[2] == 'n' && w[3] == 'e') result.push_back(9);
            else if (w[0] == 'z' && w[1] == 'e' && w[2] == 'r' && w[3] == 'o') result.push_back(0);
        } else if (word_len == 5) {
            if (w[0] == 't' && w[1] == 'h' && w[2] == 'r' && w[3] == 'e' && w[4] == 'e') result.push_back(3);
            else if (w[0] == 'e' && w[1] == 'i' && w[2] == 'g' && w[3] == 'h' && w[4] == 't') result.push_back(8);
            else if (w[0] == 's' && w[1] == 'e' && w[2] == 'v' && w[3] == 'e' && w[4] == 'n') result.push_back(7);
        } else if (word_len == 7) {
            if (w[0] == 'n' && w[1] == 'o' && w[2] == 't' && w[3] == 'h' && w[4] == 'i' && w[5] == 'n' && w[6] == 'g') result.push_back(0);
        }
    }

    return result;
}

int main() {
    const int ITERATIONS = 100000;
    string sample = "Arsenal scored three and Chelsea scored five in the final match";

    // Medición BEFORE
    auto start_b = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        volatile auto res = scoreboard_before(sample);
    }
    auto end_b = chrono::high_resolution_clock::now();
    double time_before = chrono::duration_cast<chrono::microseconds>(end_b - start_b).count() / 1000000.0;

    // Medición AFTER
    auto start_a = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        volatile auto res = scoreboard_after(sample);
    }
    auto end_a = chrono::high_resolution_clock::now();
    double time_after = chrono::duration_cast<chrono::microseconds>(end_a - start_a).count() / 1000000.0;

    // Impresión con el formato exacto del benchmark de laboratorio
    cout << fixed << setprecision(4);
    cout << "=== RESULTADOS DE OPTIMIZACION (N=" << ITERATIONS << ") ===\n";
    cout << "1. Original (Before)   : " << time_before << " s | Speedup: 1.00x\n";
    cout << "2. Optimizado (After)  : " << time_after  << " s | Speedup: " << (time_before / time_after) << "x\n";

    return 0;
}
