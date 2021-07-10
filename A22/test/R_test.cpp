#include <gtest/gtest.h>
#include "../src/CF.h"
#include "../src/realization.h"

TEST(CyclotomicFieldTest, 6thRootOfUnityTest1)
{
  using F = CF<6>;
  const F w("PRIM_ROOT_OF_UNITY");

  F expect[6] = {F(1), w, w - F(1), F(-1), -w, F(1) - w};
  EXPECT_EQ(expect[1], w);
  EXPECT_EQ(expect[2], w * w);
  EXPECT_EQ(expect[3], w * w * w);
  EXPECT_EQ(expect[4], w * w * w * w);
  EXPECT_EQ(expect[5], w * w * w * w * w);
  EXPECT_EQ(expect[0], w * w * w * w * w * w);
}

TEST(CyclotomicFieldTest, 6thRootOfUnityTest2_pow)
{
  using F = CF<6>;
  const F w("PRIM_ROOT_OF_UNITY");

  F expect[6] = {F(1), w, w - F(1), F(-1), -w, F(1) - w};
  for(int i = 0; i < 100; ++i) {
    EXPECT_EQ(expect[i % 6], (w ^ i)); 
  }
}
