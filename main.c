#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

// Disable clangd deprecation warnings
#ifdef __clang__
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

// getchar_unlocked() is a POSIX function, Win32 has it under a different name so make them match
#ifdef _WIN32
#define getchar_unlocked() _getchar_nolock()
#define putchar_unlocked(x) _putchar_nolock(x)
#endif

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

// Redirect input.txt to stdin and stdout to output.txt
#define STDIO_REDIRECT

// Use a faster method for reading input
// which uses getchar_unlocked() instead of scanf()
#define FAST_IO

// Accepted commands
#define ADD_GRAPH "AggiungiGrafo"
#define TOP_K "TopK"

typedef struct Graph_s {
  u32 index;
  u32 score;
} Graph;

typedef struct Heap_s Heap;

typedef struct Context_s {
  u32 graphSize;
  u32 rankingSize;
  u32 count;
  Heap *ranking;
} Context;

/*
---------------------------------------------------------------------------------------------------
 IO methods
---------------------------------------------------------------------------------------------------
*/
void ReadMatrixOld(u32 size, u32 matrix[size][size]) {
  u32 i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size - 1; j++) {
      assert(scanf("%d,", &matrix[i][j]) > 0);
    }
    assert(scanf("%d", &matrix[i][j]) > 0);
  }
}

// Faster alternative using getchar_unlocked()
// This implementation is ~3x faster than the scanf() one
void ReadMatrix(u32 size, u32 matrix[size][size]) {
  char c;
  u32 i, j, value;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      for (c = getchar_unlocked(), value = 0; c != ',' && c != '\n'; c = getchar_unlocked()) {
        value = value * 10 + c - 48;
      }
      matrix[i][j] = value;
    }
  }
}
/*------------------------------------------ end of IO ------------------------------------------*/

/*
---------------------------------------------------------------------------------------------------
 Heap methods
 Fixed size array-based Max-Heap implementation with unused 0th position, root node is at index 1
 Allows for quicker child/parent indexes calculations
---------------------------------------------------------------------------------------------------
*/
#define HEAP_ROOT 1

struct Heap_s {
  u32 size;
  Graph *data;
};

// Heap utils [
static inline void Swap(Graph *a, Graph *b) {
  Graph temp = *a;
  *a = *b;
  *b = temp;
}

static inline u32 LeftChild(u32 pos) { return 2 * pos; }

static inline u32 RightChild(u32 pos) { return 2 * pos + 1; }

static inline u32 Parent(u32 pos) { return pos / 2; }
// ] utils

// Create a new heap, allocating an array of size `maxSize`
Heap *HeapCreate(u32 maxSize) {
  Heap *newHeap = (Heap *) malloc(sizeof(Heap));
  // Make sure memory has been allocated or bad stuff will happen, not fun
  assert(newHeap != NULL);

  newHeap->size = 0;
  // Using an heap implementation where index 0 is unused, need one more byte
  newHeap->data = (Graph *) malloc((maxSize + 1) * sizeof(Graph));
  // See above
  assert(newHeap->data != NULL);

  return newHeap;
}

// Destroy the heap freeing all allocated memory
void HeapDestroy(Heap *heap) {
  free(heap->data);
  free(heap);
}

// Move a node up in the tree, recursively as long as needed; restore heap condition after insertion
void HeapSiftUp(Heap *heap, u32 index) {
  u32 parent = Parent(index);

  if (!(parent < HEAP_ROOT) && heap->data[parent].score < heap->data[index].score) {
    Swap(&heap->data[index], &heap->data[parent]);
    HeapSiftUp(heap, parent);
  }
}

// Move a node down in the tree, recursively as long as needed; restore heap condition after deletion or replacement
void HeapSiftDown(Heap *heap, u32 index) {
  u32 left = LeftChild(index);
  u32 right = RightChild(index);
  u32 largest;

  if (left < heap->size && heap->data[left].score > heap->data[index].score)
    largest = left;
  else
    largest = index;
  if (right < heap->size && heap->data[right].score > heap->data[largest].score)
    largest = right;

  if (largest != index) {
    Swap(&heap->data[index], &heap->data[largest]);
    HeapSiftDown(heap, largest);
  }
}

// Add a new node to the heap
void HeapPush(Heap *heap, Graph entry) {
  heap->data[++heap->size] = entry;
  HeapSiftUp(heap, heap->size);
}

// Remove an arbitrary node from the heap
void HeapDelete(Heap *heap, u32 index) {
  heap->data[index] = heap->data[heap->size--];
  HeapSiftDown(heap, index);
}

