#include <algorithm>

const double kItersFactor = 50.00;
const double limitOfMaxIters = 1000.00;

struct Complex {
    double real;
    double imag;
};

Complex multiply(Complex a, Complex b) {
    Complex product;
    product.real = a.real * b.real - a.imag * b.imag;
    product.imag = a.real * b.imag + a.imag * b.real;
    return product;
}

Complex sum(Complex a, Complex b) {
    Complex sum;
    sum.real = a.real + b.real;
    sum.imag = a.imag + b.imag;
    return sum;
}

bool converges(Complex x) {
    return x.real * x.real + x.imag * x.imag < 4; 
}

double getMaxIters(double width) {
    return std::min(kItersFactor / width, limitOfMaxIters);
}

int itersOfMandelbrot(Complex c, double width) {
    const double maxIter = getMaxIters(width);
    Complex z0 = {0, 0};
    Complex z = z0;

    int count = 0;
    while (count < maxIter && converges(z)) {
        Complex new_z = multiply(z, z);
        new_z = sum(new_z, c);
        z = new_z;
        count++;
    }

    return count;
}

double pixelToReal(double cx, double x, double width) {
    double result = cx - width / 2.0 + (x / 800.0) * width; 
    return result;
}

double pixelToImag(double cy, double y, double height) {
    double result = cy - height / 2.0 + (y / 600.0) * height;
    return result;
}

