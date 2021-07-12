#include <gtest/gtest.h>
#include "../src/CF.h"
#include "../src/realization.h"

using std::make_pair;

TEST(CyclotomicFieldTest, 6thRootOfUnityTest1)
{
  using F_test = CF<6>;
  const F_test w("PRIM_ROOT_OF_UNITY");

  F_test expect[6] = {F_test(1), w, w - F_test(1), F_test(-1), -w, F_test(1) - w};
  EXPECT_EQ(expect[1], w);
  EXPECT_EQ(expect[2], w * w);
  EXPECT_EQ(expect[3], w * w * w);
  EXPECT_EQ(expect[4], w * w * w * w);
  EXPECT_EQ(expect[5], w * w * w * w * w);
  EXPECT_EQ(expect[0], w * w * w * w * w * w);
}

TEST(CyclotomicFieldTest, 6thRootOfUnityTest2_pow)
{
  using F_test = CF<6>;
  const F_test w("PRIM_ROOT_OF_UNITY");

  F_test expect[6] = {F_test(1), w, w - F_test(1), F_test(-1), -w, F_test(1) - w};
  for(int i = 0; i < 100; ++i) {
    EXPECT_EQ(expect[i % 6], (w ^ i)); 
  }
}

TEST(CyclotomicFieldTest, 6thRootOfUnityTest3_ZZ)
{
  using F_test = CF<6>;
  using ZZ = F_test::coeff_type;
  const F_test w("PRIM_ROOT_OF_UNITY");

  ZZ expect1 = ZZ(1);
  ZZ expect2 = ZZ(2);
  EXPECT_EQ(expect1, ZZ(F(1)));
  EXPECT_EQ(expect1, ZZ(w / w));
  EXPECT_EQ(expect2, ZZ((F(2) * (w ^ 2)) / (w ^ 2)));
  EXPECT_EQ(expect2, ZZ((F(2) * (w ^ 2)) / (w ^ 2)));
}

TEST(RealizationTest, HeisenbergTest1_Addition)
{
  const F w("PRIM_ROOT_OF_UNITY");
  H expect1 = H(F(1), F(1));
  H expect2 = H(F(1) + F(2) * w, F(1) + (w ^ 2));
  EXPECT_EQ(expect1, H(F(1), F(0)) + H(F(0), F(1)));
  EXPECT_EQ(expect2, H(F(1) + w, F(1)) + H(w, (w ^ 2)));
}

TEST(RealizationTest, HeisenbergTest2_Minus)
{
  const F w("PRIM_ROOT_OF_UNITY");
  H expect1 = H(-F(1), -F(1));
  H expect2 = H(-(F(1) + F(2) * w), -(F(1) + (w ^ 2)));
  EXPECT_EQ(expect1, -H(F(1), F(1)));
  EXPECT_EQ(expect2, -H(F(1) + F(2) * w, F(1) + (w ^ 2)));
}

TEST(RealizationTest, HeisenbergTest3_projection)
{
  const F w("PRIM_ROOT_OF_UNITY");
  H expect0 = H(F(0), F(0));
  H expect1_1 = H(F(1), F(0));
  H expect1_5 = H(F(0), F(1));
  H expect2_1 = H(F(1) + F(2) * w, F(0));
  H expect2_5 = H(F(0), F(1) + (w ^ 2));
  H h1 = H(F(1), F(1));
  H h2 = H(F(1) + F(2) * w, F(1) + (w ^ 2));
  EXPECT_EQ(expect1_1, h1.proj(1));
  EXPECT_EQ(expect0, h1.proj(2));
  EXPECT_EQ(expect0, h1.proj(3));
  EXPECT_EQ(expect0, h1.proj(4));
  EXPECT_EQ(expect1_5, h1.proj(5));
  EXPECT_EQ(expect0, h1.proj(6));

  EXPECT_EQ(expect2_1, h2.proj(1));
  EXPECT_EQ(expect0, h2.proj(2));
  EXPECT_EQ(expect0, h2.proj(3));
  EXPECT_EQ(expect0, h2.proj(4));
  EXPECT_EQ(expect2_5, h2.proj(5));
  EXPECT_EQ(expect0, h2.proj(6));
}

