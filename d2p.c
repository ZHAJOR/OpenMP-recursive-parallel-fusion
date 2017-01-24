#include "d2p.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>

void sequential_merge(int* array, int p1, int r1, int p2, int r2, int* result, int p3)
{
  int offset = 0;
  while(p1 <= r1 || p2 <= r2){
    if(p1 > r1)
      result[p3 + offset++] = array[p2++];
    else if(p2 > r2)
      result[p3 + offset++] = array[p1++];
    else if(array[p1] > array[p2]){
      result[p3 + offset++] = array[p2++];
    }
    else {
      result[p3 + offset++] = array[p1++];
    }
  }
}

int binary_search(int x, int* array, int p, int r)
{

  int low = p;
  int high = p > r + 1 ? p : r + 1;
  while(low < high){
    int mid = (low + high) / 2;
    if(x <= array[mid]){
      high = mid;
    }
    else{
      low = mid + 1;
    }
  }
  return high;
}

void recursive_merge(int* array, int p1, int r1, int p2, int r2, int* result, int p3){
  int n1 = r1 - p1 + 1;
  int n2 = r2 - p2 + 1;
  if(n1 < n2){
    int exchange = p1;
    p1 = p2;
    p2 = exchange;
    exchange = r1;
    r1 = r2;
    r2=exchange;
    exchange = n1;
    n1 = n2;
    n2 = exchange;
  }
  if(n1 == 0){
    return;
  }
  else if(n1 + n2 < 100000){
    sequential_merge(array, p1, r1, p2, r2, result, p3);
  }
  else{
    int q1 = (p1+r1) /2;
    int q2 = binary_search(array[q1], array, p2, r2);
    int q3 = p3 + (q1-p1) + (q2-p2);
    result[q3] = array[q1];
    #pragma omp task
    recursive_merge(array, p1, q1-1, p2, q2-1,result,p3);
    #pragma omp task
    recursive_merge(array, q1+1, r1,q2,r2,result, q3+1);
  }
}

struct timespec get_time(void)
{
  struct timespec start_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  return start_time;
}

struct timespec time_diff(struct timespec* ts1, struct timespec* ts2)
{
  static struct timespec ts;
  ts.tv_sec = ts1->tv_sec - ts2->tv_sec;
  ts.tv_nsec = ts1->tv_nsec - ts2->tv_nsec;
  if (ts.tv_nsec < 0) {
    ts.tv_sec--;
    ts.tv_nsec += 1000000000;
  }
  return ts;
}

struct timespec get_duration(struct timespec* ts)
{
  struct timespec time = get_time();
  return time_diff(&time, ts);
}

void print_time(struct timespec* ts)
{
  long ns = ts->tv_nsec % 1000;
  long us = (ts->tv_nsec / 1000) % 1000;
  long ms = (ts->tv_nsec / 1000000) % 1000;
  long s =  (ts->tv_nsec / 1000000000) % 1000 + ts->tv_sec;
  long t = (s * 1000000000) + (ms * 1000000) + (us * 1000) + ns;
  printf("%3lds %3ldms %3ldus %3ldns %12ld\n", s, ms, us, ns, t);
}

void fillMatrice(int* matrice, int matrice_size){
  for(int i=0; i<matrice_size; ++i){
    scanf("%d", &matrice[i]);
  }
}

void printMatrice(int* matrice, int matrice_size){
  for(int i=0; i<matrice_size; ++i){
    printf("%2d ", matrice[i]);
  }
  printf("\n");
}

int checkMatrice(int* matrice, int matrice_size){
  for(int i = 0; i< matrice_size -1; ++i){
    if(matrice[i]> matrice[i+1])
      return 0;
  }
  return 1;
}

int main(int argc, char *argv[]){
  int matrice_size;
  int* A;
  int* B;
  int threads = argc > 1 ? atoi(argv[1]) : 8;
  scanf("%d", &matrice_size);
  A = malloc(2*matrice_size* sizeof(int));
  B = malloc(2*matrice_size* sizeof(int));
  fillMatrice(A, matrice_size*2);
  struct timespec time = get_time();
  omp_set_num_threads(threads);
  #pragma omp parallel
  {
    #pragma omp single
    {
      recursive_merge(A, 0, matrice_size - 1, matrice_size, (2 * matrice_size) - 1, B, 0);
    }
  }
  struct timespec duration = get_duration(&time);
  if(checkMatrice(B, matrice_size*2) && argc > 3 && !strcmp("--check", argv[3]))
    printf("GOOD\n");
  else if(argc > 3 && !strcmp("--check", argv[3]))
    printf("BAD\n");
  if (argc > 2 && !strcmp("--print", argv[2])){
    printMatrice(B, matrice_size * 2);
  }
  else if(argc > 2 && !strcmp("--time", argv[2])){
    print_time(&duration);
  }
  return 0;
}
