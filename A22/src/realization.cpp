#include <iostream>
#include <deque>
#include <utility>
#include <algorithm>
#include "realization.h"

using std::ostream;
using std::make_pair;

ostream& operator<< (ostream& os, const H& a)
{
  H::value_type val = a.getVal();
  return os << "(" << val[0] << ", " << val[1] << ")";
}

ostream& operator<< (ostream& os, const Factor& f)
{
  return os << f.first << "(" << f.second << ")";
}

ostream& operator<< (ostream& os, const Core& c)
{
  os << "[";
  for(Core::const_iterator iter = c.begin();
      iter != c.end(); ++iter) {
    if(iter != c.begin()) os << ", ";
    os << iter -> first
       << "("
       << iter ->second
       << ")";
  }
  return os << "]";
}

ostream& operator<< (ostream& os, const Monomial& m)
{
  os << "[";
  for(Core::const_iterator iter = m.begin();
      iter != m.end(); ++iter) {
    if(iter != m.begin()) os << ", ";
    os << iter -> first
       << "("
       << iter ->second
       << ")";
  }
  return os << "]";
}

ostream& operator<< (ostream& os, const S& v)
{
  using std::endl;
  for(S::const_iterator iter = v.begin();
      iter != v.end(); ++iter) {
    os << endl;
    if(iter != v.begin()) os << "+ ";
    os << "["
       << iter -> second      // coefficient
       << " * "
       << iter -> first       // Monomial
       << "]";
  }
  return os << endl;
}

H& H::operator+=(const H& rhs)
{
  val[0] += rhs.val[0];
  val[1] += rhs.val[1];
  return *this;
}

H H::operator-() const
{
  return H(-val[0], -val[1]);
}

H operator+(H lhs, const H& rhs)
{
  return lhs += rhs;
}

H operator-(H lhs, const H& rhs)
{
  return lhs += -rhs;
}

H operator*(const F& lhs, const H& rhs)
{
  const H::value_type& v = rhs.getVal();
  return H(lhs * v[0], lhs * v[1]) ;
}

bool operator== (const H& lhs, const H& rhs)
{
  return lhs.getVal() == rhs.getVal();
}

bool operator< (const Core& lhs, const Core& rhs)
{
  if(lhs.size() != rhs.size()) return lhs.size() < rhs.size();
  Core::const_iterator it1 = lhs.begin();
  Core::const_iterator it2 = rhs.begin();
  while(it1 != lhs.end()) {
    if(it1 -> second != it2 -> second)
      return it1 -> second < it2 -> second;
    it1++;
    it2++;
  }
  return false;
}

H H::proj(int n) const
{
  if((n - 1) % CoxeterNum == 0)
    return H(val[0], F(0));
  if((n + 1) % CoxeterNum == 0)
    return H(F(0), val[1]);
  return H(F(0), F(0));
}

F H::getProjVal(int n) const
{
  if((n - 1) % CoxeterNum == 0)
    return val[0];
  if((n + 1) % CoxeterNum == 0)
    return val[1];
  return F(0);
}

Factor proj(const Factor& f)
{
  Factor ret = Factor(f.first.proj(f.second), f.second);
  return ret;
}

F getCoeff(const Factor& f)
{
  return f.first.getProjVal(f.second);
}

Factor unifyCoeff(const Factor& f)
{
  if((f.second - 1) % 6 == 0)
    return Factor(H(F(1), F(0)), f.second);
  if((f.second + 1) % 6 == 0)
    return Factor(H(F(0), F(1)), f.second);
  return Factor(H(F(0), F(0)), f.second);

}

void Core::proj()
{
  for(value_type::iterator iter = val.begin();
      iter != val.end(); ++iter) {
    *iter = make_pair((iter -> first).proj(iter -> second), iter -> second);
  }
}

Core::const_iterator Core::erase(const_iterator& it)
{
  return val.erase(it);
}

F Core::getCoeff() const
{
  F ret = F(1);
  for(Core::const_iterator iter = val.begin();
      iter != val.end(); ++iter) {
    ret *= (iter -> first).getProjVal(iter -> second);
  }
  return ret;
}

