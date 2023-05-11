#ifndef MATRIX_TEMPLATE
#define MATRIX_TEMPLATE 09022023L


template<typename T>
class Matrix {
    private: 
        T *data;
        size_t rows;
        size_t columns;
    
    public:
        Matrix(): data(NULL), rows(0), columns(0) {};
        Matrix(const Matrix<T>& other): rows(other.rows), columns(other.columns) {
            data = new T[rows * columns];
            size_t length = rows * columns;
            for (size_t i = 0; i < length; ++i) {
                data[i] = other.data[i];
            }
        };
        Matrix(size_t rows, size_t cols): rows(rows), columns(cols) {
            data = new T[rows * columns]();
        };
        ~Matrix() {
            delete[] data;
            this->data = NULL;
        }
        
        Matrix<T>& operator=(const Matrix<T>& other) {
            if (&other == this) {
                return *this;
            }

            if (data) delete[] data;

            rows = other.rows;
            columns = other.columns;
            
            size_t length = rows * columns;
            data = new T[length];

            for (size_t i = 0; i < length; ++i) {
                data[i] = other.data[i];
            }

            return *this;
        };

        friend std::ostream& operator<<(std::ostream& out, const Matrix& matr) {
            for (size_t i = 0; i < matr.columns; ++i) {
                for (size_t y = 0; y < matr.rows; ++y) {
                    out << matr.data[i * matr.rows + y] << '\t';
                }
                out << std::endl;
            }

            return out;
        }

        friend std::istream& operator>> (std::istream& in, Matrix<T>& matrix) {
            size_t i, columns, rows;
            T current;

            std::cin >> columns;
            std::cin >> rows;

            T* new_data = new T[columns * rows];

            for (i = 0; i < columns * rows; ++i) {
                std::cin >> current;
                new_data[i] = current;
            }

            matrix.columns = columns;
            matrix.rows = rows;
            matrix.data = new_data;

            return in;
        }

        T&      at(size_t r, size_t c) {
            return data[c * columns + r];
        };
        const T& at(size_t r, size_t c) const {
            return data[c * columns + r];
        };

        friend Matrix<T> operator+(const Matrix<T>& left, const Matrix<T>& right) {
            Matrix<T> to_return (left.rows, left.columns);
            size_t length = left.rows * left.columns;
            for (size_t i = 0; i < length; ++i) {
                to_return.data[i] = left.data[i] + right.data[i];
            }

            return to_return;
        }

        friend Matrix<T> operator*(const Matrix<T>& left, const Matrix<T>& right) {
            Matrix<T> to_return (left.rows, left.columns); 
            /*
                i <=> current_row_second_matrix
                u <=> current_column_first_matrix
                j <=> pointer
            */
            for (size_t i = 0; i < right.rows; ++i) {
                for (size_t u = 0; u < left.columns; ++u) {
                    for (size_t j = 0; j < left.rows; ++j) {
                        to_return.at(i, u) += left.at(j, u) * right.at(i, j);
                    }
                }
            }

            return to_return;
        }

        template <typename Integer>
        friend Matrix<T> operator*(const Matrix<T>& left, const Integer& number) {
            Matrix to_return (left);
            size_t length = left.rows * left.columns;

            for (size_t i = 0; i < length; ++i) {
                to_return.data[i] *= number;
            }

            return to_return;
        }

        template <typename Integer>
        friend Matrix<T> operator*(const Integer& number, const Matrix<T>& left) {
            return left * number;
        }

        template <typename Integer>
        friend Matrix<T> operator/(const Matrix<T>& left, const Integer& number) {
            return left * (1.0 / number);
        }

        friend Matrix<T> operator-(const Matrix<T>& left, const Matrix<T>& right) {
            Matrix negative_copy (right);
            return left + (-1) * negative_copy;
        }

        T&       operator()(size_t r, size_t c)       {
            return this->at(r, c);
        }
        const T& operator()(size_t r, size_t c) const {
            return this->at(r, c);
        }
        
