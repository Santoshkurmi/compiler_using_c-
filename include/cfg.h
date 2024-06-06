#ifndef Cfg
#define Cfg 

#include "vector"
#include "iostream"
#include <vector>

using namespace std;
class Production{
    public:
        Production();
        void setLeftSymbol(string);
        void addRightSymbol(string);
        void setRightSymbol(vector<string> rightSymbols);
        void printProduction();
        void clear();
    
        vector<string> rightSymbols;
        string leftSymbol;
};

class Rule{
    public:
        Rule();
        vector<Production> productions;
        void addProduction(string production);
        void addProduction(Production production);
        void print();

};

class Grammer{
    private:
        Rule rule;
    public:
        Grammer();
        void readFile(string fileName);
        void print();
        void execute();
        void execute(string fileName);
        void solveImmediate(Production &production);
        void solveNonImmediate();
        void solveLeftFactoring();


};

#endif 