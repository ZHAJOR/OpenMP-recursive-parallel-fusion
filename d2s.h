#ifndef D2S_H
#define D2S_H
#define _POSIX_C_SOURCE 199309L
#include <time.h>

void sequential_merge(int* array, unsigned l, unsigned m, unsigned h);

struct timespec get_time(void);
struct timespec time_diff(struct timespec* ts1, struct timespec* ts2);
struct timespec get_duration(struct timespec* ts);
void print_time(struct timespec* ts);

void fillMatrice(int* matrice, int matrice_size);
void printMatrice(int* matrice, int matrice_size);
int checkMatrice(int* matrice, int matrice_size);

#endif
