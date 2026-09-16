#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <chrono>
#include <iomanip>

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

// 2. TU CÓDIGO OPTIMIZADO (AFTER)
vector<int> scoreboard_after(const string &string_score) {
    static const unordered_map<string, int> number_map = {
        {"nil", 0}, {"zero", 0}, {"nothing", 0},
        {"one", 1}, {"two", 2}, {"three", 3},
        {"four", 4}, {"five", 5}, {"six", 6},
        {"seven", 7}, {"eight", 8}, {"nine", 9}
    };

    vector<int> result;
    result.reserve(2);

    stringstream ss(string_score);
    string word;

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
