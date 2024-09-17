# Функция для шифрования строки с помощью шифра Цезаря
def caesar_encrypt(text, shift, alphabet):
    """
    Функция шифрует строку text, сдвигая символы на shift позиций в указанном алфавите alphabet.
    """
    encrypted_text = ""
    for char in text:
        if char in alphabet:
            # Находим индекс символа в алфавите
            idx = alphabet.index(char)
            # Вычисляем новый индекс с учетом сдвига
            new_idx = (idx + shift) % len(alphabet)
            # Добавляем новый символ в зашифрованную строку
            encrypted_text += alphabet[new_idx]
        else:
            # Если символ не найден в алфавите, добавляем его без изменений
            encrypted_text += char
    return encrypted_text

# Функция для расшифровки строки, зашифрованной с помощью шифра Цезаря
def caesar_decrypt(text, shift, alphabet):
    """
    Функция расшифровывает строку text, сдвигая символы на shift позиций влево в указанном алфавите alphabet.
    """
    return caesar_encrypt(text, -shift, alphabet)

# Функция для атаки полным перебором (brute force)
def caesar_bruteforce(text, alphabet):
    """
    Функция осуществляет перебор всех возможных сдвигов для расшифровки строки text с использованием алфавита alphabet.
    """
    possible_decryptions = []
    for shift in range(len(alphabet)):
        decrypted_text = caesar_decrypt(text, shift, alphabet)
        possible_decryptions.append((shift, decrypted_text))
    return possible_decryptions

# Пример использования
if __name__ == "__main__":
    # Определим алфавит
    alphabet = "abcdefghijklmnopqrstuvwxyz"

    # Исходная строка и сдвиг
    original_text = "rodion"
    shift_value = 3

    # Шифрование
    encrypted_text = caesar_encrypt(original_text, shift_value, alphabet)
    print(f"Зашифрованная строка: {encrypted_text}")

    # Расшифровка
    decrypted_text = caesar_decrypt(encrypted_text, shift_value, alphabet)
    print(f"Расшифрованная строка: {decrypted_text}")

    # полный перебор
    all_decryptions = caesar_bruteforce(encrypted_text, alphabet)
    print("Варианты расшифровки при разных сдвигах:")
    for shift, variant in all_decryptions:
        print(f"Сдвиг {shift}: {variant}")
