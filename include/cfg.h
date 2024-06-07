#ifndef Cfg
#define Cfg 

#include "vector"
#include "iostream"
#include <string>
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
        void sort();
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
        void solveNonImmediate(Production &productionCurrent,Production &previous);
        void solveLeftFactoring(Production production,int index);
        void addProductionAfterLeft(int originalLenght,int numberToAppend,string matched,int min,int max,int index);
        void test(string &str);
        void test2(string &str2);
    private:
        vector<string> splitSpacesToVector(string alternate);
        string convertToString(vector<string> alternate,int length);


};

#endif 