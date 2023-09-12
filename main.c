
#include<scrabble.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdbool.h>

int valueScrabble[26] = { //equivalent array letter <=> value at the Scrabble
        1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10
};


WINDOW* gameInput(struct NodeTrie* dico, char* userInput, char* mainDeck, int maxRow, int maxCol, int inputTextRow, int inputTextCol, int centerCol, char* uGameScoreChar, char* cGameScoreChar); //Gather the user's input word and checks its validity
void drawDeck(int row, int col, char deck[], int size, char* label);
void drawLine(int row, int col, int length);
void eraseLine(int row, int col, int length);
void playGame(struct NodeTrie* newDico, int uGameScore, int cGameScore, bool end);

void drawDeck(int row, int col, char deck[], int size, char* label) {
    if(size == 0){
        strcpy(deck, "LOOSER");
        size = 6;
    }
    int i;
    for (i = 0; i < size; i++) {
        mvaddch(row, col + i * 2, ACS_ULCORNER);    // Coin supérieur gauche du rectangle
        mvaddch(row, col + i * 2 + 1, ACS_HLINE);   // Ligne horizontale du rectangle
        mvaddch(row + 1, col + i * 2, ACS_VLINE);   // Ligne verticale gauche du rectangle
        mvaddch(row + 1, col + i * 2 + 1, deck[i]); // Lettre au milieu du rectangle
        mvaddch(row + 2, col + i * 2, ACS_LLCORNER);  // Coin inférieur gauche du rectangle
        mvaddch(row + 2, col + i * 2 + 1, ACS_HLINE);// Ligne horizontale du rectangle
        if (i == size - 1) {
            mvaddch(row, col + i * 2 + 2, ACS_URCORNER);  // Coin supérieur droit du dernier rectangle
            mvaddch(row + 1, col + i * 2 + 2, ACS_VLINE);  // Ligne verticale droite du dernier rectangle
            mvaddch(row + 2, col + i * 2 + 2, ACS_LRCORNER);  // Coin inférieur droit du dernier rectangle
        }

    }
    mvprintw(row - 1, col, label); // Afficher l'étiquette au-dessus du deck
}

void drawLine(int row, int col, int length) {
    int i;
    for (i = 0; i < length; i++) {
        mvaddch(row, col + i, ACS_HLINE);
    }
    refresh();
}

void eraseLine(int row, int col, int length) {
    int i;
    for (i = 0; i < length; i++) {
        mvaddch(row, col + i, ' '); // Effacer la ligne en la remplissant d'espaces
    }
    refresh();
}


WINDOW* gameInput(struct NodeTrie* dico, char* userInput, char* mainDeck, int maxRow, int maxCol, int inputTextRow, int inputTextCol, int centerCol, char* uGameScoreChar, char* cGameScoreChar) {
    // Initialiser une fenêtre pour afficher l'entrée de l'utilisateur en bas à gauche
    WINDOW *inputWin = newwin(1, maxCol - 1, maxRow - 1, 0);
    keypad(inputWin, TRUE);
    wrefresh(inputWin);
    int userInputLength = 0;
    int end = 0;
    mvprintw(0, maxCol - 12, "%s", "Scores:");
    mvprintw(1, maxCol - 12,"You: %s", uGameScoreChar);
    mvprintw(2, maxCol - 12,"Computer: %s", cGameScoreChar);
    wrefresh(inputWin);
    while (end == 0) {
        // Rafraîchir l'écran
        mvprintw(0, 1, "- Press F1 to get the answer -");
        int ch1 = getch();
        if(ch1 == KEY_F(1)){ //User want the answer
            memset(userInput, 0, userInputLength);
            return inputWin;
        }
        // Entrée de l'utilisateur
        if (ch1 != ERR) {
                // Lire et afficher ce que l'utilisateur tape en temps réel
                int ch;
                int col = 0;
                // Afficher la première touche saisie
                mvwaddch(inputWin, 0, col, ch1);
                col++;
                userInput[userInputLength] = ch1; // Stocker le caractère dans la chaîne
                userInputLength++; // Incrémenter la longueur de la chaîne
                while (1) {
                    ch = wgetch(inputWin);

                    if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127) { // Gérer la suppression
                        if (col > 0) {
                            col--;
                            mvwaddch(inputWin, 0, col, ' '); // Effacer le caractère précédent
                            userInputLength--; // Décrémenter la longueur de la chaîne
                        }
                    } else if (col < maxCol - 2) { // Vérifier la limite de la fenêtre
                        mvwaddch(inputWin, 0, col, ch);
                        col++;
                        userInput[userInputLength] = ch; // Stocker le caractère dans la chaîne
                        userInputLength++; // Incrémenter la longueur de la chaîne
                        userInput[userInputLength] = '\0';
                    }
                    if (ch == '\n'){ //Mot entré
                        userInput[userInputLength-1] = '\0';
                        // Convertir userInput en minuscules
                        toLowerString(userInput);
                        toLowerString(mainDeck);
                        if(wordValid(dico, userInput, mainDeck)){ //Etapes de validation
                            end = 1; //BREAK THE LOOP
                            break;
                        }
                        else{ //Word Incorrect, retype the word
                            memset(userInput, 0, userInputLength);
                            eraseLine(inputTextRow+1, inputTextCol, maxCol - inputTextCol);
                            mvprintw(inputTextRow+1, inputTextCol, " ");
                            userInputLength = 0;
                            wclear(inputWin);
                            break;
                        }

                    }
                }
             }

    }
    return inputWin;
}

