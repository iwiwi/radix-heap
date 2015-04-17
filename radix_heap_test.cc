#include "radix_heap.h"
#include <queue>
#include "gtest/gtest.h"
using namespace std;
using testing::Types;

namespace {
// xorshift64* random generator [Vigna, 2014]
uint64_t x = 123456789LL;
uint64_t xorshift64() {
  x ^= x >> 12;  // a
  x ^= x << 25;  // b
  x ^= x >> 27;  // c
  return x * 2685821657736338717LL;
}

string random_string() {
  static constexpr int kNumLength = 8;
  string s(kNumLength, 'a');
  for (int i = 0; i < kNumLength; ++i) {
    s[i] += xorshift64() % 26;
  }
  return s;
}

typedef Types<unsigned char, char, signed char,
              unsigned short, short,
              unsigned int, int,
              unsigned long, long,
              unsigned long long, long long,
              float, double> AllTypes;
}  // namespace

template<typename T>
class radix_heap_test_all_types : public testing::Test {};
TYPED_TEST_CASE(radix_heap_test_all_types, AllTypes);

TYPED_TEST(radix_heap_test_all_types, trivial) {
  radix_heap::radix_heap<TypeParam> h;
  ASSERT_TRUE(h.empty());
  ASSERT_EQ(0, h.size());

  h.push(0);
  h.push(10);

  ASSERT_FALSE(h.empty());
  ASSERT_EQ(2, h.size());
  ASSERT_EQ(0, h.top());
  h.pop();

  ASSERT_FALSE(h.empty());
  ASSERT_EQ(1, h.size());

  h.push(8);
  h.push(5);
  ASSERT_FALSE(h.empty());
  ASSERT_EQ(3, h.size());
  ASSERT_EQ(5, h.top());

  h.clear();
  ASSERT_TRUE(h.empty());
  ASSERT_EQ(0, h.size());

  h.push(1);
  ASSERT_FALSE(h.empty());
  ASSERT_EQ(1, h.size());
  ASSERT_EQ(1, h.top());
}

TYPED_TEST(radix_heap_test_all_types, extreme1) {
  radix_heap::radix_heap<TypeParam> h;
  h.push(0);
  h.push(numeric_limits<TypeParam>::max());
  h.push(5);
  h.push(numeric_limits<TypeParam>::lowest());

  ASSERT_EQ(numeric_limits<TypeParam>::lowest(), h.top());
  h.pop();

  ASSERT_EQ(0, h.top());
  h.pop();

  ASSERT_EQ(5, h.top());
  h.pop();

  ASSERT_EQ(numeric_limits<TypeParam>::max(), h.top());
  h.pop();

  ASSERT_TRUE(h.empty());
}

TYPED_TEST(radix_heap_test_all_types, extreme2) {
  radix_heap::radix_heap<TypeParam> h;

  h.push(numeric_limits<TypeParam>::lowest());
  ASSERT_EQ(numeric_limits<TypeParam>::lowest(), h.top());
  h.pop();

  h.push(0);
  ASSERT_EQ(0, h.top());
  h.pop();

  h.push(5);
  ASSERT_EQ(5, h.top());
  h.pop();

  h.push(numeric_limits<TypeParam>::max());
  ASSERT_EQ(numeric_limits<TypeParam>::max(), h.top());
  h.pop();

  ASSERT_TRUE(h.empty());
}

TYPED_TEST(radix_heap_test_all_types, copy_and_swap) {
  typedef radix_heap::radix_heap<TypeParam> heap_type;

  heap_type h1;
  h1.push(8);
  h1.push(1);
  h1.push(10);
  h1.push(5);
  h1.pop();

  heap_type h2 = h1;
  ASSERT_EQ(3, h2.size());
  ASSERT_EQ(5, h2.top());
  h2.pop();
  ASSERT_EQ(8, h2.top());
  h2.pop();
  ASSERT_EQ(10, h2.top());
  h2.pop();
  ASSERT_TRUE(h2.empty());

  heap_type h3;
  h3.push(100);
  h1.swap(h3);
  ASSERT_EQ(1, h1.size());
  ASSERT_EQ(3, h3.size());
  ASSERT_EQ(100, h1.top());
  h1.pop();
  ASSERT_TRUE(h1.empty());

  h3.push(20);
  h3.pop();
  heap_type h4(std::move(h3));
  ASSERT_EQ(8, h4.top());
  h4.pop();
  ASSERT_EQ(10, h4.top());
  h4.pop();
  ASSERT_EQ(20, h4.top());
  h4.pop();
  ASSERT_TRUE(h4.empty());
}

