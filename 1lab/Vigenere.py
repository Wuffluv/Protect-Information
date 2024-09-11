# Функция для создания повторяющегося ключа, который по длине соответствует тексту
def generate_key(text, key):
    """
    Функция создает повторяющийся ключ, соответствующий длине текста.
    """
    key = list(key)
    if len(text) == len(key):
        return "".join(key)
    else:
        for i in range(len(text) - len(key)):
            key.append(key[i % len(key)])
    return "".join(key)

# Функция для шифрования строки с помощью шифра Виженера
def vigenere_encrypt(text, key, alphabet):
    """
    Функция шифрует строку text с использованием шифра Виженера и ключевого слова key.
    """
    encrypted_text = ""
    key = generate_key(text, key)  # Генерация ключа длиной с текст
    for i in range(len(text)):
        if text[i] in alphabet:
            # Находим индекс символа текста и ключа в алфавите
            text_idx = alphabet.index(text[i])
            key_idx = alphabet.index(key[i])
            # Вычисляем индекс зашифрованного символа
            new_idx = (text_idx + key_idx) % len(alphabet)
            encrypted_text += alphabet[new_idx]
        else:
            # Если символ не в алфавите, оставляем его без изменений
            encrypted_text += text[i]
    return encrypted_text

# Функция для расшифровки строки, зашифрованной с помощью шифра Виженера
def vigenere_decrypt(text, key, alphabet):
    """
    Функция расшифровывает строку text, зашифрованную с использованием шифра Виженера и ключевого слова key.
    """
    decrypted_text = ""
    key = generate_key(text, key)  # Генерация ключа длиной с текст
    for i in range(len(text)):
        if text[i] in alphabet:
            # Находим индекс символа текста и ключа в алфавите
            text_idx = alphabet.index(text[i])
            key_idx = alphabet.index(key[i])
            # Вычисляем индекс расшифрованного символа
            new_idx = (text_idx - key_idx + len(alphabet)) % len(alphabet)
            decrypted_text += alphabet[new_idx]
        else:
            # Если символ не в алфавите, оставляем его без изменений
            decrypted_text += text[i]
    return decrypted_text

# Пример использования
if __name__ == "__main__":
    # Определяем алфавит
    alphabet = "abcdefghijklmnopqrstuvwxyz"

    # Исходная строка и ключевое слово
    original_text = "rodion"
    key_word = "key"

    # Шифрование
    encrypted_text = vigenere_encrypt(original_text, key_word, alphabet)
    print(f"Зашифрованная строка: {encrypted_text}")

    # Расшифровка
    decrypted_text = vigenere_decrypt(encrypted_text, key_word, alphabet)
    print(f"Расшифрованная строка: {decrypted_text}")
