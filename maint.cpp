#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

double func1(double x) {
    return std::sin(x + 1);
}

double func2(double x) {
    return 0.2 * x;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graphs of functions y=sin(x+1) - red and y=0.2x - blue");

    // Создание осей координат
    sf::VertexArray xAxis(sf::Lines, 2);
    xAxis[0].position = sf::Vector2f(0, 300);
    xAxis[0].color = sf::Color::Black;
    xAxis[1].position = sf::Vector2f(800, 300);
    xAxis[1].color = sf::Color::Black;

    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = sf::Vector2f(400, 0);
    yAxis[0].color = sf::Color::Black;
    yAxis[1].position = sf::Vector2f(400, 600);
    yAxis[1].color = sf::Color::Black;

    // Создание делений на осях
    std::vector<sf::CircleShape> ticks;

    // Деления на оси X
    for (int i = -10; i <= 10; ++i) {
        if (i == 0) continue; // Пропустить центр оси
        sf::CircleShape tick(2);
        tick.setFillColor(sf::Color::Black);
        tick.setPosition(400 + i * 40 - 2, 300 - 2);
        ticks.push_back(tick);
    }

    // Деления на оси Y
    for (int i = -10; i <= 10; ++i) {
        if (i == 0) continue; // Пропустить центр оси
        sf::CircleShape tick(2);
        tick.setFillColor(sf::Color::Black);
        tick.setPosition(400 - 2, 300 - i * 100 - 2);
        ticks.push_back(tick);
    }

    // Построение графиков
    sf::VertexArray graph1(sf::LineStrip);
    sf::VertexArray graph2(sf::LineStrip);

    std::vector<sf::CircleShape> intersections;

    for (double x = -10; x <= 10; x += 0.01) {
        double y1 = func1(x);
        double y2 = func2(x);

        graph1.append(sf::Vertex(sf::Vector2f(400 + x * 40, 300 - y1 * 100), sf::Color::Blue));
        graph2.append(sf::Vertex(sf::Vector2f(400 + x * 40, 300 - y2 * 100), sf::Color::Red));

        if (std::abs(y1 - y2) < 0.01) {
            sf::CircleShape intersection(3);
            intersection.setFillColor(sf::Color::Green);
            intersection.setPosition(400 + x * 40 - 3, 300 - (y1 * 100) - 3);
            intersections.push_back(intersection);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        window.draw(xAxis);
        window.draw(yAxis);

        for (const auto& tick : ticks) {
            window.draw(tick);
        }

        window.draw(graph1);
        window.draw(graph2);

        for (const auto& point : intersections) {
            window.draw(point);
        }

        window.display();
    }

    return 0;
}
