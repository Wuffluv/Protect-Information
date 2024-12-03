#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

using namespace std;

// Функция для вычисления наибольшего общего делителя (НОД)
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Функция для вычисления обратного числа по модулю
int inv(int a, int n) {
    int a_inv = 1;
    while (true) {
        if ((a_inv * a) % n == 1) {
            return a_inv;
        }
        ++a_inv;
    }
}

// Функция для вычисления функции Эйлера
int phi(int n) {
    int s = 0;
    for (int i = 1; i < n; ++i) {
        if (gcd(n, i) == 1) {
            ++s;
        }
    }
    return s;
}

// Генерация публичного и секретного ключей
pair<pair<int, int>, pair<int, int>> gen_pk_sk(int p, int q) {
    int n = p * q;
    int ph = phi(n);

    // Генерация параметра e
    vector<int> ex;
    for (int i = 3; i < ph; ++i) {
        if (gcd(i, ph) == 1) {
            ex.push_back(i);
        }
    }

    // Случайный выбор e
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, ex.size() - 1);
    int e = ex[dis(gen)];

    // Вычисление d, обратного к e по модулю phi(n)
    int d = inv(e, ph);

    // Публичный ключ (e, n) и секретный ключ (d, n)
    pair<int, int> pk = { e, n };
    pair<int, int> sk = { d, n };

    return { pk, sk };
}

// Функция для возведения в степень по модулю
int mod_exp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Функция для шифрования сообщения
int chifering(int m, const pair<int, int>& pk) {
    int e = pk.first;
    int n = pk.second;
    // Шифрование с использованием формулы c = (m^e) % n
    int c = mod_exp(m, e, n);
    return c;
}

// Функция для расшифрования сообщения
int dechifering(int c, const pair<int, int>& sk) {
    int d = sk.first;
    int n = sk.second;
    // Расшифрование с использованием формулы m = (c^d) % n
    int m = mod_exp(c, d, n);
    return m;
}

int main() {
    setlocale(LC_ALL, "Rus");

    // Генерация ключей на основе простых чисел p и q
    int p = 13;
    int q = 17;
    auto keys = gen_pk_sk(p, q);

    // Публичный и секретный ключи
    auto pk = keys.first;
    auto sk = keys.second;

    cout << "Публичный ключ: (" << pk.first << ", " << pk.second << ")\n";
    cout << "Секретный ключ: (" << sk.first << ", " << sk.second << ")\n";

    // Исходное сообщение
    int message;
    cout << "Введите сообщение (целое число): ";
    cin >> message;
    cout << "Исходное сообщение: " << message << "\n";

    // Шифрование сообщения
    int encrypted_message = chifering(message, pk);
    cout << "Зашифрованное сообщение: " << encrypted_message << "\n";

    // Расшифрование сообщения
    int decrypted_message = dechifering(encrypted_message, sk);
    cout << "Расшифрованное сообщение: " << decrypted_message << "\n";

    return 0;
}
