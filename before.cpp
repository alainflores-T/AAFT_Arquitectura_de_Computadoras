#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

// Tu código original sin modificaciones
std::vector<int> scoreboard(const std::string &exam) {
    // Your code here!
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

        if (cadena == "nothing" or cadena == "zero" or cadena == "nil") {
            scoreboard.push_back(0);
        }

        if (cadena == "one") {
            scoreboard.push_back(1);
        }
        if (cadena == "two") {
            scoreboard.push_back(2);
        }
        if (cadena == "three") {
            scoreboard.push_back(3);
        }
        if (cadena == "four") {
            scoreboard.push_back(4);
        }
        if (cadena == "five") {
            scoreboard.push_back(5);
        }
        if (cadena == "six") {
            scoreboard.push_back(6);
        }
        if (cadena == "seven") {
            scoreboard.push_back(7);
        }
        if (cadena == "eight") {
            scoreboard.push_back(8);
        }
        if (cadena == "nine") {
            scoreboard.push_back(9);
        }
    }
    return scoreboard;
}

int main() {
    // 1. Pruebas de funcionalidad
    vector<string> test_cases = {
        "The score is four nil",
        "new score: two three",
        "nothing to nine",
        "five six"
    };

    cout << "--- PRUEBAS CON TU CODIGO ORIGINAL ---\n";
    for (const auto &test : test_cases) {
        vector<int> res = scoreboard(test);
        cout << "\"" << test << "\" -> [" << res[0] << ", " << res[1] << "]\n";
    }

    // 2. Medición de rendimiento (Benchmark)
    cout << "\n--- MEDICION DE RENDIMIENTO ---\n";
    string sample = "Arsenal scored three and Chelsea scored five";
    
    auto start = chrono::high_resolution_clock::now();
    
    // Ejecutamos 100,000 iteraciones para medir el tiempo acumulado
    for (int i = 0; i < 100000; ++i) {
        volatile auto res = scoreboard(sample);
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << "Tiempo para 100,000 ejecuciones: " << duration << " us (" 
         << duration / 1000.0 << " ms)\n";

    return 0;
}
