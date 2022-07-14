# ChineseRemainderEncoding
Encoding tool for the Chinese remainder encoding to determine the existence of Hamiltonian cycles. 

To compile, run: make

Afterwards, execute one of the run scripts. Both scripts require a DIMACS graph as input:
- ./run-wo-cycle.sh only uses the DIMACS graph. It computes a cycle size larger than the
number of vertices to ensure that a satisfying assignment is a Hamiltonian cycle;
- ./run-w-cycle additional requires a cycle length as second parameter. For most graphs,
a cycle length smaller than the number of vertices is optimal. The suggested length is 420.

Both scripts expect cadical to be installed in ~/cadical/build/cadical
