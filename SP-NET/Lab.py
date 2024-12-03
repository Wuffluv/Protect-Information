import random

# Функция для перевода символа в 16-битную последовательность
def char_to_binary(char):
    """Перевод символа в 16-битную строку."""
    return format(ord(char), '016b')

# Функция для перевода строки из двух символов в 32-битную последовательность
def string_to_binary(string):
    """Перевод строки из двух символов в 32-битную строку."""
    return ''.join(char_to_binary(char) for char in string)

# Функция для создания случайной перестановки
def generate_permutation(lst):
    """Создание случайной перестановки списка."""
    perm = lst[:]
    random.shuffle(perm)
    return perm

# Функция для создания нескольких случайных перестановок
def generate_permutations(lst, n):
    """Создание n случайных перестановок списка."""
    permutations = set()
    while len(permutations) < n:
        perm = tuple(generate_permutation(lst))
        if perm not in permutations:
            permutations.add(perm)
    return [list(perm) for perm in permutations]

# Функция шифрующего P-блока
def p_block_encrypt(bits, permutation):
    """Шифрование P-блоком с использованием заданной перестановки."""
    return ''.join(bits[permutation[i]] for i in range(len(bits)))

# Функция расшифровки P-блоком
def p_block_decrypt(bits, permutation):
    """Расшифровка P-блоком с использованием обратной перестановки."""
    inverse_perm = [0] * len(permutation)
    for i, p in enumerate(permutation):
        inverse_perm[p] = i
    return ''.join(bits[inverse_perm[i]] for i in range(len(bits)))

# Функция перевода двоичного числа в десятичное
def binary_to_decimal(binary_str):
    """Перевод двоичного числа в десятичное."""
    return int(binary_str, 2)

# Функция перевода десятичного числа в двоичное с фиксированной длиной
def decimal_to_binary(decimal, length):
    """Перевод десятичного числа в двоичное с фиксированной длиной."""
    return format(decimal, f'0{length}b')

# Функция шифрующего S-блока
def s_block_encrypt(bits, substitution):
    """Шифрование S-блоком с использованием подстановки."""
    decimal = binary_to_decimal(bits)
    substituted = substitution[decimal]
    return decimal_to_binary(substituted, 4)

# Функция расшифровки S-блоком
def s_block_decrypt(bits, substitution):
    """Расшифровка S-блоком с использованием обратной подстановки."""
    inverse_substitution = {v: k for k, v in enumerate(substitution)}
    decimal = binary_to_decimal(bits)
    original = inverse_substitution[decimal]
    return decimal_to_binary(original, 4)

# Функция для перевода строки в сообщение (для расшифровки)
def binary_to_string(binary):
    """Перевод 32-битной строки обратно в два символа."""
    char1 = chr(int(binary[:16], 2))
    char2 = chr(int(binary[16:], 2))
    return char1 + char2

# Основная функция шифрования
def encrypt_message(message, p_permutation, s_substitution):
    """Шифрование сообщения с использованием SP-сети."""
    binary = string_to_binary(message)
    print("Битовая форма исходного сообщения:", binary)
    p_encrypted = p_block_encrypt(binary, p_permutation)
    print("Зашифрованная p-блоком битовая форма:", p_encrypted)
    s_encrypted = ''.join(
        s_block_encrypt(p_encrypted[i:i+4], s_substitution) for i in range(0, len(p_encrypted), 4)
    )
    print("Зашифрованная батареей s-блоков битовая форма:", s_encrypted)
    final_encrypted = p_block_encrypt(s_encrypted, p_permutation)
    print("Зашифрованная p-блоком битовая форма:", final_encrypted)
    encrypted_message = ''.join(chr(int(final_encrypted[i:i+16], 2)) for i in range(0, len(final_encrypted), 16))
    return encrypted_message

# Основная функция расшифровки
def decrypt_message(encrypted_message, p_permutation, s_substitution):
    """Расшифровка сообщения с использованием SP-сети."""
    binary = ''.join(format(ord(char), '016b') for char in encrypted_message)
    print("Зашифрованное сообщение (бинарная форма):", binary)
    p_decrypted = p_block_decrypt(binary, p_permutation)
    print("Расшифрованная p-блоком битовая форма:", p_decrypted)
    s_decrypted = ''.join(
        s_block_decrypt(p_decrypted[i:i+4], s_substitution) for i in range(0, len(p_decrypted), 4)
    )
    print("Расшифрованная батареей s-блоков битовая форма:", s_decrypted)
    final_decrypted = p_block_decrypt(s_decrypted, p_permutation)
    print("Расшифрованная p-блоком битовая форма:", final_decrypted)
    return binary_to_string(final_decrypted)

# Пример работы программы
if __name__ == "__main__":
    # Создаем пример перестановок и подстановок
    p_permutation = generate_permutation(list(range(32)))
    s_substitution = generate_permutation(list(range(16)))

    # Ввод строки пользователем
    original_message = input("Введите сообщение (два символа): ")
    if len(original_message) != 2:
        print("Ошибка: сообщение должно состоять из двух символов.")
    else:
        print("========== Шифрование ==========")
        print("Исходное сообщение:", original_message)

        # Шифрование
        encrypted = encrypt_message(original_message, p_permutation, s_substitution)
        print("Зашифрованное сообщение:", encrypted)

        print("========== Расшифрование ==========")
        # Расшифровка
        decrypted = decrypt_message(encrypted, p_permutation, s_substitution)
        print("Расшифрованное сообщение:", decrypted)
