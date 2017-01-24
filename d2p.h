#ifndef D2P_H
#define D2P_H
#define _POSIX_C_SOURCE 199309L
#include <time.h>

void sequential_merge(int* array, int p1, int r1, int p2, int r2, int* result, int p3);
int binary_search(int x, int* array, int p, int r);
void recursive_merge(int* array, int p1, int r1, int p2, int r2, int* result, int p3);

struct timespec get_time(void);
struct timespec time_diff(struct timespec* ts1, struct timespec* ts2);
struct timespec get_duration(struct timespec* ts);
void print_time(struct timespec* ts);

void fillMatrice(int* matrice, int matrice_size);
void printMatrice(int* matrice, int matrice_size);
int checkMatrice(int* matrice, int matrice_size);

#endif
