#include "radix_heap.h"
#include "gtest/gtest.h"
using namespace std;

TEST(radix_heap_test, trivial_unsigned) {
  radix_heap::radix_heap<uint32_t> h;
  h.push(1);
  h.push(1);
  h.push(100);
  h.push(5);
  h.push(30);
  h.push(0);
  h.push(3);

  ASSERT_EQ(0, h.top());
  h.pop();

  ASSERT_EQ(1, h.top());
  h.pop();

  ASSERT_EQ(1, h.top());
  h.pop();

  ASSERT_EQ(3, h.top());
  h.pop();

  ASSERT_EQ(5, h.top());
  h.pop();

  ASSERT_EQ(30, h.top());
  h.pop();

  ASSERT_EQ(100, h.top());
  h.pop();
}

TEST(radix_heap_test, trivial_signed) {
  radix_heap::radix_heap<int32_t> h;
  h.push(-1);
  h.push(1);
  h.push(-100);
  h.push(100);
  h.push(5);
  h.push(30);
  h.push(0);
  h.push(3);

  ASSERT_EQ(-100, h.top());
  h.pop();

  ASSERT_EQ(-1, h.top());
  h.pop();

  ASSERT_EQ(0, h.top());
  h.pop();

  ASSERT_EQ(1, h.top());
  h.pop();

  ASSERT_EQ(3, h.top());
  h.pop();

  ASSERT_EQ(5, h.top());
  h.pop();

  ASSERT_EQ(30, h.top());
  h.pop();

  ASSERT_EQ(100, h.top());
  h.pop();
}

TEST(radix_heap_test, trivial_decimal) {
  radix_heap::radix_heap<float> h;
  h.push(-1.0);
  h.push(-1.5f);
  h.push(1.0);
  h.push(-100.0);
  h.push(100.0);
  h.push(5.0);
  h.push(30.0);
  h.push(0.0);
  h.push(3.0);

  ASSERT_EQ(-100.0f, h.top());
  h.pop();

  ASSERT_EQ(-1.5f, h.top());
  h.pop();

  ASSERT_EQ(-1.0f, h.top());
  h.pop();

  ASSERT_EQ(0.0f, h.top());
  h.pop();

  ASSERT_EQ(1.0f, h.top());
  h.pop();

  ASSERT_EQ(3.0f, h.top());
  h.pop();

  ASSERT_EQ(5.0f, h.top());
  h.pop();

  ASSERT_EQ(30.0f, h.top());
  h.pop();

  ASSERT_EQ(100.0f, h.top());
  h.pop();
}
