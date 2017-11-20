# algorithms-library
c++ template of Introduction to Algorithms

算法导论上的算法和数据结构的模板和测试文件，以及利用数据结构对stl的一些改良，所有模板都在命名空间qmj中，所有文件均曾在vs2015中运行通过，主要包括：

1.算法导论上的各种排序算法，归并，插入，快排，计数，以及优化的快排，各种排序算法全在一个文件“sort.h”中，测试文件为“sort_test.h”，经测试优化的快速排序
要快与其它算法，但慢于std::sort.

2.不相交集合和不相交森林在文件“disjoint_set.h”中，无测试文件。

3.二叉堆和斐波那契堆在文件“heap.h”中，测试文件“heap_test.h"。

4.红黑树，区间树，顺序统计数，都在同名文件中，没有独立测试文件，但类中都含有一个类似于print的打印函数用于验证。

5.以红黑树为基础重新设计了map,multimap,set,multiset,因为都以红黑树为基础，所以只测试了qmj::set,测试文件为“test_set.h”。

6.以哈希表为基础重写hash_set,hash_map,hash_multiset,hash_multimap,因都以hashtable为基础，所以只测试了qmj::hash_set。

7以斐波那契额堆优化的各种图算法，最小生成树，单源最短的路路径，所有节点的最短路径，拓扑排序，BFS，DFS，等，没有单独测试文件。所有算法名称均以书上的名字命名。



![标题](https://github.com/MouJieQin/algorithms-library/blob/master/Untitled%20Diagram.png)
