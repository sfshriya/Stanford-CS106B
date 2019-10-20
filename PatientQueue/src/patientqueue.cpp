//Natalie Nova and Shriya Kagolanu
#include "patientqueue.h"
//add onto the array
void PatientQueue::pushBack(Patient* patient){

    //cout << endl;

    if(numPatients == capacity){
        //ran out of room
        //increase capacity
        int newCapacity = capacity * 2;

        Patient** newPatients = (Patient**) malloc(newCapacity*sizeof(struct Patient *));
        for(int i = 0; i < capacity; i++){
            newPatients[i] = patients[i];
        }

        //delete gave us a problem when we had an array of structures and delete[]
        //did not put step statement between every step
        //delete patients;
        free(patients); //ask in class

        patients = newPatients;
        capacity = newCapacity;

    }
    patients[numPatients++] = patient;

}

PatientQueue::PatientQueue(bool isMinHeap) {
    // TODO: write this constructor
    //initalize numPatients and patients
    this->isMinHeap = isMinHeap;
    numPatients = 0;
    capacity = 1;
    //patients is created"
    patients = (Patient**) malloc(capacity*sizeof(Patient*));
}

PatientQueue::~PatientQueue() {
    //loop through the whole pateint array and delete the array
    delete patients;
}

void PatientQueue::changePriority(string value, int newPriority) {
    //find the value point
    //get patient from patients[i]
    for(int i = 0; i < numPatients; i++){
        if (patients[i]->name == value){
            int oldPriority = patients[i]->priority;
            patients[i]->priority = newPriority;
            if(isMinHeap){
                oldPriority *= -1;
                newPriority *= -1;
            }
            if(oldPriority < newPriority){
                bubbleUp(i);
                return;
            }
            else{
                bubbleDown(i);
                return;
            }
        }
    }


}





void PatientQueue::clear() {
    // TODO: write this function
    for(int i = 0; i < numPatients; i++){
        //get patient from patients[i]
        patients[i]->name = "";
        patients[i]->priority= 0;
    }
    numPatients = 0;
}

void PatientQueue::debug() {
    // empty
    //enqueue("Joe", 1);
    //enqueue("Mary", 3);
    /*
    Patient* joe = new Patient("Joe", 1);
    Patient* mary = new Patient("Mary", 3);
    pushBack(joe);
    pushBack(mary);
   */
}

string PatientQueue::dequeue() {
    // TODO: write this function
    //this function with dequeue the foremost element and return their name
    //then remove(Patient[0]);
    //dequeue
    Patient* firstPatient = patients[0];
    string name = firstPatient->name;


    //replace the first element with the last
    int lastIndex = numPatients - 1;
    patients[0] = patients[lastIndex];
    //copied last to first

    numPatients--;

    //maintain as heap so you have to bubble down
    bubbleDown(0);
    //nothing else will happen to first patient because it's next to the return
    delete firstPatient;
    return name;
}

void PatientQueue::bubbleUp(int problemIndex0){
    if(problemIndex0 == 0){
        return;
    }
    int parentIndex = ((problemIndex0 - 1) / 2);
    int parentPriority = patients[parentIndex]->priority;
    int problemPriority = patients[problemIndex0]->priority;
    if(!isMinHeap){
        parentPriority *= -1;
        problemPriority *= -1;
    }
    if(parentPriority > problemPriority){
        Patient* temp = patients[problemIndex0];
        patients[problemIndex0] = patients[parentIndex];
        patients[parentIndex] = temp;
        bubbleUp(parentIndex);
    }
    else if (parentPriority == problemPriority && isMinHeap)
    {
        if (patients[parentIndex]->name > patients[problemIndex0]->name)
        {
            Patient* temp = patients[problemIndex0];
            patients[problemIndex0] = patients[parentIndex];
            patients[parentIndex] = temp;
            bubbleUp(parentIndex);
        }
    }

    else if (parentPriority == problemPriority && !isMinHeap)
    {
        if (patients[parentIndex]->name < patients[problemIndex0]->name)
        {
            Patient* temp = patients[problemIndex0];
            patients[problemIndex0] = patients[parentIndex];
            patients[parentIndex] = temp;
            bubbleUp(parentIndex);
        }
    }
}