void playGame(struct NodeTrie* newDico, int uGameScore, int cGameScore, bool end) {
    // NCurse init
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    //Window related definition
    int maxRow, maxCol;
    getmaxyx(stdscr, maxRow, maxCol);
    int inputTextRow = maxRow - 2;
    int inputTextCol = 0;
    int centerCol = (maxCol - DECK_SIZE * 4) / 2;

    //Init dico
    struct NodeTrie* dico;
    if(newDico == NULL){
        dico = createDico("dicoFR.txt");
        if(dico == NULL){
            printf("Error openning the file!");
            return;
        }
    }
    char userInput[256] = ""; // input's buffer
    char computerWord[DECK_SIZE+1] = "";
    char mainDeck[DECK_SIZE] = "";
    pickLetters(mainDeck);
    char currentWord[DECK_SIZE] = "";

    //Prints the scores
    char uGameScoreChar[2] = "";
    char cGameScoreChar[2] = "";
    sprintf(uGameScoreChar, "%d", uGameScore);
    sprintf(cGameScoreChar, "%d", cGameScore);
    uGameScoreChar[1] = '\0';
    cGameScoreChar[1] = '\0';

    // Draws computer's label
    clear();
    mvprintw(4, centerCol, "Ordinateur");
    drawLine(5, centerCol, 10);
    // Draws main deck label
    mvprintw(9, centerCol, "Tirage");
    toUpperString(mainDeck),
    drawDeck(10, centerCol, mainDeck, DECK_SIZE, "Tirage");
    // Draws user's label
    mvprintw(17, centerCol, "Vous");
    drawLine(18, centerCol, 10);

    //Input text
    mvprintw(inputTextRow, inputTextCol, "Entrer votre mot:");
    refresh();

    // Draw the deck
    int* wordedDeck = wordToArray(mainDeck);
    WINDOW* inputWin = gameInput(dico, userInput, mainDeck, maxRow, maxCol, inputTextRow, inputTextCol, centerCol, uGameScoreChar, cGameScoreChar);
    findLongestWordInTrie(dico, wordedDeck, currentWord, computerWord, dico);
    free(wordedDeck);
    //Score init value
    char userScore[2] = "";
    char computerScore[2] = "";
    int valueUserWord = valueOfWord(userInput);
    int valueComputerWord = valueOfWord(computerWord);
    sprintf(userScore, "%d", valueUserWord);
    sprintf(computerScore, "%d", valueComputerWord);
    userScore[1] = '\0';
    computerScore[1] = '\0';
    unsigned int userLen = strlen(userInput);
    unsigned int computerLen = strlen(computerWord);

    //Format user's output
    eraseLine(18, centerCol, 10);
    toUpperString(userInput);
    drawDeck(15, centerCol, userInput, userLen,"Vous");
    mvprintw(16, centerCol + 20, "%s%s%s", "Your score: ", userScore, "pts");

    //Format computer's output
    eraseLine(5, centerCol, 10);
    toUpperString(computerWord);
    drawDeck(5, centerCol, computerWord, computerLen, "Ordinateur");
    mvprintw(6, centerCol + 20, "%s%s%s", "Computer's score: ", computerScore, "pts");
    if(valueUserWord >= valueComputerWord){
        uGameScore++;
    } else cGameScore++;

    //Ask for replay or not
    int chend = 0;
    mvprintw(20, centerCol, "%s", "Replay? (y/n)");
    refresh();
    while(chend != 121  || chend != 110){ //ASCII(y) = 121 ; ASCII(n) = 110
        chend = getch();
        if(chend == 121){
            deleteNodeTrie(&dico);
            delwin(inputWin);
            playGame(dico, uGameScore, cGameScore, false);
        }
        else if(chend == 110){
            deleteNodeTrie(&dico);
            delwin(inputWin);
            clear();
            return;
        }
    }

}

int main(){
    int uGameScore = 0;
    int cGameScore = 0;
    playGame(NULL, uGameScore, cGameScore, false);
    endwin();
    return 0;
}

