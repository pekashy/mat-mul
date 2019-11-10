## Fast matrix multiplication

Cache request speed allows us to significantly
boost perfomance of even such simple operations like matrix multiplication.

Optimisations show their best (5x boost) when being applied to multiplication of big matrices 
(over 2000x2000) which can not be stored at once in L1-cache of most of the CPUs.

Applied optimisations:

1. One matrix is being transposed before multiplication, which allows CPU
to keep the line in cache as long as possible.
2. For big matrices was used 
[Block Matrix Multiplication Algorythm](https://en.wikipedia.org/wiki/Block_matrix#Block_matrix_multiplication) , which 
allowed us to store the whole part of matrix in cache while we are performing operations 
on it.
3. In multiplication cycles pointers instead of indices were widely used, which 
allowed the compiller to make additional optimisations

`main.cpp` - example of usage, `blockmatr.hpp` - multiplicator class.
