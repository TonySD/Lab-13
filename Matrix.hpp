#ifndef MATRIX_TEMPLATE
#define MATRIX_TEMPLATE 09022023L

#include <iostream>

template <typename T>
class Matrix {
    private:
        T* data_;
        size_t side_size_;
        size_t squared_size_;
    
    public:
        long coefficient = 1;
        Matrix(): side_size_(0), data_(NULL), squared_size_(0) {}
        Matrix(size_t size): side_size_(size), data_(new T[size * size]), squared_size_(size * size) {}
        Matrix(T* data, size_t size): side_size_(size), squared_size_(size * size) {
            data_ = new T[squared_size_];
            for (size_t i = 0; i < squared_size_; ++i) {
                data_[i] = data[i];
            }
        }
        ~Matrix() {
            delete[] data_;
        }

        Matrix& operator=(const Matrix& other) {
            if (this == &other) return *this;

            if (data_) delete[] data_;

            this->side_size_ = other.side_size_;
            this->squared_size_ = other.squared_size_;
            this->data_ = new T[this->squared_size_];
            this->coefficient = other.coefficient;

            for (size_t i = 0; i < this->squared_size_; ++i) {
                data_[i] = other.data_[i];
            }

            return *this;
        } 

        size_t side() const {
            return side_size_;
        }

        size_t size() const {
            return squared_size_;
        }

        T& at(size_t index) {
            return data_[index];
        }
        const T& at(size_t index) const {
            return data_[index];
        }
        T& at(size_t columns, size_t rows) {
            return data_[side_size_ * columns + rows];
        }
        const T& at(size_t columns, size_t rows) const {
            return data_[side_size_ * columns + rows];
        }

        template <class U>
        friend std::ostream& operator<< (std::ostream& out, const Matrix<U>& matrix);
        
};

template <class U>
std::ostream& operator<<(std::ostream& out, const Matrix<U>& matrix) {   
    size_t i, u;
    for (i = 0; i < matrix.side_size_; ++i) {
        for (u = 0; u < matrix.side_size_; ++u) {
            out << matrix.at(i, u) << '\t';
        }
        out << std::endl;
    }
    return out; 
}

#endif /* MATRIX_TEMPLATE */