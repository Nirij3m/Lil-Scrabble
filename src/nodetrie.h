//
// Created by nirin on 28/08/2023.
//

#ifndef LIL_SCRABBLE_NODETRIE_H
#define LIL_SCRABBLE_NODETRIE_H
#define MAX_SIZE 26
#define MAX_BUFFER 326

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct NodeTrie {
    struct NodeTrie* alphabets[MAX_SIZE]; //letter <=> position in the alphabet//
    int value; //value of letter at the scrabble
    bool isWord;
};

struct NodeTrie* createEmptyNodeTrie(int value);
void insertWord(struct NodeTrie* trie, char* word);
bool belongs(struct NodeTrie* trie, char* word);
struct NodeTrie* createDico(char* dicoName);
void deleteNodeTrie(struct NodeTrie** trie);
#endif //LIL_SCRABBLE_NODETRIE_H
