#include "lookup.hpp"

// lacal variables to track memory
long int tableSize, used;

// returns the page-table size
long int getTBSize() {
    return tableSize;
}

// returns the #of bytes used
long int getUsed() {
    return used;
}

// creates a mask for a given int
u32 mask (int bitfield) {
    return (1 << bitfield) - 1;
}

// instance method for Pagetable
Pagetable::Pagetable (int *bitfield, int levelCount) {
    tableSize = used = DEF_INT;

    this->levelCount = levelCount;
    // init new arrays
    bitmask = new u32[levelCount];
    shift = new int[levelCount];
    entry = new int[levelCount];
    
    shift[0] = MAX_BF;
    // fills each array with the right values for each level
    for (int i = 0; i < levelCount; i++) {
        shift[i] -= bitfield[i];
        bitmask[i] = mask(bitfield[i]) << shift[i];
        entry[i] = pow(2, bitfield[i]);
        shift[i + 1] = shift[i];
    }

    // entry[0] = #of level 1 entries
    zero = new Level(entry[0], DEF_INT, this);
    tableSize += sizeof(Pagetable);
    used = tableSize + sizeof(Level);
}

// instance method for Level
Level::Level (int entries, int depth, Pagetable *table) {
    this->depth = depth;
    this->table = table;

    if (depth < table->levelCount) { // branch node
        isLeaf = false;
        next = new Level *[entries];
        for (int i = 0; i < entries; i++) {
            next[i] = nullptr;
        }
        tableSize += sizeof(Level) + sizeof(Level *) * entries;
    } else { // leaf node, has Map, not Levels
        isLeaf = true;
        map = new Map();
        tableSize += sizeof(Level) + sizeof(Map);
    }
}

// returns page# @ any level
u32 logicalToPage(u32 logAddr, u32 mask, u32 shift) {
    return (logAddr & mask) >> shift;
}

// return correct page from table
Map *Pagetable::pageLookup(u32 logAddr) {
    Level *temp = zero;
    while (temp) { // find the leaf node
        if (temp->isLeaf) break;
        temp = temp->next[logicalToPage(logAddr, bitmask[temp->depth], 
            shift[temp->depth])];
    }
    if (temp) { // if it's there, return it
        return temp->map;
    } else { // return invalid
        return new Map();
    }
}

// "overload" for the other pageInsert()
void Level::pageInsert(u32 logAddr, u32 frame) {
    if (isLeaf) {
        // set appropriate page index to valid, store frame
        map->valid = true;
        map->frame = frame;
        map->page = logAddr;
        used += sizeof(Map);
    } else {
        // create a new Level and set level to current depth + 1
        u32 page = logicalToPage(logAddr, table->bitmask[depth], 
            table->shift[depth]);
        next[page] = new Level(table->entry[depth + 1], depth + 1, table);
        used += sizeof(Level) + sizeof(Level *);
        // the constructor inits the Map/Levels
        next[page]->pageInsert(logAddr, frame);
    }
}

// insert a page, starting from level 0
void Pagetable::pageInsert(u32 logAddr, u32 frame) {
    zero->pageInsert(logAddr, frame);
}
