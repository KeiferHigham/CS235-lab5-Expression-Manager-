#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "ExpressionManager.h"


using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Please provide name of input and output files";
        return 1;
    }
    cout << "Input file: " << argv[1] << endl;
    ifstream in(argv[1]);
    if (!in) {
        cerr << "Unable to open " << argv[1] << " for input";
        return 2;
    }
    cout << "Output file: " << argv[2] << endl;
    ofstream out(argv[2]);
    if (!out) {
        in.close();
        cerr << "Unable to open " << argv[2] << " for output";
        return 3;
    }

         ExpressionManager set;
         string userLine = "";
        for(string userInput; getline(in, userInput);) {
            try {
                stringstream in(userInput);
                string inputExpression;
                string command;
                in >> command;
                getline(in, inputExpression);
                ExpressionManager add = ExpressionManager(userLine);
                if (command == "Expression:") {
                   userLine = inputExpression;
                    add = ExpressionManager(inputExpression);
                    out << "Expression:" << add.GetInitialExpression() << endl;

                } else if (command == "Infix:") {
                     cout << add.GetExpression();
                    string error;
                    out << "Infix:";
                    if(add.GetExpression() == add.infix()) {
                        out << add.GetExpression() << endl;
                    }
                    else { error = add.infix();
                    throw error;

                    }

                } else if (command == "Postfix:") {
                   out << command << add.postfix() << endl;
                } else if (command == "Prefix:") {
                       out << command << " " << add.prefix() << endl;
                } else if (command == "Value:") {
                    out  << command << " " << add.value() << endl;

                }
            }
            catch(string error)  {
              out << error << endl;
            }

            }


        ExpressionManager add;
        if(add.isBalenced("(3+4)")) {
            cout << "true";
        }


        return 0;
        }