void Core::unifyCoeff()
{
  for(value_type::iterator iter = val.begin();
      iter != val.end(); ++iter) {
    if((iter -> second - 1) % 6 == 0) {
      (iter -> first) = H(F(1), F(0));
      continue;
    }

    if((iter -> second + 1) % 6 == 0) {
      (iter -> first) = H(F(0), F(1));
      continue;
    }

    (iter -> first) = H(F(0), F(0));
  }
}

std::pair<S::iterator, bool> S::insert(Term x)
{
  auto ret = val.insert(unify(x));
  return ret;
}

S::Term& S::unify(Term& x)
{
  x.second *= x.first.getCoeff();
  x.first.unifyCoeff();
  return x;
}

S& S::operator+=(const S& v)
{
  for(S::const_iterator iter = v.begin();
      iter != v.end(); ++iter) {
    value_type::iterator it = val.find(iter -> first);
    if(it != val.end()) {
      it -> second += iter -> second;
    }
    else
      val.insert(*iter);
  }

  omit();

  return *this;
}

void S::omit()
{
  for(value_type::const_iterator iter = val.begin();
      iter != val.end();) {
    if(iter -> second == F(0))
      iter = val.erase(iter);
    else
      ++iter;
  }
}

bool operator==(const S& lhs, const S& rhs)
{
  if(lhs.size() != rhs.size()) return false;
  for(auto iter = lhs.begin();
      iter != lhs.end(); ++iter) {
    auto target = rhs.find(iter -> first);
    if(target == rhs.end()) return false;
    if(iter -> second != target -> second) return false;
  }
  return true;
}

bool operator==(const Actions& lhs, const Actions& rhs)
{
  if(lhs.size() != rhs.size()) return false;
  for(auto iter = lhs.begin();
      iter != lhs.end(); ++iter) {
    auto target = rhs.find(iter -> first);
    if(target == rhs.end()) return false;
    if(iter -> second != target -> second) return false;
  }
  return true;
}

bool isIncluded(const Actions& sub, const Actions& whole)
{
  for(auto iter = sub.begin();
      iter != sub.end(); ++iter) {
    auto target = whole.find(iter -> first);
    if(target == whole.end()) return false;
    if(iter -> second != target -> second) return false;
  }
  return true;
}

S& S::operator-=(const S& v)
{
  return *this += (F(-1) * v);
}

S operator+(S lhs, const S& rhs)
{
  return lhs += rhs;
}

S operator-(S lhs, const S& rhs)
{
  return lhs -= rhs;
}

S operator*(const Action& a, const S& v)
{
  S ret = v;
  for(Action::const_reverse_iterator riter = a.rbegin();
      riter != a.rend(); ++riter) {
    ret = (*riter) * ret;
  }
  return ret;
}

S operator*(const F& a, const S& v)
{
  S ret = v;
  for(S::iterator iter = ret.begin();
      iter != ret.end(); ++iter) {
    (iter -> second) *= a;
  }
  return ret;
}

S operator*(const Actions& a, const S& v)
{
  if(a.empty()) return S();
  S ret;
  for(Actions::const_iterator iter = a.begin();
      iter != a.end(); ++iter) {
    ret += (iter -> second) * ((iter -> first) * v);
  }
  return ret;
}

void Core::push_back(const Factor& f)
{
  val.push_back(::proj(f));
}

Monomial append(const Factor& f, Monomial m)
{
  m.push_back(f);
  return m;
}

S append(const Factor& f, const S& v)
{
  S ret;
  for(S::const_iterator iter = v.begin();
      iter != v.end(); ++iter) {
    const Monomial& m = iter -> first;
    const F& coeff = (iter -> second);
    ret.insert(make_pair(append(f, m), coeff));
  }
  return ret;
}

Factor deriveTarget(int n)
{
  if (n <= 0) return Factor();
  if((n - 1) % 6 == 0)
    return Factor(H(F(0), F(1)), -n);
  if((n + 1) % 6 == 0)
    return Factor(H(F(1), F(0)), -n);
  return Factor();
}

S derive(Factor f, const S::Term& t)
{
	using std::count;
  S ret;
  const F coeff = getCoeff(f);
  Factor target = deriveTarget(f.second);
  Monomial m = t.first;
  const unsigned int mul = count(m.begin(), m.end(), target);
  if(mul == 0) return ret;
  Core::const_iterator it = find(m.begin(), m.end(), target);
  m.erase(it);
  ret.insert(make_pair(m, coeff * t.second * F(mul * 3 * f.second) / F(CoxeterNum)));
  return ret;
}

