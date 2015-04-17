#include "radix_heap.h"
#include <iostream>

int main() {
  radix_heap::pair_radix_heap<double, std::string> h;

  h.push(0.5, "hoge");
  h.push(-10, "piyo");

  std::cout << h.top_key() << ": " << h.top_value() << std::endl;  // "-10: piyo"

  h.pop();

  std::cout << h.top_key() << ": " << h.top_value() << std::endl;  // "0.5: hoge"

  return 0;
}
