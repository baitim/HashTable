#ifndef _H_HASH_TABLE_
#define _H_HASH_TABLE_
#include <stddef.h>

typedef struct _HTable HTable;

typedef size_t (*hf)(const void*); /* hash function */

typedef void* (*hcopy)(const void*); /* copy function */

typedef int (*heq)(const void* a, const void* b); /* comparator */

typedef void  (*hfree)(void*); /* destructor */

HTable* ht_init(size_t initial_sz, hf h, hcopy c, heq eq, hfree f);

void ht_destroy(HTable*);

int ht_walk(HTable*, int (*cb)(const void*, size_t, size_t, void*), void*);

int ht_get(HTable*, const void* key, void** res);
int ht_set(HTable*, const void* pair, double fill_factor);
int ht_del(HTable*, const void* key);


#endif /* _H_HASH_TABLE_ */
