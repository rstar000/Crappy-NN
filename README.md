# Neural network in plain C++

This is an implementation of a Multilayer Perceptron neural network using a computational graph. It's written entirely in pure C++!

### Create a network like this:
```
Network n = Network();
    n.addLayer(5);
    n.addLayer(30);
    n.addLayer(30);
    n.addLayer(10);
    n.setup();
```
The first and last layers are input and output

### Then run the computations and update the weights

```
n.run(x,y);
n.update(lr);
n.resetGradients();
```

### You can get the predictions vector like so:

`vector<float> res = n.getPrediction();`
