#ifndef GRAPH_H
#define GRAPH_H

#include "operations.h"
#include <vector>
#include <memory>
#include <stack>

class Graph
{
public:
    Graph() { }

    void addVar(Variable *var)
    {
        vars.push_back(var);

    }

    void addRPN(Operation *op) {
        if(op->type == (CONSTANT | VARIABLE)) {
            ops.push_back(op);
        }

        if(op->type == UNARY) {
            Operation *last = ops.back();
            ops.push_back(op);
            ops.back()->setFirst(last);
        }

        if(op->type == BINARY) {
            Operation *left = (*(ops.end() - 2));
            Operation *right = (*(ops.end() - 1));
            ops.push_back(op);
            ops.back()->setBoth(left, right);
        }
    }

    void run()
    {
        Operation *final_node = ops.back();
        final_node->compute();
        final_node->startGradient();
    }

    void clear() {
        for(Operation *op : ops) {
            delete op;
        }

        for(Variable *var : vars) {
            delete var;
        }
    }

    //friend void connect(DenseLayer &left, DenseLayer &right, Graph &g);
    std::vector<Operation*> ops;
    std::vector<Variable*> vars;
    Operation *final;
protected:
};


#endif // GRAPH_H
