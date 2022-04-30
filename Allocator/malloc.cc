#include <iostream>
#include <cstdint>
#include "a10p1.h"

// the freelist pointer, initialized to point at the start of arena
int64_t * freelist = arena();

// helper function for mymalloc
// traverse the freelist through the given node until a block that's
// large enough for allocation is found
// here cur_node is not pointer to a block, rather the pointer storing
// the address that points to a block; recall the second word of a free
// block points to the next free block, cur_node is the address storing
// the second word
// also note that allo_size is size needed in bytes including the word
// for book keeping
int64_t * allochelper(int64_t allo_size, int64_t * cur_node) {
    if (*cur_node == 0) {
        // reached the end of freelist, no space is large enough
        return NULL;
    }
    else {
        // frag_node is the actual start of the fragment currently looking at
        int64_t * frag_node = reinterpret_cast<int64_t *>(*cur_node);
        int64_t ori_size = *frag_node;
        
        // the case that the current fragment is big enough
        if (ori_size >= allo_size) {
            int64_t next = *(frag_node + 1);
            int64_t * allo = frag_node;
            *allo = allo_size;
            // consider when the current fragment is used up by this allocation
            if (ori_size == allo_size) {
                // want cur_node to point to the next free fragment
                *cur_node = next;
            }
            else {
                // fragment not used up, want cur_node to point right pass allocated
                // memory
                frag_node = frag_node + allo_size / 8;
                *frag_node = ori_size - allo_size;
                // if reamining block smaller than 16 bytes, allocate them too
                if (*frag_node < 16) {
                    *allo = *allo + *frag_node;
                    *cur_node = next;
                }
                else {
                    *(frag_node + 1) = next;
                    *cur_node = reinterpret_cast<int64_t>(frag_node);
                }
            }
            return allo;
        }
        else {
            // recurse to continue traverse the freelist
            return allochelper(allo_size, frag_node + 1);
        }
    }
}

// allocates a block of memory of at least size words (not bytes), 
// and returns the address of that memory or 0 (NULL) if memory could not be allocated.
int64_t *mymalloc(int64_t size) {
    // in this case must be that no memory is available
    if (freelist == NULL) {
        return NULL;
    }
    
    // store the current size of the free blocks
    int64_t original_size = *freelist;
    // store the address of the next free block fragment
    int64_t next_block = *(freelist + 1);
    
    int64_t * allo = NULL;
    
    // want to allocate size + 1 words, extra word for book keeping
    // so the number of bytes to be allocated is given below
    int64_t allo_size = size * 8 + 8;
    
    if (original_size >= allo_size) {
        // current block large, enough, allocate
        allo = freelist;
        *allo = allo_size;
        if (original_size == allo_size) {
            if (next_block != 0) {
                // used up current block, freelist pointer point to next
                // free block
                freelist = reinterpret_cast<int64_t *>(next_block);
            }
            else {
                // in this case the arena will be used up
                freelist = NULL;
            }
        }
        else {
            // the current fragment is greater than required, increment freelist pointer
            // to point at the address just pass the allocated words
            freelist = freelist + size + 1;
            *freelist = original_size - allo_size;
            // the convention I am using is that if the remaining block is not large
            // enough to hold 2 words (16 bytes), which is the minimum needed to
            // allocate space for a word, I will not split, I will just allocate the
            // entire block
            if (*freelist < 16) {
                *allo = *allo + *freelist;
                freelist = reinterpret_cast<int64_t *>(next_block);
            }
            else {
                *(freelist + 1) = next_block;
            }
        }
    }
    else {
        // current block not big enough, traverse the freelist to find block large
        // enough for allocation, using first fit heuristic, allocate the first
        // block found that is large enough
        allo = allochelper(allo_size, freelist + 1);
    }
    return allo + 1;
}


// helper function for myfree
// want to free address and insert it in the freelist in a way that the freelist
// is sorted in increasing order with respect to their location in memory
// traverse the freelist through the given node cur_node
// note that cur_node points to the address of a free block, it is not a pointer
// to the block
// here address is not the address passed to the caller of myalloc, instead it is
// the full block allocated including the book keeping word
void freehelper(int64_t * address, int64_t * cur_node) {
    if (*cur_node == 0) {
        // at the end of freelist
        // check if need to merge
        if (cur_node - 1 + *(cur_node - 1) / 8 == address) {
            *(cur_node - 1) = *(cur_node - 1) + *address;
            return;
        }
        else {
            // already at the end of the list, append address at the end
            *(address + 1) = 0;
            *cur_node = reinterpret_cast<int64_t>(address);
            return;
        }
    }
    else if (reinterpret_cast<int64_t *>(*cur_node) > address) {
        // found a spot to insert
        // check if need to merge
        int64_t * next = reinterpret_cast<int64_t *>(*cur_node);
        int64_t * prev = cur_node - 1;
        if (prev + *prev / 8 == address && address + *address / 8 == next) {
            // update size of this merged block
            *prev = *prev + *address + *next;
            *cur_node = *(next + 1);
            return;
        }
        else if (prev + *prev / 8 == address) {
            // merge prev and address only
            *prev = *prev + *address;
            return;
        }
        else if (address + *address / 8 == next) {
            // merge address and next only
            *address = *address + *next;
            *(address + 1) = *(next + 1);
            *cur_node = reinterpret_cast<int64_t>(address);
        }
        else {
            *cur_node = reinterpret_cast<int64_t>(address);
            *(address + 1) = reinterpret_cast<int64_t>(next);
            return;
        }
    }
    else {
        // continue to traverse the freelist
        int64_t * next_frag = reinterpret_cast<int64_t *>(*cur_node) + 1;
        freehelper(address, next_frag);
        return;
    }
}

// deallocates the memory stored at address.
// assumes that address contains either an address allocated by mymalloc, in which case it deallocates that memory, 
// or the value 0 (NULL), in which case myfree does nothing.
void myfree(int64_t *address) {
    if (address == NULL) {
        return;
    }
    
    if (freelist == NULL) {
        // the arena was all filled, 
        freelist = address - 1;
        *address = 0;
        return;
    }
    // retreive size of the block
    int64_t size = *(address - 1);
    // get the starting address of this previous allocated block
    int64_t * allo_address = address - 1;
    
    if (freelist > allo_address) {
        if (freelist == allo_address + size / 8) {
            // adjacent blocks
            // address should store the next fragment freelist + 1 stores
            *(allo_address + 1) = *(freelist + 1);
            *allo_address = size + *freelist;
            freelist = allo_address;
            return;
        }
        else {
            // want the blocks to be in increasing order of address
            // insert freed address as head of the free blocks
            int64_t * next_block = freelist;
            freelist = allo_address;
            *(freelist + 1) = reinterpret_cast<int64_t>(next_block);
            return;
        }
    }
    else {
        freehelper(allo_address, freelist + 1);
    }
    return;
}