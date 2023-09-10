
#include</mnt/c/Users/nirin/Documents/GitHub/Lil-Scrabble/src/scrabble.h>
#include <unistd.h>
#include <ncurses.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
int valueScrabble[26] = { //equivalent array letter <=> value at the Scrabble
        1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10
};


void displayScrabbleDecks(struct NodeTrie* dico);
void drawDeck(int row, int col, char deck[], int size, char* label);
void drawLine(int row, int col, int length);
void eraseLine(int row, int col, int length);

void drawDeck(int row, int col, char deck[], int size, char* label) {
    int i;
    for (i = 0; i < size; i++) {
        mvaddch(row, col + i * 2, ACS_ULCORNER);    // Coin supérieur gauche du rectangle
        mvaddch(row, col + i * 2 + 1, ACS_HLINE);   // Ligne horizontale du rectangle
        mvaddch(row + 1, col + i * 2, ACS_VLINE);   // Ligne verticale gauche du rectangle
        mvaddch(row + 1, col + i * 2 + 1, deck[i]); // Lettre au milieu du rectangle
        mvaddch(row + 2, col + i * 2, ACS_LLCORNER);  // Coin inférieur gauche du rectangle
        mvaddch(row + 2, col + i * 2 + 1, ACS_HLINE);  // Ligne horizontale du rectangle
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


void displayScrabbleDecks(struct NodeTrie* dico) {

    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    // Obtenir les dimensions de la fenêtre
    int maxRow, maxCol;
    getmaxyx(stdscr, maxRow, maxCol);
    // Position du texte "Press F1 to exit"
    int exitTextRow = maxRow - 1;
    int exitTextCol = maxCol - 16;
    // Position du texte "Entrer votre mot:"
    int inputTextRow = maxRow - 2;
    int inputTextCol = 0;

    char userInput[256] = ""; // Déclarer une chaîne de caractères pour stocker l'entrée
    int userInputLength = 0;

    char mainDeck[DECK_SIZE] = "eajeann";
    //pickLetters(mainDeck);
    toUpperString(mainDeck);


    int centerCol = (maxCol - DECK_SIZE * 4) / 2;
    int end = 0;
    while (end == 0) {
        clear();
        userInput[0] = '\0';
        userInputLength = 0;
        // Afficher le deck de l'ordinateur (deck du dessus) avec l'étiquette
        mvprintw(4, centerCol, "Ordinateur");
        drawLine(5, centerCol, 10);
        // Afficher le tirage principal (deck central, de la même taille que les autres)
        mvprintw(9, centerCol, "Tirage");
        drawDeck(10, centerCol, mainDeck, DECK_SIZE, "Tirage");

        // Afficher le deck du joueur (deck du dessous) avec l'étiquette
        mvprintw(17, centerCol, "Vous");
        //drawLine(18, centerCol, 10);
        //drawDeck(18, centerCol, userInput, DECK_SIZE,"Vous");

        // Afficher le texte "Press F1 to exit"
        mvprintw(exitTextRow, exitTextCol, "Press F1 to exit");

        // Afficher le texte "Entrer votre mot:"
        mvprintw(inputTextRow, inputTextCol, "Entrer votre mot:");

        // Rafraîchir l'écran
        refresh();
        int ch1 = getch();
        if (ch1 == KEY_F(1)) {
            break; // Quitter le jeu
        }
        // Entrée de l'utilisateur
        if (ch1 != ERR) {
                // Initialiser une fenêtre pour afficher l'entrée de l'utilisateur en bas à gauche
                WINDOW *inputWin = newwin(1, maxCol - 1, maxRow - 1, 0);
                keypad(inputWin, TRUE);
                wrefresh(inputWin);

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
                    }
                    if (ch == '\n'){ //Mot entré
                        userInput[userInputLength-1] = '\0';
                        // Convertir userInput en minuscules
                        toLowerString(userInput);
                        if(wordValid(dico, userInput, mainDeck)){ //Etapes de validation
                            end = 1; //BREAK THE LOOP
                            break;
                        }
                        else{ //Word Incorrect, retype the word
                            mvprintw(inputTextRow, inputTextCol + strlen("Entrer votre mot:") + 3, "Word Invalid !");
                        }
                    }
                }
        }
    }
    endwin();
}

int main() {
    // Initialiser la bibliothèque NCurses
    struct NodeTrie* dico = createDico("dicoFR.txt");

    // Appeler la fonction pour afficher les decks
    displayScrabbleDecks(dico);
    endwin();
    deleteNodeTrie(&dico);

    return 0;
}