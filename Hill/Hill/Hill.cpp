#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Функция для умножения матриц по модулю N
vector<int> multiplyMatrix(vector<int> X, vector<vector<int>> A, int N) {
    int m = X.size();
    vector<int> result(m, 0);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            result[i] += X[j] * A[j][i];
        }
        result[i] = (result[i] % N + N) % N;
    }
    return result;
}

// Функция для шифрования текста с использованием шифра Хилла
string hillEncrypt(string text, vector<vector<int>> A, vector<int> H, string alphabet) {
    int N = alphabet.size();
    int m = A.size();
    while (text.size() % m != 0) {
        text += alphabet.back(); // Добавить последний символ алфавита, чтобы дополнить текст
    }

    string encrypted;
    for (int i = 0; i < text.size(); i += m) {
        vector<int> X(m);
        for (int j = 0; j < m; j++) {
            X[j] = alphabet.find(text[i + j]);
        }
        vector<int> Y = multiplyMatrix(X, A, N);
        for (int j = 0; j < m; j++) {
            Y[j] = (Y[j] + H[j]) % N;
            encrypted += alphabet[Y[j]];
        }
    }
    return encrypted;
}

// Функция для расшифровки текста с использованием шифра Хилла
string hillDecrypt(string encryptedText, vector<vector<int>> A_inv, vector<int> H, string alphabet) {
    int N = alphabet.size();
    int m = A_inv.size();

    string decrypted;
    for (int i = 0; i < encryptedText.size(); i += m) {
        vector<int> Y(m);
        for (int j = 0; j < m; j++) {
            Y[j] = alphabet.find(encryptedText[i + j]);
            Y[j] = (Y[j] - H[j] + N) % N;
        }
        vector<int> X = multiplyMatrix(Y, A_inv, N);
        for (int j = 0; j < m; j++) {
            decrypted += alphabet[X[j]];
        }
    }
    return decrypted;
}

int main() {
    setlocale(LC_ALL, "RUS");
    // Определение алфавита, матрицы A и вектора H
    string alphabet = "АОУЫНТ_";
    vector<vector<int>> A = { {1, 5, 3}, {1, 6, 2}, {0, 1, 0} }; // Пример матрицы для шифрования
    vector<vector<int>> A_inv = { {5, 3, 6}, {0, 0, 1}, {1, 6, 1} }; // Пример обратной матрицы
    vector<int> H = { 1, 2, 3 }; // Пример вектора H

    // Исходный текст
    string text = "У_АННЫ_НОТЫ";
    cout << "Исходный текст: " << text << endl;

    // Шифрование текста
    string encryptedText = hillEncrypt(text, A, H, alphabet);
    cout << "Зашифрованный текст: " << encryptedText << endl;

    // Расшифровка текста
    string decryptedText = hillDecrypt(encryptedText, A_inv, H, alphabet);
    cout << "Расшифрованный текст: " << decryptedText << endl;

    return 0;
}
