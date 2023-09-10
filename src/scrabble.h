//
// Created by nirin on 28/08/2023.
//

#ifndef LIL_SCRABBLE_SCRABBLE_H
#define LIL_SCRABBLE_SCRABBLE_H
#include "nodetrie.h"
#include <ctype.h>
#define DECK_SIZE 7
int valueOfWord(char* word);
int* wordToArray(char* word); //Translate the word to an array contaning the number of time the letters of the word repete themself
void shuffle(char* array, int length); //shuffle the letters 'int alphabetLength = sizeof(alphabet) - 1;' for the length
char* pickLetters(char* deck); //Pass the deck as a parameter to fill it
bool wordValid(struct NodeTrie* trie, char* word, char* deck); //check if the word exists and is composed of all the letters from the deck
void findLongestWordInTrie(struct NodeTrie* trie, int wordedDeck[26], char currentWord[DECK_SIZE], char* longestWord, struct NodeTrie* originalTrie);
void findHighestValueWordInTrie(struct NodeTrie* trie, int wordedDeck[26], char currentWord[DECK_SIZE], char* highestValueWord, int* highestValue, struct NodeTrie* originalTrie);
void printDeck(char deck[DECK_SIZE]);
void toLowerString(char string[]);
void toUpperString(char string[]);

#endif //LIL_SCRABBLE_SCRABBLE_H
