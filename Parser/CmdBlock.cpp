#ifndef CMDBLCK
#define CMDBLCK

//
struct CmdObj {
    unsigned type:4;
    unsigned encoding:4;
    unsigned lru; /* LRU time (relative to global lru_clock) or
                            * LFU data (least significant 8 bits frequency
                            * and most significant 16 bits access time). */
    int refcount;
    void *ptr;
};

typedef struct CmdlistNode {
    struct CmdlistNode *prev;
    struct CmdlistNode *next;
    unsigned char *entry;
    size_t sz;             /* entry size in bytes */
    unsigned int count : 16;     /* count of items in listpack */
    bool recompress; /* was this node previous compressed? */
    unsigned int attempted_compress : 1; /* node can't compress; too small */
    unsigned int dont_compress : 1; /* prevent compression of entry that will be used later */
    unsigned int extra : 9; /* more bits to steal for future usage */
};

#endif