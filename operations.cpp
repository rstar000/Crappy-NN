#include "operations.h"

std::vector<Operation*> getSum(std::vector<Operation*> ops)
{
    std::vector<Operation*> sum_ops;
    Operation *cur_op = ops[0];
    for(int i = 1; i < ops.size(); i++) {
        Add *a = new Add(cur_op, ops[i]);
        sum_ops.push_back(a);
        cur_op = a;
    }
    return sum_ops;
}

std::default_random_engine Operation::generator = std::default_random_engine();