TEST(RealizationTest, HeisenbergTest3_getProjectedValue)
{
  const F w("PRIM_ROOT_OF_UNITY");
  F expect0 = F(0);
  F expect1_1 = F(1);
  F expect1_5 = F(1);
  F expect2_1 = F(1) + F(2) * w;
  F expect2_5 = F(1) + (w ^ 2);
  H h1 = H(F(1), F(1));
  H h2 = H(F(1) + F(2) * w, F(1) + (w ^ 2));
  EXPECT_EQ(expect1_1, h1.getProjVal(1));
  EXPECT_EQ(expect0, h1.getProjVal(2));
  EXPECT_EQ(expect0, h1.getProjVal(3));
  EXPECT_EQ(expect0, h1.getProjVal(4));
  EXPECT_EQ(expect1_5, h1.getProjVal(5));
  EXPECT_EQ(expect0, h1.getProjVal(6));

  EXPECT_EQ(expect2_1, h2.getProjVal(1));
  EXPECT_EQ(expect0, h2.getProjVal(2));
  EXPECT_EQ(expect0, h2.getProjVal(3));
  EXPECT_EQ(expect0, h2.getProjVal(4));
  EXPECT_EQ(expect2_5, h2.getProjVal(5));
  EXPECT_EQ(expect0, h2.getProjVal(6));
}

TEST(RealizationTest, FactorTest1_projection)
{
  using std::make_pair;
  const F w("PRIM_ROOT_OF_UNITY");
  Factor expect1_1 = make_pair(H(F(1), F(0)), 1);
  Factor expect1_2 = make_pair(H(F(0), F(0)), 2);
  Factor expect1_3 = make_pair(H(F(0), F(0)), 3);
  Factor expect1_4 = make_pair(H(F(0), F(0)), 4);
  Factor expect1_5 = make_pair(H(F(0), F(1)), 5);
  Factor expect1_6 = make_pair(H(F(0), F(0)), 6);

  Factor expect2_1 = make_pair(H(F(1) + F(2) * w, F(0)), 1);
  Factor expect2_2 = make_pair(H(F(0), F(0)), 2);
  Factor expect2_3 = make_pair(H(F(0), F(0)), 3);
  Factor expect2_4 = make_pair(H(F(0), F(0)), 4);
  Factor expect2_5 = make_pair(H(F(0), F(1) + (w ^ 2)), 5);
  Factor expect2_6 = make_pair(H(F(0), F(0)), 6);

  Factor f1_1 = make_pair(H(F(1), F(1)), 1);
  Factor f1_2 = make_pair(H(F(1), F(1)), 2);
  Factor f1_3 = make_pair(H(F(1), F(1)), 3);
  Factor f1_4 = make_pair(H(F(1), F(1)), 4);
  Factor f1_5 = make_pair(H(F(1), F(1)), 5);
  Factor f1_6 = make_pair(H(F(1), F(1)), 6);

  Factor f2_1 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 1);
  Factor f2_2 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 2);
  Factor f2_3 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 3);
  Factor f2_4 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 4);
  Factor f2_5 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 5);
  Factor f2_6 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 6);

  EXPECT_EQ(expect1_1, proj(f1_1));
  EXPECT_EQ(expect1_2, proj(f1_2));
  EXPECT_EQ(expect1_3, proj(f1_3));
  EXPECT_EQ(expect1_4, proj(f1_4));
  EXPECT_EQ(expect1_5, proj(f1_5));
  EXPECT_EQ(expect1_6, proj(f1_6));

  EXPECT_EQ(expect2_1, proj(f2_1));
  EXPECT_EQ(expect2_2, proj(f2_2));
  EXPECT_EQ(expect2_3, proj(f2_3));
  EXPECT_EQ(expect2_4, proj(f2_4));
  EXPECT_EQ(expect2_5, proj(f2_5));
  EXPECT_EQ(expect2_6, proj(f2_6));
}

