#include <iostream>
#include <thread>
#include <chrono>
#include "Matrix.hpp"

template <typename T>
long determinant(const Matrix<T>& matrix) {
    if (matrix.side() == 1) return matrix.coefficient * matrix.at(0);
    return matrix.coefficient * (matrix.at(0) * matrix.at(2) - matrix.at(1) * matrix.at(3));
}

template <typename T>
Matrix<T>* matrix_below(const Matrix<T>& base) {
    size_t i, u, counter;
    Matrix<T>* to_return = new Matrix<T>[base.side()]();
    Matrix<T> current (base.side() - 1);
    for (i = 0; i < base.side(); ++i) {
        current.coefficient = base.coefficient * base.at(i);
        if (i % 2 != 0) current.coefficient *= (-1);
        counter = 0;
        for (u = 0; u < base.size(); ++u) {
            if ((u + 1 > base.side()) && (u % base.side() != i)) {
                current.at(counter) = base.at(u);
                ++counter;
            }
        }
        to_return[i] = current;
    }
    return to_return;
}

template <typename T>
long calculateDeterminant(const Matrix<T>& base, long& to_write) {
    size_t i, u, current_side_size, current_size = 1;
    long to_return = 0;
    Matrix<T>* returned_matrixes = NULL, *current_matrixes = NULL;
    Matrix<T>* matrixes = new Matrix<T>[current_size]();
    matrixes[0] = base;
    while (matrixes[0].size() > 0) {
        if (matrixes[0].size() <= 2) {
            for (i = 0; i < current_size; ++i) {
                to_return += determinant(matrixes[i]);
            }
            break;
        }

        if (current_matrixes) delete[] current_matrixes;
        current_side_size = matrixes[0].side();
        current_matrixes = new Matrix<T>[current_size * current_side_size]();
        for (i = 0; i < current_size; ++i) {
            returned_matrixes = matrix_below(matrixes[i]);
            for (u = 0; u < current_side_size; ++u) {
                current_matrixes[i * current_side_size + u] = returned_matrixes[u];
            }
        }
        current_size *= current_side_size;
        delete[] matrixes;
        matrixes = new Matrix<T>[current_size]();
        for (i = 0; i < current_size; ++i) {
            matrixes[i] = current_matrixes[i];
        }
    }

    to_write += to_return;

    return to_return;
}

template <typename T>
long linear_algorithm(const Matrix<T>& matrix) {
    long determinant = 0;
    calculateDeterminant<long>(matrix, std::ref(determinant));
    return determinant;
}

template <typename T>
long parallel_algorithm(const Matrix<T>& matrix) {
    long determinant = 0;
    Matrix<T>* matrixes = matrix_below(matrix);
    for (size_t i = 0; i < matrix.side(); ++i) {
        std::thread current (calculateDeterminant<T>, matrixes[i], std::ref(determinant));
        current.join();
    }
    return determinant;
}

Matrix<long> defaultMatrix() {
    int size_of_matrix = 4;
    long* numbers = new long[16]{11, 19, 3, 12, 2, 86, 43, 16, 64, 3, 7, 4, 6, 4, 13, 6};
    Matrix<long> matrix (numbers, size_of_matrix);
    return matrix;
}

Matrix<long> randomMatrix() {
    srand(time(0));
    size_t size_of_matrix = 4 + rand() % 6;
    long* numbers = new long[size_of_matrix * size_of_matrix];
    for (size_t i = 0; i < size_of_matrix * size_of_matrix; ++i) {
        numbers[i] = rand() % 40;
    } 
    Matrix<long> matrix (numbers, size_of_matrix);
    return matrix;
}

template <typename T>
void checkAlgorithm(const Matrix<T>& matrix) {
    long determinant1 = 0;
    long determinant2 = 0;

    std::cout << matrix << std::endl << std::endl;

    auto start1 = std::chrono::steady_clock::now();
    determinant1 = linear_algorithm<long>(matrix);
    auto end1 = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_linear = end1 - start1;

    auto start2 = std::chrono::steady_clock::now();
    determinant2 = parallel_algorithm<long>(matrix);
    auto end2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_parallel = end2 - start2;

    std::cout << "Linear algorithm's determinant: " << determinant1 << std::endl;
    std::cout << "Linear algorithm's time (s): " << elapsed_linear.count() << std::endl;
    std::cout << "Parallel algorithm's determinant: " << determinant2 << std::endl;
    std::cout << "Parallel algorithm's time (s): " << elapsed_parallel.count() << std::endl << std::endl;
}

int main() {
    Matrix<long> matrix = defaultMatrix();    
    checkAlgorithm(matrix);
    checkAlgorithm(randomMatrix());
    return 0;
}