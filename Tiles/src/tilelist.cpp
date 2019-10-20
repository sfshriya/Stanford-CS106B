// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "tilelist.h"
#include <algorithm>
#include "strlib.h"
using namespace std;

void TileList::deleteNode(TileNode* node)
{
    if (node == back)
    {
        back = back->prev;
        back->next = nullptr;
        delete node;
    }
    else if (node == front)
    {
        front = front->next;
        front->prev = nullptr;
        delete node;
    }
}

TileNode TileList::detach(TileNode *node){
    //if head node
    if (front == node){
        front = node->next;
    }
    //not the final node
    if (node->next != nullptr){
        node->next->prev = node->prev;
    }
    //not the head node
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    }
    return *node;

}


TileList::TileList() {
    // O(1)
    //sets pointers to null since they cant be linked to anything
    front = nullptr;
    back = nullptr;
}

TileList::~TileList() {
    // TODO: write this destructor
}



int TileList::sizeOfList()
{
    TileNode * tempNode = front;
    int count = 0;
    while(tempNode!=nullptr)
    {
        count++;
        tempNode = tempNode->next;
    }
    return count;
}


void TileList::addBack(int x, int y, int width, int height, string color) {
    // TODO: write this function
    struct TileNode* node = new TileNode(x, y, width, height, color);

    if(back != nullptr){
        back->next = node;
        node->prev = back;
        back = node;
        node->next = nullptr;
    }
    else{
        front = node;
        back = node;
    }

}

void TileList::addFront(int x, int y, int width, int height, string color) {
    // TODO: write this function
    struct TileNode* node = new TileNode(x, y, width, height, color);

    if(front != nullptr){
        front->prev = node;
        node->next = front;
        front = node;
        node->prev = nullptr;
    }
    else{
        front = node;
        back = node;
    }
}

void TileList::clear() {
    // TODO: write this function
    //clears the graphics and starts over again
    TileNode* node = getFront();
    TileNode* temp = nullptr;
    while(node != nullptr){
        temp = node->next;
        detach(node);
        node = temp;
    }
    front = nullptr;
    back = nullptr;
}


void TileList::debug() {
    // TODO: write this function (optional)
    cout << "# of nodes in the list " << sizeOfList() << endl;


}

void TileList::drawHelper(TileNode* node, GWindow& window) const{
    if(node == nullptr){
        return;
    }
    node->draw(window);
    drawHelper(node->prev, window);
}

void TileList::drawAll(GWindow& window) const {
    // TODO: write this function
    //call the draw member function on each of your TileNode objects and pass this window as a parameter
    drawHelper(getBack(), window);
}

TileNode* TileList::getBack() const {
    return back;
}

TileNode* TileList::getFront() const {
    return front;
}

bool TileList::highlight(int x, int y) {
    // TODO: write this function
    //If these coordinates touch any tiles, you should set the
    //topmost (closest to the front) of these tiles to have a color member value of "yellow"
    TileNode* node = getFront();
    while(node != nullptr){
        if(node->contains(x, y)){
            if(node->color == "yellow"){
                return true;
            }
            else{
                node->color = "yellow";
                return true;
            }
        }
        node = node->next;
    }
    return false;
}

bool TileList::lower(int x, int y) {
    // TODO: write this function
    //moves the node to the bottom of the linked list
    TileNode *node = getFront();
    while(node != nullptr && !node->contains(x,y) )
    {
        node = node->next;
    }

    if(node!= nullptr && node->contains(x,y)){
        TileNode *newBack = new TileNode(detach(node));
        //TileNode *temp = back;
        back->next = newBack;
        newBack->prev = back;
        newBack->next = nullptr;
        back = newBack;
        return true;
    }
    return false;
}

void TileList::merge(int x, int y) {
    // TODO: write this function
    TileNode* node = getFront();
    TileNode* temp = nullptr;
    int newX = 0;
    int newY = 0;
    int newWidth = 0;
    int newHeight = 0;

    while(node != nullptr){

        TileNode* nextNode = node->next;
        if(node->contains(x, y)){
            if(temp == nullptr){
                temp = node;
            }
            else{
                if(node->x < temp->x){
                    newX = node->x;
                }else {
                    newX = temp->x;
                }

                if(node->y < temp->y){
                    newY = node->y;
                }
                else {
                   newY = temp->y;
                }

                if((node->width + node->x) > (temp->width + temp->x)){
                    newWidth = node->width + node->x - newX;
                }
                else{
                    newWidth = temp->width + temp->x - newX;
                }

                if((node->height + node->y) > (temp->height + temp->y)){
                    newHeight = node->height + node->y - newY;
                }
                else{
                    newHeight = temp->height + temp->y - newY;
                }
                temp->x = newX;
                temp->y = newY;
                temp->width = newWidth;
                temp->height = newHeight;
                //delete node
                cout << "delete node lmao " << node->x << " " << node->y << endl;
                node->prev->next = node->next;
                deleteNode(node);
            }
        }
        node = nextNode;
    }
}

bool TileList::raise(int x, int y) {
    //moves the node from the current spot in the linked list and adds it to the front
    TileNode *node = getBack();

    while(node != nullptr && !node->contains(x,y) )
    {
        node = node->prev;
    }

    if(node!= nullptr){
        TileNode *newFront = new TileNode(detach(node));
        newFront->next = front;
        newFront->prev = nullptr;
        front->prev = newFront;
        return true;
    }
    return false;
}

bool TileList::remove(int x, int y) {

    TileNode *node = getFront();
    if(node != nullptr && node->contains(x,y)){

        cout<<"Time to delete";
        deleteNode(node);


        delete node;
        return true;
    }
    return false;
}

int TileList::removeAll(int x, int y) {
    // TODO: write this function
    //whatever tiles are being touched by those coordinates need to all be removed from the linked list
    TileNode* nodeFront = getFront();
    TileNode* nodeBack = getBack();
    TileNode* temp = nullptr;
    if(nodeFront->contains(x, y)){
        if(temp == nullptr){
            temp = nodeFront;
         }
         detach(nodeBack);
         detach(nodeFront);
    }
    return 0;
}
