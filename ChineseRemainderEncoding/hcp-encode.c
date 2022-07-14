/* Encoding of the Hamiltonian Cycle Problem         */
/* by Marijn Heule, last edited on March 3, 2021     */

/******************************************************
The encoding consists of three constraints. First, when
using LFSR, none of the bit-vectors can be all-zero.
Although this constraint it redundant, it is useful in
practice. Second, apart from the last position, each
position must have exactly one successor. Third, apart
from the first position, each position musht have
exactly one predecessor. Finally, the successor is
encoded using two linear-feedback shift registers:
one with 1 + x^1 + x^2 and one with 1 + x^2 + x^3.
*******************************************************/

#include <stdio.h>
#include <stdlib.h>

int nNode, nEdge, nBits;
int maxVar;

int lfsr (int n, int size, int xor) {
  int m = n << 1;
  int x = m & (1 << size);
  if (x) m = m - x + 1;
  m ^= x >> xor;
  return m; }

int bit (int s, int b) {
  return 2*nEdge + s * nBits + b + 1; }

void atmostone (int* array, int size) {
  if (size > 1) {
    printf ("-%i -%i 0\n", array[0], array[1]); }

  if (size > 2) {
    printf ("-%i -%i 0\n", array[0], array[2]);
    printf ("-%i -%i 0\n", array[1], array[2]); }

  if (size == 4) {
    printf ("-%i -%i 0\n", array[0], array[3]);
    printf ("-%i -%i 0\n", array[1], array[3]);
    printf ("-%i -%i 0\n", array[2], array[3]); }

  if (size > 4) {
    printf ("-%i %i 0\n", array[0], maxVar);
    printf ("-%i %i 0\n", array[1], maxVar);
    printf ("-%i %i 0\n", array[2], maxVar);

    for (int i = 3; i < size; i++)
      array[i - 3] = array[i];
    array[size-3] = maxVar++;
    atmostone (array, size - 2); }
}

