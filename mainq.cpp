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
