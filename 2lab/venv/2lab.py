import math

# Функция для нахождения обратного элемента по модулю
def mod_inverse(a, N):
    # Перебираем все числа от 1 до N-1, чтобы найти такое x, при котором (a * x) % N == 1
    for x in range(1, N):
        if (a * x) % N == 1:
            return x
    return None  # Возвращаем None, если обратного элемента не существует

# Функция для шифрования текста с помощью аффинного шифра
def affine_encrypt(text, a, b, alphabet):
    N = len(alphabet)  # Длина алфавита (модуль N)
    encrypted_text = ""  # Переменная для хранения зашифрованного текста

    # Проходим по каждому символу текста
    for char in text:
        if char in alphabet:
            # Находим индекс символа в алфавите (это x)
            x = alphabet.index(char)
            # Вычисляем зашифрованный индекс по формуле y = (a * x + b) % N
            y = (a * x + b) % N
            # Добавляем зашифрованный символ в результат
            encrypted_text += alphabet[y]
        else:
            # Если символ не принадлежит алфавиту, оставляем его без изменений
            encrypted_text += char
    return encrypted_text  # Возвращаем зашифрованную строку

# Функция для расшифровки текста, зашифрованного аффинным шифром
def affine_decrypt(cipher_text, a, b, alphabet):
    N = len(alphabet)  # Длина алфавита (модуль N)
    decrypted_text = ""  # Переменная для хранения расшифрованного текста

    # Находим обратный элемент для коэффициента a
    a_inv = mod_inverse(a, N)
    if a_inv is None:
        # Если обратного элемента не существует, выводим ошибку
        raise ValueError(f"Не существует обратного для {a} по модулю {N}.")

    # Проходим по каждому символу зашифрованного текста
    for char in cipher_text:
        if char in alphabet:
            # Находим индекс символа в алфавите (это y)
            y = alphabet.index(char)
            # Вычисляем расшифрованный индекс по формуле x = (a_inv * (y - b)) % N
            x = (a_inv * (y - b)) % N
            # Добавляем расшифрованный символ в результат
            decrypted_text += alphabet[x]
        else:
            # Если символ не принадлежит алфавиту, оставляем его без изменений
            decrypted_text += char
    return decrypted_text  # Возвращаем расшифрованную строку


alphabet = ['А', 'О', 'И', 'Н', 'Т', '_']
original_text = "НАТА_ИННА_И_АНТОН"
a = 5
b = 2


encrypted = affine_encrypt(original_text, a, b, alphabet)
print("Зашифрованный текст:", encrypted)


decrypted = affine_decrypt(encrypted, a, b, alphabet)
print("Расшифрованный текст:", decrypted)
