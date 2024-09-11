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
