# Crappy-NN

This is a shitty implementation of a Multilayer Perceptron neural network using a computational graph. It's written entirely in pure C++!

### Create a network like this:
```
Network n = Network();
    n.addLayer(5);
    n.addLayer(30);
    n.addLayer(30);
    n.addLayer(10);
    n.setup();
```
### Then run the computations and update the weights

```
n.run(x,y);
n.update(lr);
n.resetGradients();
```

### You can get the predictions vector like so:

vector<float> res = n.getPrediction();
