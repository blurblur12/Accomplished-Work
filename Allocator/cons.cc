#include <iostream>
#include <cstdint>
#include "a10p1.h"
#include "a10p2.h"

// the freelist pointer, initialized to point at the start of arena
int64_t * freelist = arena();

// helper function for cons
// traverse the freelist through the given node cur_node until finds a block
// large enough to hold 16 bytes (two words)
// node that here cur_node is not the address of the start of the fragment
// instead *cur_node = address of the fragment
// so this is essentially the address of the second word of a free block
int64_t *conshelper(int64_t a, int64_t *b, int64_t * cur_node) {
    if (*cur_node == 0) {
        // traversed the whole freelist but did not find big enough block
        // to allocate, return null
        return NULL;
    }
    else {
        // frag_node is the actual start of the fragment currently looking at
        int64_t * frag_node = reinterpret_cast<int64_t *>(*cur_node);
        int64_t ori_size = *frag_node;
        
        // the case that the current fragment is big enough
        if (ori_size >= 16) {
            int64_t next = *(frag_node + 1);
            int64_t * pair = frag_node;
            *pair = a;
            *(pair + 1) = reinterpret_cast<int64_t>(b);
            // consider when the current fragment is used up by this allocation
            if (ori_size == 16) {
                // want cur_node to point to the next free fragment
                *cur_node = next;
            }
            else {
                // fragment not used up, want cur_node to point right pass pair
                frag_node = frag_node + 2;
                *cur_node = reinterpret_cast<int64_t>(frag_node);
                *frag_node = ori_size - 16;
                *(frag_node + 1) = next;
            }
            return pair;
        }
        else {
            // recurse to continue traverse the freelist
            return conshelper(a, b, frag_node + 1);
        }
    }
}
//returns the address of a pair of words from the arena, initialized to a and b respectively, or 0 if no memory is available.
int64_t *cons(int64_t a, int64_t *b) {
    // in this case must be that no memory is available
    if (freelist == NULL) {
        return 0;
    }
    
    // store the current size of the free blocks
    int64_t original_size = *freelist;
    // store the address of the next free block fragment
    int64_t next_block = *(freelist + 1);
    
    int64_t * pair = NULL;
    
    // want to allocate 2 words, so 16 bytes needed
    if (original_size >= 16) {
        // store the pair into allocated block
        pair = freelist;
        *pair = a;
        *(pair + 1) = reinterpret_cast<int64_t>(b);
        if (original_size == 16) {
            if (next_block != 0) {
                // used up the a 2-word fragment, freelist points to the next
                // fragment
                freelist = reinterpret_cast<int64_t *>(next_block);
            }
            else {
                // in this case the arena will be used up
                freelist = NULL;
            }
        }
        else {
            // the current fragment is greater than 16 bytes, increment freelist pointer
            // to point at the address just pass the allocated two words
            freelist = freelist + 2;
            *freelist = original_size - 16;
            *(freelist + 1) = next_block;
        }
    }
    else {
        // the current fragment is not large enough to hold two words, look
        // for next fragment
        if (next_block != 0) {
            pair = conshelper(a, b, freelist + 1);
        }
    }
    return pair;
}

//returns the first element of the pair whose address is p.
int64_t car(int64_t *p) {
    return *p;
}

//returns the second element of the pair whose address is p.
int64_t *cdr(int64_t *p) {
    int64_t second = *(p + 1);
    return reinterpret_cast<int64_t *>(second);
}

//sets the first element of p to the value v and returns p.
int64_t *setcar(int64_t *p, int64_t v) {
    *p = v;
    return p;
}

//sets the second element of p to the value v and returns p.
int64_t *setcdr(int64_t *p, int64_t *v) {
    *(p + 1) = reinterpret_cast<int64_t>(v);
    return p;
}


// helper function for snoc, given a pair to be freed p, a current node
// in the freelist, want to free p and insert it at the appropriate spot
// note that here cur_node is not really the "current" node in the list 
// it's not the address that points to the start of a fragment
// recall that for each fragment, the first word stores the size of the
// free block and the second word store address pointing to the next
// block, the cur_node is actually the address storing this second word
// which storing the address pointing to the fragment that we are going
// to be looking at
void snochelper(int64_t *p, int64_t * cur_node) {
    if (*cur_node == 0) {
        // at the end of freelist
        // check if need to merge
        if (cur_node - 1 + *(cur_node - 1) / 8 == p) {
            *(cur_node - 1) = *(cur_node - 1) + *p;
            return;
        }
        else {
            // already at the end of the list, append p at the end
            *(p + 1) = 0;
            *cur_node = reinterpret_cast<int64_t>(p);
            return;
        }
    }
    else if (reinterpret_cast<int64_t *>(*cur_node) > p) {
        // found a spot to insert
        // check if need to merge
        int64_t * next = reinterpret_cast<int64_t *>(*cur_node);
        int64_t * prev = cur_node - 1;
        if (prev + *prev / 8 == p && p + 2 == next) {
            // update size of this merged block
            *prev = *prev + *p + *next;
            *(cur_node) = *(next + 1);
            return;
        }
        else if (prev + *prev / 8 == p) {
            // merge prev and address only
            *prev = *prev + *p;
            return;
        }
        else if (p + 2 == next) {
            // merge address and next only
            *p = *p + *next;
            *(p + 1) = *(next + 1);
            *cur_node = reinterpret_cast<int64_t>(p);
            return;
        }
        else {
            *cur_node = reinterpret_cast<int64_t>(p);
            *(p + 1) = reinterpret_cast<int64_t>(next);
            return;
        }
    }
    else {
        // continue to traverse the freelist
        int64_t * next_frag = reinterpret_cast<int64_t *>(*cur_node) + 1;
        snochelper(p, next_frag);
        return;
    }
}
//deletes the pair whose address is p, or does nothing if p is null.
void snoc(int64_t *p) {
    if (p == NULL) {
        return;
    }
    // deleting the pair would result a free block fragment of 16 bytes
    *p = 16;
    
    if (freelist == NULL) {
        // the arena was all filled, 
        *(p + 1) = 0;
        freelist = p;
        return;
    }
    if (freelist > p) {
        if (freelist == p + 2) {
            // adjacent blocks
            // p + 1 should store the next fragment freelist + 1 stores
            *(p + 1) = *(freelist + 1);
            *p = *p + *freelist;
            freelist = p;
            return;
        }
        else {
            // want the blocks to be in increasing order of address
            // insert freed p as head of the free blocks
            int64_t * next_block = freelist;
            freelist = p;
            *(freelist + 1) = reinterpret_cast<int64_t>(next_block);
            return;
        }
    }
    else {
        int64_t * next = freelist + 1;
        snochelper(p, next);
    }
}