TEST(RealizationTest, FactorTest1_getCoefficient)
{
  using std::make_pair;
  const F w("PRIM_ROOT_OF_UNITY");
  F expect1_1 = F(1);
  F expect1_2 = F(0);
  F expect1_3 = F(0);
  F expect1_4 = F(0);
  F expect1_5 = F(1);
  F expect1_6 = F(0);

  F expect2_1 = F(1) + F(2) * w;
  F expect2_2 = F(0);
  F expect2_3 = F(0);
  F expect2_4 = F(0);
  F expect2_5 = F(1) + (w ^ 2);
  F expect2_6 = F(0);

  Factor f1_1 = make_pair(H(F(1), F(1)), 1);
  Factor f1_2 = make_pair(H(F(1), F(1)), 2);
  Factor f1_3 = make_pair(H(F(1), F(1)), 3);
  Factor f1_4 = make_pair(H(F(1), F(1)), 4);
  Factor f1_5 = make_pair(H(F(1), F(1)), 5);
  Factor f1_6 = make_pair(H(F(1), F(1)), 6);

  Factor f2_1 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 1);
  Factor f2_2 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 2);
  Factor f2_3 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 3);
  Factor f2_4 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 4);
  Factor f2_5 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 5);
  Factor f2_6 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 6);

  EXPECT_EQ(expect1_1, getCoeff(f1_1));
  EXPECT_EQ(expect1_2, getCoeff(f1_2));
  EXPECT_EQ(expect1_3, getCoeff(f1_3));
  EXPECT_EQ(expect1_4, getCoeff(f1_4));
  EXPECT_EQ(expect1_5, getCoeff(f1_5));
  EXPECT_EQ(expect1_6, getCoeff(f1_6));

  EXPECT_EQ(expect2_1, getCoeff(f2_1));
  EXPECT_EQ(expect2_2, getCoeff(f2_2));
  EXPECT_EQ(expect2_3, getCoeff(f2_3));
  EXPECT_EQ(expect2_4, getCoeff(f2_4));
  EXPECT_EQ(expect2_5, getCoeff(f2_5));
  EXPECT_EQ(expect2_6, getCoeff(f2_6));
}

TEST(RealizationTest, FactorTest1_unifyCoefficient)
{
  const F w("PRIM_ROOT_OF_UNITY");
  Factor expect1_1 = make_pair(H(F(1), F(0)), 1);
  Factor expect1_2 = make_pair(H(F(0), F(0)), 2);
  Factor expect1_3 = make_pair(H(F(0), F(0)), 3);
  Factor expect1_4 = make_pair(H(F(0), F(0)), 4);
  Factor expect1_5 = make_pair(H(F(0), F(1)), 5);
  Factor expect1_6 = make_pair(H(F(0), F(0)), 6);

  Factor expect2_1 = make_pair(H(F(1), F(0)), 1);
  Factor expect2_2 = make_pair(H(F(0), F(0)), 2);
  Factor expect2_3 = make_pair(H(F(0), F(0)), 3);
  Factor expect2_4 = make_pair(H(F(0), F(0)), 4);
  Factor expect2_5 = make_pair(H(F(0), F(1)), 5);
  Factor expect2_6 = make_pair(H(F(0), F(0)), 6);

  Factor f1_1 = make_pair(H(F(1), F(1)), 1);
  Factor f1_2 = make_pair(H(F(1), F(1)), 2);
  Factor f1_3 = make_pair(H(F(1), F(1)), 3);
  Factor f1_4 = make_pair(H(F(1), F(1)), 4);
  Factor f1_5 = make_pair(H(F(1), F(1)), 5);
  Factor f1_6 = make_pair(H(F(1), F(1)), 6);

  Factor f2_1 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 1);
  Factor f2_2 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 2);
  Factor f2_3 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 3);
  Factor f2_4 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 4);
  Factor f2_5 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 5);
  Factor f2_6 = make_pair(H(F(1) + F(2) * w, F(1) + (w ^ 2)), 6);

  EXPECT_EQ(expect1_1, unifyCoeff(f1_1));
  EXPECT_EQ(expect1_2, unifyCoeff(f1_2));
  EXPECT_EQ(expect1_3, unifyCoeff(f1_3));
  EXPECT_EQ(expect1_4, unifyCoeff(f1_4));
  EXPECT_EQ(expect1_5, unifyCoeff(f1_5));
  EXPECT_EQ(expect1_6, unifyCoeff(f1_6));

  EXPECT_EQ(expect2_1, unifyCoeff(f2_1));
  EXPECT_EQ(expect2_2, unifyCoeff(f2_2));
  EXPECT_EQ(expect2_3, unifyCoeff(f2_3));
  EXPECT_EQ(expect2_4, unifyCoeff(f2_4));
  EXPECT_EQ(expect2_5, unifyCoeff(f2_5));
  EXPECT_EQ(expect2_6, unifyCoeff(f2_6));
}

