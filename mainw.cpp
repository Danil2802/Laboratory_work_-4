#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <map>
#include <numeric>
#include <string>
#include <chrono>
#include <iomanip>
#include <cctype> // Для функции std::isdigit
#include <sstream> // Для функции std::istringstream

// Функция для создания случайного вещественного числа в диапазоне [min, max]
double generate_random_double(std::ranlux24_base& generator, double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

// Функция для создания случайного целого числа в диапазоне [min, max]
int generate_random_int(std::ranlux24_base& generator, int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

// Функция для вычисления количества элементов, больших A
int count_greater_than(const std::vector<double>& arr, double A) {
    return std::count_if(arr.begin(), arr.end(), [A](double x) { return x > A; });
}

// Функция для вычисления произведения элементов после максимального по модулю элемента
long double product_after_max_abs(const std::vector<double>& arr) {
    if (arr.empty()) return 0;

    auto max_abs_it = std::max_element(arr.begin(), arr.end(),
        [](double a, double b) { return std::abs(a) < std::abs(b); });

    long double product = 1;
    if (max_abs_it + 1 != arr.end()) {
        for (auto it = max_abs_it + 1; it != arr.end(); ++it) {
            product *= *it;
        }
    }
    return product;
}

// Функция для перестановки элементов
void swap_elements(std::vector<double>& arr) {
    for (size_t i = 0; i < arr.size() - 1; i += 2) {
        int tens = static_cast<int>(arr[i]) / 10 % 10;
        int hundreds = static_cast<int>(arr[i]) / 100;
        if (tens > hundreds) {
            std::swap(arr[i], arr[i + 1]);
        }
    }
}

// Функция для нахождения наиболее часто встречающегося элемента
int most_frequent_element(const std::vector<int>& arr) {
    std::map<int, int> frequency;
    for (int num : arr) {
        ++frequency[num];
    }

    return std::max_element(frequency.begin(), frequency.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second < b.second;
        })->first;
}

// Функция для сортировки цифр числа в порядке убывания
int sort_digits_desc(int num) {
    std::string num_str = std::to_string(num);
    std::sort(num_str.begin(), num_str.end(), std::greater<char>());
    return std::stoi(num_str);
}

// Функция для нормализации числа и вывода в формате 46*10^30
void print_scientific_notation(long double num) {
    if (num == 0) {
        std::cout << "0\n";
        return;
    }

    int exponent = 0;

    for (; num >= 10.0; ++exponent) {
        num /= 10.0;
    }

    for (; num < 1.0; --exponent) {
        num *= 10.0;
    }

    std::cout << std::fixed << std::setprecision(2) << num << " * 10^" << exponent << std::endl;
}

// Функция для ввода и проверки корректности введенного значения
double input_valid_number() {
    std::string input;
    bool valid = false;

    for (; !valid;) {
        std::cout << "Введите одно слово, содержащее только цифры: ";
        std::getline(std::cin, input);

        std::istringstream stream(input);
        std::vector<std::string> words{ std::istream_iterator<std::string>{stream},
                                       std::istream_iterator<std::string>{} };

        if (words.size() != 1) {
            std::cout << "Ошибка! Введите одно слово.\n";
            continue;
        }

        valid = true; // Предполагаем, что ввод корректный, пока не докажем обратного

        // Проверяем каждый символ в строке
        for (char ch : input) {
            if (!std::isdigit(ch)) {
                valid = false; // Если символ не является цифрой, помечаем ввод как невалидный
                break;
            }
        }

        if (!valid) {
            std::cout << "Ошибка! Введите только цифры.\n";
        }
    }

    return std::stod(input);
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Инициализация генератора случайных чисел с текущим временем в качестве seed
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::ranlux24_base generator(seed);

    // Генерация случайного количества элементов для первого массива
    int n1 = generate_random_int(generator, 10, 50);
    std::vector<double> array1(n1);
    for (auto& num : array1) {
        num = generate_random_double(generator, 100.0, 900.0);
    }

    std::cout << "Массив из случайных вещественных чисел (" << n1 << " элементов):\n";
    for (const auto& num : array1) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Ввод числа A и обработка массива
    double A = input_valid_number();
    std::cout << "Вы ввели корректное число: " << A << std::endl;

    int count_greater_A = count_greater_than(array1, A);
    std::cout << "Количество элементов, больших A: " << count_greater_A << std::endl;

    long double product_after_max = product_after_max_abs(array1);
    std::cout << "Произведение элементов после максимального по модулю: ";
    print_scientific_notation(product_after_max);

    // Перестановка элементов массива
    swap_elements(array1);
    std::cout << "Массив после перестановки:\n";
    for (const auto& num : array1) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Генерация случайного количества элементов для второго массива
    int n2 = generate_random_int(generator, 15, 100);
    std::vector<int> array2(n2);
    for (auto& num : array2) {
        num = generate_random_int(generator, 10, 20);
    }

    std::cout << "Массив из целых чисел (" << n2 << " элементов):\n";
    for (const auto& num : array2) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    int most_frequent = most_frequent_element(array2);
    std::cout << "Наиболее часто встречающийся элемент: " << most_frequent << std::endl;

    // Новая функциональность
    int n3 = generate_random_int(generator, 20, 200);
    std::vector<int> array3(n3);
    for (auto& num : array3) {
        num = generate_random_int(generator, 1, 10000);
    }

    std::cout << "Новый массив из случайных целых чисел (" << n3 << " элементов):\n";
    for (const auto& num : array3) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    int initial_sum = std::accumulate(array3.begin(), array3.end(), 0);
    std::cout << "Сумма элементов исходного массива: " << initial_sum << std::endl;

    // Меняем цифры в порядке убывания
    for (auto& num : array3) {
        num = sort_digits_desc(num);
    }

    std::cout << "Массив после изменения цифр в порядке убывания:\n";
    for (const auto& num : array3) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Сортировка массива в порядке убывания
    std::sort(array3.begin(), array3.end(), std::greater<int>());
    std::cout << "Отсортированный массив в порядке убывания:\n";
    for (const auto& num : array3) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    int sorted_sum = std::accumulate(array3.begin(), array3.end(), 0);
    std::cout << "Сумма элементов отсортированного массива: " << sorted_sum << std::endl;

    // Сравнение сумм и вывод результата
    if (sorted_sum > initial_sum) {
        std::cout << "Сумма элементов после сортировки больше суммы до сортировки." << std::endl;
    }
    else if (sorted_sum < initial_sum) {
        std::cout << "Сумма элементов после сортировки меньше суммы до сортировки." << std::endl;
    }
    else {
        std::cout << "Суммы элементов до и после сортировки равны." << std::endl;
    }
    return 0;
}
