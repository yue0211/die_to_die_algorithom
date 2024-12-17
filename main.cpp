#include <iostream>
#include <fstream>
#include <string>
#include "showQT.h"
#include "algorithm.h"

string input_file, output_file, design_rule_file;
int num_of_layers;

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file> <design_rule_file> <num_of_layers>" << endl;
        return 1;
    }

    input_file = argv[1];
    output_file = argv[2];
    design_rule_file = argv[3];
    num_of_layers = stoi(argv[4]);


    //todo: run algorithms and write the result to output file
    //--------------------------------------------------------------------------------
    // given the input file, design rule file and number of layers
    // run algorithms and write the result to output file
    //--------------------------------------------------------------------------------


    QApplication app(argc, argv); // create a Qt application
    QGraphicsScene scene; // create a scene
    scene.setSceneRect(0, 0, 5000, 2000); // set the scene size

    QtDraw(&scene, input_file, design_rule_file); // given the output file and draw the graphics on Qt

    QtView view(&scene); // create a view
    view.resize(1000, 1000); 
    view.centerOn(0, 0); 
    view.show(); 

    return app.exec();
}
