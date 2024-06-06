#include <iostream>
#include "../include/cfg.h"

int main(int argc, char *argv[])
{
    Grammer grammer;
    grammer.execute("tests/cfg.txt");
    grammer.print();

}
