#ifndef NETWORK_H
#define NETWORK_H

#include <math.h>
#include <vector>
#include "graph.h"
#include "activations.h"

template <typename T>
void extend(std::vector<T> &a, std::vector<T> &b) {
    a.insert(a.end(), b.begin(), b.end());
}

struct Neuron
{
    Neuron() { }
    Operation *value;
};

struct DenseLayer
{
    DenseLayer(int num = 0)
    {
        neurons.resize(num);
        size = num;
    }

    void setSize(int num)
    {
        neurons.resize(num);
        size = num;
    }

    std::vector<Neuron> neurons;
    int size;

    Neuron& operator[](int index)
    {
        return neurons[index];
    }

    void makeVariables(Graph &g, bool update_rule = false)
    {
        for(auto &neuron : neurons) {
            Variable *v = new Variable();
            v->setUpdateRule(update_rule);
            neuron.value = v;
            g.vars.push_back(v);
        }
    }

    void setValues(const std::vector<float> &new_values) {
        if(new_values.size() != size) {
            std::cout << "Wrong size!";
            return;
        }

        for(int i = 0; i < size; i++) {
            Operation *op = neurons[i].value;
            op->setValue(new_values[i]);
        }
    }
};

void connect(DenseLayer &left, DenseLayer &right, Graph &g);

void createReluActivations(DenseLayer &d, Graph &g);


Operation* createMSELoss(DenseLayer &last, DenseLayer &true_vals, Graph &g);

// High level neural network class
class Network : public Graph
{
public:
    Network();

    void addLayer(int n)
    {
        layers.emplace_back(n);
    }

    void setup() {
        layers[0].makeVariables(g);
        for(int i = 0; i < layers.size()-1; i++) {
            connect(layers[i], layers[i+1], g);
        }

        true_values.setSize(layers.back().size);
        true_values.makeVariables(g);

        loss = createMSELoss(layers.back(), true_values, g);
        gradients.resize(g.vars.size());
        std::fill(gradients.begin(), gradients.end(), 0);
    }

    void run(std::vector<float> x, std::vector<float> y) {
        layers[0].setValues(x);
        true_values.setValues(y);

        loss->compute();
        loss->startGradient();
    }

    float getLossValue()
    {
        return loss->getValue();
    }

    float update(float lr) {
        for(Variable *v : g.vars) {
            if(v->getUpdateRule()) {
                float grad = v->getGradient();
                v->cur_value -= grad * lr;
            }
        }
    }

    float resetGradients()
    {
        for(Variable *var : g.vars) {
            var->resetGradient();
        }

        for(Operation *op : g.ops) {
            op->resetGradient();
        }
    }

    std::vector<float> getPrediction()
    {
        std::vector<float> res;
        auto last = layers.back();

        for(Neuron &n : last.neurons) {
            res.push_back(n.value->getValue());
        }
        return res;
    }

private:
    std::vector<DenseLayer> layers;
    DenseLayer true_values;
    Operation *loss;
    std::vector<float> gradients;
    Graph g;
};

#endif // NETWORK_H
