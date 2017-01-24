#include "d2s.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void sequential_merge(int* array, unsigned l, unsigned m, unsigned h)
{
  unsigned i = l, j = m + 1, k = 0;
  int* tmp = malloc(sizeof(int)*(h-l+1));
  while (i <= m && j <= h) {
    if (array[i] <= array[j]) {
      tmp[k++] = array[i++];
    }
    else {
      tmp[k++] = array[j++];
    }
  }
  while (i <= m) {
    tmp[k++] = array[i++];
  }
  while (j <= h) {
    tmp[k++] = array[j++];
  }
  for (--k; k != (unsigned)-1; --k) {
    array[l + k] = tmp[k];
  }
  free(tmp);
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
  scanf("%d", &matrice_size);
  A = malloc(2*matrice_size* sizeof(int));
  fillMatrice(A, matrice_size*2);
  struct timespec time = get_time();
  sequential_merge(A,0, matrice_size -1, 2*matrice_size -1);
  struct timespec duration = get_duration(&time);
  if(checkMatrice(A, matrice_size*2) && argc > 2 && !strcmp("--check", argv[2]))
    printf("GOOD\n");
  else if(argc > 2 && !strcmp("--check", argv[2]))
    printf("BAD\n");
  if (argc > 1 && !strcmp("--print", argv[1])){
    printMatrice(A, matrice_size * 2);
  }
  else if(argc > 1 && !strcmp("--time", argv[1])){
    print_time(&duration);
  }
  return 0;
}
