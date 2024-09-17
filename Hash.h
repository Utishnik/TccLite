#pragma once
#include "tools.h"
#include "types.h"

const short Size_Upper_To_Lower=256;
unsigned char sqlite3UpperToLower[Size_Upper_To_Lower];

struct __ht {              /* the hash table */
  unsigned int count;        /* Number of entries with this hash */
  HashElem *chain;           /* Pointer to first entry with this hash */
};

struct HashElem {
  HashElem *next, *prev;       /* Next and previous elements in the table */
  void *data;                  /* Data associated with this element */
  const char *pKey;            /* Key associated with this element */
};

struct Hash {
  unsigned int htsize;      /* Number of buckets in the hash table */
  unsigned int count;       /* Number of entries in this table */
  HashElem *first;          /* The first element of the array */
  struct __ht *ht;
};

static HashElem *findElementWithHash(
  const Hash *pH,     /* The pH to be searched */
  const char *pKey,   /* The key we are searching for */
  unsigned int *pHash /* Write the hash value here */
);

void Fill_Upper_To_Lower();