void PatientQueue::bubbleDown(int problemIndex0){
    //cout<< " in bubble down " << problemIndex0;
    int leftChildIndex = 2*problemIndex0 + 1;
    int rightChildIndex = 2*problemIndex0 + 2;
    if(leftChildIndex > numPatients){

        return;
    }

    int minChildIndex = leftChildIndex;

    int problemPriority = patients[problemIndex0]->priority;
    int LeftChildPriority = patients[leftChildIndex]->priority;
    int rightChildPriority = 0;
    if(rightChildIndex <= numPatients -1) {
        rightChildPriority = patients[rightChildIndex]->priority;
    }
    int minChildPriority = LeftChildPriority;


    if(!isMinHeap){
        problemPriority *= -1;
        LeftChildPriority *= -1;
        rightChildPriority *= -1;
        minChildPriority *= -1;
    }


    if(rightChildIndex <= numPatients -1){
        if(rightChildPriority < LeftChildPriority){
            minChildIndex = rightChildIndex;
            minChildPriority = rightChildPriority;
        }
        else if (rightChildPriority == LeftChildPriority){
            if (patients[rightChildIndex]->name < patients[leftChildIndex]->name && isMinHeap)
            {
                minChildIndex = rightChildIndex;
                minChildPriority = rightChildPriority;
            }
            else if (patients[rightChildIndex]->name > patients[leftChildIndex]->name && !isMinHeap)
            {
                minChildIndex = rightChildIndex;
                minChildPriority = rightChildPriority;
            }


        }
    }
    if(problemPriority > minChildPriority){
        Patient* temp = patients[problemIndex0];
        patients[problemIndex0] = patients[minChildIndex];
        patients[minChildIndex] = temp;
        bubbleDown(minChildIndex);
    }

    else if(problemPriority == minChildPriority && isMinHeap)
    {
        if (patients[problemIndex0]->name > patients[minChildIndex]->name)
        {
            Patient* temp = patients[problemIndex0];
            patients[problemIndex0] = patients[minChildIndex];
            patients[minChildIndex] = temp;
            bubbleDown(minChildIndex);
        }
    }
    else if(problemPriority == minChildPriority && !isMinHeap)
    {
        if (patients[problemIndex0]->name < patients[minChildIndex]->name)
        {
            Patient* temp = patients[problemIndex0];
            patients[problemIndex0] = patients[minChildIndex];
            patients[minChildIndex] = temp;
            bubbleDown(minChildIndex);
        }
    }

}


void PatientQueue::enqueue(string value, int priority) {
    //use pushBack
    Patient* patient = new Patient(value, priority);
    pushBack(patient);

    int parentIndex = ((numPatients - 2) / 2);
    //use bubble up to sort this list
    bubbleUp(numPatients - 1);

}

bool PatientQueue::isEmpty() const {
    // TODO: write this function
    if(numPatients == 0){
        return true;
    }else{
        return false;
    }

}

string PatientQueue::peek() const {
    // TODO: write this function
    //checks the front most and d
    if(isEmpty()){
        throw "The Queue is Empty";
    }
    string name = patients[0]->name;
    return name;
}

int PatientQueue::peekPriority() const {
    // TODO: write this function
    // checks the priority of them
    if(isEmpty()){
        throw "The Queue is Empty";
    }
    int priority = patients[0]->priority;
    return priority;

}

int PatientQueue::size() const {
    // TODO: write this function
    int size = numPatients;
    return size;
}

ostream& operator <<(ostream& out, const PatientQueue& queue) {
    // TODO: write this function
    int numPatients = queue.numPatients;
    Patient** patients = queue.patients;
    int lastIndex = numPatients - 1;
    //enclose in brackets {}
    out << "{";
    for(int i = 0; i < numPatients; i++){
        //get patient from patients[i]
        Patient* patient = patients[i];
        if(i == lastIndex){
            out << *patient;
        }
        else{
            out << *patient << ", ";
        }
    }
    out << "}";
    return out;
}



