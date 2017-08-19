#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include "operations.h"

class Relu : public Operation
{
    // F(x) = max(x,0)
public:
    Relu() { type = UNARY; }
    Relu(Operation *op) : Operation(op) {
        type = UNARY;
        reset();
    }

    void computeDerivatives() {
        float val = left_op->getValue() ;
        derivatives[0] = (val > 0);
        derivatives[1] = 0;
    }

    void compute()
    {
        left_op->checkAndCompute();
        float val = left_op->getValue() ;
        cur_value = (val > 0) * val;
        computeDerivatives();
    }
};

#endif // ACTIVATIONS_H
