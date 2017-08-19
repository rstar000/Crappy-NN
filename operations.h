#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <linear.h>
#include <functional>
#include <vector>
#include <iostream>
#include <random>
#include <string>

//define INIT_RANDOM
//typedef Vector<1> Variable;

enum op_type {
    UNARY = 1,
    BINARY = 2,
    VARIABLE = 4,
    CONSTANT = 8,
    VEC = 16
};

class DummyOperation;

class Operation
{
public:
    Operation() : left_op(nullptr), right_op(nullptr)
    { reset(); }

    Operation(Operation *left_param, Operation *right_param = nullptr) {
        left_op = left_param;
        left_op->addConnection();
        type = UNARY;
        if(right_param != nullptr) {
            type = BINARY;
            right_op = right_param;
            right_op->addConnection();
        }
        reset();
    }

    Operation(const Operation &other) {
        outputs = other.outputs;
        num_gradients = other.num_gradients;
        needs_computation = other.needs_computation;
        cur_value = other.cur_value;
        cur_gradient = other.cur_gradient;
        left_op = other.left_op;
        right_op = other.right_op;
        derivatives[0] = other.derivatives[0];
        derivatives[1] = other.derivatives[1];
    }

    virtual ~Operation() { }

    float getValue()
    {
        return cur_value;
    }

    void setValue(float val) {
        cur_value = val;
    }

    float getDerivative(bool position) {
        return derivatives[int(position)];
    }

    float getGradient()
    {
        return cur_gradient;
    }

    //virtual void computeDerivatives() = 0;

    virtual void compute() = 0;

    void setCompute(bool new_needs_compute)
    {
        needs_computation = new_needs_compute;
    }

    void checkAndCompute()
    {
        if(needs_computation) {
            compute();
            needs_computation = false;
        }
    }

    void setParameter(bool position, Operation &new_op)
    {
        if(position == 0) left_op = &new_op;
        else right_op = &new_op;
    }

    void setFirst(Operation &new_left)
    {
        left_op = &new_left;
        left_op->addConnection();
    }

    void setFirst(Operation *new_left)
    {
        left_op = new_left;
        left_op->addConnection();
    }

    void setBoth(Operation &new_left, Operation &new_right)
    {
        left_op = &new_left;
        right_op = &new_right;
        left_op->addConnection();
        right_op->addConnection();
    }

    void setUpdateRule(bool new_needs_update)
    {
        needs_update = new_needs_update;
    }

    bool getUpdateRule() { return needs_update; }

    void setBoth(Operation *new_left, Operation *new_right)
    {
        left_op = new_left;
        right_op = new_right;
        left_op->addConnection();
        right_op->addConnection();
    }

    void addConnection() { outputs++; }

    int id;

    void reset()
    {
        needs_computation = true;
        cur_gradient = 0.0;
        outputs = 0;
        num_gradients = 0;
    }

    void gradient(float prev)
    {
        //std::cout << "New gradient! " << prev << std::endl;
        //std::cout << name << " " << left_op << " " << right_op << std::endl;
        cur_gradient += prev;
        num_gradients++;
        if(type & (CONSTANT | VARIABLE)) {
            //std::cout << "It's a VARIABLE! Not going further!" << std::endl;
            return;
        }

        if(num_gradients == outputs) {
            //std::cout << "GOING LEFT!" << std::endl;
            left_op->gradient(cur_gradient * derivatives[0]);
            if(type == BINARY && right_op) {
                //std::cout << "GOING RIGHT!" << std::endl;
                right_op->gradient(cur_gradient * derivatives[1]);
            }
        }
    }

    void startGradient()
    {
        outputs = 1;

        gradient(1.0);
    }

    void resetGradient() {
        cur_gradient = 0.0;
        num_gradients = 0;
        needs_computation = true;
    }

    op_type type;
    float cur_value;
    float cur_gradient;

    std::string name;

protected:
    int outputs;
    int num_gradients;
    bool needs_computation;
    bool needs_update;
    Operation *left_op;
    Operation *right_op;
    float derivatives[2];
    static std::default_random_engine generator;
private:
};

