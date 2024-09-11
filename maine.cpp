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
