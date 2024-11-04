#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

// ������� ��� ��������� ������� � ������������� 1
MatrixXd generateMatrix(int m, int N) {
    MatrixXd matrix(m, m);
    double det;

    do {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                matrix(i, j) = rand() % N; // ��������� ����� �� 0 �� N-1
            }
        }
        det = matrix.determinant();
    } while (det == 0 || det != 1); // ���������� �� ��� ���, ���� ������������ �� ������ ������ 1 � �� ����� 0

    return matrix;
}

// ������� ��� ���������� �������� �������
MatrixXd modularInverse(const MatrixXd& matrix, int N) {
    MatrixXd inverse = matrix.inverse(); // ������� �������� �������
    MatrixXd modInverse = inverse.unaryExpr([N](double val) { return static_cast<int>(fmod(val + N, N)); }); // ��������� ������

    return modInverse;
}

// ������� ��� ��������, �������� �� ��������� ������������ ������ ��������� �������
MatrixXd checkIdentity(const MatrixXd& A, const MatrixXd& A_inv, int N) {
    MatrixXd product = A * A_inv; // ����������� �������
    return product.unaryExpr([N](double val) { return static_cast<int>(fmod(val + N, N)); }); // ��������� ������
}

// ������� ��� ���������� ��������
VectorXd encryptVector(const VectorXd& X, const MatrixXd& A, const VectorXd& H, int N) {
    VectorXd Y = (A * X + H).unaryExpr([N](double val) { return static_cast<int>(fmod(val + N, N)); }); // ����������
    return Y;
}

// ������� ��� ����������� ��������
VectorXd decryptVector(const VectorXd& Y, const MatrixXd& A_inv, const VectorXd& H, int N) {
    VectorXd adjustedY = (Y - H).unaryExpr([N](double val) { return (static_cast<int>(val) % N + N) % N; }); // ��������� Y
    VectorXd X = (A_inv * adjustedY).unaryExpr([N](double val) { return (static_cast<int>(val) % N + N) % N; }); // �����������
    return X;
}

// ������� ��� �������������� ������� ����� � ������ ��������
string vectorToString(const VectorXd& vec, const map<int, char>& indexToChar) {
    string result;
    for (int i = 0; i < vec.size(); ++i) {
        int index = static_cast<int>(vec(i));
        // ���������, ���������� �� ������ � ����
        auto it = indexToChar.find(index);
        if (it != indexToChar.end()) {
            result += it->second; // ��������� ������ � ���������
        }
        else {
            result += '?'; // ��������� ������ ��� ������������ �������
        }
    }
    return result;
}
