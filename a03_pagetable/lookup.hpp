#pragma once
#include <cstdint>
#include <cmath>

#define MAX_BF 32 // number of bits in the logical address
#define DEF_INT 0 // the default integer value
#define DEF_DBL 0. // likewise for double values
#define DEF_BOOL false // likewise for booleans

typedef uint32_t u32; // shortcut

// need the forward declarations
class Level;
class Map;

class Pagetable {
public:
    /**
     * levelCount - #of levels
     * shift - array containing amounts to shift masks by
     * entry - array specifying the #of entries in each Level
     */
    int levelCount, *shift, *entry;
    u32 *bitmask; // array containing each Level's bitmask
    Level *zero; // the first (0th) level

    Pagetable (int *bitfield, int levelCount);

    void pageInsert(u32 logAddr, u32 frame);
    Map *pageLookup(u32 logAddr);
};

class Level {
public:
    int depth;
    bool isLeaf;
    Pagetable *table; // backwards reference
    // TODO: use a union dummy!
    // one of these two will be used
    Level **next; // for branches
    Map *map; // for leaves

    Level (int entries, int depth, Pagetable *table);

    void pageInsert(u32 logAddr, u32 frame);
};

class Map {
public:
    bool valid = false; // valid bit
    u32 frame, page; // reference to frame and page#
};

u32 logicalToPage(u32 logAddr, u32 mask, u32 shift);
u32 mask (int bitfield);
long int getTBSize();
long int getUsed();
