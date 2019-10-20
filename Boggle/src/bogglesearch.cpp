#include "bogglegui.h"
#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
using namespace std;

bool getIsUsed(int row, int column, Vector<Vector<int>> usedCubes) {
    bool isUsed = false;
    //check if it's in usedCubes
    //use a loop
    int numCubes = usedCubes.size();
    for(int i = 0; i < numCubes; i++){
        Vector<int> cube = usedCubes[i];
        //check if the indeces of row column are = cube
        int cubeRow = cube[0];
        int cubeColumn = cube[1];
        if(cubeRow == row && cubeColumn == column){
            isUsed = true;
            break;
        }
    }
    return isUsed;
}
//check first letter and
//explore the neighbors and check if the remaining letters in the word can be found starting at the neighbors
//You don't want to visit the same letter cube twice during a given exploration, so for the search algorithm to work,
//your code needs some way to "mark" whether a letter cube has been visited or not.
//use a vector to store the cubes that are used and push back the ones that are not used
//how do you identify a cube?
//use a vector of vectors of ints
//where do I use it?
bool isInGridHelper(Grid<char>& board, string word, int row, int column, Vector<Vector<int>> usedCubes){
    bool isUsed = getIsUsed(row, column, usedCubes);
    if(isUsed){
        return false;
    }

    //highlight row column
    BoggleGUI::setHighlighted(row, column, true);
    Vector<int> cube = {row, column};
    usedCubes.push_back(cube);

    char firstLetter = word.at(0);
    char cubeLetter = board[row][column];
    if(cubeLetter != firstLetter){
        //call isInGridHelper recursively
        //clear highlight
        BoggleGUI::setHighlighted(row, column, false);
        int lastIndex = usedCubes.size()-1;
        usedCubes.remove(lastIndex);
        return false;
    }

    //check base case
    if(word.size() == 1){
        return true;
    }
    string remainingWord = word.substr(1);

    int numRows = board.numRows();
    int numColumns = board.numCols();

    //look at the eight neighbors
    for(int neighborRow = row-1; neighborRow <= row+1; neighborRow++){

        //check that it's on the board. if not, skip over - continue
        if(neighborRow < 0 || numRows <= neighborRow){
            continue;
        }

        for(int neighborColumn = column-1; neighborColumn <= column+1; neighborColumn++){

            //check that it's on the board. if not, skip over - continue
            if(neighborColumn < 0 || numColumns <= neighborColumn){
                continue;
            }

            //if you reach your location, skip over
            if(neighborRow == row && neighborColumn == column){
                continue;
            }

            //call isInGridHelper recursively
            bool result = isInGridHelper(board, remainingWord, neighborRow, neighborColumn, usedCubes);
            if(result){
                return true;
            }
            //else keep looking
        }
    }

    //clear highlight
    BoggleGUI::setHighlighted(row, column, false);
    int lastIndex = usedCubes.size()-1;
    usedCubes.remove(lastIndex);
    return false;
}

bool isInGrid(Grid<char>& board, string word){
    Vector<Vector<int>> usedCubes = Vector<Vector<int>>();

    //loop through board (2d array)
    int numRows = board.numRows();
    int numColumns = board.numCols();
    for(int row = 0; row < numRows; row++){
        for(int column = 0; column < numColumns; column++){
            bool inGrid = isInGridHelper(board, word, row, column, usedCubes);
            if(inGrid){
                return true;
            }
        }
    }
    return false;
}

int getScore(string word){
    int score = 0;
    int numLetters = word.size();
    if(numLetters <= 4){
        score = 1;
    }
    else if(numLetters == 5){
        score = 2;
    }
    else if(numLetters == 6){
        score = 3;
    }
    else if(numLetters == 7){
        score = 5;
    }
    else {
        score = 11;
    }
    return score;
}
bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    //check if word is at least 4 letters
    if(word.size() < 4){
        return false;
    }
    //check if word is in the dictionary
    if(!dictionary.contains(word)){
        return false;
    }
    //check if word is formed by neighbors in the grid
    bool inGrid = isInGrid(board, word);
    if(inGrid){
        int score = getScore(word);
        BoggleGUI::scorePointsHuman(score);
    }
    return inGrid;
}

void computerWordSearchHelper(Grid<char>& board, Lexicon& dictionary, Set<string>& words, int row, int column, Vector<Vector<int>> usedCubes,
                              Set<string>& humanWords, string prefix){
    cout << "computerWordSearchHelper" << " " << prefix << " " << row << ", " << column << endl;
    //check if cube is used
    bool isUsed = getIsUsed(row, column, usedCubes);
    if(isUsed){
 //       cout << "isUsed" << endl;
        return;
    }

    char cubeLetter = board[row][column];
    string newPrefix = prefix + cubeLetter;

    bool newPrefixFound = dictionary.containsPrefix(newPrefix);
    if(!newPrefixFound){
        cout << "new prefix not found" << endl;
        return;
    }

    //cube is used so add it to usedCubes vector
    Vector<int> cube = {row, column};
    usedCubes.push_back(cube);

    int numChar = newPrefix.size();
    bool atLeast4 = numChar >= 4;
    if(atLeast4){
        //check base case
        bool wordFound = dictionary.contains(newPrefix);
        if(wordFound) {
            //add to words if newPrefix >= 4
 //           cout << "words add" << " " << newPrefix << endl;
            //add word to words
            words.add(newPrefix);
            int score = getScore(newPrefix);
            BoggleGUI::scorePointsComputer(score);
        }
    }

    int numRows = board.numRows();
    int numColumns = board.numCols();

    //look at the eight neighbors
    for(int neighborRow = row-1; neighborRow <= row+1; neighborRow++){

        //check that it's on the board. if not, skip over - continue
        if(neighborRow < 0 || numRows <= neighborRow){
            continue;
        }

        for(int neighborColumn = column-1; neighborColumn <= column+1; neighborColumn++){

            //check that it's on the board. if not, skip over - continue
            if(neighborColumn < 0 || numColumns <= neighborColumn){
                continue;
            }

            //if you reach your location, skip over
            if(neighborRow == row && neighborColumn == column){
                continue;
            }

            //call computerWordSearchHelper recursively
            computerWordSearchHelper(board, dictionary, words, neighborRow, neighborColumn, usedCubes, humanWords, newPrefix);
        }
    }
    int lastIndex = usedCubes.size()-1;
    usedCubes.remove(lastIndex);
    cout << "the end" << endl;
}

Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    cout << "my computer word search" << endl;
    string prefix = "";
    Set<string> words;
    Vector<Vector<int>> usedCubes = Vector<Vector<int>>();

    //loop through board (2d array)
    int numRows = board.numRows();
    int numColumns = board.numCols();


    for(int row = 0; row < numRows; row++){
        for(int column = 0; column < numColumns; column++){

            computerWordSearchHelper(board, dictionary, words, row, column, usedCubes, humanWords, prefix);
        }
    }


    //computerWordSearchHelper(board, dictionary, words, 0, 0, usedCubes, humanWords, prefix);
    //int numWords = words.size();
    //cout << "number of words "<< numWords << endl;
    //cout << words << endl;

    return words;
}