TEST(RealizationTest, appendTest)
{
  const F w("PRIM_ROOT_OF_UNITY");
  const H alpha1 = {(F(2) - w) / F(3),
                    (F(1) + w) / F(3)};
  S expect1;
  expect1.insert(S::Term(Monomial({Factor(alpha1, -1)}), F(1)));
  S expect2;
  expect2.insert(S::Term(Monomial({Factor(alpha1, -1), Factor(alpha1, -1)}), F(1)));
  S v;
  v.insert(S::Term(Monomial(), F(1)));
  EXPECT_EQ(expect1, Factor(alpha1, -1) * v);
  EXPECT_EQ(expect2, Factor(alpha1, -1) * (Factor(alpha1, -1) * v));
}

TEST(RealizationTest, derivationTest)
{
  const F w("PRIM_ROOT_OF_UNITY");
  const H alpha1 = {(F(2) - w) / F(3),
                    (F(1) + w) / F(3)};
  S expect1;
  expect1.insert(S::Term(Monomial(), F(1) / F(6)));
  S expect2;
  expect2.insert(S::Term(Monomial({Factor(alpha1, -1)}), F(1) / F(3)));
  S v1;
  v1.insert(S::Term(Monomial({Factor(alpha1, -1)}), F(1)));
  S v2;
  v2.insert(S::Term(Monomial({Factor(alpha1, -1), Factor(alpha1, -1)}), F(1)));
  EXPECT_EQ(expect1, Factor(alpha1, 1) * v1);
  EXPECT_EQ(expect2, Factor(alpha1, 1) * v2);
}

TEST(RealizationTest, straightenTest)
{
  const F w("PRIM_ROOT_OF_UNITY");
  const H alpha1 = {(F(2) - w) / F(3),
                    (F(1) + w) / F(3)};
  Actions expect1;
  expect1.insert(Actions::Term(Action({Factor(alpha1, -1), Factor(alpha1, 1)}), F(1)));
  expect1.insert(Actions::Term(Action(), F(1) / F(6)));
  Actions expect2;
  expect2.insert(Actions::Term(Action({Factor(alpha1, -1), Factor(alpha1, 1)}), F(1)));
  Actions expect3;
  expect3.insert(Actions::Term(Action({Factor(alpha1, -1)}), F(1) / F(3)));
  expect3.insert(Actions::Term(Action({Factor(alpha1, -1), Factor(alpha1, -1),
                                       Factor(alpha1, 1)}), F(1)));
  Actions expect4;
  expect4.insert(Actions::Term(Action({Factor(alpha1, -1), Factor(alpha1, -1), Factor(alpha1, 1)}), F(1)));
  Action v1 = Action({Factor(alpha1, 1), Factor(alpha1, -1)});
  Action v2 = Action({Factor(alpha1, -1), Factor(alpha1, 1)});
  Action v3 = Action({Factor(alpha1, 1), Factor(alpha1, -1), Factor(alpha1, -1)});
  Action v4 = Action({Factor(alpha1, -1), Factor(alpha1, -1), Factor(alpha1, 1)});
  EXPECT_EQ(expect1, straighten(v1));
  EXPECT_EQ(expect2, straighten(v2));
  EXPECT_EQ(expect3, straighten(v3));
  EXPECT_EQ(expect4, straighten(v4));
}

TEST(RealizationTest, TestE_minus1)
{
  const F w("PRIM_ROOT_OF_UNITY");
  const H alpha1 = {(F(2) - w) / F(3),
                    (F(1) + w) / F(3)};
  S expect1;
  expect1.insert(S::Term(Monomial({Factor(alpha1, -1)}), F(-6)));
  S expect2;
  expect2.insert(S::Term(Monomial({Factor(alpha1, -1), Factor(alpha1, -1)}), F(18)));
  S expect3;
  expect3.insert(S::Term(Monomial({Factor(alpha1, -1), Factor(alpha1, -1)}), F(36)));
  S v;
  v.insert(S::Term(Monomial(), F(1)));
  EXPECT_EQ(expect1, E_minus(alpha1)[1]* v);
  EXPECT_EQ(expect2, E_minus(alpha1)[2]* v);
  EXPECT_EQ(expect3, E_minus(alpha1)[1] * (E_minus(alpha1)[1]* v));
  EXPECT_EQ((E_minus(alpha1)[1] * E_minus(alpha1)[1]) * v,
            E_minus(alpha1)[1] * (E_minus(alpha1)[1]* v));
}

