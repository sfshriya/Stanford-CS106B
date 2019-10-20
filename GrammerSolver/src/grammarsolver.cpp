// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "grammarsolver.h"
#include "simpio.h"
using namespace std;

bool getTerminal(string symbol, Map<string, Vector<Vector<string>>> productionsFromNonTerminal){
    //containsKey()
//    bool count = productionsFromNonTerminal.count(symbol);
//    return count == 0;
    bool count = productionsFromNonTerminal.containsKey(symbol);
    return count;
}

string getGeneratedString(string symbol, Map<string, Vector<Vector<string>>> productionsFromNonTerminal) {

    //string of terminal symbols generated by grammar & symbol
    string generatedString = "";

    //look the symbol up in the map and save the productions=vector of choices
    Vector<Vector<string>> productions = productionsFromNonTerminal[symbol];

    //choose one of the (2) vectors in productions
    int numChoices = productions.size();
    int choice = rand() % numChoices;
    Vector<string> production = productions[choice];

    //production = ["<dp>","<adjp>", "<n>"]
    //look at each symbol ^
    int numOfSymbols = production.size();
    for(int i = 0; i < numOfSymbols; i++){
        string symbol = production[i];

        //check if symbol is terminal or nonterminal
        //terminal = not one of the keys in map
        bool terminal = getTerminal(symbol, productionsFromNonTerminal);
        if(terminal){
            // add to returnString
            generatedString += " " + symbol; //$$$$$fix me for first one
        }
        else{
            string newString = getGeneratedString(symbol, productionsFromNonTerminal);
            generatedString += newString;
        }
    }
    return generatedString;
}

map<string, Vector<Vector<string>>> getProductionsFromNonTerminal(istream& input){
    map<string, Vector<Vector<string>>> productionsFromNonTerminal = map<string, Vector<Vector<string>>>();
    //generate map using input
    //loop through the lines of the file processing each one - getLine();
    string line;
    while(getline(input, line)){
        //process line
        //look at key and then values
        //Vector<string> stringSplit(string s, string delimiter)
        Vector<string> keyValue = stringSplit(line, "::=");
        string key = keyValue[0];
        string value = keyValue[1];
        // value = <dp> <adjp> <n>|<pn>
        //string split again between each choice
        Vector<string> choices = stringSplit(value, "|");
        //choices = ["<dp> <adjp> <n>", "<pn>"]
        Vector<Vector<string>> productionsValue = Vector<Vector<string>>();
        int numChoices = choices.size();
        for(int i = 0; i < numChoices; i++){
            string choice = choices[i];
            //i = 0, choice = "<dp> <adjp> <n>"
            Vector<string> symbols = stringSplit(choice, " ");
            //.add()
            productionsValue.push_back(symbols);
        }
        //now have key and value=productions for one rule in grammar
        productionsFromNonTerminal.emplace(key, productionsValue);
    }

    return productionsFromNonTerminal;
}


Vector<string> grammarGenerate(istream& input, string symbol, int times) {

    // Create the map
    map<string, Vector<Vector<string>>> productionsFromNonTerminal = getProductionsFromNonTerminal(input);

    //list of generated strings, there are times number of strings
    Vector<string> returnVector = Vector<string>();

    //adds the string of terminal symbols to the return vector times number of times
    for(int i = 0; i < times; i++){
        getGeneratedString(symbol, productionsFromNonTerminal);
        //returnVector.push_back(generatedString);
    }

    return returnVector;
}