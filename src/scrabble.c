//
// Created by nirin on 28/08/2023.
//

#include "scrabble.h"
extern int valueScrabble[MAX_SIZE];

int valueOfWord(char* word){
    unsigned int len = strlen(word);
    int value = 0;
    for(unsigned int i = 0; i < len; i++){
        value += valueScrabble[word[i] - 'a']; //uses the equivalent array letter<=>value at the letter position
    };
    return value;
}
int* wordToArray(char* word){
    int* array = malloc(26*sizeof(int));
    for(int j = 0; j < 26; j++){
        array[j] = 0;
    }
    unsigned int len = strlen(word);
    for(unsigned int i = 0; i < len; i++){
        array[word[i]-'a']++;
    }
    return array;
}
void shuffle(char* array, int length) {
    srand(time(NULL));
    for (int i = length - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

char* pickLetters(char* deck){
    char scrabbleAlphabet[93] = "AAAAAAAAABBCCDDDEEEEEEEEEEEEFFGGGHHIIIIIIIIIJKLLLMMNNNNNNOOOOOOOOPPQRRRRRRSSSSSSTTTTTTUUVWXYZ";
    //shuffle(scrabbleAlphabet, 93);
    srand(time(NULL));

    for(int i = 0; i < DECK_SIZE; i++){
        int r = rand() % 93; //102 to fit in the scrabbleAlphabet
        deck[i] = tolower(scrabbleAlphabet[r]); //pick a random letter in the scrabbleAlphabet and put it in the deck
        //printf("%c", deck[i]);
    }
    return deck;
}
bool wordValid(struct NodeTrie* trie, char* word, char* deck){
    bool exists = belongs(trie, word);
    int* wordedArray = wordToArray(deck);
    unsigned int len = strlen(word);

    for(unsigned int i = 0; i < len; i++){
        int index = word[i] - 'a';
        if(wordedArray[index] == 0){ //Your word contains a letter which is not in the deck
            free(wordedArray);
            return false;
        }
        else{ //the letter exists in the deck
            wordedArray[index]--;
        }
    }
    //All the letters choosen belongs to the deck
    free(wordedArray);
    if(exists){
        return true;
    }
    else return false;
}

/*void findLongestWordInTrie(struct NodeTrie* trie, int wordedDeck[26], char currentWord[DECK_SIZE], char* longestWord, struct NodeTrie* originalTrie) {
    // Vérifier si le mot actuel est plus long que le plus long mot trouvé jusqu'à présent
    if (strlen(currentWord) > strlen(longestWord) && belongs(originalTrie, currentWord)) {
        memset(longestWord, 0, strlen(longestWord));
        strncpy(longestWord, currentWord, 7);
        longestWord[7] = '\0'; // Assurer la terminaison correcte
    }

    // Explorer toutes les lettres possibles dans le deck
    for (int i = 0; i < MAX_SIZE; i++) {
        if (wordedDeck[i] > 0 && trie->alphabets[i] != NULL) {
            char nextLetter = i + 'a';

            // Créer une copie locale du wordedDeck pour cet appel récursif
            int wordedDeckCopy[26];
            memcpy(wordedDeckCopy, wordedDeck, sizeof(wordedDeckCopy));

            // Utiliser la lettre du wordedDeck
            wordedDeckCopy[i]--;

            // Construire le prochain mot en ajoutant la lettre
            char newWord[8] = "";
            snprintf(newWord, sizeof(newWord), "%s%c", currentWord, nextLetter);

            findLongestWordInTrie(trie->alphabets[i], wordedDeckCopy, newWord, longestWord, originalTrie);
        }
    }

    //I did not found a valid word and used all the letters from the deck
    memset(currentWord, 0, strlen(currentWord));
    return;
}*/
void printDeck(char deck[DECK_SIZE]){
    for(int i = 0; i < DECK_SIZE; i++){
        printf("%c-", deck[i]);
    }
}

void findLongestWordInTrie(struct NodeTrie* trie, int wordedDeck[26], char currentWord[DECK_SIZE], char* longestWord, struct NodeTrie* originalTrie) {
    // Si toutes les lettres du deck sont épuisées, sortir de la recherche
    bool allLettersExhausted = true;
    for (int i = 0; i < MAX_SIZE; i++) {
        if (wordedDeck[i] > 0) {
            allLettersExhausted = false;
            break;
        }
    }
    if (allLettersExhausted) {
        return;
    }

    // Vérifier si le mot actuel est plus long que le plus long mot trouvé jusqu'à présent
    if (strlen(currentWord) > strlen(longestWord) && belongs(originalTrie, currentWord)) {
        strncpy(longestWord, currentWord, DECK_SIZE);
        longestWord[DECK_SIZE] = '\0'; // Assurer la terminaison correcte
    }

    // Explorer toutes les lettres possibles dans le deck
    for (int i = 0; i < MAX_SIZE; i++) {
        if (wordedDeck[i] > 0 && trie->alphabets[i] != NULL) {
            char nextLetter = i + 'a';

            // Utiliser la lettre du wordedDeck
            wordedDeck[i]--;

            // Construire le prochain mot en ajoutant la lettre
            char newWord[DECK_SIZE];
            snprintf(newWord, sizeof(newWord), "%s%c", currentWord, nextLetter);

            // Appel récursif pour explorer le nœud suivant
            findLongestWordInTrie(trie->alphabets[i], wordedDeck, newWord, longestWord, originalTrie);

            // Restaurer le wordedDeck après l'appel récursif
            wordedDeck[i]++;
        }
    }
}



















