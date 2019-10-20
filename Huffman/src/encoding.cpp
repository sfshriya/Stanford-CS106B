// TODO: remove and replace this file header comment
// This is the CPP file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.

#include "encoding.h"
#include "priorityqueue.h"
#include <queue>
#include "map.h"
#include "filelib.h"
#include "queue.h"
#include "stack.h"


Map<char, int> buildFrequencyTable(istream& input){
    Map<char, int> frequency;
    char ch;
    while (input.get(ch)) { //  true if successful, false if fail/EOF
        frequency[ch]++;
    }

    return frequency;
}

HuffmanNode* buildEncodingTree(Map<char, int>& freqTable){
    // TODO: implement this function
    //PriorityQueue(Map<char, int>);
    PriorityQueue<HuffmanNode*> priQueue;
    //put the frequencies in a pririoty queue
    //the ones with the lowest frequencies have the highest priority
    for(char ch : freqTable){
        HuffmanNode* node = new HuffmanNode(ch);
        priQueue.enqueue(node, freqTable[ch]);
    }
    while(priQueue.size() != 1){
        //once the queue is made dequeue the 2 lowest frequencies and make the leaf nodes
        int firstPri = priQueue.peekPriority();
        HuffmanNode* firstHuff = priQueue.dequeue();
        int secondPri = priQueue.peekPriority();
        HuffmanNode* secondHuff = priQueue.dequeue();
        //then take the next lowest frequency and add it to the left side of the tree
        HuffmanNode* newNode;
        if(firstPri <= secondPri){
            newNode = new HuffmanNode(firstHuff, secondHuff);
        }
        else{
            newNode = new HuffmanNode(secondHuff, firstHuff);
        }
        //the root value of that tree will be the added frequencies of the two leaf nodes
        //create a root node that adds the values of the previous root node and new new node
        priQueue.enqueue(newNode, firstPri + secondPri);
    }
    //continue until everything is in one tree
    //return that root node
    //cout<< priQueue;
    return priQueue.peek();
}

void FlattenTree(HuffmanNode* t, string& encodingStr);

string flattenTreeToHeader(HuffmanNode* t)
{
    // TODO: implement this function
    //    cout << "Print the encoding key" << endl;

    string encode;
    FlattenTree(t, encode);

    //cout << "final: " << encode << endl;
    return encode;
}

void FlattenTree(HuffmanNode* t, string& encodingStr)
{
    if (t != nullptr) {
        if (t->isLeaf() ) {
            encodingStr = encodingStr + "." + t->ch;
            //   cout << "base case: " << encodingStr << endl;
            return;
        }
        else {
            encodingStr = encodingStr + "(";
            //  cout << "left case: " << encodingStr << endl;
            FlattenTree(t->zero, encodingStr);
            FlattenTree(t->one, encodingStr);
            encodingStr = encodingStr + ")";
            //    cout << "right case: " << encodingStr << endl;
        }
    }
    return;
}





HuffmanNode* evaluate(Queue<char>& tokens) {
    char token;

    HuffmanNode * root = nullptr;
    if (tokens.isEmpty()) {
        return nullptr;
    }
    else {
        // recursive case: expression (add left and add right)
        token = tokens.dequeue();

        if (token == '(' ){
            root = new HuffmanNode(nullptr, nullptr);
            root->zero = evaluate(tokens);
            root->one = evaluate(tokens);
            tokens.dequeue();
        }
        else if (token == '.')
        {
            token = tokens.dequeue();
            root = new HuffmanNode(token);
            return root;
        }
    }
    return root;
}




HuffmanNode* recreateTreeFromHeader(string str)
{
    Queue<char> tokenQueue;
    for (int i = 0; i < str.length(); i++) {

        tokenQueue.enqueue(str[i]);
    }
    // cout << "length " << sizeof(str) << "  queue length" + tokenQueue.size();
    return evaluate(tokenQueue);
}

//helper recusion function to traverse the HuffManEncodingTree
void encodingMapHelper(HuffmanNode* encodingTree, string encodingStr, Map<char, string> &encodingMap){
    if(encodingTree == nullptr){
        return;
    }
    if(encodingTree->isLeaf()){
        encodingMap[encodingTree->ch] = encodingStr;
    }
    encodingMapHelper(encodingTree->zero, encodingStr + "0", encodingMap);
    encodingMapHelper(encodingTree->one, encodingStr + "1", encodingMap);
}

Map<char, string> buildEncodingMap(HuffmanNode* encodingTree)
{

    // TODO: implement this function
    Map<char, string> encodingMap;
    encodingMapHelper(encodingTree, "", encodingMap);
    return encodingMap;
}

void freeTree(HuffmanNode* t)
{
    // TODO: implement this function
    if(t == nullptr){
        return;
    }
    freeTree(t->zero);
    freeTree(t->one);
    delete t;
}

void compress(istream& input, HuffmanOutputFile& output)
{
    // TODO: implement this function

    Map<char, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* t = buildEncodingTree(freqTable);
    Map<char, string> encodingMap = buildEncodingMap(t);
    string header = flattenTreeToHeader(t);
    output.writeHeader(header);
    rewindStream(input);
    char ch;
    while (input.get(ch)) { //  true if successful, false if fail/EOF
        string bits = encodingMap[ch];

        for (int i = 0; i < bits.length(); i++) {
            output.writeBit((int)bits[i]- '0');
        }
    }
}

void decode (HuffmanNode * huffTree, int &index, string message, ostream& output)
{
    if(huffTree == nullptr)
        return;

    if(huffTree->isLeaf())
    {
        output << huffTree->ch;
        return;
    }
    index++;
    if(message[index]=='0')
        decode(huffTree->zero, index, message, output);
    else
        decode(huffTree->one, index, message, output);

}

void decompress(HuffmanInputFile& input, ostream& output)
{
    // TODO: implement this function
    //read header from file
    string treeString =  input.readHeader();
    HuffmanNode * huffTree = recreateTreeFromHeader(treeString);
    //Map<char, string> encodingMap = buildEncodingMap(huffTree);
    //read the encoded string from file
    string encodedMessage = "";
    int mbit = 0;
    while ((mbit = input.readBit()) >=0)
    {
        encodedMessage +=to_string(mbit);
    }

    int index = -1;
    while (index < (int(encodedMessage.size()) -1))
    {
        decode( huffTree, index, encodedMessage, output);
    }

}
