#include <iostream>
#include <string>
using namespace std;

//Classic Linked List

template<typename DataType>
struct Node {
    Node<DataType>* next;
    DataType data;
    
    Node<DataType>* insert(DataType newData){ //to add to the linked list we prepend a new node 
    //and we point it to the current node, returning the address of the new node
      Node<DataType>* newHead = new Node<DataType>(newData);
      newHead->next = this;
      return newHead; //update the pointer to this list to reflect the new node
    }
    Node(DataType inputData);
};

//Constructor takes an input
//Notice how odd it is to define a templated class function
template<typename DataType>
Node<DataType>::Node(DataType inputData){
    data = inputData;
    next = 0;
}

template<typename DataType>
void print_list(Node<DataType>* anode){
   Node<DataType>* temp = anode;
   while(temp != 0){
      temp = temp->next;
      cout << temp->data << endl;      
   }
}


int main() {
   Node<int>* head = new Node<int>(100);
   for(int i = 90; i >= 0; i=i-10){
      head = head->insert(i);
   }
   
   print_list(head);
   return 0;
} 