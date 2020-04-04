#include "Matrix.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>

template <typename T>
T Random(size_t min, size_t max) {
  return min + rand() % (max - min);
}

template <typename T>
std::vector<T> randomVector(size_t size) {
    std::vector<T> vec;
    for (auto i = 0; i < size; ++i) {
        vec.push_back(Random<T>(0, 1000) / 1000);
    }
    return vec;
}

template <typename T>
class Neuro {
private:
    std::vector<Matrix<T>> weights;
    int layers;
    T learn_coef;

public:
    Neuro(std::vector<int> data, T _learn_coef) {
        learn_coef = _learn_coef;
        layers = data.size() - 1;
        for (auto i = 0; i < data.size() - 1; ++i) {
            auto vec = randomVector<T>(data[i] * data[i+1]);
            Matrix<T> m(vec, data[i], data[i+1]);
            weights.push_back(m);
        }
    }

    T sygmoid (T x) {
        T e = std::exp(-x);
        T rez = 1/(1+e);
        return rez;
    }

    size_t size() const {
        return layers;
    }

    Matrix<T> get_weights(int i) const {
        return (*this).weights[i];
    }

    friend std::ostream& operator<< (std::ostream &out, const Neuro n) { // <<
        for (auto i = 0; i < n.size(); ++i) {
            out << "Layer #" << i + 1 << ":\n";
            out << n.get_weights(i) << '\n';
        }
        return out;
    }

    std::vector<T> process (std::vector<T> input) {
        for (auto i = 0; i < layers - 1; ++i) {
            for (auto j = 0; j < layers; ++j) {
                input[j] = sygmoid(input[j]);
            }
            Matrix<T> vec(input, input.size(), 1);
            vec = weights[i] * vec;
            input = vec.get_vec();
        }
        for (auto j = 0; j < layers; ++j) {
                input[j] = sygmoid(input[j]);
        }
        return input;
    }

    std::vector<std::vector<T>> process_tr (std::vector<T> input) {
        std::vector<std::vector<T>> rez;
        rez.push_back(input);
        for (auto i = 0; i < layers - 1; ++i) {
            for (auto j = 0; j < layers; ++j) {
                input[j] = sygmoid(input[j]);
            }
            Matrix<T> vec(input, input.size(), 1);
            vec = weights[i] * vec;
            input = vec.get_vec();
            rez.push_back(input);
        }
        for (auto j = 0; j < layers; ++j) {
            input[j] = sygmoid(input[j]);
        }
        rez.push_back(input);
        return rez;
    }
    
    void train (std::vector<T> input, std::vector<T> expected) {
        auto rez = process_tr(input);
        if (rez.size() != expected) {
            std::cerr << "Sizes of output and expected arrays are different\n";
            throw std::runtime_error("");
        } else {
            std::vector<T> error;
            auto t = rez.size() - 1;
            for (auto i = 0; i < rez.size(); ++i) {
                error.push_back(expected[i] - rez[t][i]);
            }
            for (auto i = layers - 1; i >= 0; --i) {
                std::vector<T> nexterr;
                Matrix<T> m(error, error.size(), 1);
                nexterr = (weights[i].transposed * m).get_vec();
                //numpy.dot ((output_errors * final_outputs * (1.0 - final_outputs)), numpy.transpose (hidden_outputs))
                Matrix<T> next_layer(rez[t - 1], rez[t - 1].size(), 1);
                Matrix<T> layer(rez[t], 1, rez[t].size());
                auto rez = ( * ());
                rez *= learn_coef;
                weights[i] = 
                error = nexterr;
            }
        }
    }
};
