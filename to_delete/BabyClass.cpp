#include "BabyClass.h"
#include <iostream>
/*
using namespace std;

class BabyClass {
    int width, height;
  public:
    BabyClass (int,int);
    int area () {return (width*height);}
};

BabyClass::BabyClass (int a, int b) {
  width = a;
  height = b;
}
*/


BabyClass::BabyClass()
{
    foo = 42;
}
BabyClass::BabyClass(int bar,int bar2)
{
    foo = bar;
}

int BabyClass::getFoo()
{
    return foo;
}

