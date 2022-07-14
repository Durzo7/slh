#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv) {

  int cols = atoi (argv[1]);
  int rows = atoi (argv[2]);

  int nVtx  = rows * cols;
  int nEdge = 0;
  for (int i = 1; i <= cols; i++)
    for (int j = 1; j <= rows; j++) {
      if (j < rows) nEdge++;
      if (i < cols) nEdge++; }

  printf ("p edge %i %i\n", nVtx, nEdge);

  for (int i = 1; i <= cols; i++)
    for (int j = 1; j <= rows; j++) {
      if (j < rows) printf ("e %i %i\n", (i-1) * rows + j, (i-1) * rows + j + 1);
      if (i < cols) printf ("e %i %i\n", (i-1) * rows + j, i * rows + j); }


}
