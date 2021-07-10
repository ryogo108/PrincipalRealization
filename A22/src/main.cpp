#include <iostream>
#include "realization.h"

using std::cout;  using std::endl;

int main()
{
  const F w("PRIM_ROOT_OF_UNITY");
  cout << CoxeterNum << "th RootOfUnity w = " << w << endl;
  cout << "CF<" << CoxeterNum
       << ">::MOD = " << CF<CoxeterNum>::MOD << endl;
  const H alpha1 = {(F(2) - w) / F(3),
                    (F(1) + w) / F(3)};
  const H alpha2 = {(F(-1) + F(2) * w) / F(3),
                    (F(1) - F(2) * w) / F(3)};
  cout << "A2 simple roots are followings:"
       << endl
       << "alpha1 = " << alpha1 << endl
       << endl
       << "alpha2 = " << alpha2 << endl
       << endl;

  H a = alpha1;
  S v;
  v.insert(std::make_pair(Monomial({Factor(alpha1, -1)}), F(1)));
  cout << "a = " << a << endl;
  cout << "v = " << v << endl;
  const Operators Xal1 = X(alpha1), Xal2 = X(alpha2), Xal12 = X(alpha1 + alpha2);
  cout << "[X(alpha1)[1], X(alpha1)[0]] * v = "
       << X(alpha1)[1] * (X(alpha1)[0] * v) - X(alpha1)[0] * (X(alpha1)[1] * v) << endl;
  cout << "X(alpha1 + alpha2)[1] * v = " << X(alpha1 + alpha2)[1] * v << endl;
  cout << "X(-alpha2)[1] * v = " << X(-alpha2)[1] * v << endl;
  cout << "Factor(alpha1, 1) * v = " << Factor(alpha1, 1) * v << endl;

  return 0;
}
