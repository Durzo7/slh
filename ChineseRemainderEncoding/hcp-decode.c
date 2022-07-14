#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv) {
  int i, j;
  int nNode, nEdge;

  FILE* graph = fopen (argv[1], "r");
  int tmp = fscanf (graph, " p edge %i %i ", &nNode, &nEdge);

  int *lookup;
  lookup = (int*) malloc (sizeof (int) * (4*nEdge));

  int** adj;
  adj = (int **) malloc (sizeof (int*) * nNode);
  for (i = 0; i < nNode; i++) {
    adj[i] = (int*) malloc (sizeof (int) * nNode);
    for (j = 0; j < nNode; j++) adj[i][j] = 0; }

  // parse graph
  int max = 0;
  while (tmp != EOF) {
    tmp = fscanf (graph, " e %i %i ", &i, &j);
    if (tmp == 2) {
      adj[i-1][j-1] = ++max;
      lookup[2*max  ] = i - 1;
      lookup[2*max+1] = j - 1;
      adj[j-1][i-1] = ++max;
      lookup[2*max  ] = j - 1;
      lookup[2*max+1] = i - 1;
    }
  }

//  for (i = 1; i <= 2*nEdge; i++)
//    printf ("%i %i %i\n", i, lookup[2*i], lookup[2*i+1]);

  fclose (graph);

  int* next;
  next = (int *) malloc (sizeof(int) * (nNode+1));

  int a, b;
  FILE* sol = fopen(argv[2], "r");
  tmp = fscanf (sol, " s SATISFIABLE " );

  int edge;
  do {
    tmp = fscanf (sol, " v " );
    tmp = fscanf (sol, " %i ", &edge);

    if (edge > 0 && edge <= 2 * nEdge) {
      a = lookup[2*edge  ] + 1;
      b = lookup[2*edge+1] + 1;

//      printf ("%i %i\n", a, b);

      next[a] = b;
    }
  }
  while (tmp != EOF);

  fclose (sol);

  int* visited;
  visited = (int*) malloc (sizeof(int) * (nNode+1));
  for (i = 0; i <= nNode; i++) visited[i] = 0;

  a = 1;
  for (i = 1; i <= nNode + 1; i++) {
//    printf ("%i\n", a);
    if (visited[a]) {
      if ((i - visited[a]) == nNode) printf ("c VERIFIED HCP of size %i\n", nNode);
      else printf ("c ERROR: cycle of size %i out of %i\n", i - visited[a], nNode);
//      printf ("c found cycle of length %i\n", i - visited[a]);
      break;
    }
    visited[a] = i;
    a = next[a];
  }
}
