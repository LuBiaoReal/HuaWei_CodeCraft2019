#include "iostream"
#include "Graph.h"
int main(int argc, char *argv[])
{
    if(argc < 5){
        std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
        exit(1);
    }

    std::string carPath(argv[1]);
    std::string roadPath(argv[2]);
    std::string crossPath(argv[3]);
    std::string answerPath(argv[4]);

    std::cout << "carPath is " << carPath << std::endl;
    std::cout << "roadPath is " << roadPath << std::endl;
    std::cout << "crossPath is " << crossPath << std::endl;
    std::cout << "answerPath is " << answerPath << std::endl;

    GraphLink Graph(roadPath,crossPath,carPath,answerPath);
    Graph.AnalogConsequence();
    Graph.TrafficRules();

	return 0;
}