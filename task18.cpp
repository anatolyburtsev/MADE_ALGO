//
// Created by Анатолий Бурцев on 2019-12-28.
//Построение выпуклой оболочки (5 баллов)
//Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
//Вариант 1. С помощью алгоритма Грэхема.
//Вариант 2. С помощью алгоритма Джарвиса.
//Обратите внимание: три и более точки из множества могут лежать на одной прямой.
//Формат входного файла
//        Количество точек, далее на каждой строке координаты точек x, y.
//Общее количество точек не превосходит 100000.
//Координаты точек - числа с плавающей точкой в диапазоне [-1000, 1000].
//Формат выходного файла
//        Выведите периметр выпуклой оболочки.
//Время: 100мс, память 3Mb.
//

#include <vector>
#include <stack>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

struct Point {
    double x, y;

    void Print() {
        cout << "(" << x << "," << y << ")";
    }
};

class ConvexHull {
public:
    ConvexHull(vector<Point> points) : points(std::move(points)) {}

    void buildConvexHull();

    double calcPerimeter();

private:
    constexpr static const double EPS = 1e-6;
    vector<Point> points;
    stack<Point> convexHull;

    static double distance(Point P1, Point P2) {
        double d1 = P1.x - P2.x;
        double d2 = P1.y - P2.y;
        double dist = sqrt(d1 * d1 + d2 * d2);
        return dist;
    }

    static double getAngle(Point P1, Point P2, Point P3) {
        return (P2.x - P1.x) * (P3.y - P1.y) - (P2.y - P1.y) * (P3.x - P1.x);
    }

    static vector<Point> removeParallel(vector<Point> points);

    static bool isParallel(Point initialP, Point p1, Point p2);
};

struct cmp_angles {
    explicit cmp_angles(Point p) : zero(p) {};
    Point zero;

    inline bool operator()(const Point &p1, const Point &p2) {
        return getAngle(zero, p1, p2) > 0;
    }

    double getAngle(Point P1, Point P2, Point P3) {
        return (P2.x - P1.x) * (P3.y - P1.y) - (P2.y - P1.y) * (P3.x - P1.x);
    }
};


void ConvexHull::buildConvexHull() {
    // put point with min y into first position in vector
    for (int i = 1; i < points.size(); ++i) {
        Point p = points.at(i);
        if (p.x < points.at(0).x) {
            swap(points.at(0), points.at(i));
        } else if (abs(p.x - points.at(0).x) < EPS) {
            if (p.y < points.at(0).y) {
                swap(points.at(0), points.at(i));
            }
        }

    }
    // sort
    sort(points.begin() + 1, points.end(), cmp_angles(points.at(0)));
    points = removeParallel(points);

    // start building convex hull
    Point initialPoint = points.at(0);
    Point lastPoint = points.at(1), prelastPoint = initialPoint;
    convexHull.push(initialPoint);
    convexHull.push(lastPoint);
    for (int j = 2; j < points.size(); ++j) {
        Point newPoint = points.at(j);
        while (true) {
            double angle = getAngle(prelastPoint, lastPoint, newPoint);
            if (angle > -EPS) break;
            convexHull.pop();
            convexHull.pop();
            lastPoint = prelastPoint;
            prelastPoint = convexHull.top();
            convexHull.push(lastPoint);
        }
        prelastPoint = lastPoint;
        lastPoint = newPoint;
        convexHull.push(newPoint);

    }
}

double ConvexHull::calcPerimeter() {
    double perimeter = 0.;
    Point startPoint = convexHull.top();
    Point lastPoint = startPoint;
    Point currentPoint{};
    convexHull.pop();
    while (!convexHull.empty()) {
        currentPoint = convexHull.top();
        convexHull.pop();
        perimeter += distance(lastPoint, currentPoint);
        lastPoint = currentPoint;
    }
    perimeter += distance(lastPoint, startPoint);
    return perimeter;
}

bool ConvexHull::isParallel(Point p0, Point p1, Point p2) {
    double angle = getAngle(p0, p1, p2);
    return abs(angle) < EPS;
}

vector<Point> ConvexHull::removeParallel(vector<Point> points) {
    // first is an initial
    Point initial = points.at(0);
    vector<Point> newPoints;
    newPoints.push_back(initial);

    Point last = points.at(1);
    Point current{};
    for (int i = 2; i < points.size(); ++i) {
        current = points.at(i);
        if (isParallel(initial, last, current)) {
            if (distance(initial, last) > distance(initial, current)) {
                continue;
            } else {
                last = current;
                continue;
            }
        } else {
            newPoints.push_back(last);
            last = current;
        }
    }
    newPoints.push_back(last);
    points.clear();
    return newPoints;
}


int main() {
    int n = 3;
    double x, y;
    vector<Point> points;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> x >> y;
        points.push_back(Point{x, y});
    }
    if (points.size() < 2) {
        cout << 0.;
        return 0;
    }
    ConvexHull ch(points);
    ch.buildConvexHull();
    cout.precision(12);
    cout << ch.calcPerimeter();

}