S derive(const Factor& f, const S& v)
{
  S ret;
  for(S::const_iterator iter = v.begin();
      iter != v.end(); ++iter) {
    ret += derive(f, *iter);
  }
  return ret;
}

S operator*(const Factor& f, const S& v)
{
  if(f.second < 0)
    return append(f, v);
  if(f.second > 0)
    return derive(f, v);
  std::cerr << "Factor (*, 0) is not defined action of this realization." << std::endl;
  exit(1);
}

std::pair<Actions::iterator, bool> Actions::insert(Term x)
{
  auto ret = val.insert(unify(x));
  return ret;
}

Actions::Term& Actions::unify(Term& x)
{
  x.second *= x.first.getCoeff();
  x.first.unifyCoeff();
  return x;
}

F factorial(int n)
{
  if(n < 0) return F(0);
  if(n == 0) return F(1);
  return F(n) * factorial(n - 1);
}

void Actions::omit()
{
  for(value_type::const_iterator iter = val.begin();
      iter != val.end();) {
    if(iter -> second == F(0))
      iter = val.erase(iter);
    else
      ++iter;
  }
}

Actions operator*(const F& a, const Actions& v)
{
  Actions ret = v;
  for(Actions::iterator iter = ret.begin();
      iter != ret.end(); ++iter) {
    (iter -> second) *= a;
  }
  return ret;
}

Actions& Actions::operator+=(const Actions& rhs)
{
  for(Actions::const_iterator iter = rhs.begin();
      iter != rhs.end(); ++iter) {
    value_type::iterator it = val.find(iter -> first);
    if(it != val.end()) {
      it -> second += iter -> second;
    }
    else
      val.insert(*iter);
  }

  omit();

  return *this;
}

Actions operator+(Actions lhs, const Actions& rhs)
{
  return lhs += rhs;
}

Actions& Actions::operator-=(const Actions& rhs)
{
  for(Actions::const_iterator iter = rhs.begin();
      iter != rhs.end(); ++iter) {
    value_type::iterator it = val.find(iter -> first);
    if(it != val.end()) {
      it -> second -= iter -> second;
    }
    else
      val[iter -> first] = - iter -> second;
  }

  omit();

  return *this;
}

Actions operator-(Actions lhs, const Actions& rhs)
{
  return lhs -= rhs;
}

Action operator*(Action lhs, const Action& rhs)
{
  for(Action::const_iterator iter = rhs.begin();
      iter != rhs.end(); ++iter) {
    lhs.push_back(*iter);
  }
  return lhs;
}

Actions& Actions::operator*=(const Actions& rhs)
{
  if(val.empty() || rhs.empty()) return *this =  Actions();
  Actions::value_type ret;
  for(Actions::const_iterator it1 = val.begin();
      it1 != val.end(); ++it1) {
    for(Actions::const_iterator it2 = rhs.begin();
        it2 != rhs.end(); ++it2) {
      Actions a = straighten((it1 -> first) * (it2 -> first));
      F coeff = (it1 -> second) * (it2 -> second);
      for(auto j = a.begin(); j != a.end(); ++j) {
        ret[j -> first] += coeff * j -> second;
      }
    }
  }

  val = ret;
  omit();

  return *this;
}

F commutant(const Factor& f1, const Factor& f2)
{
  if(f1.second != -f2.second) return F(0);
  return (F(f1.second) / F(6)) * f1.first.innerProd(f2.first);
}

bool isStraightened(const Action& a)
{
  if(a.size() < 2) return true;
  for(auto it = a.begin(); it + 1 != a.end(); ++it) {
    if(it -> second > (it + 1) -> second) {
      return false;
    }
  }
  return true;
}