class DummyOperation : public Operation
{
public:
    DummyOperation() { }
    void computeDerivatives() {
        derivatives[0] = 0;
        derivatives[1] = 0;
    }
    void compute() { cur_value = 0; }
};

class Equals : public Operation
{
public:
    Equals(float val)
    {
        cur_value = val;
        type = CONSTANT;
        needs_computation = false;
        reset();
    }

    void computeDerivatives() {
        derivatives[0] = 0;
        derivatives[1] = 0;
    }

    void compute() { computeDerivatives(); }

private:

};

class Variable : public Operation
{
public:
    Variable(float val = 0.0) {
        type = VARIABLE;
        needs_computation = false;
        reset();
#ifdef INIT_RANDOM
        auto d = std::uniform_real_distribution<float>(0.0, 1.0);
        cur_value = d(Operation::generator);
#else
        cur_value = val;
#endif
        derivatives[0] = 0;
        derivatives[1] = 0;
    }

//    void computeDerivatives() {
//        derivatives[0] = 0;
//        derivatives[1] = 0;
//    }

    void compute()
    {
        //computeDerivatives();
    }
};

class Add : public Operation
{
public:
    Add() { type = BINARY; reset(); }
    Add(Operation *left_param, Operation *right_param) :
        Operation(left_param, right_param) { reset(); }

    void computeDerivatives() {
        derivatives[0] = 1;
        derivatives[1] = 1;
    }

    void compute()
    {
        left_op->checkAndCompute();
        right_op->checkAndCompute();
        cur_value = left_op->getValue() + right_op->getValue();
        computeDerivatives();
    }
};


class Multiply : public Operation
{
public:
    Multiply() { type = BINARY; reset(); }
    Multiply(Operation *left_param, Operation *right_param) :
        Operation(left_param, right_param) { reset(); }

    void computeDerivatives() {
        derivatives[0] = right_op->getValue();
        derivatives[1] = left_op->getValue();
    }

    void compute()
    {
        left_op->checkAndCompute();
        right_op->checkAndCompute();
        cur_value = left_op->getValue() * right_op->getValue();
        computeDerivatives();
    }
};


class Subtract : public Operation
{
public:
    Subtract() { type = BINARY; reset(); }
    Subtract(Operation *left_param, Operation *right_param) :
        Operation(left_param, right_param) { reset(); }

    void computeDerivatives() {
        derivatives[0] = 1;
        derivatives[1] = -1;
    }

    void compute()
    {
        left_op->checkAndCompute();
        right_op->checkAndCompute();
        cur_value = left_op->getValue() - right_op->getValue();
        computeDerivatives();
    }
};


class OneOverX : public Operation
{
public:
    OneOverX() { type = UNARY; }

    void computeDerivatives() {
        float t_val = left_op->getValue();
        derivatives[0] = -1/(t_val * t_val);
    }

    void compute()
    {
        left_op->checkAndCompute();
        cur_value = 1 / left_op->getValue();
        computeDerivatives();
    }
};


class Power : public Operation
{
public:
    Power(float p) : power(p)  { type = UNARY; }
    Power(float p, Operation &left_param) : power(p)
    {
        type = UNARY;
        left_op = &left_param;
    }

    void computeDerivatives() {
        float t_val = left_op->getValue();
        derivatives[0] =  power * pow(t_val, power - 1);
    }

    void compute()
    {
        left_op->checkAndCompute();
        cur_value = pow(left_op->getValue(), power);
    }

    float power;
};

class Exponential : public Operation
{
public:
    Exponential() { type = BINARY; reset(); }

    Exponential(Operation *left_param, Operation *right_param) :
        Operation(left_param, right_param) { reset(); }


    void computeDerivatives() {
        float t_val = left_op->getValue();
        float exp_val = right_op->getValue();
        derivatives[0] = exp_val * pow(t_val, exp_val-1);
        derivatives[1] = log(t_val) * pow(t_val, exp_val);
    }

    void compute()
    {
        left_op->checkAndCompute();
        right_op->checkAndCompute();
        cur_value = pow(left_op->getValue(), right_op->getValue());
        computeDerivatives();
    }
};

std::vector<Operation*> getSum(std::vector<Operation*> ops);

#endif
