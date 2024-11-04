#include <iostream>
#include <vector>
#include <map>
#include "LibMatrix.h"

using namespace std;


int main() {
    srand(static_cast<unsigned>(time(0))); // Инициализация генератора случайных чисел

    setlocale(LC_ALL, "Rus");
    string Alphabet = "АОУЫНТ_";
    cout << "Алфавит: " << Alphabet << endl;

    string Text = "У_НАТЫ_ТОННЫ";
    cout << "Шифруемое сообщение: " << Text << endl;
    int N = Alphabet.size();
    int m = 3;

    map<char, int> charToIndex;
    map<int, char> indexToChar; // Обратная мапа для преобразования индексов в символы
    for (int i = 0; i < N; ++i) {
        charToIndex[Alphabet[i]] = i;
        indexToChar[i] = Alphabet[i]; // Заполняем обратную мапу
    }

    // Разбиваем текст на последовательности длины m
    vector<vector<int>> sequences;
    for (size_t i = 0; i < Text.length(); i += m) {
        vector<int> sequence;
        for (int j = 0; j < m; ++j) {
            if (i + j < Text.length()) {
                sequence.push_back(charToIndex[Text[i + j]]);
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
        cout << " -> ";

        VectorXd vec(seq.size());
        for (size_t k = 0; k < seq.size(); ++k) {
            vec(k) = seq[k];
        }

        cout << vectorToString(vec, indexToChar) << endl; // Выводим символы
    }

    MatrixXd matrix = generateMatrix(m, N);
    cout << "Случайная матрица A с определителем 1:\n" << matrix << endl;

    MatrixXd modInverseMatrix = modularInverse(matrix, N);
    cout << "Обратная матрица A^-1 (по модулю " << N << "):\n" << modInverseMatrix << endl;

    MatrixXd identityCheck = checkIdentity(matrix, modInverseMatrix, N);
    cout << "Результат перемножения A и A^-1 (по модулю " << N << "):\n" << identityCheck << endl;

    //// Определяем вектор H
    //VectorXd H(m);
    //H << 1, 2, 3; // Пример вектора H

    VectorXd H(m);
    for (int i = 0; i < m; ++i) {
       /* H(i) = i + 1;*/ // Пример: задаем H как последовательность от 1 до m
        // Вы можете заменить это на случайные значения, если нужно
        H(i) = rand() % N; // Пример случайных значений
    }

    // Шифруем векторы
    cout << "Зашифрованные векторы:\n";
    string encryptedText; // Для хранения зашифрованного текста
    vector<VectorXd> encryptedVectors;
    for (const auto& seq : sequences) {
        VectorXd X = VectorXd::Zero(m); // Создаем нулевой вектор
        for (int j = 0; j < seq.size(); ++j) {
            X(j) = seq[j]; // Заполняем вектор X
        }
        VectorXd Y = encryptVector(X, matrix, H, N);
        encryptedVectors.push_back(Y); // Сохраняем зашифрованный вектор
        cout << Y.transpose() << " -> " << vectorToString(Y, indexToChar) << endl; // Выводим символы
        // Добавляем зашифрованный вектор в строку
        encryptedText += vectorToString(Y, indexToChar);
    }
    cout << "Зашифрованное сообщение: " << encryptedText << endl;

    cout << "Расшифрованные векторы:\n";
    string decryptedText; // Для хранения расшифрованного текста
    for (const auto& Y : encryptedVectors) {
        VectorXd X = decryptVector(Y, modInverseMatrix, H, N);
        cout << X.transpose() << " -> " << vectorToString(X, indexToChar) << endl; // Выводим символы
        // Добавляем расшифрованный вектор в строку
        decryptedText += vectorToString(X, indexToChar);
    }

    cout << "Расшифрованное сообщение: " << decryptedText << endl;

    return 0;
}