int main (int argc, char** argv) {
  int b, i, j, k, l;

  int cycle = 2;

  FILE* graph = fopen (argv[1], "r");
  int tmp = fscanf (graph, " p edge %i %i ", &nNode, &nEdge);

  if (cycle <= nNode) cycle *= 3;
  if (cycle <= nNode) cycle *= 5;
  if (cycle <= nNode) cycle *= 7;
  while (cycle <= nNode) cycle *= 2;

  if (argc > 2) cycle = atoi (argv[2]);

  nBits = 0;
  k = 1;
  while (1) {
    if ((cycle % (1 << k)) == 0) nBits++;
    else break;
    k++; }

  if ((cycle %    3) == 0) nBits +=  2;
  if ((cycle %    5) == 0) nBits +=  3;
  if ((cycle %    7) == 0) nBits +=  3;
  if ((cycle %  511) == 0) nBits +=  9;
  if ((cycle % 1023) == 0) nBits += 10;
  if ((cycle % 2047) == 0) nBits += 11;

  int** adj;
  adj = (int **) malloc (sizeof (int*) * nNode);
  for (i = 0; i < nNode; i++) {
    adj[i] = (int*) malloc (sizeof (int) * nNode);
    for (j = 0; j < nNode; j++) adj[i][j] = 0; }

  // parse graph
  int max = 0;
  while (tmp != EOF) {
    tmp = fscanf (graph, " e %i %i ", &i, &j);
    if (tmp == 2) { adj[i-1][j-1] = ++max; adj[j-1][i-1] = ++max; } }

  int *degree;
  degree = (int*) malloc (sizeof(int) * nNode);
  for (i = 0; i < nNode; i++) degree[i] = 0;

  for (i = 0; i < nNode; i++) {
    for (j = i + 1; j < nNode; j++)
      if (adj[i][j]) { degree[i]++; degree[j]++; } }

  int minDegree = nNode;
  int minVertex = 0;
  int maxDegree = 0;

  for (i = 0; i < nNode; i++) {
    if (degree[i] < minDegree) { minDegree = degree[i]; minVertex = i; }
    if (degree[i] > maxDegree) { maxDegree = degree[i]; } }

  int extra = 0;
  if (minDegree > 2) extra = minDegree - 1;
  int first = minVertex;

#ifdef DEBUG
  printf ("c minDegree[%i] = %i\n", minVertex, minDegree);
  for (i = 0; i < nNode; i++)
    for (j = 0; j < nNode; j++)
      if (adj[i][j]) printf ("c var %i connects %i and %i\n", adj[i][j], i + 1, j + 1);
#endif

  int stepCls = 0;
  k = 1;
  while (1) {
    if ((cycle % (1 << k)) == 0) stepCls += 2*k;
    else break;
    k++; }

  if ((cycle %    3) == 0) stepCls +=  6;
  if ((cycle %    5) == 0) stepCls += 10;
  if ((cycle %    7) == 0) stepCls +=  8;
  if ((cycle %  511) == 0) stepCls += 20;
  if ((cycle % 1023) == 0) stepCls += 22;
  if ((cycle % 2047) == 0) stepCls += 24;

  // count the number of variables and clauses
  int nVar = 2*nEdge + nBits * nNode;
  int nCls = (minDegree+1) * nBits + stepCls * (2 * nEdge - minDegree) + 1 + minDegree;

  if ((cycle %    3) == 0) nCls += nNode;
  if ((cycle %    5) == 0) nCls += nNode * 2;
  if ((cycle %    7) == 0) nCls += nNode;
  if ((cycle %  511) == 0) nCls += nNode;
  if ((cycle % 1023) == 0) nCls += nNode;
  if ((cycle % 2047) == 0) nCls += nNode;

  for (i = 0; i < nNode; i++) {
    int size = 0;
    for (j = 0; j < nNode; j++) {
      if (adj[i][j]) size++; }
    if (size > 4) nVar += (size - 3) / 2;
    if (size == 2) nCls += 2;
    else if (size > 2) nCls += 3*(size-2) + 1; }

  for (i = 0; i < nNode; i++) {
    int size = 0;
    for (j = 0; j < nNode; j++) {
      if (adj[j][i]) size++; }
    if (size > 4) nVar += (size - 3) / 2;
    if (size == 2) nCls += 2;
    else if (size > 2) nCls += 3*(size-2) + 1; }

  printf ("p cnf %i %i\n", nVar, nCls);

  // the bitvectors representing LFSR cannot be ZERO
  for (i = 0; i < nNode; i++) {
    b = 0;
    k = 1;
    while (1) {
      if ((cycle % (1 << k)) == 0) b++;
      else break;
      k++; }

    if ((cycle %    3) == 0) { printf ("%i %i 0\n", bit (i, b), bit(i, b+1)); b+=2; }
    if ((cycle %    5) == 0) { printf ("-%i -%i 0\n", bit (i, b  ), bit(i, b+2));
                               printf ("-%i -%i 0\n", bit (i, b+1), bit(i, b+2)); b+=3; }
    if ((cycle %    7) == 0) { printf ("%i %i %i 0\n", bit (i, b), bit(i, b+1), bit (i, b+2)); b+=3; }
    if ((cycle %  511) == 0) { for (j = 0; j <  9; j++) printf ("%i ", bit (i, b+j)); printf ("0\n"); b+= 9; }
    if ((cycle % 1023) == 0) { for (j = 0; j < 10; j++) printf ("%i ", bit (i, b+j)); printf ("0\n"); b+=10; }
    if ((cycle % 2047) == 0) { for (j = 0; j < 11; j++) printf ("%i ", bit (i, b+j)); printf ("0\n"); b+=11; }
  }

  // Exactly one successor
  maxVar = 2*nEdge + nBits * nNode + 1;

  int* neighbors; // Initialize with max
  neighbors = (int *) malloc (sizeof (int) * maxDegree);

  for (i = 0; i < nNode; i++) {
    int size = 0;
    for (j = 0; j < nNode; j++) {
      if (adj[i][j]) neighbors[size++] = adj[i][j]; }

    for (j = 0; j < size; j++)
      printf ("%i ", neighbors[j]);
    printf ("0\n");

    atmostone (neighbors, size); }

  // Exactly one predecessor
  for (i = 0; i < nNode; i++) {
    int size = 0;
    for (j = 0; j < nNode; j++) {
      if (adj[j][i]) neighbors[size++] = adj[j][i]; }

    for (j = 0; j < size; j++)
      printf ("%i ", neighbors[j]);
    printf ("0\n");

    atmostone (neighbors, size); }

  // one of first neighbors must be the final connection
  int size = 0;
  for (i = 0; i < nNode; i++)
    if (adj[i][first])
      neighbors[size++] = i;

  for (j = 0; j < size; j++)
    printf ("%i ", adj[neighbors[j]][first]);
  printf("0\n");

  // symmetry breaking
  for (i = 0; i < size; i++) {
    for (j = 0; j < i; j++)
      printf ("%i ", adj[first][neighbors[j]]);
    printf("-%i 0\n", adj[neighbors[j]][first]); }

  // initialize the starting position
  b = 0;
  k = 1;
  while (1) {
    if ((cycle % (1 << k)) == 0) {
      printf ("-%i 0\n", bit (first, b)); b++; }
    else break;
    k++; }

  if ((cycle % 3) == 0) {
    printf ("%i 0\n",  bit (first, b)); b++;
    printf ("-%i 0\n", bit (first, b)); b++; }
  if ((cycle % 5) == 0) {
    printf ("-%i 0\n", bit (first, b)); b++;
    printf ("-%i 0\n", bit (first, b)); b++;
    printf ("-%i 0\n", bit (first, b)); b++; }
  if ((cycle % 7) == 0) {
    printf ("%i 0\n",  bit (first, b)); b++;
    printf ("-%i 0\n", bit (first, b)); b++;
    printf ("-%i 0\n", bit (first, b)); b++; }

  if ((cycle % 511) == 0) {
    printf ("%i 0\n",  bit (first, b)); b++;
    for (k = 2; k <= 9; k++) {
      printf ("-%i 0\n", bit (first, b)); b++; } }
  if ((cycle % 1023) == 0) {
    printf ("%i 0\n",  bit (first, b)); b++;
    for (k = 2; k <= 10; k++) {
      printf ("-%i 0\n", bit (first, b)); b++; } }
  if ((cycle % 2047) == 0) {
    printf ("%i 0\n",  bit (first, b)); b++;
    for (k = 2; k <= 11; k++) {
      printf ("-%i 0\n", bit (first, b)); b++; } }

  // initialize the termination position (one of the neighbors of first)
  for (j = 0; j < minDegree; j++) {
    b = 0;

    k = 1;
    while (1) {
      if ((cycle % (1 << k)) == 0) {
        printf ("-%i ", adj[neighbors[j]][first]);
        if (((nNode-1) & (1 << k)/2) == 0) printf ("-");
        printf ("%i 0\n", bit (neighbors[j], b));
        b++; }
      else break;
      k++; }

    if ((cycle % 3) == 0) {
      int mask = 1;
      for (i = 0; i < (nNode-1) % 3; i++) mask = lfsr (mask, 2, 1);
      for (i = 0; i < 2; i++) {
        printf ("-%i ", adj[neighbors[j]][first]);
        if ((mask & 1) == 0) printf ("-");
        mask = mask >> 1;
        printf ("%i 0\n", bit (neighbors[j], b+i)); }
      b+=2; }

    if ((cycle % 5) == 0) {
      int mask = (nNode+4) % 5;
      for (i = 0; i < 3; i++) {
        printf ("-%i ", adj[neighbors[j]][first]);
        if ((mask & 1) == 0) printf ("-");
        mask = mask >> 1;
        printf ("%i 0\n", bit (neighbors[j], b+i)); }
      b+=3; }

    if ((cycle % 7) == 0) {
      int mask = 1;
      for (i = 0; i < (nNode - 1) % 7; i++) mask = lfsr (mask, 3, 1);
      for (i = 0; i < 3; i++) {
        printf ("-%i ", adj[neighbors[j]][first]);
        if ((mask & 1) == 0) printf ("-");
        mask = mask >> 1;
        printf ("%i 0\n", bit (neighbors[j], b+i)); }
      b+=3; }

    if ((cycle % 511) == 0) {
      int mask = 1;
      for (i = 0; i < (nNode - 1) % 511; i++) mask = lfsr (mask, 9, 4);
      for (i = 0; i < 9; i++) {
        printf ("-%i ", adj[neighbors[j]][first]);
        if ((mask & 1) == 0) printf ("-");
        mask = mask >> 1;
        printf ("%i 0\n", bit (neighbors[j], b+i)); }
      b+=9; }

    if ((cycle % 1023) == 0) {
      int mask = 1;
      for (i = 0; i < (nNode - 1) % 1023; i++) mask = lfsr (mask, 10, 3);
      for (i = 0; i < 10; i++) {
        printf ("-%i ", adj[neighbors[j]][first]);
        if ((mask & 1) == 0) printf ("-");
        mask = mask >> 1;
        printf ("%i 0\n", bit (neighbors[j], b+i)); }
      b+=10; }

    if ((cycle % 2047) == 0) {
      int mask = 1;
      for (i = 0; i < (nNode - 1) % 2047; i++) mask = lfsr (mask, 11, 2);
      for (i = 0; i < 11; i++) {
        printf ("-%i ", adj[neighbors[j]][first]);
        if ((mask & 1) == 0) printf ("-");
        mask = mask >> 1;
        printf ("%i 0\n", bit (neighbors[j], b+i)); }
      b+=11; }
  }

  // enforce the next relationship
  for (i = 0; i < nNode; i++) {
    for (j = 0; j < nNode; j++)
      if (adj[i][j] && (j != first)) {
        b = 0;
        if ((cycle % 2) == 0) {
          printf("-%i %i %i 0\n",       adj[i][j], bit(j, b  ), bit(i, b  ));
          printf("-%i -%i -%i 0\n",     adj[i][j], bit(j, b  ), bit(i, b  ));
          b++; }

        k = 2;
        while (1) {
          if ((cycle % (1 << k)) == 0) {
            for (l = 1; l < k; l++) {
              printf("-%i %i %i -%i 0\n",   adj[i][j], bit (i, b-l), bit(j, b  ), bit(i, b  ));
              printf("-%i %i -%i %i 0\n",   adj[i][j], bit (i, b-l), bit(j, b  ), bit(i, b  )); }
            for (l = 1; l < k; l++) printf ("-%i ", bit (i, b-l));
            printf ("-%i %i %i 0\n", adj[i][j], bit(j, b  ), bit(i, b  ));
            for (l = 1; l < k; l++) printf ("-%i ", bit (i, b-l));
            printf ("-%i -%i -%i 0\n", adj[i][j], bit(j, b  ), bit(i, b  ));
            b++; }
          else break;
          k++; }

        if ((cycle % 3) == 0) {
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b  ), bit(i, b+1));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b  ), bit(i, b+1));
          printf("-%i %i %i -%i 0\n",   adj[i][j], bit(j, b+1), bit(i, b  ), bit(i, b+1));
          printf("-%i %i -%i %i 0\n",   adj[i][j], bit(j, b+1), bit(i, b  ), bit(i, b+1));
          printf("-%i -%i %i %i 0\n",   adj[i][j], bit(j, b+1), bit(i, b  ), bit(i, b+1));
          printf("-%i -%i -%i -%i 0\n", adj[i][j], bit(j, b+1), bit(i, b  ), bit(i, b+1));
          b+=2; }

        if ((cycle % 5) == 0) {
          printf("-%i -%i -%i 0\n",     adj[i][j], bit(j, b  ), bit(i, b  ));
          printf("-%i -%i -%i 0\n",     adj[i][j], bit(j, b  ), bit(i, b+2));
          printf("-%i %i %i %i 0\n",    adj[i][j], bit(j, b  ), bit(i, b  ), bit(i, b+2));
          printf("-%i %i %i -%i 0\n",   adj[i][j], bit(j, b+1), bit(i, b  ), bit(i, b+1));
          printf("-%i %i -%i %i 0\n",   adj[i][j], bit(j, b+1), bit(i, b  ), bit(i, b+1));
          printf("-%i -%i %i %i 0\n",   adj[i][j], bit(j, b+1), bit(i, b  ), bit(i, b+1));
          printf("-%i -%i -%i -%i 0\n", adj[i][j], bit(j, b+1), bit(i, b  ), bit(i, b+1));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+2), bit(i, b  ));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+2), bit(i, b+1));
          printf("-%i %i -%i -%i 0\n",  adj[i][j], bit(j, b+2), bit(i, b  ), bit(i, b+1));
          b+=3; }

        if ((cycle % 7) == 0) {
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b  ), bit(i, b+2));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b  ), bit(i, b+2));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+1), bit(i, b  ));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+1), bit(i, b  ));
          printf("-%i %i %i -%i 0\n",   adj[i][j], bit(j, b+2), bit(i, b+1), bit(i, b+2));
          printf("-%i %i -%i %i 0\n",   adj[i][j], bit(j, b+2), bit(i, b+1), bit(i, b+2));
          printf("-%i -%i %i %i 0\n",   adj[i][j], bit(j, b+2), bit(i, b+1), bit(i, b+2));
          printf("-%i -%i -%i -%i 0\n", adj[i][j], bit(j, b+2), bit(i, b+1), bit(i, b+2));
          b+=3; }

        if ((cycle % 511) == 0) {
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b  ), bit(i, b+8));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b  ), bit(i, b+8));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+1), bit(i, b  ));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+1), bit(i, b  ));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+2), bit(i, b+1));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+2), bit(i, b+1));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+3), bit(i, b+2));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+3), bit(i, b+2));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+4), bit(i, b+3));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+4), bit(i, b+3));
          printf("-%i %i %i -%i 0\n",   adj[i][j], bit(j, b+5), bit(i, b+4), bit(i, b+8));
          printf("-%i %i -%i %i 0\n",   adj[i][j], bit(j, b+5), bit(i, b+4), bit(i, b+8));
          printf("-%i -%i %i %i 0\n",   adj[i][j], bit(j, b+5), bit(i, b+4), bit(i, b+8));
          printf("-%i -%i -%i -%i 0\n", adj[i][j], bit(j, b+5), bit(i, b+4), bit(i, b+8));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+6), bit(i, b+5));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+6), bit(i, b+5));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+7), bit(i, b+6));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+7), bit(i, b+6));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+8), bit(i, b+7));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+8), bit(i, b+7));
          b+=9; }

        if ((cycle % 1023) == 0) {
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b  ), bit(i, b+9));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b  ), bit(i, b+9));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+1), bit(i, b  ));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+1), bit(i, b  ));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+2), bit(i, b+1));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+2), bit(i, b+1));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+3), bit(i, b+2));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+3), bit(i, b+2));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+4), bit(i, b+3));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+4), bit(i, b+3));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+5), bit(i, b+4));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+5), bit(i, b+4));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+6), bit(i, b+5));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+6), bit(i, b+5));
          printf("-%i %i %i -%i 0\n",   adj[i][j], bit(j, b+7), bit(i, b+6), bit(i, b+9));
          printf("-%i %i -%i %i 0\n",   adj[i][j], bit(j, b+7), bit(i, b+6), bit(i, b+9));
          printf("-%i -%i %i %i 0\n",   adj[i][j], bit(j, b+7), bit(i, b+6), bit(i, b+9));
          printf("-%i -%i -%i -%i 0\n", adj[i][j], bit(j, b+7), bit(i, b+6), bit(i, b+9));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+8), bit(i, b+7));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+8), bit(i, b+7));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+9), bit(i, b+8));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+9), bit(i, b+8));
          b+=10; }

        if ((cycle % 2047) == 0) {
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b  ), bit(i, b+10));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b  ), bit(i, b+10));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+1), bit(i, b  ));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+1), bit(i, b  ));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+2), bit(i, b+1));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+2), bit(i, b+1));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+3), bit(i, b+2));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+3), bit(i, b+2));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+4), bit(i, b+3));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+4), bit(i, b+3));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+5), bit(i, b+4));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+5), bit(i, b+4));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+6), bit(i, b+5));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+6), bit(i, b+5));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+7), bit(i, b+6));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+7), bit(i, b+6));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+8), bit(i, b+7));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+8), bit(i, b+7));
          printf("-%i %i %i -%i 0\n",   adj[i][j], bit(j, b+9), bit(i, b+8), bit(i, b+10));
          printf("-%i %i -%i %i 0\n",   adj[i][j], bit(j, b+9), bit(i, b+8), bit(i, b+10));
          printf("-%i -%i %i %i 0\n",   adj[i][j], bit(j, b+9), bit(i, b+8), bit(i, b+10));
          printf("-%i -%i -%i -%i 0\n", adj[i][j], bit(j, b+9), bit(i, b+8), bit(i, b+10));
          printf("-%i %i -%i 0\n",      adj[i][j], bit(j, b+10), bit(i, b+9));
          printf("-%i -%i %i 0\n",      adj[i][j], bit(j, b+10), bit(i, b+9));
          b+=11; }
      }
   }
}
