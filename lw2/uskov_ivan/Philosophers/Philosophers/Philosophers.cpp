#include "stdafx.h"
#include <iostream>

#include "Table.h"

using namespace std;

int main()
{
    try
    {
        Table table(cout);
        table.plantPhilosophers();
        table.start();
    }
    catch (exception & e)
    {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}