#include <stdio.h>
#include "src/nodetrie.h"
#include "src/scrabble.h"

int valueScrabble[26] = { //equivalent array letter <=> value at the Scrabble
        1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10
};

void debugFunction();
int main() {

    



    return 0;
}

void debugFunction(){
    struct NodeTrie* trie = createDico("dicoFR.txt");
    char deck[DECK_SIZE] = "";
    pickLetters(deck);
    printDeck(deck);
    int* wordedDeck = wordToArray(deck);

    //bool valid = wordValid(trie, "zizanie", deck);
    //printf("%d\n", valid);


    char longestWord[DECK_SIZE] = ""; // Stocker le mot le plus long trouvé
    char highestValueWord[DECK_SIZE] = ""; // Pour stocker le mot de plus grande valeur
    int highestValue = 0; // Pour stocker la plus grande valeur trouvée

    findLongestWordInTrie(trie, wordedDeck, "", longestWord, trie);
    findHighestValueWordInTrie(trie, wordedDeck, "", highestValueWord, &highestValue, trie);

    printf("Longest word: %s\n", longestWord);
    printf("Highest Value Word: %s\n", highestValueWord);
    printf("Value: %d\n", highestValue);


    deleteNodeTrie(&trie);
    free(wordedDeck);
}