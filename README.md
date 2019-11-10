## Fast matrix multiplication

Speed of cache requests compared to the others allows us to significantly
boost perfomance of even such simple operations like matrix multiplication.

Optimisations show their best when applied to multiplying of big matrices 
(over 2000x2000) which can not be stored in L1-cache of most of the CPUs,
showing 5x perfomance boost.

Appled optimisations:

1. One matrix is being transposed before multiplication, which allows CPU
to keep the line imn cache as long as possible.
2. For big matrices was used 
[Block Matrix Multiplication Algorythm](https://en.wikipedia.org/wiki/Block_matrix#Block_matrix_multiplication) , which 
allowed to store whole part of matrix in cache while we perform operations 
on it.
3. In multiplication cycles widely used pointers instead of indices, which 
allowed the compiller to make additional optimisations

`main.cpp` - example of usage, `blockmatr.hpp` - multiplicator class.
