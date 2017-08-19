#include "network.h"

Network::Network()
{

}

void connect(DenseLayer &left, DenseLayer &right, Graph &g)
{
    std::vector<Variable*> all_vars;
    std::vector<Operation*> all_ops;
    for(int i = 0; i < right.size; i++) {
        std::vector<Variable*> cur_neuron_vars;
        std::vector<Operation*> cur_muls;
        for(int j = 0; j < left.size; j++) {
            Variable *v = new Variable();
            v->name = std::to_string(j) + "->" + std::to_string(i);
            v->setUpdateRule(true);
            cur_neuron_vars.push_back(v);
            Multiply *m = new Multiply(left.neurons[j].value, v);
            m->name = "mul " + v->name;
            cur_muls.push_back(m);
        }

        auto sums = getSum(cur_muls);
        all_ops.insert(all_ops.end(), sums.begin(), sums.end());
        all_ops.insert(all_ops.end(), cur_muls.begin(), cur_muls.end());
        all_vars.insert(all_vars.end(), cur_neuron_vars.begin(), cur_neuron_vars.end());

        Variable *bias = new Variable();
        bias->setUpdateRule(true);
        Add *a = new Add(sums.back(), bias);
        all_ops.push_back(a);
        all_vars.push_back(bias);
        right[i].value = a;
    }

    //g.ops.insert(g.ops.end(), all_ops.begin(), all_ops.end());
    //g.vars.insert(g.vars.end(), all_vars.begin(), all_vars.end());
    extend(g.ops, all_ops);
    extend(g.vars, all_vars);
}

void createReluActivations(DenseLayer &d, Graph &g)
{
    std::vector<Operation*> all_ops;
    for(int i = 0; i < d.size; i++) {
        Relu *relu = new Relu(d[i].value);
        d[i].value = relu;
        all_ops.push_back(relu);
    }

    extend(g.ops, all_ops);
}

void createSoftmaxActivations(DenseLayer &d, Graph &g)
{
    //TODO
}

Operation* createMSELoss(DenseLayer &last, DenseLayer &true_vals, Graph &g)
{
    std::vector<Operation*> all_ops;

    std::vector<Operation*> pows;
    Operation *pow_value = new Variable(2);
    all_ops.push_back(pow_value);
    pow_value->setUpdateRule(false);
    for(int i = 0; i < last.size; i++) {
        Operation *dif = new Subtract(last[i].value, true_vals[i].value);
        Operation *power = new Exponential(dif, pow_value);

        all_ops.push_back(dif);
        all_ops.push_back(power);

        pows.push_back(power);
    }

    auto sums = getSum(pows);
    extend(all_ops, sums);
    extend(g.ops, all_ops);
    return sums.back();
}