        void Transpose() {
            Matrix<T> result (this->columns, this->rows);

            for (size_t i = 0; i < this->columns; ++i) {
                for (size_t y = 0; y < this->rows; ++y) {
                    result.at(i, y) = this->at(y, i);
                }
            }

            delete[] this->data;
            this->data = result.data;
            result.data = NULL;
        };
        
        Matrix<T>& M() const {
            Matrix<T> *M_MATRIX = new Matrix<T>();
            this->LU(NULL, NULL, M_MATRIX);
            return *M_MATRIX;
        };

        Matrix<T>& L() const {
            Matrix<T> *L_MATRIX = new Matrix<T>();
            this->LU(NULL, L_MATRIX, NULL);
            return *L_MATRIX;
        }

        Matrix<T>& U() const {
            Matrix<T> *U_MATRIX = new Matrix<T>();
            this->LU(U_MATRIX, NULL, NULL);
            return *U_MATRIX;
        }
        
        // если u != 0, записывает в u верхнетреугольную матрицу
        // если l != 0, записывает в l нижнетреугольную матрицу
        // если m != 0, записывает в m матрицу перестановок
        void LU(Matrix<T> *u, Matrix<T> *l, Matrix<T> *m) const {
            size_t i, y, counter;
            double coeff;
            Matrix<T> M_MATRIX (this->rows, this->columns);
            Matrix<T> U_MATRIX (*this);
            Matrix<T> L_MATRIX (this->rows, this->columns);

            for (i = 0; i < this->columns; ++i) {
                for (y = 0; y < this->rows; ++y) {
                    if (i == y) {
                        M_MATRIX.at(y, i) = 1;
                        L_MATRIX.at(y, i) = 1;
                    }
                }
            }

            /*
                i <=> current leading column and row
                y <=> current leading row
                counter <=> pointer
            */

            for (i = 0; i < this->columns; ++i) {
                for (counter = i + 1; counter < this->columns; ++counter) {
                    coeff = U_MATRIX.at(i, counter) * 1.0 / U_MATRIX.at(i, i) * 1.0;
                    L_MATRIX.at(i, counter) = coeff;
                    for (y = i; y < this->rows; ++y) {
                        U_MATRIX.at(y, counter) -= U_MATRIX.at(y, i) * coeff;
                    }
                }
            }

            if (u != NULL) {
                *u = U_MATRIX;
            }
            if (l != NULL) {
                *l = L_MATRIX;
            }
            if (m != NULL) {
                *m = M_MATRIX;
            }
        };
        
        // решение СЛАУ A*x=b
        // вход:  b - матрица размера rows*1
        // воход: x - матрица размера rows*1
        Matrix<T>& Solve(const Matrix<T>& b) const {
            /*
            L * X = B
            https://courses.physics.illinois.edu/cs357/sp2020/notes/ref-9-linsys.html
            l11 * x1 = b1
            l12 * x1 + l22 * x2 = b2
            ...
            */ 
            T current_x;
            size_t i, y;
            Matrix<T> *X = new Matrix(1, this->rows);
            Matrix<T> X_MATRIX (1, this->rows);
            Matrix<T> RES_MATRIX (1, this->rows);
            Matrix<T> L_MATRIX, U_MATRIX;
            this->LU(&U_MATRIX, &L_MATRIX, NULL); 
            for (i = 0; i < this->rows; ++i) {
                current_x = b.at(i, 0);
                for (y = 0; y < i; ++y) {
                    current_x = current_x - X_MATRIX.at(y, 0) * L_MATRIX.at(y, i);
                }
                X_MATRIX.at(i, 0) = current_x / L_MATRIX.at(i, i);
            }

            for (i = this->rows - 1; i > -1; --i) {
                current_x = X_MATRIX.at(i, 0);
                for (y = i + 1; y < this->rows; ++y) {
                    current_x -= U_MATRIX.at(y, i) * RES_MATRIX.at(y, 0);
                }
                RES_MATRIX.at(i, 0) = current_x;
            }

            *X = RES_MATRIX;
            return *X;
        };
        
};

#endif /* MATRIX_TEMPLATE */