TEST(radix_heap_test_int, large) {
  const int kNumTrials = 10;
  const int kNumPop = 10000;
  const int kMaxDiff = 1000;
  const int kMaxInsert = 10;

  radix_heap::radix_heap<int> rh;
  for (int trial = 0; trial < kNumTrials; ++trial) {
    rh.clear();
    ASSERT_TRUE(rh.empty());
    priority_queue<int, vector<int>, greater<int>> pq;

    int last = static_cast<int>(xorshift64()) / 2;
    for (int i = 0; i < kNumPop; ++i) {
      int num_insert = 1 + xorshift64() % kMaxInsert;
      for (int j = 0; j < num_insert; ++j) {
        int x = last + xorshift64() % kMaxDiff;
        rh.push(x);
        pq.push(x);
      }

      if (i % 2 == 0) ASSERT_EQ(rh.top(), pq.top());
      last = pq.top();

      rh.pop();
      pq.pop();
    }
  }
}

TEST(pair_radix_heap_test, trivial) {
  radix_heap::pair_radix_heap<double, string> h;

  h.push(-100.0, "hoge");
  h.push(-0.5, "piyo");
  h.push(0.0, "huga");
  ASSERT_EQ(-100.0, h.top_key());
  ASSERT_EQ("hoge", h.top_value());

  h.pop();
  ASSERT_EQ(-0.5, h.top_key());
  ASSERT_EQ("piyo", h.top_value());

  h.pop();
  h.push(-0.25, "nya");
  ASSERT_EQ(-0.25, h.top_key());
  ASSERT_EQ("nya", h.top_value());

  h.pop();
  ASSERT_EQ(0.0, h.top_key());
  ASSERT_EQ("huga", h.top_value());

  h.pop();
  ASSERT_EQ(0, h.size());
  ASSERT_TRUE(h.empty());
}

TEST(pair_radix_heap_test_double_string, emplace) {
  radix_heap::pair_radix_heap<double, string> h;
  h.emplace(10, "hoge");
  h.emplace(20, 10, 'a');
  ASSERT_EQ("hoge", h.top_value());
  h.pop();
  ASSERT_EQ("aaaaaaaaaa", h.top_value());
}

TEST(pair_radix_heap_test_int_string, large) {
  const int kNumTrials = 10;
  const int kNumPop = 10000;
  const int kMaxDiff = 1000;
  const int kMaxInsert = 10;

  radix_heap::pair_radix_heap<int, string> rh;
  for (int trial = 0; trial < kNumTrials; ++trial) {
    rh.clear();
    ASSERT_TRUE(rh.empty());
    set<pair<int, string>> se;

    int last = static_cast<int>(xorshift64()) / 2;
    for (int i = 0; i < kNumPop; ++i) {
      int num_insert = 1 + xorshift64() % kMaxInsert;
      for (int j = 0; j < num_insert; ++j) {
        int x = last + xorshift64() % kMaxDiff;
        string s = random_string();
        se.emplace(x, s);

        if (i % 2 == 0) rh.push(x, s);
        else rh.emplace(x, s);
      }

      int top_key = rh.top_key();
      string top_value = rh.top_value();
      ASSERT_EQ(top_key, (*se.begin()).first);
      ASSERT_TRUE(se.count(make_pair(top_key, top_value)));

      rh.pop();
      se.erase(make_pair(top_key, top_value));
      last = top_key;
    }
  }
}
