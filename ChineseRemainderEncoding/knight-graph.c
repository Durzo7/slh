/* Generation of Knight Graphs                       */
/* by Marijn Heule, last edited on February 27, 2021 */

/******************************************************
There are 168 different knight moves (modulo polarity).
This encoding uses the variables 1 to 336 to express
the moves.

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

#define SIZE	8

int size;

int edge (int a, int b) {
  int r = abs((a % size) - (b % size));
  int c = abs((a / size) - (b / size));

  if ((r == 1) && (c == 2)) return 1;
  if ((r == 2) && (c == 1)) return 1;
  return 0; }

int main (int argc, char** argv) {
  int i, j;

  size = SIZE;
  if (argc > 1) size = atoi (argv[1]);

  int sqsize = size * size;

  int nEdges = 0;
  for (i = 0; i < sqsize; i++)
    for (j = i + 1; j < sqsize; j++)
      if (edge (i, j)) nEdges++;

  printf ("p edge %i %i\n", sqsize, nEdges);

  for (i = 0; i < sqsize; i++)
    for (j = i+1; j < sqsize; j++)
      if (edge (i, j)) printf ("e %i %i\n", i + 1, j + 1);
}
