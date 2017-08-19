#include <iostream>
#include <math.h>
#include "network.h"

int main()
{
    Network n = Network();
    n.addLayer(5);
    n.addLayer(30);
    n.addLayer(30);
    n.addLayer(10);
    n.setup();

    std::cout << "Created network" << std::endl;
    std::vector<float> x = {0.1,0.5,0.4,0.2,0.7};
    std::vector<float> y = {0.3,0.3,0.1,0.05,0.5,0.0,0.7,0.2,0,7};

    float lr = 0.05;
    for(int i = 0; i < 100; i++) {
        n.run(x,y);
        std::cout << "Epoch " << i << ", ";
        std::cout << "Loss = " << n.getLossValue() << std::endl;
        n.update(lr);
        n.resetGradients();
    }

    auto res = n.getPrediction();
    for(float x : res) {
        std::cout << x << " ";
    }

    std::vector<float> new_x = {0.8,0.4,0.1,0.9,0.2};

    n.run(new_x,y);
    std::cout << "New loss = " << n.getLossValue() << std::endl;

    auto res2 = n.getPrediction();
    for(float x : res2) {
        std::cout << x << " ";
    }

    return 0;
}
