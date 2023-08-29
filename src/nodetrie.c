//
// Created by nirin on 28/08/2023.
//

#include "nodetrie.h"
extern int valueScrabble[MAX_SIZE];

struct NodeTrie* createEmptyNodeTrie(int value){
    struct NodeTrie* trie = malloc(sizeof(struct NodeTrie));
    trie->value = value;
    memset(trie->alphabets, 0, MAX_SIZE);
    trie->isWord = false;
    return trie;
}
void insertWord(struct NodeTrie* trie, char* word){

    unsigned int len = strlen(word);

    if(trie == NULL || len == 0){
        return;
    }
    for(unsigned int i = 0; i < len; i++){
        if(trie->alphabets[word[i] -'a'] != NULL){ //Si la lettre existe
            trie = trie->alphabets[word[i] - 'a'];
        }
        else{ //create the letter
            int value = valueScrabble[word[i] -'a'];
            trie->alphabets[word[i] -'a'] = createEmptyNodeTrie(value); //create the Node with the corresponding letter<->value equivalence
            trie = trie->alphabets[word[i] -'a']; //move to the created Node
        }
    }
    trie->isWord = true; //inserted all the letters
}
bool belongs(struct NodeTrie* trie, char* word){
    unsigned int len = strlen(word);
    if(trie == NULL || len == 0 ){
        return false;
    }
    for(unsigned int i = 0; i < len; i++){
        if(trie->alphabets[word[i] -'a'] == NULL){
            return false;
        } //letter doesnt exist thus the word neither
        else{
            trie = trie->alphabets[word[i] -'a'];
        }
    }
    if(trie->isWord == true){
        return true;
    } //test the last Node if its a word
    else return false;
}
void deleteNodeTrie(struct NodeTrie** trie){
    if(trie == NULL){
        return;
    }
    for(int i = 0; i < MAX_SIZE; i++){
        if((*trie)->alphabets[i] != NULL) {
            deleteNodeTrie(&(*trie)->alphabets[i]);
        }
    }
    free(*trie);
    *trie = NULL;
}
struct NodeTrie* createDico(char* dicoName){
    FILE* dico;
    dico = fopen(dicoName, "r");
    char word[MAX_BUFFER];
    struct NodeTrie* trie = createEmptyNodeTrie(0);

    while(!feof(dico)){
        fgets(word, MAX_BUFFER, dico); //read line by line the dico
        word[strcspn(word, "\r")] = '\0'; //retire l'éventuel "\n"
        insertWord(trie, word);
    }
    fclose(dico);
    return trie;
}