Actions straighten(const Action& origin)
{
  using std::deque;
  using std::pair;
  using std::vector;
	using std::find_if;
  using std::copy;
  if(isStraightened(origin)) return Actions(origin);
  Actions ret;
  deque<pair<Action, F> > dq; // A deque containing Actions which are not straightened with some coefficient.
  dq.push_back(make_pair(origin, F(1)));
  while(!dq.empty()) {
    const Action a = dq.front().first;
    const F coeff = dq.front().second;
    dq.pop_front();
    auto it = a.begin();
    // Find an iterator it such that it -> second > (it + 1) -> second.
    while(it + 1 != a.end() && it -> second <= (it + 1) -> second) ++it;
    // it and (it + 1) are commutable
    if(it -> second != -((it + 1) -> second)) {
      // Calculate commutedAction
      vector<Factor> v1(a.size());
      auto i1 = v1.begin();
      i1 = copy(a.begin(), it, i1);
      *(i1++) = *(it + 1);
      *(i1++) = *it;
      i1 = copy(it + 2, a.end(), i1);
      Action commutedAction(v1);

      const F c1  = coeff * commutedAction.getCoeff();
      commutedAction.unifyCoeff();
      if(isStraightened(commutedAction)) {
        ret[commutedAction] += c1;
      }
      else
        dq.push_front(make_pair(commutedAction, c1));
     continue;
    }

    // it and (it + 1) are NOT commutable
    const auto it_next = find_if(it + 1, a.end(),
      [&](const Factor& f) { return f.second != -(it -> second); });
    // Calculate commutedAction
    vector<Factor> v1(a.size());
    auto i1 = v1.begin();
    i1 = copy(a.begin(), it, i1);
    i1 = copy(it + 1, it_next, i1);
    *(i1++) = *it;
    i1 = copy(it_next, a.end(), i1);
    Action commutedAction(v1);

    const F c1  = coeff * commutedAction.getCoeff();
    commutedAction.unifyCoeff();
    if(isStraightened(commutedAction)) {
      ret[commutedAction] += c1;
    }
    else
      dq.push_front(make_pair(commutedAction, c1));

    // Calculate change term
    vector<Factor> v2(a.size() - 2);
    auto i2 = v2.begin();
    i2 = copy(a.begin(), it, i2);
    i2 = copy(it + 2, a.end(), i2);
    Action change(v2);

    const F c2  = F(int(it_next - it - 1)) *
                  coeff *
                  commutant(*it, *(it + 1)) *
                  change.getCoeff();
    change.unifyCoeff();
    if(isStraightened(change)) {
      ret[change] += c2;
      continue;
    }

    auto j = find_if(dq.rbegin(), dq.rend(),
      [&](const pair<Action, F>& f) { return f.first == change; });
    if(j == dq.rend())
      dq.push_back(make_pair(change, c2));
    else{
      j -> second += c2;
    }
  }
  return ret;
}

Actions operator*(Actions lhs, const Actions& rhs)
{
  return lhs *= rhs;
}

void Operators::create()
{
  for(size_type i = MIN_DEG; i <= MAX_DEG; ++i) {
    val[i] = Actions();
  }
}

void Operators::create(const F& f)
{
  val[DEG0] = Actions(f);
}

Operators& Operators::operator+=(const Operators& rhs)
{
  for(Operators::size_type i = MIN_DEG;
      i <= MAX_DEG; ++i) {
    val[i] += rhs.val[i];
  }
  return *this;
}

Operators operator*(const F& lhs, const Operators& rhs)
{
  const int MAX_DEG = Operators::MAX_DEG - Operators::DEG0;
  const int MIN_DEG = int(Operators::MIN_DEG) - int(Operators::DEG0);

  Operators ret;
  for(int i = MIN_DEG; i <= MAX_DEG; ++i) {
    ret[i] = lhs * rhs[i];
  }
  return ret;
}

Operators operator*(const Operators& lhs, const Operators& rhs)
{
  using size_type = Operators::size_type;
  const int MAX_DEG = Operators::MAX_DEG - Operators::DEG0;
  const int MIN_DEG = int(Operators::MIN_DEG) - int(Operators::DEG0);

  Operators ret;
  for(int i = MIN_DEG; i <= MAX_DEG; ++i) {
    for(int j = MIN_DEG; j <= MAX_DEG; ++j) {
      int target = i + j;
      if(target > MAX_DEG || target < MIN_DEG)
        continue;
      ret[target] += lhs[i] * rhs[j];
    }
  }
  return ret;
}

ostream& operator<< (ostream& os, const Actions& a)
{
  os << "[";
  for(Actions::const_iterator iter = a.begin();
      iter != a.end(); ++iter) {
    if(iter != a.begin())
      os << ", ";
    os << (iter -> second) << " * " << (iter -> first);
  }
  return os << "]";
}

