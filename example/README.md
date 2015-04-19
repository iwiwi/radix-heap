Experimental Results
=========

We compared our `pair_radix_heap` (denoted as *rheap*) with `std::priority_queue` (denoted as *stl*) by a standard implementation of Dijkstra's algorithm. See `benchmark_dijkstra_main.cc` for details.

## Setup

### Environment

* Intel(R) Xeon(R) CPU E5-2690 0 @ 2.90GHz
* 256 GB Memory
* Linux 3.2.0
* gcc 4.6.3, -O3

### Datasets

* http://www.dis.uniroma1.it/challenge9/download.shtml (Real USA road networks)

## Results

* **Overall**: Overall time consumption of Dijkstra's algorithm (including edge traversal)
* **Workload**: Pure priority queue time consumption using the workload generated from Dijkstra's algorithm

| Graph | #Vertices | #Edges | rheap(overall) | stl(overall) | rheap(workload) | stl(workload) |
| --- | ---:| ---:| ---:| ---:| ---:| ---:|
| USA-road-d.NY.gr | 264,346 | 733,846 | 0.024032 | 0.027614 | 0.013619 | 0.019662 |
| USA-road-t.NY.gr | 264,346 | 733,846 | 0.026797 | 0.030916 | 0.014686 | 0.020601 |
| USA-road-d.BAY.gr | 321,270 | 800,172 | 0.031049 | 0.034741 | 0.015932 | 0.022237 |
| USA-road-t.BAY.gr | 321,270 | 800,172 | 0.032057 | 0.035989 | 0.017317 | 0.023417 |
| USA-road-d.COL.gr | 435,666 | 1,057,066 | 0.046762 | 0.053059 | 0.021215 | 0.031727 |
| USA-road-t.COL.gr | 435,666 | 1,057,066 | 0.048709 | 0.055512 | 0.022051 | 0.032772 |
| USA-road-d.FLA.gr | 1,070,376 | 2,712,798 | 0.132191 | 0.150148 | 0.053410 | 0.078457 |
| USA-road-t.FLA.gr | 1,070,376 | 2,712,798 | 0.136837 | 0.159468 | 0.056365 | 0.082315 |
| USA-road-d.NW.gr | 1,207,945 | 2,840,208 | 0.149918 | 0.173923 | 0.060497 | 0.090399 |
| USA-road-t.NW.gr | 1,207,945 | 2,840,208 | 0.159884 | 0.185500 | 0.065148 | 0.095352 |
| USA-road-d.NE.gr | 1,524,453 | 3,897,636 | 0.195856 | 0.232844 | 0.078223 | 0.116773 |
| USA-road-t.NE.gr | 1,524,453 | 3,897,636 | 0.208221 | 0.250998 | 0.082880 | 0.125620 |
| USA-road-d.CAL.gr | 1,890,815 | 4,657,742 | 0.242936 | 0.285539 | 0.097603 | 0.142917 |
| USA-road-t.CAL.gr | 1,890,815 | 4,657,742 | 0.256544 | 0.304676 | 0.103668 | 0.151534 |
| USA-road-d.LKS.gr | 2,758,119 | 6,885,658 | 0.362616 | 0.421768 | 0.137884 | 0.208801 |
| USA-road-t.LKS.gr | 2,758,119 | 6,885,658 | 0.379501 | 0.453178 | 0.145405 | 0.226780 |
| USA-road-d.E.gr | 3,598,623 | 8,778,114 | 0.476271 | 0.573831 | 0.183668 | 0.279030 |
| USA-road-t.E.gr | 3,598,623 | 8,778,114 | 0.506606 | 0.617901 | 0.194780 | 0.301337 |
| USA-road-d.W.gr | 6,262,104 | 15,248,146 | 0.858675 | 1.042036 | 0.320199 | 0.510726 |
| USA-road-t.W.gr | 6,262,104 | 15,248,146 | 0.918141 | 1.124565 | 0.330706 | 0.545862 |
| USA-road-d.CTR.gr | 14,081,816 | 34,292,496 | 2.360498 | 2.905690 | 0.727219 | 1.244478 |
| USA-road-t.CTR.gr | 14,081,816 | 34,292,496 | 2.479214 | 3.148338 | 0.809260 | 1.333198 |
| USA-road-d.USA.gr | 23,947,347 | 58,333,344 | 3.562287 | 4.456032 | 1.222667 | 2.119727 |
| USA-road-t.USA.gr | 23,947,347 | 58,333,344 | 3.817832 | 4.934690 | 1.302186 | 2.266316 |