// Remove the root node from the heap
void HeapDeleteRoot(Heap *heap) {
  HeapDelete(heap, HEAP_ROOT);
}

// Return the root node after removing it from the heap
Graph HeapPopRoot(Heap *heap) {
  Graph result = heap->data[HEAP_ROOT];
  HeapDeleteRoot(heap);
  return result;
}

// Replace the root node with entry and restore heap condition
void HeapReplace(Heap *heap, Graph entry) {
  heap->data[HEAP_ROOT] = entry;
  HeapSiftDown(heap, HEAP_ROOT);
}

// Handles heap insertion
// when the heap is not full HeapPush() is used
// when the heap is full HeapReplace() is used if conditions are met
void HeapInsert(Heap *heap, u32 maxSize, Graph entry) {
  if (heap->size < maxSize) {
    HeapPush(heap, entry);
  }
  else {
    if (entry.score < heap->data[HEAP_ROOT].score)
      HeapReplace(heap, entry);
  }
}
/*----------------------------------------- end of Heap -----------------------------------------*/

/*
---------------------------------------------------------------------------------------------------
 Score computation methods
---------------------------------------------------------------------------------------------------
*/
// Compute the score of a graph based on project metrics
u32 ComputeScore(u32 graphSize, u32 graph[graphSize][graphSize]) {
  u32 score = 0;

  u32 i, minDistance, current = 0;
  u32 distances[graphSize];
  u8 visited[graphSize];

  // Set up working arrays
  memset(visited, 0, sizeof(visited));
  memset(distances, UINT32_MAX, sizeof(distances));
  distances[0] = 0;

  // Dijkstra algorithm
  for(;;) {
    // Find minimum distance path
    for(minDistance = UINT32_MAX, i = 0; i < graphSize; i++)
      if (!visited[i] && distances[i] <= minDistance)
        minDistance = distances[current = i];
    
    visited[current] = 1;
    
    if (current >= graphSize || minDistance == UINT32_MAX)
      break;
    
    // Update path lengths for reachable nodes from the current node
    for(i = 0; i < graphSize; i++)
      if (graph[current][i] && distances[i] > distances[current] + graph[current][i])
        distances[i] = distances[current] + graph[current][i];
  }

  for (i = 1; i < graphSize; i++) {
    // Skip infinite (UINT32_MAX) path lengths
    score += distances[i] * !(distances[i] == UINT32_MAX);
  }
  return score;
}
/*----------------------------------------- end of Score ----------------------------------------*/

/*
---------------------------------------------------------------------------------------------------
 Commands methods
---------------------------------------------------------------------------------------------------
*/
// "AggiungiGrafo"
void AddGraph(Context *ctx) {
  u32 newGraph[ctx->graphSize][ctx->graphSize];
  
  #ifdef FAST_IO
  ReadMatrix(ctx->graphSize, newGraph);
  #else
  ReadMatrixOld(ctx->graphSize, newGraph);
  #endif

  u32 score = ComputeScore(ctx->graphSize, newGraph);

  Graph entry = {ctx->count++, score};
  HeapInsert(ctx->ranking, ctx->rankingSize, entry);
}

// "TopK"
void TopK(Context *ctx) {
  #define heap ctx->ranking
  u32 size = heap->size < ctx->rankingSize ? heap->size : ctx->rankingSize;
  size++;
  
  const char *format = "%d";
  for (u32 i = 1; i < size; i++) {
    assert(printf(format, heap->data[i].index) > 0);
    format = " %d";
  }
  putchar_unlocked('\n');
  #undef heap
}
/*--------------------------------------- end of Commands ---------------------------------------*/

#ifndef UNIT_TEST
int main(int argc, char **argv) {
  #if defined(STDIO_REDIRECT) && !defined(EVAL)
  assert(freopen("input.txt", "r", stdin) != NULL);
  assert(freopen("output.txt", "w", stdout) != NULL);
  #endif

  Context ctx = {0, 0, 0, NULL};
  assert(scanf("%u %u", &ctx.graphSize, &ctx.rankingSize) > 1);
  
  ctx.ranking = HeapCreate(ctx.rankingSize);

  char command[14];
  while (!feof(stdin)) {
    assert(scanf("%s\n", command) > 0);

    if (!strcmp(command, ADD_GRAPH)) {
      AddGraph(&ctx);
    }
    else if (!strcmp(command, TOP_K)) {
      TopK(&ctx);
    }
  }
  
  HeapDestroy(ctx.ranking);
  return 0;
}
#endif