ostream& operator<< (ostream& os, const Operators& op)
{
  for(int i = Operators::MIN_DEG; i <= Operators::MAX_DEG; ++i) {
    if(i != Operators::MIN_DEG)
      os << std::endl;
    os << "z^" << i - int(Operators::DEG0) << " : " << std::endl
       << op.val[i];
  }
  return os;
}

Operators pow(const Operators& A, int n)
{
  if(n == 0) return Operators(F(1));
  if(n < 0) {
    std::cerr << "pow(Action A, int n) は n < 0 で未定義" << std::endl;
    exit(1);
  }
  if(n % 2 == 0) {
    Operators tmp = pow(A, n / 2);
    return tmp * tmp;
  }
  return A * pow(A, n - 1);
}

const Operators E_minus(const H& a)
{
  using std::size_t;
  const auto MAX_DEG = Operators::MAX_DEG - Operators::DEG0;
  const auto MIN_DEG = Operators::MIN_DEG - Operators::DEG0;
  Operators ret;
  Operators A;
  for(int i = -1; i >= int(MIN_DEG); --i) {
    A[-i] = (F(CoxeterNum) / F(i)) * Actions(Factor(a, i));
  }
  for(int i = 0; i <= MAX_DEG; ++i) {
    ret += (F(1) / factorial(i)) * pow(A, i);
  }
  return ret;
}

const Operators E_plus(const H& a)
{
  const auto MAX_DEG = Operators::MAX_DEG - Operators::DEG0;
  Operators ret;
  Operators A;
  for(int i = 1; i <= MAX_DEG; ++i) {
    A[-i] = (F(CoxeterNum) / F(i)) * Actions(Factor(a, i));
  }
  for(int i = 0; i <= MAX_DEG; ++i) {
    ret += (F(1) / factorial(i)) * pow(A, i);
  }
  return ret;
}

H nu(int n, const H& a)
{
  const F w("PRIM_ROOT_OF_UNITY");
  H::value_type v = a.getVal();
  return H((w ^ n) * v[0], (w ^ -n) * v[1]);
}

F sigma(const H& a)
{
  using ZZ = F::coeff_type;
  const F w("PRIM_ROOT_OF_UNITY");
  return (F(2) ^ (ZZ(nu(3, a).innerProd(a)) / ZZ(2))) *
         ((F(1) - (w ^ -1)) ^ (nu(1, a)).innerProd(a)) *
         ((F(1) - (w ^ -2)) ^ (nu(2, a)).innerProd(a));
}

const Actions X(const H& a, int n)
{
  using ZZ = F::coeff_type;
  F coeff = (F(6) ^ (ZZ(-a.innerProd(a)) / ZZ(2))) * sigma(a);
  Actions ret = (coeff * E_minus(-a) * E_plus(-a))[-n];
  return ret;
}

F epsilon_2(const H& a1, const H& a2)
{
  using ZZ = F::coeff_type;
  const F w("PRIM_ROOT_OF_UNITY");
  return (ZZ((nu(-1, a1) + nu(-2, a1)).innerProd(a2)) % ZZ(2) == ZZ(0) ? F(1) : F(-1))
         * (w ^ ZZ((nu(-1, a1) + F(2) * nu(-2, a1)).innerProd(a2)));
}

const Actions comX(const H& a, int n1, int n2)
{
  const F w("PRIM_ROOT_OF_UNITY");
  return (F(1) / F(6)) * epsilon_2(nu(2, a), a) * (w ^ (-2 * n1)) * X(nu(2, a) + a, n1 + n2)
         + (F(1) / F(6)) * epsilon_2(nu(-2, a), a) * (w ^ (2 * n1)) * X(nu(-2, a) + a, n1 + n2)
         + (n1  != -n2 ? Actions() :
            (F(1) / F(36)) * epsilon_2(-a, a) * F(n2) * (n2 % 2 == 0 ? F(1) : F(-1)) * Actions(Action()))
         - (F(1) / F(6)) * epsilon_2(-a, a) * (n1 % 2 == 0 ? F(1) : F(-1))
                                            * Actions(Action({Factor(a, n1 + n2)}));
}
