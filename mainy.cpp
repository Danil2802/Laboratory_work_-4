#include <iostream>
#include <vector>
#include <cstdint>
#include <random>
#include <cstring>

class RC4 {
public:
    RC4(const std::vector<uint8_t>& key) {
        initialize(key);
    }

    uint8_t generate() {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        std::swap(S[i], S[j]);
        uint8_t t = (S[i] + S[j]) % 256;
        return S[t];
    }

    void print_S_table() const {
        std::cout << "\nS table: ";
        for (auto s : S) {
            std::cout << static_cast<int>(s) << " ";
        }
        std::cout << std::endl;
    }

private:
    void initialize(const std::vector<uint8_t>& key) {
        uint8_t keyLength = key.size();
        for (int i = 0; i < 256; ++i) {
            S[i] = i;
        }
        uint8_t j = 0;
        for (int i = 0; i < 256; ++i) {
            j = (j + S[i] + key[i % keyLength]) % 256;
            std::swap(S[i], S[j]);
        }
    }

    std::vector<uint8_t> S = std::vector<uint8_t>(256);
    uint8_t i = 0, j = 0;
};

// Функция для генерации криптографически стойкого ключа
std::vector<uint8_t> generate_secure_key(size_t length) {
    std::vector<uint8_t> key(length);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (auto& byte : key) {
        byte = static_cast<uint8_t>(dis(gen));
    }

    return key;
}


int main() {
    // Генерация криптографически стойкого ключа длиной 16 байт
    std::vector<uint8_t> key = generate_secure_key(16);

    // Вывод ключа (для демонстрации, в реальных приложениях ключ не должен выводиться)
    std::cout << "Generated key: ";
    for (uint8_t byte : key) {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    // Создание объекта RC4 с сгенерированным ключом
    RC4 rc4(key);

    // Вывод таблицы S после инициализации
    rc4.print_S_table();

    // Генерация и вывод 50 псевдослучайных чисел
    std::cout << "\nGenerated pseudorandom numbers: ";
    for (int i = 0; i < 50; ++i) {
        std::cout << static_cast<int>(rc4.generate()) << " ";
    }
    std::cout << std::endl;

    return 0;
}
