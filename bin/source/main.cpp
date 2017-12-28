#include "editor.h"
#include <QApplication>

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);
    Editor w;
    w.show();
    /*
    std::vector<unsigned> topology = {3 , 6 , 3, 1};
    neuralNet *net = new neuralNet(topology);

    std::vector<std::vector<double>> input = {{0.0 ,0.0 ,0.0}, {0.0,0.0,1.0},{0.0,1.0,0.0},{0.0,1.0,1.0},{1.0,0.0,0.0},{1.0,0.0,1.0},{1.0,1.0,0.0}};
    std::vector<std::vector<double>> target = {{0.0},{1.0},{0.0},{1.0},{0.0},{0.0},{0.0}};

    for(int j=0; j<100000; j++)
    {
    for(int i=0; i< input.size(); i++)
    {
        net->showVectorVals(": Inputs:", input[i]);
        net->feedForward(input[i]);

        std::vector<double> resultVals;

        net->getResults(resultVals);
        net->showVectorVals("Outputs:", resultVals);

        net->showVectorVals("Targets:", target[i]);
        assert(target[i].size() == topology.back());

        net->backProp(target[i]);

    }
    }

    std::vector<double> newInput = {1,1,1};
    net->showVectorVals(": Unknown Input:", newInput);
    net->feedForward(newInput);

    std::vector<double> resultVals;

    net->getResults(resultVals);
    net->showVectorVals("Outputs:", resultVals);
    */
    return a.exec();
}
