Radix-Heap
========
[![Build Status](https://travis-ci.org/iwiwi/radix-heap.svg?branch=master)](https://travis-ci.org/iwiwi/radix-heap)

高速な単調順位キューである基数ヒープ (*radix heap*) の C++ 実装です．テンプレートで記述してあり，符号付き整数，符号無し整数，浮動小数点数を扱うことができます．

## 例による紹介

ヘッダ `radix_heap.h` を取り込むだけで利用できます．数値の集合を管理するクラス `radix_heap` と数値（キー）とそれに関連づいた値を管理するクラス `pair_radix_heap` を提供しています．数値は小さい順に出てきます．以下では `pair_radix_heap` の利用例を紹介します．

```c++
#include "radix_heap.h"
...
radix_heap::pair_radix_heap<double, std::string> h;  // キーを double, 値を string としたヒープ
h.push(0.5, "hoge");
h.push(-10, "piyo");
std::cout << h.top_key() << ": " << h.top_value() << std::endl;  // "-10: piyo"
h.pop();
std::cout << h.top_key() << ": " << h.top_value() << std::endl;  // "0.5: hoge"
```

## もう少し詳しい説明

### 単調順位キューとは
基数ヒープは単調順位キュー (*monotone priority queue*) の一種です．単調順位キューとは，順位キューであって，「追加できる数値は最後に取り出した数値以上のものに限られる」という制約を持つものです．グラフの最短経路を計算する Dijkstra のアルゴリズムなどで利用することができます．

※ここで「取り出す」とはメンバ関数 `pop` 及び `top` （`pair_radix_sort` では `top_key` か `top_value`） を呼び出すことに相当します．


### クラス radix_heap

テンプレート引数としてキー（数値）の型を受け取ります．例えば `radix_heap<int>` や `radix_heap<double>` のようにして使って下さい．符号付き整数 (char, short, int, long, long long)，符号無し整数 (unsigned をつけたもの)，浮動小数点数 (float, double) に対応しています．メンバ関数は以下の通りです．

|　返り値 | 関数    | 意味           |
| ------------- | ------------- | ---- |
| bool | empty();   | 空なら `true` |
| size_t | size();   | 含んでいる要素数 |
| *キーの型* | top(); | 最小のキー |
| void | push(キー); | 要素を追加       |
| void | pop(); | 最小の要素を削除 |
| void | swap(別のヒープ); | 中身を交換      |


### クラス pair_radix_heap

テンプレート引数としてキー（数値）とそれに関連付ける値（なんでも）の型を受け取ります．例えば `pair_radix_heap<int, string>` や `pair_radix_heap<double, std::tuple<int, int, int>>` のように使って下さい．キーとして対応している型は `radix_heap` と同じです．メンバ関数は以下の通りです．

|　返り値 | 関数    | 意味           |
| ------------- | ------------- | ---- |
| bool | empty();   | 空なら `true` |
| size_t | size();   | 含んでいる要素数 |
| *キーの型* | top_key(); | 最小のキー |
| *値の型* | top_value(); | 最小のキーに関連づいている値 |
| void | push(キー, 値); | 要素を追加       |
| void | emplace(キー, ...); | 要素を in-place で構築して追加      |
| void | pop(); | 最小の要素を削除 |
| void | swap(別のヒープ); | 中身を交換      |


## 参考文献
* Ravindra K. Ahuja, Kurt Mehlhorn, James Orlin, and Robert E. Tarjan. **Faster algorithms for the shortest path problem.** *J. ACM 37, 2 (April 1990), 213-223.*
* **ダイクストラ法の高速化いろいろ** (http://www.slideshare.net/yosupo/ss-46612984)

## ライセンス

The MIT License (MIT)

Copyright (c) 2014 Takuya Akiba

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
