#include <iostream>
#include "realization.h"

using std::cout;  using std::endl;

int main()
{
  const F w("PRIM_ROOT_OF_UNITY");
  cout << CoxeterNum << "th RootOfUnity w = " << w << endl;
  cout << "CF<" << CoxeterNum
       << ">::MOD = " << CF<CoxeterNum>::MOD << endl;
  const H alpha1 = {w / (w - (w^ -1)), -(w^ -1) / (w - (w^ -1))};
  const H alpha2 = {F(-1) / (w - (w^ -1)), F(1) / (w - (w^ -1))};

  H a;
  S v;
  v.insert(make_pair(monomial({std::make_pair(a, -1)}), F(1)));
  cout << "A2 simple roots are followings:"
       << endl
       << "alpha1 = " << alpha1
       << endl
       << "alpha2 = " << alpha2
       << endl;
  cout << "a = " << a << endl;
  cout << "v = " << v << endl;
  cout << "X(a, -3) * v = " << X(a, -3) * v << endl;
  return 0;
}
