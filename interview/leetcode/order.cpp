#include <iostream>
#include <set>
#include <functional>

using namespace std;

struct intCompare {
  bool operator()(const int&a, const int&b) const {
    return a>b;
  }
};

int main() {
  // set <int, intCompare> se;
  // se.insert(9);
  // se.insert(5);
  // se.insert(2);
  // se.insert(7);

  set <int> se;
  se.insert(9);
  se.insert(5);
  se.insert(2);
  se.insert(7);

  for (const auto& elem:se) {
    cout << elem << "";
  }
  cout << endl;
  return 0;
}
