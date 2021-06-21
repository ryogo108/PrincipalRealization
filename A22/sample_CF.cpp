#include <iostream>
#include "CF.h"

using std::cout; using std::endl;

const int p = 6;
using F = CF<p>;
const F w("PRIM_ROOT_OF_UNITY");

int main()
{
  cout << p << "th RootOfUnity w = " << w << endl;
  cout << "CF<" << p << ">::MOD = " << CF<p>::MOD << endl;
  cout << "(F(1) - (w ^ -1)) * ((F(1) - (w ^ -2)) ^ 3) * ((F(1) - (w ^ -3)) ^ 4) * ((F(1) - (w ^ -4)) ^ 3) * (F(1) - (w ^ -5)) = " 
       << (F(1) - (w ^ -1)) *
          ((F(1) - (w ^ -2)) ^ 3) *
          ((F(1) - (w ^ -3)) ^ 4) *
          ((F(1) - (w ^ -4)) ^ 3) *
          (F(1) - (w ^ -5))
       << endl;
  return 0;
}
