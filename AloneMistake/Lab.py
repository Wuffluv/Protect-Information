# Импортируем необходимые библиотеки
import random

# Функция для вычисления количества проверочных бит
# Формула: 2^k >= m + k + 1
# Определяем количество проверочных разрядов для заданного количества информационных разрядов
def calculate_redundant_bits(m):
    k = 0
    while (2 ** k) < (m + k + 1):
        k += 1
    return k

# Функция для преобразования символов алфавита в двоичный вид с заданной длиной
# Если длина меньше, дополняем ведущими нулями
def text_to_binary(text, length):
    return [bin(ord(char))[2:].zfill(length) for char in text]

# Функция для вставки проверочных разрядов в двоичное сообщение
# Проверочные разряды вставляются на позиции, которые являются степенями двойки (1, 2, 4, 8 и т.д.)
def insert_redundant_bits(binary_message, k):
    n = len(binary_message) + k
    message_with_redundant = ['0'] * n
    j = 0
    for i in range(n):
        # Проверочные разряды располагаются на позициях, которые являются степенями двойки
        if (i + 1) & (i) == 0:
            continue
        message_with_redundant[i] = binary_message[j]
        j += 1
    return message_with_redundant

# Функция для вычисления значений проверочных разрядов
# Каждый проверочный разряд вычисляется на основе контрольных позиций, зависящих от его положения
def calculate_parity_bits(message_with_redundant, k):
    n = len(message_with_redundant)
    for i in range(k):
        position = (2 ** i) - 1
        parity = 0
        # Проверочный бит контролирует разряды, номера которых имеют соответствующий бит в позиции равный 1
        for j in range(1, n + 1):
            if j & (position + 1) == (position + 1) and j != (position + 1):
                if message_with_redundant[j - 1] == '1':
                    parity ^= 1
        message_with_redundant[position] = str(parity)
    return message_with_redundant

# Кодирование по Хэммингу, включающая в себя вставку проверочных разрядов и их вычисление
def encode_hamming(message, alphabet_size):
    # Вычисляем количество бит, необходимых для представления символов алфавита
    m = len(bin(alphabet_size - 1)[2:])
    # Вычисляем количество проверочных бит
    k = calculate_redundant_bits(m)
    encoded_message = []
    for symbol in message:
        # Преобразуем символ в двоичный вид с необходимой длиной
        binary = bin(ord(symbol))[2:].zfill(m)
        # Вставляем проверочные разряды
        with_redundant = insert_redundant_bits(list(binary), k)
        # Вычисляем значения проверочных разрядов
        encoded = calculate_parity_bits(with_redundant, k)
        encoded_message.append(encoded)
    return encoded_message

# Функция для инвертирования одного разряда в случайной позиции
# "Портим" сообщение, чтобы смоделировать ошибку передачи
def corrupt_message(encoded_message):
    corrupted_message = [list(symbol) for symbol in encoded_message]
    for symbol in corrupted_message:
        # Выбираем случайную позицию для инвертирования бита
        index_to_corrupt = random.randint(0, len(symbol) - 1)
        # Инвертируем выбранный бит (0 -> 1 или 1 -> 0)
        symbol[index_to_corrupt] = '1' if symbol[index_to_corrupt] == '0' else '0'
    return corrupted_message

# Функция для поиска и исправления ошибок в закодированном сообщении
# Находим позицию ошибки с помощью проверочных разрядов и исправляем её
def correct_errors(corrupted_message, k):
    corrected_message = []
    for symbol in corrupted_message:
        n = len(symbol)
        error_position = 0
        # Вычисляем значения проверочных разрядов для поиска ошибки
        for i in range(k):
            position = (2 ** i) - 1
            parity = 0
            for j in range(1, n + 1):
                if j & (position + 1) == (position + 1):
                    if symbol[j - 1] == '1':
                        parity ^= 1
            if parity != 0:
                error_position += (position + 1)
        # Если позиция ошибки найдена, инвертируем бит в этой позиции
        if error_position != 0:
            symbol[error_position - 1] = '1' if symbol[error_position - 1] == '0' else '0'
        corrected_message.append(symbol)
    return corrected_message

# Декодирование исправленного сообщения
# Убираем проверочные разряды и получаем исходное сообщение
def decode_message(corrected_message, k):
    decoded_message = []
    for symbol in corrected_message:
        n = len(symbol)
        decoded_symbol = ''
        # Убираем проверочные разряды (те, которые находятся на позициях степеней двойки)
        for i in range(n):
            if (i + 1) & (i) != 0:
                decoded_symbol += symbol[i]
        # Преобразуем двоичный код обратно в символ
        decoded_message.append(chr(int(decoded_symbol, 2)))
    return ''.join(decoded_message)

# Основная программа для выполнения всех этапов
if __name__ == "__main__":
    # Вводим исходное сообщение и размер алфавита
    message = input("Введите сообщение: ")
    alphabet_size = int(input("Введите количество символов в алфавите: "))

    # Кодирование сообщения
    encoded = encode_hamming(message, alphabet_size)
    print("\nЗакодированное сообщение:")
    for symbol in encoded:
        print(''.join(symbol))

    # "Портим" сообщение
    corrupted = corrupt_message(encoded)
    print("\nИспортим сообщение:")
    for symbol in corrupted:
        print(''.join(symbol))

    # Исправляем ошибки в сообщении
    k = calculate_redundant_bits(len(bin(alphabet_size - 1)[2:]))
    corrected = correct_errors(corrupted, k)
    print("\nИсправленное сообщение:")
    for symbol in corrected:
        print(''.join(symbol))

    # Декодируем сообщение
    decoded = decode_message(corrected, k)
    print("\nДекодированное сообщение: ", decoded)
