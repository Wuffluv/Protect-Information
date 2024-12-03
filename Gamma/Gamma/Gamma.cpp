#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <clocale>
#include <string>
#include <bitset>

using namespace std;

// Генерация случайной перестановки ключа
vector<int> generateKey() {
    vector<int> key(256);
    for (int i = 0; i < 256; ++i) {
        key[i] = i;
    }
    shuffle(key.begin(), key.end(), mt19937(random_device()()));
    return key;
}

// Инициализация генератора случайных чисел
vector<int> initializeGenerator(const vector<int>& key) {
    vector<int> gen = key;
    int j = 0;
    for (int i = 0; i < 256; ++i) {
        j = (j + gen[i] + key[i]) % 256;
        swap(gen[i], gen[j]);
    }
    return gen;
}

// Генерация гаммы
vector<int> generateGamma(vector<int>& gen, int messageLength) {
    vector<int> gamma(messageLength);
    int i = 0, j = 0;
    for (int k = 0; k < messageLength; ++k) {
        i = (i + 1) % 256;
        j = (j + gen[i]) % 256;
        swap(gen[i], gen[j]);
        int t = (gen[i] + gen[j]) % 256;
        gamma[k] = gen[t];
    }
    return gamma;
}

// Разбиение символа на два байта
vector<int> splitUnicodeToAscii(const string& message) {
    vector<int> splitted;
    for (char c : message) {
        int code = static_cast<unsigned char>(c);
        splitted.push_back((code & 0xFF00) >> 8);
        splitted.push_back(code & 0x00FF);
    }
    return splitted;
}

// Шифрование/расшифрование сообщения
vector<int> xorMessage(const vector<int>& message, const vector<int>& gamma) {
    vector<int> result(message.size());
    for (size_t k = 0; k < message.size(); ++k) {
        result[k] = message[k] ^ gamma[k];
    }
    return result;
}

// Печать первых n элементов вектора
void printVector(const string& label, const vector<int>& vec, int n) {
    cout << label << ": [";
    for (int i = 0; i < n && i < vec.size(); ++i) {
        cout << vec[i] << (i < n - 1 && i < vec.size() - 1 ? ", " : "");
    }
    cout << (vec.size() > n ? ", ..." : "") << "]" << endl;
}

// Сборка сообщения из байтов
string assembleFromAscii(const vector<int>& bytes) {
    string result;
    for (size_t i = 0; i < bytes.size(); i += 2) {
        char character = static_cast<char>((bytes[i] << 8) | bytes[i + 1]);
        result += character;
    }
    return result;
}

int main() {
    // Установка локали для вывода на русском языке
    setlocale(LC_ALL, "Rus");

    // Генерация ключа
    vector<int> key = generateKey();
    printVector("Ключ", key, 10);

    // Инициализация генератора случайных чисел
    vector<int> gen = initializeGenerator(key);
    printVector("Инициализированный генератор случайных чисел", gen, 10);

    // Ввод сообщения для шифрования
    string message;
    cout << "Введите сообщение для шифрования: ";
    getline(cin, message);
    vector<int> splittedMessage = splitUnicodeToAscii(message);
    printVector("Пары байтов сообщения", splittedMessage, 10);

    // Генерация гаммы
    vector<int> gamma = generateGamma(gen, splittedMessage.size());
    printVector("Гамма", gamma, 10);

    // Шифрование сообщения
    vector<int> encryptedMessage = xorMessage(splittedMessage, gamma);
    printVector("Зашифрованное сообщение (список)", encryptedMessage, 10);

    // Печать зашифрованного сообщения в виде символов юникода
    string encryptedText = assembleFromAscii(encryptedMessage);
    cout << "Зашифрованное сообщение (символы юникода): " << encryptedText << endl;

    // Расшифрование сообщения
    vector<int> decryptedMessage = xorMessage(encryptedMessage, gamma);
    printVector("Расшифрованное сообщение (список)", decryptedMessage, 10);
    string decryptedText = assembleFromAscii(decryptedMessage);
    cout << "Расшифрованное сообщение (символы юникода): " << decryptedText << endl;

    return 0;
}
