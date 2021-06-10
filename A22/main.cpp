#include <iostream>
#include "realization.h"

using std::cout;  using std::endl;

int main()
{
  H a;
  S v;
  cout << "a = " << a << endl;
  cout << "v = " << v << endl;
  cout << "X(a, -3) * v = " << X(a, -3) * v << endl;
  return 0;
}
