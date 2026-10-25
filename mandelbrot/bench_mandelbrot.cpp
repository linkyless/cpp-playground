// Benchmark of the Mandelbrot kernel from cpp-playground, without SFML.
// Compile:  g++ -O2 -fopenmp bench_mandelbrot.cpp -o bench
// Run:      ./bench
// It renders the same 800x600 frame with 1 thread and with all threads,
// and prints the time of each and the speedup.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <omp.h>

constexpr int kWidth = 800, kHeight = 600;

int iters(double cr, double ci, int maxIter) {
    double zr = 0, zi = 0;
    int n = 0;
    while (n < maxIter && zr * zr + zi * zi < 4.0) {
        double t = zr * zr - zi * zi + cr;
        zi = 2 * zr * zi + ci;
        zr = t;
        n++;
    }
    return n;
}

double render(int threads, std::vector<int>& out) {
    omp_set_num_threads(threads);
    // A zoomed-in view on the boundary, where the iteration cap is high
    double cx = -0.743643887, cy = 0.131825904, width = 0.05, height = width * 3.0 / 3.5;
    int maxIter = (int)std::min(50.0 / width, 1000.0);
    double t0 = omp_get_wtime();
    #pragma omp parallel for collapse(2)
    for (int x = 0; x < kWidth; x++)
        for (int y = 0; y < kHeight; y++) {
            double cr = cx - width / 2 + (double)x / kWidth * width;
            double ci = cy - height / 2 + (double)y / kHeight * height;
            out[x * kHeight + y] = iters(cr, ci, maxIter);
        }
    return omp_get_wtime() - t0;
}

int main() {
    std::vector<int> buf(kWidth * kHeight);
    int all = omp_get_max_threads();
    render(all, buf); // warm-up
    double best1 = 1e9, bestN = 1e9;
    for (int r = 0; r < 5; r++) {
        best1 = std::min(best1, render(1, buf));
        bestN = std::min(bestN, render(all, buf));
    }
    std::printf("1 thread:   %.1f ms per frame\n", best1 * 1000);
    std::printf("%d threads: %.1f ms per frame\n", all, bestN * 1000);
    std::printf("speedup:    %.1fx\n", best1 / bestN);
}
