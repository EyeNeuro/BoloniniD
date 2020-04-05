#include "Matrix.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>


/*
    creates a random number in segment (min, max)
*/
template <typename T>
T Random(size_t min, size_t max) {
  return min + rand() % (max - min);
}


/*
    creates a vector of random numbers
*/
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
    /*
        network constructor
    */
    Neuro(std::vector<int> data, T _learn_coef) {
        learn_coef = _learn_coef;
        layers = data.size() - 1;
        for (auto i = 0; i < data.size() - 1; ++i) {
            auto vec = randomVector<T>(data[i] * data[i+1]);
            Matrix<T> m(vec, data[i+1], data[i]);
            weights.push_back(m);
        }
    }

    /*
        sygmoid activation function
    */
    T sygmoid (T x) {
        T e = std::exp(-x);
        T rez = 1/(1+e);
        return rez;
    }

    /*
        actually returns the number of weights matriced
    */
    size_t size() const {
        return layers;
    }

    /*
        returns a weights matrix
    */
    Matrix<T> get_weights(int i) const {
        return (*this).weights[i];
    }

    /*
        overloaded neuro operator <<
    */
    friend std::ostream& operator<< (std::ostream &out, const Neuro n) { // <<
        for (auto i = 0; i < n.size(); ++i) {
            out << "Layer #" << i + 1 << ":\n";
            out << n.get_weights(i) << '\n';
        }
        return out;
    }

    /*
        gets a vector of start values
        returns a vector of result values
    */
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

    /*
        Gets a vector of start values.
        Returns a vector of vectors. Each of them contains a result for each layer.
    */
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
    
    /*
        Gets a vector of start values.
        Return nothing, changes weights inside network.
    */
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
                vector<T> vec;
                for (auto j = 0; j < error.size(); ++j) {
                    vec.push_back(error[j] * rez[t][j] * (1 - rez[t][j]));
                }
                Matrix<T> f(vec, vec.size(), 1);
                Matrix<T> s(rez[t], 1, rez[t].size());
                auto rez = s * f;
                rez *= learn_coef;
                weights[i] += rez;
                error = nexterr;
                --t;
            }
        }
    }
};
