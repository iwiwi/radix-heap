/*
  Conducts Dijkstra's SSSP algorithm by using |radix_heap| and |std::priority_queue|.
  Reads graphs in the DIMACS format (http://www.dis.uniroma1.it/challenge9/format.shtml).
  100 Sources are randomly selected and the average times are reported.
*/

#include "radix_heap.h"
#include <sys/time.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <limits>
#include <queue>
#include <iostream>
#include <iomanip>
using namespace std;

typedef int vertex_t;
typedef int weight_t;
typedef pair<vertex_t, weight_t> edge_t;
typedef vector<vector<edge_t>> graph_t;

#define CHECK(expr)                                                     \
  if (expr) {                                                           \
  } else {                                                              \
    fprintf(stderr, "CHECK Failed (%s:%d): %s\n",                       \
            __FILE__, __LINE__, #expr);                                 \
    exit(EXIT_FAILURE);                                                 \
  }

namespace {
// xorshift64* random generator [Vigna, 2014]
uint64_t x = 123456789LL;
uint64_t xorshift64() {
  x ^= x >> 12;  // a
  x ^= x << 25;  // b
  x ^= x >> 27;  // c
  return x * 2685821657736338717LL;
}

inline double current_time_sec() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

void read_header(istream &ifs, vertex_t *num_vs, size_t *num_es) {
  string line;
  CHECK(getline(ifs, line));
  istringstream ss(line);

  string head;
  CHECK(ss >> head);
  if (head == "c") {
    read_header(ifs, num_vs, num_es);
    return;
  }

  CHECK(head == "p");
  CHECK((ss >> head) && head == "sp");
  CHECK(ss >> *num_vs >> *num_es);
}

bool read_edge(istream &ifs, vertex_t *s, vertex_t *t, weight_t *w) {
  string line;
  if (!getline(ifs, line) || line == "") return false;
  istringstream ss(line);

  string head;
  CHECK(ss >> head);
  if (head == "c") return read_edge(ifs, s, t, w);

  CHECK(head == "a");
  return (bool)(ss >> *s >> *t >> *w);
}

////////////////////////////////////////////////////////////////////////////////
// Overall time consumption of Dijkstra's algorithm
////////////////////////////////////////////////////////////////////////////////

vector<weight_t> benchmark_dijkstra_stlpque(const graph_t &g, vertex_t s) {
  vector<weight_t> pot(g.size(), numeric_limits<weight_t>::max());
  pot[s] = 0;

  typedef pair<weight_t, vertex_t> queue_entry_t;
  priority_queue<queue_entry_t, vector<queue_entry_t>, greater<queue_entry_t>> que;
  que.emplace(0, s);

  while (!que.empty()) {
    vertex_t v = que.top().second;
    weight_t p = que.top().first;
    que.pop();
    if (p > pot[v]) continue;

    for (const auto &e : g[v]) {
      vertex_t tv = e.first;
      weight_t tp = p + e.second;
      if (tp < pot[tv]) {
        pot[tv] = tp;
        que.emplace(tp, tv);
      }
    }
  }

  return pot;
}

vector<weight_t> benchmark_dijkstra_rheap(const graph_t &g, vertex_t s) {
  vector<weight_t> pot(g.size(), numeric_limits<weight_t>::max());
  pot[s] = 0;

  radix_heap::pair_radix_heap<weight_t, vertex_t> que;
  que.emplace(0, s);

  while (!que.empty()) {
    vertex_t v = que.top_value();
    weight_t p = que.top_key();
    que.pop();
    if (p > pot[v]) continue;

    for (const auto &e : g[v]) {
      vertex_t tv = e.first;
      weight_t tp = p + e.second;
      if (tp < pot[tv]) {
        pot[tv] = tp;
        que.emplace(tp, tv);
      }
    }
  }

  return pot;
}

////////////////////////////////////////////////////////////////////////////////
// Pure priority queue performance by Dijkstra's algorithm workloads
////////////////////////////////////////////////////////////////////////////////

enum operation_t { OP_PUSH, OP_POP, OP_CLEAR };
typedef vector<tuple<operation_t, weight_t, vertex_t>> workload_t;

void add_workload(const graph_t &g, vertex_t s, workload_t *wl) {
  vector<weight_t> pot(g.size(), numeric_limits<weight_t>::max());
  pot[s] = 0;

  radix_heap::pair_radix_heap<weight_t, vertex_t> que;
  que.emplace(0, s);
  wl->emplace_back(OP_CLEAR, 0, 0);
  wl->emplace_back(OP_PUSH, 0, 0);

  while (!que.empty()) {
    vertex_t v = que.top_value();
    weight_t p = que.top_key();
    que.pop();
    wl->emplace_back(OP_POP, p, 0);
    if (p > pot[v]) continue;

    for (const auto &e : g[v]) {
      vertex_t tv = e.first;
      weight_t tp = p + e.second;
      if (tp < pot[tv]) {
        pot[tv] = tp;
        que.emplace(tp, tv);
        wl->emplace_back(OP_PUSH, tp, tv);
      }
    }
  }
}

void benchmark_workload_stlpque(const workload_t &wl) {
  typedef pair<weight_t, vertex_t> queue_entry_t;
  priority_queue<queue_entry_t, vector<queue_entry_t>, greater<queue_entry_t>> que;

  for (auto w : wl) {
    switch (get<0>(w)) {
      case OP_CLEAR:
        decltype(que)().swap(que);
        break;
      case OP_POP:
        assert(que.top().first == get<1>(w));
        que.pop();
        break;
      case OP_PUSH:
        que.emplace(get<1>(w), get<2>(w));
        break;
    }
  }
}

void benchmark_workload_rheap(const workload_t &wl) {
  radix_heap::pair_radix_heap<weight_t, vertex_t> que;

  for (auto w : wl) {
    switch (get<0>(w)) {
      case OP_CLEAR:
        que.clear();
        break;
      case OP_POP:
        assert(que.top_key() == get<1>(w));
        que.pop();
        break;
      case OP_PUSH:
        que.emplace(get<1>(w), get<2>(w));
        break;
    }
  }
}
}  // namespace

////////////////////////////////////////////////////////////////////////////////
// Entry point
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
  cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
  cout.imbue(std::locale(""));
  cout << "#File\tV\tE\trheap(overall)\tstl(overall)\trheap(workload)\tstl(workload)" << endl;

  // Load
  graph_t g;
  {
    CHECK(argc == 2);
    ifstream ifs(argv[1]);
    CHECK(ifs);
    ifs.sync_with_stdio(false);
    {
      string f(argv[1]);
      size_t p = f.rfind('/');
      if (p != string::npos) f = f.substr(p + 1);
      cout << f;
    }

    vertex_t num_vs;
    size_t num_es;
    read_header(ifs, &num_vs, &num_es);
    cout << "\t" << num_vs << "\t" << num_es;

    g.resize(num_vs + 1);
    for (size_t i = 0; i < num_es; ++i) {
      vertex_t s, t;
      weight_t w;
      read_edge(ifs, &s, &t, &w);
      g[s].emplace_back(t, w);
    }
  }

  // Easy verification
  {
    constexpr int kNumVerificationSources = 10;
    for (int i = 0; i < kNumVerificationSources; ++i) {
      vertex_t s = xorshift64() % g.size();
      vector<weight_t> p1 = benchmark_dijkstra_stlpque(g, s);
      vector<weight_t> p2 = benchmark_dijkstra_rheap(g, s);
      CHECK(p1 == p2);
    }
  }

  // Benchmark (overall)
  constexpr int kNumBenchmarkSources = 100;
  vector<vertex_t> ss(kNumBenchmarkSources);
  for (int i = 0; i < kNumBenchmarkSources; ++i) {
    ss[i] = xorshift64() % g.size();
  }

  {
    {
      double t = current_time_sec();
      for (vertex_t s : ss) {
        benchmark_dijkstra_rheap(g, s);
      }
      cout << "\t" << (current_time_sec() - t) / kNumBenchmarkSources;
    }
    {
      double t = current_time_sec();
      for (vertex_t s : ss) {
        benchmark_dijkstra_stlpque(g, s);
      }
      cout << "\t" << (current_time_sec() - t) / kNumBenchmarkSources;
    }
  }

  // Benchmark (workload)
  {
    workload_t workload;
    for (vertex_t s : ss) {
      add_workload(g, s, &workload);
    }
    {
      double t = current_time_sec();
      benchmark_workload_rheap(workload);
      cout << "\t" << (current_time_sec() - t) / kNumBenchmarkSources;
    }
    {
      double t = current_time_sec();
      benchmark_workload_stlpque(workload);
      cout << "\t" << (current_time_sec() - t) / kNumBenchmarkSources;
    }
  }

  cout << endl;
  return 0;
}
