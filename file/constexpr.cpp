//g++ constexpr.cpp --std=c++11

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
//#include "absl/base/macros.h"

using namespace std;

#define kMaxSize 10

static constexpr int CheckLengthInternal(int len) {
  //return ABSL_ASSERT(len <= kMaxSize), len;
  //return (len <= kMaxSize)?true:false, len;
  return (len <= kMaxSize)?-1:1, len;
}

int main() {
  int len = 5;
  int ret = CheckLengthInternal(len);
  //constexpr int retcs = CheckLengthInternal(len);//During build len is not const expression
  constexpr int retcs = CheckLengthInternal(11);
  cout<<ret<<std::endl;
  cout<<retcs<<std::endl;
}

