#include <iostream>
#include <vector>
#include <map>
#include <ctime> // Для генерации случайных чисел

using namespace std;

// Функция для умножения вектора на матрицу
vector<int> multiplyMatrix(const vector<int>& vec, const vector<vector<int>>& matrix, int mod) {
    vector<int> result(vec.size(), 0);
    for (int i = 0; i < vec.size(); ++i) {
        for (int j = 0; j < vec.size(); ++j) {
            result[i] += vec[j] * matrix[j][i];
        }
        result[i] = (result[i] % mod + mod) % mod;
    }
    return result;
}

// Функция для шифрования вектора с использованием матрицы A и вектора H
vector<int> encryptVector(const vector<int>& vec, const vector<vector<int>>& A, const vector<int>& H, int mod) {
    vector<int> result = multiplyMatrix(vec, A, mod);
    for (int i = 0; i < vec.size(); ++i) {
        result[i] = (result[i] + H[i]) % mod;
    }
    return result;
}

// Функция для преобразования вектора индексов в строку
string vectorToString(const vector<int>& vec, const map<int, char>& indexToChar) {
    string result;
    for (int value : vec) {
        result += indexToChar.at(value);
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "RUS");
    srand(static_cast<unsigned>(time(0))); // Инициализация генератора случайных чисел

    // Задаем алфавит и строку для шифрования
    string alphabet = "АОУЫНТ_";
    string text = "У_АННЫ_НОТЫ_";
    cout << "Алфавит: " << alphabet << endl;
    cout << "Шифруемое сообщение: " << text << endl;

    int N = alphabet.size();
    int m = 3;

    // Создаем отображение символов в индексы и обратно
    map<char, int> charToIndex;
    map<int, char> indexToChar;
    for (int i = 0; i < N; ++i) {
        charToIndex[alphabet[i]] = i;
        indexToChar[i] = alphabet[i];
    }

    // Разбиваем текст на последовательности длины m
    vector<vector<int>> sequences;
    for (size_t i = 0; i < text.length(); i += m) {
        vector<int> sequence;
        for (int j = 0; j < m; ++j) {
            if (i + j < text.length()) {
                sequence.push_back(charToIndex[text[i + j]]);
            }
        }
        sequences.push_back(sequence);
    }

    // Выводим полученные векторы
    cout << "Исходные векторы:\n";
    for (const auto& seq : sequences) {
        for (int value : seq) {
            cout << value << " ";
        }
        cout << " -> " << vectorToString(seq, indexToChar) << endl;
    }

    // Задаем матрицу A и вектор H
    vector<vector<int>> A = { {1, 5, 3}, {1, 6, 2}, {0, 1, 0} };
    vector<int> H(m);
    for (int i = 0; i < m; ++i) {
        H[i] = rand() % N; // Пример случайных значений для вектора H
    }

    // Шифруем векторы
    cout << "Зашифрованные векторы:\n";
    string encryptedText;
    for (const auto& seq : sequences) {
        vector<int> encryptedVec = encryptVector(seq, A, H, N);
        cout << vectorToString(encryptedVec, indexToChar) << endl;
        encryptedText += vectorToString(encryptedVec, indexToChar);
    }

    cout << "Зашифрованное сообщение: " << encryptedText << endl;

    return 0;
}