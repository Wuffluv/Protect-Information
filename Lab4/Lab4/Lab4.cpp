#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

using namespace std;

// Генерация случайной перестановки массива целых чисел от 0 до 255
vector<int> generateKey() {
    vector<int> key(256);
    for (int i = 0; i < 256; ++i) key[i] = i;

    // Случайная перестановка
    random_device rd;
    mt19937 gen(rd());
    shuffle(key.begin(), key.end(), gen);

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

// Генерация гаммы для шифрования сообщения
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

// Шифрование сообщения с использованием гаммы
vector<int> encryptMessage(const vector<int>& message, const vector<int>& gamma) {
    vector<int> encryptedMessage(message.size());
    for (size_t k = 0; k < message.size(); ++k) {
        encryptedMessage[k] = message[k] ^ gamma[k]; // Применяем XOR
    }
    return encryptedMessage;
}

// Вспомогательная функция для вывода вектора
void printVector(const string& label, const vector<int>& vec) {
    cout << label << ": ";
    for (int i = 0; i < min(10, (int)vec.size()); ++i) { // Выводим первые 10 элементов для наглядности
        cout << vec[i] << " ";
    }
    cout << "... ]" << endl;
}

int main() {
    setlocale(LC_ALL, "RUS");
    // Генерация ключа
    vector<int> key = generateKey();
    printVector("Ключ", key);

    // Инициализация генератора случайных чисел
    vector<int> gen = initializeGenerator(key);
    printVector("Инициализированный генератор случайных чисел", gen);

    // Ввод сообщения пользователем
    cout << "Введите сообщение для шифрования: ";
    string message;
    getline(cin, message);

    vector<int> messageBytes(message.begin(), message.end());

    // Генерация гаммы
    vector<int> gamma = generateGamma(gen, messageBytes.size());
    printVector("Гамма", gamma);

    // Шифрование сообщения
    vector<int> encryptedMessage = encryptMessage(messageBytes, gamma);
    printVector("Зашифрованное сообщение", encryptedMessage);

    // Расшифрование сообщения (повторное применение XOR)
    vector<int> decryptedMessage = encryptMessage(encryptedMessage, gamma);
    string decryptedText(decryptedMessage.begin(), decryptedMessage.end());
    cout << "Расшифрованное сообщение: " << decryptedText << endl;

    return 0;
}
