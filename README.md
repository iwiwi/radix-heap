Radix-Heap
========
[![Build Status](https://travis-ci.org/iwiwi/radix-heap.svg?branch=master)](https://travis-ci.org/iwiwi/radix-heap)

This library implements a fast monotone priority queue called the *radix heap*.
It is written as C++ template classes and capable of handling
signed integers, unsigned integers and floating-point numbers.

### Features

* **Fast** --- It often outperforms `std::priority_queue`.
As discussed later, it was about *2X faster* in experiments using real workloads.
* **Easy** --- The implementation is in a single header file.
* **Tested** -- It is unit-tested with gcc 4.8 and clang 3.4 (https://travis-ci.org/iwiwi/radix-heap).


## Quick Introduction

One can start using by just including header `radix_heap.h`,
which offers the following two classes:
class `radix_heap` manages a set of numbers,
and class `pair_radix_heap` manages a set of numbers (*keys*)
and *values* that are associated to keys.

### Example

```c++
#include "radix_heap.h"
...
radix_heap::pair_radix_heap<double, std::string> h;  // a radix heap where the types of keys and values are double and strings, respectively.
h.push(0.5, "foo");
h.push(-10, "bar");
std::cout << h.top_key() << ": " << h.top_value() << std::endl;  // "-10: foo"
h.pop();
std::cout << h.top_key() << ": " << h.top_value() << std::endl;  // "0.5: bar"
```

## Detailed Description

### Monotone Priority Queue?
The radix heap is a *monotone priority queue*.
A monotone priority queue is a priority queue
with the restriction that
a key cannot be pushed if it is less than the last key extracted from the queue.
Monotone priority queue can be used in many algorithms
such as Dijkstra's shortest-path algorithm.

(In this implementation, the word 'extract' above corresponds
calling member functions `pop`, `top`, `top_key` or `top_value`.)


### Radix Heap?
The *radix heap* is a monotone priority queue
using binary representation of numbers. Please see the paper below for details.

In theory, it takes O(b) amortized time for each item in total, where b is the number of bits representing keys.
More precisely, it rather depends on the ranges of keys; for example, if keys are unsigned integers less than 2^k, it becomes O(k) amortized time.

In practice, see the experimental results summarized in `example/README.md`. Workloads from Dijkstra's shortest-path algorithm against real road networks are used (see `example/benchmark_dijkstra_main.cc` for details). The results show that this radix heap implementation is about 2X faster than `std_priority_queue` at maximum.


### Class radix_heap

It takes the type of keys (numbers) as a template argument, e.g., `radix_heap<int>` or `radix_heap<double>`.
It can handle signed integers (char, short, int, long, longlong), unsigned integers, and floating-point numbers (float, double). Its member functions are as follows:

|　Return value | Name    | Description           |
| ------------- | ------------- | ---- |
| bool | empty();   | `true` if empty. |
| size_t | size();   | The number of keys. |
| *Key type* | top(); | The minimum key. |
| void | push(key); | Add a key.       |
| void | pop(); | Remove the minimum key. |
| void | swap(another radix heap); | Swap the contents.      |


### Class pair_radix_heap

It takes two template arguments: the types of keys (numbers) and values (anything that can be moved),
e.g., `pair_radix_heap<int, std::string>` or `pair_radix_heap<double, std::tuple<int, int, int>>`. Its member functions are as follows:

|　Return value | Name    | Description           |
| ------------- | ------------- | ---- |
| bool | empty();   | `true` if empty. |
| size_t | size();   | The number of pairs. |
| *Key type* | top_key(); | The minimum key. |
| *Value type* | top_value(); | The value of a pair with the minimum key. |
| void | push(key, value); | Add a pair.       |
| void | emplace(key, ...); | Construct and add a pair in place. |
| void | pop(); | Remove a pair with the minimum key. |
| void | swap(another radix heap); | Swap the contents.       |


## Reference
* Ravindra K. Ahuja, Kurt Mehlhorn, James Orlin, and Robert E. Tarjan. **Faster algorithms for the shortest path problem.** *J. ACM 37, 2 (April 1990), 213-223.*

## Licence

The MIT License (MIT)

Copyright (c) 2015 Takuya Akiba

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
