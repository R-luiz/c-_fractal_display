#ifndef FRACTAL_HPP
#define FRACTAL_HPP

#include <complex>
#include <iostream>
#include <SDL2/SDL.h>

struct Vector3
{
    double r, g, b;

    Vector3() : r(0), g(0), b(0) {}
    Vector3(double r, double g, double b) : r(r), g(g), b(b) {}

    Vector3 operator*(double scalar) const {
        return Vector3(r * scalar, g * scalar, b * scalar);
    }

    Vector3 operator+(const Vector3& other) const {
        return Vector3(r + other.r, g + other.g, b + other.b);
    }

    SDL_Color toSDLColor() const {
        return SDL_Color {
            static_cast<Uint8>(std::min(255.0, std::max(0.0, r * 255.0))),
            static_cast<Uint8>(std::min(255.0, std::max(0.0, g * 255.0))),
            static_cast<Uint8>(std::min(255.0, std::max(0.0, b * 255.0))),
            255
        };
    }
};

struct ComplexPlane
{
    double x_min, x_max, y_min, y_max;
    int width, height, iterations;

    ComplexPlane(double x_min, double x_max, double y_min, double y_max, int width, int height, int iterations)
        : x_min(x_min), x_max(x_max), y_min(y_min), y_max(y_max), width(width), height(height), iterations(iterations) {}

    std::complex<double> pixelToComplex(int px, int py) const {
        double real = x_min + (x_max - x_min) * px / width;
        double imag = y_min + (y_max - y_min) * py / height;
        return std::complex<double>(real, imag);
    }

    void zoom(double factor, double center_x, double center_y) {
        double x_range = (x_max - x_min) / factor;
        double y_range = (y_max - y_min) / factor;
        x_min = center_x - x_range / 2;
        x_max = center_x + x_range / 2;
        y_min = center_y - y_range / 2;
        y_max = center_y + y_range / 2;
    }

    void pan(double dx, double dy) {
        double x_range = x_max - x_min;
        double y_range = y_max - y_min;
        x_min += dx * x_range;
        x_max += dx * x_range;
        y_min += dy * y_range;
        y_max += dy * y_range;
    }
};

// Function prototype for fractal calculation
Vector3 calculateFractalColor(const std::complex<double>& c, int maxIterations);

#endif // FRACTAL_HPP