TEST(RealizationTest, TestE_plus1)
{
  const F w("PRIM_ROOT_OF_UNITY");
  const H alpha1 = {(F(2) - w) / F(3),
                    (F(1) + w) / F(3)};
  S expect1;
  expect1.insert(S::Term(Monomial(), F(1)));
  S expect2;
  expect2.insert(S::Term(Monomial(), F(2)));
  S v1;
  v1.insert(S::Term(Monomial({Factor(alpha1, -1)}), F(1)));
  S v2;
  v2.insert(S::Term(Monomial({Factor(alpha1, -1), Factor(alpha1, -1)}), F(1)));
  EXPECT_EQ(expect1, E_plus(alpha1)[-1] * v1);
  EXPECT_EQ(expect2, E_plus(alpha1)[-1] * (E_plus(alpha1)[-1]* v2));
  EXPECT_EQ(expect1, E_plus(alpha1)[-2]* v2);
  EXPECT_EQ((E_plus(alpha1)[-1] * E_plus(alpha1)[-1]) * v2,
            E_plus(alpha1)[-1] * (E_plus(alpha1)[-1]* v2));
}

TEST(RealizationTest, TestX1)
{
  const F w("PRIM_ROOT_OF_UNITY");
  const H alpha1 = {(F(2) - w) / F(3),
                    (F(1) + w) / F(3)};
  S expect1;
  expect1.insert(S::Term(Monomial({Factor(alpha1, -1)}), (F(1) + w) / F(72)));
  S expect2;
  expect2.insert(S::Term(Monomial({Factor(alpha1, -1), Factor(alpha1, -1)}),
                         -w / F(864)));
  S v;
  v.insert(S::Term(Monomial(), F(1)));
  EXPECT_EQ(expect1, X(alpha1, -1) * v);
  EXPECT_EQ(expect2, X(alpha1, -1) * (X(alpha1, -1) * v));
  EXPECT_EQ((X(alpha1, -1) * X(alpha1, -1)) * v,
            X(alpha1, -1) * (X(alpha1, -1) * v));
}

TEST(RealizationTest, TestNu)
{
  const F w("PRIM_ROOT_OF_UNITY");
  const H alpha1 = {(F(2) - w) / F(3),
                    (F(1) + w) / F(3)};
  const H alpha2 = {(F(-1) + F(2) * w) / F(3),
                    (F(1) - F(2) * w) / F(3)};
  EXPECT_EQ(alpha1, nu(-6, alpha1));
  EXPECT_EQ(alpha1 + alpha2, nu(-5, alpha1));
  EXPECT_EQ(alpha2, nu(-4, alpha1));
  EXPECT_EQ(-alpha1, nu(-3, alpha1));
  EXPECT_EQ(-alpha1 - alpha2, nu(-2, alpha1));
  EXPECT_EQ(-alpha2, nu(-1, alpha1));
  EXPECT_EQ(alpha1, nu(0, alpha1));
  EXPECT_EQ(alpha1 + alpha2, nu(1, alpha1));
  EXPECT_EQ(alpha2, nu(2, alpha1));
  EXPECT_EQ(-alpha1, nu(3, alpha1));
  EXPECT_EQ(-alpha1 - alpha2, nu(4, alpha1));
  EXPECT_EQ(-alpha2, nu(5, alpha1));
  EXPECT_EQ(alpha1, nu(6, alpha1));

  EXPECT_EQ(-alpha1, nu(1, alpha2));
}

TEST(RealizationTest, TestEpsilon_2)
{
  const F w("PRIM_ROOT_OF_UNITY");
  const H alpha1 = {(F(2) - w) / F(3),
                    (F(1) + w) / F(3)};
  F expect = (w ^ -1);
  EXPECT_EQ(expect, epsilon_2(alpha1, alpha1));
}

TEST(RealizationTest, TestComX1)
{
  const F w("PRIM_ROOT_OF_UNITY");
  const H alpha1 = {(F(2) - w) / F(3),
                    (F(1) + w) / F(3)};
  Actions expect = X(alpha1, -1) * X(alpha1, -1) - X(alpha1, -1) * X(alpha1, -1);
  EXPECT_EQ(expect, comX(alpha1, -1, -1));
}
