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
