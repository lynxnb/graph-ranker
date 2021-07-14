#define UNIT_TEST
#include "main.c"

int main(int argc, char **argv) {
  freopen("test.in", "r", stdin);
  freopen("test.out", "w", stdout);

  u32 graphSize;
  scanf("%u\n", &graphSize);

  while (!feof(stdin)) {
    u32 graph[graphSize][graphSize];
    ReadMatrix(graphSize, graph);
    getchar_unlocked();

    u32 score = ComputeScore(graphSize, graph);
    printf("Computed score: %u\n", score);
  }
  
  return 0;
}