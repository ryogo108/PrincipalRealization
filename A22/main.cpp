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
                    (F(1) - F(-2) * w) / F(3)};
  cout << "A2 simple roots are followings:"
       << endl
       << "alpha1 = " << alpha1 << endl
       << "alpha1.proj(1) = " << alpha1.proj(1) << endl
       << "alpha1.proj(-5) = " << alpha1.proj(-5) << endl
       << "alpha1.proj(-1) = " << alpha1.proj(-1) << endl
       << "alpha1.proj(5) = " << alpha1.proj(5) << endl
       << "alpha1.proj(2) = " << alpha1.proj(2) << endl
       << endl
       << "alpha2 = " << alpha2 << endl
       << endl;

  H a = alpha1;
  S v;
  v.insert(std::make_pair(Monomial({Factor(a, -1),
                                    Factor(a, -5),
                                    Factor(a, -1),
                                    Factor(a, -7)}), F(1)));
  cout << "a = alpha1 = " << a << endl;
  cout << "v = " << v << endl;
  cout << "X(a, -3) * v = " << X(a, -3) * v << endl;
  return 0;
}
