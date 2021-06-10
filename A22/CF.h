#ifndef GUARD_CF_h
#define GUARD_CF_h
// CF.h
#include <NTL/ZZX.h>
#include <iostream>
#include <string>

static NTL::ZZX phi(int p)
{
  using namespace NTL;
  Vec<ZZX> phi(INIT_SIZE, 100);  

  for (long i = 1; i <= 100; i++) {
    ZZX t;
    t = 1;

    for (long j = 1; j <= i-1; j++)
       if (i % j == 0)
          t *= phi(j);

    phi(i) = (ZZX(INIT_MONO, i) - 1)/t;  
  }
  return phi(p);
}

template <int p> class CF {
  typedef NTL::ZZX ZZX;
  typedef NTL::ZZX::coeff_type coeff_type;
public:
  CF() { num = ZZX(0); den = ZZX(1);}
  CF(coeff_type a) { num = ZZX(a); den = ZZX(1); }
  CF(long a) { num = ZZX(a); den = ZZX(1); }
  CF(const ZZX& f, const ZZX& g = ZZX(1)) { create(f, g); }
  CF(const std::string& s) { create(s); }
  ZZX getNum() const { return num; }
  ZZX getDen() const { return den; }
  CF operator+=(const CF& rhs);
  CF operator-=(const CF& rhs);
  CF operator*=(const CF& rhs);
  CF operator/=(const CF& rhs);
  static const ZZX MOD;
private:
  ZZX num;
  ZZX den;
  void create(const ZZX&, const ZZX&);
  void create(const std::string&);
  void normalize();
};

template <int p>
const NTL::ZZX CF<p>::MOD = phi(p);

template <int p>
void CF<p>::create(const NTL::ZZX& f, const NTL::ZZX& g)
{
  num = f % MOD;
  den = g % MOD;
}

template <int p>
void CF<p>::create(const std::string& s)
{
  if (s == "PRIM_ROOT_OF_UNITY") {
    num = ZZX(NTL::INIT_MONO, 1);
    den = ZZX(1);
  }
  else {
    num = ZZX(0);
    den = ZZX(0);
  }
}

template <int p>
void CF<p>::normalize()
{
  num %= MOD;
  den %= MOD;
  // 分母の有理化, 一般の場合が分からないため p = 6 の場合のみ有理化を実装する.
  if(p == 6 && deg(den) == 1) {
    coeff_type a = coeff(den, 0);
    coeff_type b = coeff(den, 1);
    ZZX f; // f = [a + b, -b] として den * f = [a^2 + a*b + b^2)].
    SetCoeff(f, 0, a + b);
    SetCoeff(f, 1, -b);
    num = (num * f) % MOD;
    clear(den);
    SetCoeff(den, 0, a * a + a * b + b * b);
  }

  const ZZX d = NTL::GCD(num, den);
  num /= d;
  den /= d;
  return;
}

template <int p>
CF<p> CF<p>::operator+=(const CF& rhs)
{
  num = num * rhs.den
      + den * rhs.num;
  den *= rhs.den;
  normalize();
  return *this;
}

template <int p>
CF<p> operator+(CF<p> lhs, const CF<p>& rhs)
{
  return lhs += rhs;
}

template <int p>
CF<p> CF<p>::operator-=(const CF& rhs)
{
  num = num * rhs.den
      - den * rhs.num;
  den *= rhs.den;
  normalize();
  return *this;
}

template <int p>
CF<p> operator-(CF<p> lhs, const CF<p>& rhs)
{
  return lhs -= rhs;
}

template <int p>
CF<p> CF<p>::operator*=(const CF<p>& rhs)
{
  num *= rhs.num;
  den *= rhs.den;
  normalize();
  return *this;
}

template <int p>
CF<p> operator*(CF<p> lhs, const CF<p>& rhs)
{
  return lhs *= rhs;
}

template <int p>
CF<p> CF<p>::operator/=(const CF<p>& rhs)
{
  num *= rhs.den;
  den *= rhs.num;
  normalize();
  return *this;
}

template <int p>
CF<p> operator/(CF<p> lhs, const CF<p>& rhs)
{
  return lhs /= rhs;
}

template <int p>
CF<p> operator^(CF<p> lhs, int a)
{
  if( a < 0) return (CF<p>(1) / lhs) ^ (-a);
  if(a == 0) return CF<p>(1);
  if(a % 2 == 0) {
    CF<p> tmp = lhs ^ (a / 2);
    return tmp * tmp;
  }
  return lhs * (lhs ^ (a - 1));
}

template <int p>
std::ostream& operator<<(std::ostream& os, const CF<p>& v)
{
  if (v.getDen() == NTL::ZZX(1))
    return os << v.getNum();
  return os << v.getNum() << "/" << v.getDen();
}
#endif
