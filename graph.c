#include "stdio.h"
#include "stdlib.h"

//функция построения кучи
void siftDown(int *numbers, int root, int bottom, int **graph)
{
  int maxChild;
  int done = 0;

  while ((root * 2 <= bottom) && (!done))
  {
    if (root * 2 == bottom)
      maxChild = root * 2;

    else if (numbers[root * 2] > numbers[root * 2 + 1])
      maxChild = root * 2;
    else
      maxChild = root * 2 + 1;

    if (numbers[root] < numbers[maxChild])
    {
      int temp = numbers[root];
      int* temp2 = graph[root];
      numbers[root] = numbers[maxChild];
      graph[root] = graph[maxChild];
      numbers[maxChild] = temp;
      graph[maxChild] = temp2;
      root = maxChild;
    }
    else
      done = 1;
  }
}

//функция сортировки по куче
void heapSort(int *numbers, int array_size, int **graph)
{

  for (int i = (array_size / 2); i >= 0; i--)
    siftDown(numbers, i, array_size - 1, graph);

  for (int i = array_size - 1; i >= 1; i--)
  {
    int temp = numbers[0];
    int* temp2 = graph[0];
    numbers[0] = numbers[i];
    graph[0] = graph[i];
    numbers[i] = temp;
    graph[i] = temp2;
    siftDown(numbers, 0, i - 1, graph);
  }
}

//функция подсчёиа кол-ва соединений вершины и параллельной сортировки
void sortGraph(int** graph, int x, int y) {
  int* coll = (int*) calloc(sizeof(int), y);
  if (coll == NULL) {
    printf("Error\n");
    exit(1);
  }

  for (int j = 0; j < y; j++) {
    for (int i = 0; i < x; i++) {
      if (graph[j][i] == 1) {
        ++coll[j];
      }
    }
  }
  heapSort(coll, y, graph);
}

//функция, рисующая граф по массиву
void paint_gr(int** graph, int x, int y) {
  FILE* output = fopen("out.dot", "w");
  int fir;
  int flversh;
  int* soed = (int*) calloc(sizeof(int), y);
  if (soed == NULL) {
    printf("Error\n");
    exit(1);
  }
  fprintf(output, "graph {\n");
  for (int i = 0; i < x; i++) {
    flversh = 0;
    for (int j = 0; j < y; j++) {
      if (graph[j][i] == 1) {
        if (flversh > 0) {
          fprintf(output, " -- %d", j);
        }
        else {
          fprintf(output, "%d", j);
          fir = j;
        }
        ++soed[j];
        ++flversh;
      }
    }
    if (flversh > 2) {
      printf("Incorrect matrix\n");
      exit(1);
    }
    if (flversh == 1) {
      fprintf(output, " -- %d", fir);
    }
    if (flversh > 0) {
      fprintf(output, " [label = %d]\n", i);
    }
  }
  for (int j = 0; j < y; j++) {
    if (soed[j] == 0) {
      fprintf(output, "%d\n", j);
    }
  }
  fprintf(output, "}");
  fclose(output);
  system("dot -T png out.dot -o picture.png && sxiv picture.png");
}

int main(void)
{
  FILE* input = fopen("incend.txt", "r");

  int** graph;
  char c;
  int x, y;

  fscanf(input ,"%d %d", &y, &x);

  graph = (int**) malloc(sizeof(int*)*y);
  if (graph == NULL) {
    printf("Error\n");
    exit(1);
  }
  for (int i = 0; i < y; i++) {
    graph[i] = (int*)malloc(sizeof(int)*x);
    if (graph[i] == NULL) {
      printf("Error\n");
      exit(1);
    }
  }

  while (c != '\n') {
    c = fgetc(input);
  }

  int ukx = 0;
  int uky = 0;
  while ((c = fgetc(input)) != EOF) {
    if ((uky >= y) || (ukx > x)) {
      printf("Incorrect data\n");
      exit(1);
    }
    switch (c) {
      case '1':
        graph[uky][ukx] = 1;
        ++ukx;
        break;
      case '0':
        graph[uky][ukx] = 0;
        ++ukx;
        break;
      case ' ':
        continue;
        break;
      case '\n':
        if (ukx < x) {
          printf("Incorrect data\n");
          exit(1);
        }
        ukx = 0;
        ++uky;
        break;
      default:
        printf("Incorrect symbol\n");
        exit(1);
        break;
    }
  }
  fclose(input);

  paint_gr(graph, x, y);

  int deleted;
  printf("Input edge:\n");
  scanf("%d", &deleted);
  if (deleted >= x) {
    printf("Edge does not exist\n");
    exit(1);
  }

 int delitions = 0;
  for (int j = 0; j < y; j++) {
    if (j == (y - delitions)) {
      break;
    }
    if (graph[j][deleted] == 1) {
      free(graph[j]);
      for (int i = j + 1; i < y; i++) {
        graph[i - 1] = graph[i];
      }
      if (delitions == 0) {
        graph[y - 1] = NULL;
      }
      --j;
      ++delitions;
    }
  }

  y = y - delitions;
  graph = (int**) realloc(graph, y*sizeof(int*));
  if (graph == NULL) {
    printf("Error\n");
    exit(1);
  }

  sortGraph(graph, x, y);

  paint_gr(graph, x, y);

  return 0;
}
