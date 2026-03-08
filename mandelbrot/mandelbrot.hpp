#pragma once
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>

constexpr double kItersFactor = 50.0;
constexpr double kMaxItersLimit = 10000.0;
constexpr double kZoomIn = 0.9; // +10%
constexpr double kZoomOut = 1.1; // -10%
constexpr int kWidth = 800;
constexpr int kHeight = 600;

struct Complex {
    double real;
    double imag;
};

Complex operator*(Complex a, Complex b) {
    return {
        a.real * b.real - a.imag * b.imag,
        a.real * b.imag + a.imag * b.real
    };
}

Complex operator+(Complex a, Complex b) {
    return {a.real + b.real, a.imag + b.imag};
}

bool converges(Complex x) {
    return x.real * x.real + x.imag * x.imag < 4.0;
}

int getMaxIters(double width) {
    return (int)std::min(kItersFactor / width, kMaxItersLimit);
}

int itersOfMandelbrot(Complex c, double width) {
    const int maxIter = getMaxIters(width);
    Complex z = {0.0, 0.0};
    int count = 0;
    while (count < maxIter && converges(z)) {
        z = z * z + c;
        count++;
    }
    return count;
}

double pixelToReal(double cx, double x, double width) {
    return cx - width / 2.0 + (x / kWidth) * width;
}

double pixelToImag(double cy, double y, double height) {
    return cy - height / 2.0 + (y / kHeight) * height;
}

sf::Color hsvToRgb(double h, double s, double v) {
    double c = v * s;
    double x = c * (1 - std::abs(std::fmod(h / 60.0, 2) - 1));
    double m = v - c;
    double r, g, b;

    if      (h < 60)  { r = c; g = x; b = 0; }
    else if (h < 120) { r = x; g = c; b = 0; }
    else if (h < 180) { r = 0; g = c; b = x; }
    else if (h < 240) { r = 0; g = x; b = c; }
    else if (h < 300) { r = x; g = 0; b = c; }
    else              { r = c; g = 0; b = x; }

    return sf::Color(
        (int)((r + m) * 255),
        (int)((g + m) * 255),
        (int)((b + m) * 255)
    );
}
