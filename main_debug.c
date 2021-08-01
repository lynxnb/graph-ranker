#include <time.h>

#define UNIT_TEST
#include "main.c"

void PrintDebug(Context *ctx) {
  #define heap ctx->ranking
  u32 end = heap->size + 1;
  
  puts("Heap dump:");
  const char *format = "(%u, %u)";
  for (u32 i = 1; i < end; i++) {
    printf(format, heap->data[i].index, heap->data[i].score);
    format = "  (%u, %u)";
  }
  puts("\n"); // actually prints two newline chars
  #undef heap
}

int main(int argc, char **argv) {
  #if defined(STDIO_REDIRECT) && !defined(EVAL)
  freopen("input.txt", "r", stdin);
  #endif

  u32 topk_requests = 0;
  Context ctx = {0, 0, 0, NULL};
  scanf("%u %u", &ctx.graphSize, &ctx.rankingSize);
  
  ctx.ranking = HeapCreate(ctx.rankingSize);
  if (ctx.ranking == NULL) {
    fprintf(stderr, "Failed to allocate memory for the ranking.\n");
    return 1;
  }

  clock_t tLap, tDelta, tSum;
  char command[14];
  while (!feof(stdin)) {
    scanf("%s\n", command);

    if (!strcmp(command, ADD_GRAPH)) {
      tLap = clock();
      AddGraph(&ctx);
      tDelta = clock() - tLap;
      tSum += tDelta;
      printf("Graph %u took %.3fs\n", ctx.count, (double) tDelta / CLOCKS_PER_SEC);
    }
    else if (!strcmp(command, TOP_K)) {
      topk_requests++;
      printf("TopK output:\n");
      tLap = clock();
      TopK(&ctx);
      tDelta = clock() - tLap;
      tSum += tDelta;
      putchar_unlocked('\n');
    }

    PrintDebug(&ctx);
  }
  printf("Total time taken: %.3fs\nTopK requested %u time(s)", (double) tSum / CLOCKS_PER_SEC, topk_requests);

  HeapDestroy(ctx.ranking);
  return 0;
}