#ifndef VECTOR_OPERATIONS_H
#define VECTOR_OPERATIONS_H

#include "operations.h"
#include "linear.h"

template <unsigned N>
class VectorOperation : public Operation
{
public:
    Vector<N>& getValue()
    {
        return vec_value;
    }

protected:
    Vector<N> vec_value;
};

template <unsigned HEIGHT, unsigned WIDTH>
class MatrixOperator : public VectorOperation<HEIGHT>
{
public:
    void compute()
    {
        left_vec_op->compute();
        Vector<WIDTH> &val = left_vec_op->getValue();
        this->vec_value = m * val;
    }

    void computeDerivatives()
    {
        for(int i = 0; i < HEIGHT; i++) {
            for(int j = 0; j < WIDTH; j++) {

            }
        }
    }

protected:
    Matrix<HEIGHT, WIDTH> m;
    VectorOperation<WIDTH> *left_vec_op;

    Matrix<HEIGHT, WIDTH> dm;
    Vector<WIDTH> dleft;

};


#endif // VECTOR_OPERATIONS_H
