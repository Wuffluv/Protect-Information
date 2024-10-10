#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

// Функция для генерации матрицы с определителем 1
MatrixXd generateMatrix(int m, int N) {
    MatrixXd matrix(m, m);
    double det;

    do {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                matrix(i, j) = rand() % N; // случайные числа от 0 до N-1
            }
        }
        det = matrix.determinant();
    } while (det == 0 || det != 1); // продолжаем до тех пор, пока определитель не станет равным 1 и не равен 0

    return matrix;
}

// Функция для нахождения обратной матрицы
MatrixXd modularInverse(const MatrixXd& matrix, int N) {
    MatrixXd inverse = matrix.inverse(); // Находим обратную матрицу
    MatrixXd modInverse = inverse.unaryExpr([N](double val) { return static_cast<int>(fmod(val + N, N)); }); // Применяем модуль

    return modInverse;
}

// Функция для проверки, является ли результат произведения равным единичной матрице
MatrixXd checkIdentity(const MatrixXd& A, const MatrixXd& A_inv, int N) {
    MatrixXd product = A * A_inv; // Перемножаем матрицы
    return product.unaryExpr([N](double val) { return static_cast<int>(fmod(val + N, N)); }); // Применяем модуль
}

// Функция для шифрования векторов
VectorXd encryptVector(const VectorXd& X, const MatrixXd& A, const VectorXd& H, int N) {
    VectorXd Y = (A * X + H).unaryExpr([N](double val) { return static_cast<int>(fmod(val + N, N)); }); // Шифрование
    return Y;
}

// Функция для расшифровки векторов
VectorXd decryptVector(const VectorXd& Y, const MatrixXd& A_inv, const VectorXd& H, int N) {
    VectorXd adjustedY = (Y - H).unaryExpr([N](double val) { return (static_cast<int>(val) % N + N) % N; }); // Коррекция Y
    VectorXd X = (A_inv * adjustedY).unaryExpr([N](double val) { return (static_cast<int>(val) % N + N) % N; }); // Расшифровка
    return X;
}

// Функция для преобразования вектора чисел в строку символов
string vectorToString(const VectorXd& vec, const map<int, char>& indexToChar) {
    string result;
    for (int i = 0; i < vec.size(); ++i) {
        int index = static_cast<int>(vec(i));
        // Проверяем, существует ли индекс в мапе
        auto it = indexToChar.find(index);
        if (it != indexToChar.end()) {
            result += it->second; // Добавляем символ в результат
        }
        else {
            result += '?'; // Добавляем символ для неизвестного индекса
        }
    }
    return result;
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Инициализация генератора случайных чисел

    setlocale(LC_ALL, "Rus");
    string Alphabet = "АОУЫНТ_";
    cout << "Алфавит: " << Alphabet << endl;

    string Text = "У_АННЫ_НОТЫ";
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

    // Определяем вектор H
    VectorXd H(m);
    for (int i = 0; i < m; ++i) {
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
        encryptedText += vectorToString(Y, indexToChar);
    }
    cout << "Зашифрованное сообщение: " << encryptedText << endl;

    // Расшифровка текста
    cout << "Расшифрованные векторы:\n";
    string decryptedText; // Для хранения расшифрованного текста
    for (const auto& Y : encryptedVectors) {
        VectorXd X = decryptVector(Y, modInverseMatrix, H, N);
        cout << X.transpose() << " -> " << vectorToString(X, indexToChar) << endl; // Выводим символы
        decryptedText += vectorToString(X, indexToChar);
    }

    cout << "Расшифрованное сообщение: " << decryptedText << endl;

    return 0;
}