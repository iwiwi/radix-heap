#include "radix_heap.h"
#include <iostream>

int main() {
  radix_heap::radix_heap<int> h;
  h.push(10);
  h.push(5);

  std::cout << h.top() << std::endl;  // "5"

  h.pop();
  h.push(8);
  h.push(20);

  std::cout << h.top() << std::endl;  // "8"

  h.pop();

  std::cout << h.top() << std::endl;  // "10"

  return 0;
}
