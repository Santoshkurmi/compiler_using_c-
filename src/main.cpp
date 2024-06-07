#include <iostream>
#include "../include/cfg.h"

int main(int argc, char *argv[])
{
    Grammer grammer;
    grammer.execute("tests/cfg.txt");
    grammer.print();

    // Production production;
    // production.setLeftSymbol("A"); 
    // production.addRightSymbol("A a"); 
    // production.addRightSymbol("B a"); 
    // production.addRightSymbol("A a"); 
    // production.addRightSymbol("B a"); 
    // grammer.solveLeftFactoring(production);
    // grammer.print();

}
