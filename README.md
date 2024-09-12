# Лабораторная работа №4
## «Последовательные одномерные контейнеры»
## по дисциплине: «Программирование»
### Цели и задачи работы: изучение алгоритмов формирования и обработки одномерных массивов и последовательных контейнеров, программирование и отладка программ формирования и обработки массивов. 
### Задание к работе: Написать программу решения задачи в соответствии с индивидуальным вариантом. 
### Методика выполнения работы:
#### 1. Разработать алгоритм решения задачи по индивидуальному заданию. 
#### 2. Написать и отладить программу решения задачи. 
#### 3. Протестировать работу программы на различных исходных данных.
### Задания:
### Задание №1 «Методы решения нелинейный уравнений» 
#### Отделите корни заданного уравнения, согласно варианту из табл.1, и уточните их одним из методов с точностью до ε=10-4 . Решить уравнения методом половинного деления, методом Ньютона и методом простых итераций. Либо другими методами, перечисленными в теоретической части к заданию
#### Вариант 5 - 𝑠𝑖𝑛( 𝑥 + 1) = 0.2 𝑥
#### Решение задачи в mainq.cpp
```C++
#include <iostream>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <vector>

const double EPSILON = 0.0001; // точность
const int MAX_ITERATIONS = 20; // максимальное количество итераций

// Определяем функцию
double f(double x) {
    return sin(x + 1) - 0.2 * x;
}

// Производная функции для метода Ньютона
double f_prime(double x) {
    return cos(x + 1) - 0.2;
}

// Вторая производная функции для оценки сходимости
double f_double_prime(double x) {
    return -sin(x + 1);
}

// Функция разложения x = asin(0.2x) - 1
double func1(double x) {
    return asin(0.2 * x) - 1;
}

// Функция разложения x = sin(x+1) / 0.2
double func2(double x) {
    return sin(x + 1) / 0.2;
}

// Производная функции x = asin(0.2x) - 1
double derivativeFunc1(double x) {
    return 0.2 / sqrt(1 - pow(0.2 * x, 2));
}

// Производная функции x = sin(x+1) / 0.2
double derivativeFunc2(double x) {
    return cos(x + 1) / 0.2;
}

// Метод простых итераций
bool simpleIteration(double (*func)(double), double x0, double epsilon, int maxIterations, double& totalDuration, int& totalIterations, double& convergenceRate) {
    double x = x0;
    int iteration = 0;
    double lastX = x;
    double lastError = 0.0;

    // Вывод заголовка перед началом итераций
    std::cout << std::setw(5) << "N"
        << std::setw(16) << "x_старое"
        << std::setw(14) << "x_новое"
        << std::setw(26) << "|x_новое-x_старое|"
        << std::setw(20) << "Скорость сходимости" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    for (iteration = 0; iteration < maxIterations; ++iteration) {
        double newX = func(x);
        double diff = fabs(newX - x);

        // Проверка на недопустимые значения
        if (std::isnan(newX) || std::isnan(diff)) {
            std::cout << "Недопустимые значения, корень не найден." << std::endl;
            return false;
        }

        double currentConvergenceRate = 0.0;
        if (iteration > 0) {
            currentConvergenceRate = diff / lastError;
        }
        lastError = diff;

        std::cout << std::setw(5) << iteration + 1
            << std::setw(15) << std::fixed << std::setprecision(4) << x
            << std::setw(15) << std::fixed << std::setprecision(4) << newX
            << std::setw(20) << std::fixed << std::setprecision(4) << diff
            << std::setw(20) << std::fixed << std::setprecision(4) << currentConvergenceRate << std::endl;

        if (diff < epsilon) {
            lastX = newX;
            convergenceRate += currentConvergenceRate;
            break;
        }

        x = newX;
        lastX = newX;
        convergenceRate += currentConvergenceRate;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    totalDuration += duration.count();
    totalIterations += iteration;

    if (iteration < maxIterations) {
        std::cout << "Найден корень: x = " << std::fixed << std::setprecision(4) << lastX << " после " << iteration + 1 << " итераций" << std::endl;
    }

    return iteration < maxIterations;
}

// Проверка сходимости и вычисление корней
void checkConvergenceAndFindRoots() {
    std::cout << "\nРешение методом простых итераций\n" << std::endl;
    std::cout << std::fixed << std::setprecision(4);

    double totalDuration = 0.0;
    int totalIterations = 0;
    double convergenceRateSum = 0.0;

    std::vector<double> xValues;
    for (double x = -10; x <= 10; x += 1) {
        xValues.push_back(x);
    }

    std::cout << "Анализ метода x = asin(0.2x) - 1\n" << std::endl;
    for (double x : xValues) {
        double df1 = derivativeFunc1(x);

        std::cout << "x = " << std::fixed << std::setprecision(4) << x << ": ";
        if (fabs(df1) >= 1) {
            std::cout << "Метод не сходится (|производная| >= 1)." << std::endl;
        }
        else {
            std::cout << "Метод сходится, выполняем итерации." << std::endl;
            if (!simpleIteration(func1, x, EPSILON, MAX_ITERATIONS, totalDuration, totalIterations, convergenceRateSum)) {
                std::cout << "Метод не нашел корень после " << MAX_ITERATIONS << " итераций." << std::endl;
            }
        }

        std::cout << "\n\n";
    }

    std::cout << "Анализ метода x = sin(x + 1) / 0.2\n" << std::endl;
    for (double x : xValues) {
        double df2 = derivativeFunc2(x);

        std::cout << "x = " << std::fixed << std::setprecision(4) << x << ": ";
        if (fabs(df2) >= 1) {
            std::cout << "Метод не сходится (|производная| >= 1)." << std::endl;
        }
        else {
            std::cout << "Метод сходится, выполняем итерации." << std::endl;
            if (!simpleIteration(func2, x, EPSILON, MAX_ITERATIONS, totalDuration, totalIterations, convergenceRateSum)) {
                std::cout << "Метод не нашел корень после " << MAX_ITERATIONS << " итераций." << std::endl;
            }
        }

        std::cout << "\n\n";
    }

    std::cout << "Скорость схождения: " << std::endl;
    std::cout << "Количество итераций всего: " << totalIterations << std::endl;
    std::cout << "Количество времени общее: " << totalDuration << " секунд" << std::endl;
    std::cout << "Средняя скорость сходимости: " << "y = " << (totalIterations > 0 ? convergenceRateSum / totalIterations : 0.0) << "x" << std::endl;
}

// Метод половинного деления
void bisection(double a, double b, int q, int& total_iterations, double& total_time) {
    auto start = std::chrono::high_resolution_clock::now(); // Начало замера времени

    double c;
    int n = 1;

    std::cout << "\nМетод половинного деления для интервала [" << std::fixed << std::setprecision(4)
        << a << "; " << b << "]" << std::endl;
    std::cout << "N\tc\t\ta\t\tb\t\t|b - a|" << std::endl;

    for (n = 1; (b - a) / 2.0 > EPSILON; ++n) {
        c = (a + b) / 2.0;

        std::cout << n << "\t" << std::fixed << std::setprecision(4) << c << "\t\t"
            << a << "\t\t" << b << "\t\t" << (b - a) << std::endl;

        if (std::abs(f(c)) < EPSILON) {
            a = c;
            std::cout << ++n << "\t" << std::fixed << std::setprecision(4) << c << "\t\t"
                << a << "\t\t" << b << "\t\t" << (b - a) << std::endl;

            std::cout << "Корень найден: " << c << " после " << n << " итераций." << std::endl;

            total_iterations += n;
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            total_time += elapsed.count();

            return;
        }

        if (f(c) * f(a) < 0) {
            b = c;
        }
        else {
            a = c;
        }
    }

    std::cout << "Корень найден: " << (a + b) / 2.0 << " после " << n << " итераций." << std::endl;

    total_iterations += n;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    total_time += elapsed.count();
}

// Метод Ньютона
void newton(double x0, int max_iter, int& total_iterations, double& total_time) {
    auto start = std::chrono::high_resolution_clock::now(); // Начало замера времени

    double x = x0;
    int n = 1;

    std::cout << "\nНачальное приближение x0 = " << std::fixed << std::setprecision(4) << x0 << std::endl;
    std::cout << "N\tx\t\tf(x)\t\tf'(x)\t\t|f(x)/f'(x)|" << std::endl;

    // Вывод начальной итерации
    double fx = f(x);
    double fpx = f_prime(x);
    std::cout << n << "\t" << std::fixed << std::setprecision(4) << x << "\t\t"
        << fx << "\t\t" << fpx << "\t\t" << std::abs(fx / fpx) << std::endl;

    for (; std::abs(fx / fpx) > EPSILON && n < max_iter; ++n) {
        if (fpx == 0) {
            std::cout << "Производная равна нулю. Метод не применим." << std::endl;
            break;
        }

        x = x - fx / fpx;
        fx = f(x);
        fpx = f_prime(x);

        std::cout << n + 1 << "\t" << std::fixed << std::setprecision(4) << x << "\t\t"
            << fx << "\t\t" << fpx << "\t\t" << std::abs(fx / fpx) << std::endl;
    }

    if (n == max_iter) {
        std::cout << "Достигнуто максимальное количество итераций." << std::endl;
    }
    else {
        std::cout << "Корень найден: " << x << " после " << n << " итераций." << std::endl;
    }

    total_iterations += n;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    total_time += elapsed.count();
}

// Основная функция для нахождения интервалов и поиска корней
void find_roots() {
    double Alfa = -10;
    double Beta = 10;
    int max_iter = 100; // Максимальное количество итераций
    int total_iterations = 0;
    double total_time = 0.0;
    double K = (std::abs(Alfa) + std::abs(Beta)) / 2;

    // Сначала выполнение метода половинного деления
    std::cout << "Решение методом половинного деления" << std::endl;
    for (int q = 0; q < 10; ++q) {
        double a = Alfa + q * (Beta - Alfa) / K;
        double b = Alfa + (q + 1) * (Beta - Alfa) / K;

        if (f(a) * f(b) < 0) {
            bisection(a, b, q, total_iterations, total_time);
        }
        else {
            std::cout << "\nКорней нет на интервале [" << std::fixed << std::setprecision(4)
                << a << "; " << b << "]" << std::endl;
        }
    }
    std::cout << "\nСкорость сходимости метода половинного деления: " << std::endl;
    std::cout << "\nОбщее время выполнения: " << std::fixed << std::setprecision(4)
        << total_time << " секунд" << std::endl;
    std::cout << "Общее количество итераций: " << total_iterations << std::endl;
    std::cout << "Параметр сходимости: y = 0.5x " << std::endl;

    // Теперь выполнение метода Ньютона
    total_iterations = 0;
    total_time = 0.0;
    std::cout << "\nРешение методом Ньютона" << std::endl;

    // Вычисление параметра сходимости t для метода Ньютона
    double M2 = 0.0;
    double m1 = std::abs(f_prime(Alfa));
    for (double x = Alfa; x <= Beta; x += 1.0) {
        double fppx = std::abs(f_double_prime(x));
        double fpx = std::abs(f_prime(x));

        if (fppx > M2) M2 = fppx;
        if (fpx < m1) m1 = fpx;
    }
    double t = M2 / (2 * m1);

    for (int q = 0; q < 10; ++q) {
        double a = Alfa + q * (Beta - Alfa) / K;
        double b = Alfa + (q + 1) * (Beta - Alfa) / K;

        // Проверка, если a равно 0, заменить его на 0.1
        if (a == 0) {
            a = 1.0;
        }

        // Проверяем, лежит ли корень в интервале [a, b]
        if (f(a) * f(b) < 0) {
            std::cout << "\nКорень лежит в отрезке [" << a << "; " << b << "]. Начинаем вычисление корня." << std::endl;
            newton(a, max_iter, total_iterations, total_time);
        }
        else {
            std::cout << "\nКорень не найден в интервале [" << std::fixed << std::setprecision(4) << a << "; " << std::fixed << std::setprecision(4) << b << "]." << std::endl;
        }
    }

    std::cout << "\nСкорость сходимости метода Ньютона: " << std::endl;
    std::cout << "\nОбщее время выполнения: " << std::fixed << std::setprecision(4)
        << total_time << " секунд" << std::endl;
    std::cout << "Общее количество итераций: " << total_iterations << std::endl;
    std::cout << "Параметр сходимости: y = " << std::fixed << std::setprecision(4) << t << "x^2" << std::endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    find_roots(); // Вызов функции поиска корней
    checkConvergenceAndFindRoots();
    return 0;
}
```
#### Решение 1 задачи на языке Java в maina.txt
### Задание №2 «Вычисления в одномерных массивах»
#### Вариант 5 – алгоритм ranlux24_base; вариант 15 - 
#### 1. Напишите программу, в которой определен массив из 𝑛 чисел (𝑛 ≥ 10) и инициализирован случайными вещественными числами из диапазона [100, 900]. 
#### 2.  Введите некоторое число 𝐴 и найдите количество элементов массива из пункта 1, больших 𝐴. Вычислите произведение элементов, расположенных после максимального по модулю элемента. 
#### 3. Поменяйте в массиве из пункта 1 местами попарно соседние элементы, если в элементе с чётным номером количество десятков больше, чем количество сотен. Если количество элементов нечётное, последний элемент не рассматривается. 
#### 4. Определите и инициализируйте массив из 𝑛 чисел (𝑛 ≥ 15) случайными целыми числами из диапазона [10, 20]. Определите, какой из элементов повторяется наибольшее количество раз. 
#### 5. Определите и инициализируйте массив случайными числами. Вычислите сумму элементов массива. Напишите программу, меняющую местами цифры в элементах массива в порядке убывания (например, 9164 → 9641). Отсортируйте массив в порядке убывания. Вычислите сумму элементов массива и сравните с суммой элементов исходного массива.
#### Решение задачи в mainw.cpp
```C++
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
```
#### Решение 2 задачи на языке Java в mains.txt
### Задание №3 «Работа с генераторами рандомных чисел» 
#### Задание выполняется на языке C++. В соответствии с вариантом необходимо описать распределение генератора, с помощью этого генератора заполнить 3 массива размером 50, 100 и 1000 соответственно числами от 1 до 100. Использовать критерий хи-квадрат для проверки гипотезы о нормальном распределении выборки. Значение критерия хи-квадрат для каждой из выборок рассчитывается по формуле: х^2=сумма((v1-v2)^2)/v2), где v1 - количество элементов, попавших в i-ый интервал, v2 - ожидаемое число попаданий в i-ый интервал. Вариант 5 - алгоритм ranlux24_base
#### Решение задачи в maine.cpp
```C++
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <numeric>
#include <iomanip> // Для более красивого вывода

// Функция для генерации нормально распределенных чисел в рамках от 1 до 100
std::vector<double> generate_normal_distribution(int size, double mean, double stddev) {
    std::random_device rd;
    std::ranlux24_base gen(rd());
    std::normal_distribution<> d(mean, stddev);

    std::vector<double> data(size);
    for (auto& number : data) {
        for (;;) {
            number = d(gen);
            if (number >= 1 && number <= 100) {
                break;
            }
        }
    }
    return data;
}

// Функция для расчета критерия хи-квадрат
double calculate_chi_square(const std::vector<double>& data, int num_intervals, double mean, double stddev, std::vector<int>& intervals) {
    double interval_width = (100.0 - 1.0) / num_intervals;

    for (double value : data) {
        if (value >= 1 && value <= 100) {
            int interval = static_cast<int>((value - 1) / interval_width);
            intervals[interval]++;
        }
    }

    double expected = data.size() / num_intervals;
    double chi_square = 0;

    for (int count : intervals) {
        chi_square += pow(count - expected, 2) / expected;
    }

    return chi_square;
}

int main() {
    setlocale(LC_ALL, "Russian");
    const std::vector<int> sizes = { 50, 100, 1000 };
    const double mean = 50.5; // Среднее значение для чисел от 1 до 100
    const double stddev = 29.0; // Стандартное отклонение для чисел от 1 до 100
    const int num_intervals = 10; // Количество интервалов для критерия хи-квадрат

    for (int size : sizes) {
        std::vector<double> data = generate_normal_distribution(size, mean, stddev);

        std::vector<int> intervals(num_intervals, 0);
        double chi_square = calculate_chi_square(data, num_intervals, mean, stddev, intervals);

        double real_mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();

        std::cout << "Размер выборки: " << size << std::endl;
        std::cout << "Значение критерия хи-квадрат: " << chi_square << std::endl;
        std::cout << "Ожидаемое математическое ожидание: " << mean << std::endl;
        std::cout << "Реальное математическое ожидание: " << real_mean << std::endl;

        std::cout << "Интервалы: " << num_intervals << std::endl;
        std::cout << "Ширина интервала: " << (100.0 - 1.0) / num_intervals << std::endl;

        // Вывод данных и интервалов
        std::cout << "Сгенерированные числа: ";
        for (double value : data) {
            std::cout << std::fixed << std::setprecision(2) << value << " ";
        }
        std::cout << std::endl;

        for (int i = 0; i < num_intervals; ++i) {
            double interval_start = 1.0 + i * ((100.0 - 1.0) / num_intervals);
            double interval_end = interval_start + ((100.0 - 1.0) / num_intervals);
            std::cout << "Интервал " << i + 1 << " [" << interval_start << ", " << interval_end << "): ";
            std::cout << intervals[i] << " чисел" << std::endl;
        }

        std::cout << "Результат проверки гипотезы: ";
        if (chi_square < 15) {
            std::cout << "Гипотеза принята" << std::endl;
        }
        else {
            std::cout << "Гипотеза отклонена" << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}
```
### Задание №4 «Дилемма заключенного» 
#### Требуется реализовать игру «Предать или сотрудничать» и реализовать 3 алгоритма поведения в игре. 
#### Игра состоит из случайного кол-ва раундов от 100 до 200 (итоговое кол-во раундов при каждом запуске игры генерируется случайно). На протяжении игровой сессии сражаются 2 алгоритма. В каждом раунде каждый алгоритм выбирает, либо сотрудничество, либо предательство. Если алгоритм А выбирает предательство и алгоритм Б выбирает предательство они получают по 4 очка. Если алгоритм А выбирает сотрудничество, а алгоритм Б выбирает предательство - алгоритм А получает 0 очков, а алгоритм Б получает 20 очков. Если оба алгоритма выбирают сотрудничество оба получают 24 очка. Каждому алгоритму в каждом раунде известны результаты всех предыдуще раундов текущей игровой сессии, на основе этих данных алгоритм может выбирать будет он сотрудничать или предаст. 
#### Каждый алгоритм должен представлять из себя функцию с сигнатурой: 
#### boolean func(int32 round_number, array[boolean] self_choices, array[boolean] enemy_choices) 
#### round_number – номер текущего раунда 
#### self_choices – массив булевых значений, содержит информацию о собственных выборов (предать или сотрудничать) за все предыдущие раунды 
#### enemy_choices - массив булевых значений, содержит информацию о выборов (предать или сотрудничать) противника за все предыдущие раунды 
#### true – сотрудничество 
#### false – предательство 
#### Каждому студенту нужно реализовать 3 алгоритма и запустить игру 3 раза чтобы выявить лучший алгоритм. Лучший алгоритм будет сражаться в финальной битве между всеми студентами группы на очном занятии.
#### Решение задачи в mainr.cpp
```C++
#include <iostream>
#include <vector>
#include <random>
#include <utility>

// Функции для алгоритмов поведения
bool always_cooperate(int round_number, const std::vector<bool>& self_choices, const std::vector<bool>& enemy_choices) {
    return true; // Всегда сотрудничает
}

bool always_betray(int round_number, const std::vector<bool>& self_choices, const std::vector<bool>& enemy_choices) {
    return false; // Всегда предает
}

bool tit_for_tat(int round_number, const std::vector<bool>& self_choices, const std::vector<bool>& enemy_choices) {
    static bool betrayed_last = false;

    if (round_number == 0) {
        betrayed_last = false;
        return true; // Сотрудничает в первом раунде
    }

    if (betrayed_last) {
        betrayed_last = false;
        return true; // Сотрудничает после предательства в предыдущем раунде
    }

    if (!enemy_choices.back()) {
        betrayed_last = true;
        return false; // Предает после предательства оппонента
    }

    return true; // Сотрудничает в остальных случаях
}

bool eye_for_two_eyes(int round_number, const std::vector<bool>& self_choices, const std::vector<bool>& enemy_choices) {
    static int betray_count = 0;
    static bool betrayed_last = false;

    if (round_number == 0) {
        betray_count = 0;
        betrayed_last = false;
        return true;
    }

    if (betrayed_last) {
        betrayed_last = false;
        betray_count = 0;
        return true;
    }

    if (!enemy_choices.back()) {
        betray_count++;
    }

    if (betray_count >= 2) {
        betray_count = 0;
        betrayed_last = true;
        return false;
    }

    return true; // Сотрудничает, если не было двух предательств подряд
}

bool tricky(int round_number, const std::vector<bool>& self_choices, const std::vector<bool>& enemy_choices) {
    if (round_number == 0) return false; // Предает в первом раунде
    return !enemy_choices.back(); // Предает, если с ним сотрудничают, сотрудничает, если его предают
}

bool vengeful(int round_number, const std::vector<bool>& self_choices, const std::vector<bool>& enemy_choices) {
    if (round_number == 0) return true; // Сотрудничает в первом раунде
    return enemy_choices.back(); // Повторяет выбор противника
}

// Функция для проведения игры между двумя алгоритмами
std::pair<int, int> play_game(bool (*alg1)(int, const std::vector<bool>&, const std::vector<bool>&),
    bool (*alg2)(int, const std::vector<bool>&, const std::vector<bool>&), int& num_rounds) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(100, 200);
    num_rounds = dist(gen);

    std::vector<bool> choices1, choices2;
    int score1 = 0, score2 = 0;

    for (int round = 0; round < num_rounds; ++round) {
        bool choice1 = alg1(round, choices1, choices2);
        bool choice2 = alg2(round, choices2, choices1);

        choices1.push_back(choice1);
        choices2.push_back(choice2);

        if (choice1 && choice2) {
            score1 += 24;
            score2 += 24;
        }
        else if (!choice1 && !choice2) {
            score1 += 4;
            score2 += 4;
        }
        else if (choice1 && !choice2) {
            score2 += 20;
        }
        else {
            score1 += 20;
        }

        // Вывод ходов каждого алгоритма в текущем раунде
        std::cout << "Round " << round + 1 << ": alg1 = " << (choice1 ? "Cooperate" : "Betray")
            << ", alg2 = " << (choice2 ? "Cooperate" : "Betray") << "\n";
    }

    return std::make_pair(score1, score2);
}

// Функция для запуска игры и сравнения алгоритмов
void run_games() {
    // Алгоритмы
    std::vector<bool (*)(int, const std::vector<bool>&, const std::vector<bool>&)> algorithms = {
        always_cooperate, always_betray, tit_for_tat, eye_for_two_eyes, tricky, vengeful
    };
    std::vector<std::string> names = {
        "Always Cooperate", "Always Betray", "Tit for Tat", "Eye for Two Eyes", "Tricky", "Vengeful"
    };
    const int num_games = 3;

    // Итоговые результаты для каждого алгоритма
    std::vector<int> total_scores(algorithms.size(), 0);
    std::vector<double> overall_real_scores(algorithms.size(), 0.0);

    for (size_t i = 0; i < algorithms.size(); ++i) {
        for (size_t j = i + 1; j < algorithms.size(); ++j) {
            int total_score1 = 0, total_score2 = 0;
            double total_real_score1 = 0.0, total_real_score2 = 0.0;
            for (int game = 0; game < num_games; ++game) {
                std::cout << "Starting Game " << game + 1 << " between " << names[i] << " and " << names[j] << ":\n";
                int num_rounds = 0;
                std::pair<int, int> scores = play_game(algorithms[i], algorithms[j], num_rounds);
                int score1 = scores.first;
                int score2 = scores.second;

                total_score1 += score1;
                total_score2 += score2;
                std::cout << "Game " << game + 1 << " between " << names[i] << " and " << names[j] << ": "
                    << names[i] << " score: " << score1 << ", " << names[j] << " score: " << score2 << "\n";

                // Вычисление и вывод реальных очков после игры
                double real_score1 = (static_cast<double>(score1) / num_rounds) * 200;
                double real_score2 = (static_cast<double>(score2) / num_rounds) * 200;
                total_real_score1 += real_score1;
                total_real_score2 += real_score2;
                std::cout << "Real scores after Game " << game + 1 << ": " << names[i] << " score: " << real_score1
                    << ", " << names[j] << " score: " << real_score2 << "\n";
            }
            std::cout << "Total score for " << names[i] << " in " << num_games << " games: " << total_score1 << "\n";
            std::cout << "Total score for " << names[j] << " in " << num_games << " games: " << total_score2 << "\n";
            std::cout << "Total real score for " << names[i] << " in " << num_games << " games: " << total_real_score1 << "\n";
            std::cout << "Total real score for " << names[j] << " in " << num_games << " games: " << total_real_score2 << "\n\n";

            // Обновление общих результатов
            total_scores[i] += total_score1;
            total_scores[j] += total_score2;
            overall_real_scores[i] += total_real_score1;
            overall_real_scores[j] += total_real_score2;
        }
    }

    // Вывод общих результатов каждого алгоритма
    std::cout << "Overall scores for all algorithms:\n";
    for (size_t i = 0; i < names.size(); ++i) {
        std::cout << names[i] << ": " << total_scores[i] << ", Overall real score: " << overall_real_scores[i] << "\n";
    }
}

int main() {
    run_games();
    return 0;
}
```
#### Решение 4 задачи на языке Java в maind.txt
### Задание №5 «Генерация псевдослучайных чисел» 
#### Задание выполняется на языке C++. Для генерации чисел использовать алгоритмы (таблица 4): Вариант 5 – алгоритм RC4
#### Решение задачи в mainy.cpp
```C++
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
```
