#include <iostream>

long determinant_of_2d(long* numbers) {
    return numbers[0] * numbers[2] - numbers[1] * numbers[3];
}

long** matrix_below(long* numbers, size_t size) {
    long** to_return = new long*[size];
    long current_coef;
    long* current_array;
    size_t i, u, counter, squared_size = size * size;
    for (i = 0; i < size; ++i) {
        current_coef = numbers[i];
        current_array = new long[(size - 1) * (size - 1)];
        counter = 0;
        for (u = 0; u < squared_size; ++u) {
            if ((u % size != i) && (u / size != 0)) {
                current_array[counter++] = current_coef * numbers[u]; // Передача массива по ссылке с последующей записью коэффициента
            }
        }
        to_return[i] = current_array;
    }
    return to_return;
}

void print_matrix(long* matrix, size_t size) {
    for (size_t i = 0; i < size * size; ++i) {
        if (i % size == 0) std::cout << '\n';
        std::cout << matrix[i] << '\t';
    }
}


// size - длина строки / столбца
// long** determinants(long* numbers, size_t size) {
//     size_t i, u, current_size;
//     long** current[size];
//     for (i = size; i > 2; --i) {
//         for (u = 0; u < )
//     }
// }

int main() {
    long* smth = new long[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};

    long** wtf = matrix_below(smth, 3);
    for (size_t i = 0; i < 3; ++i) {
        std::cout << std::endl;
        print_matrix(wtf[i], 2);
    }

    return 0;
}