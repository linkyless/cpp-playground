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

int itersOfMandelbrot(Complex c) {
    const int maxIter = 200;
    
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

double pixelToReal(int original) {
    double result = -2.5 + (double) original / 800.0 * 3.5;
    return result;
}

double pixelToImag(int original) {
    double result = -1.5 + (double) original / 600.0 * 3.0;
    return result;
}
