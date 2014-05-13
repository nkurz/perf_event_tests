#pragma once

#include "types.h"

extern unsigned int seed;
unsigned int init_seed(unsigned int seed);
void set_seed(unsigned int pidslot);
void reseed(void);
unsigned int new_seed(void);

unsigned int rand_bool(void);
unsigned int rand32(void);
u64 rand64(void);
int rand_range(int min, int max);

void generate_random_page(char *page);
