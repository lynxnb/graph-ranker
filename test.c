#define UNIT_TEST
#include "main.c"

int main(int argc, char **argv) {
  assert(freopen("test.in", "r", stdin) != NULL);
  assert(freopen("test.out", "w", stdout) != NULL);

  u32 graphSize;
  assert(scanf("%u\n", &graphSize) > 0);

  while (!feof(stdin)) {
    u32 graph[graphSize][graphSize];
    ReadMatrix(graphSize, graph);
    getchar_unlocked();

    u32 score = ComputeScore(graphSize, graph);
    printf("Computed score: %u\n", score);
  }
  
  return 0;
}