#include <iostream>

template <typename T>
struct Item{
    T value;
    int priority;
};

template <typename T>
class Queue{
    //private:
    public:
        int front;
        int rear;
        bool empty;
        int size;
        Item<T> *array; 

    public:
        Queue<T>(int _size = 1)
        {
            front = 0;
            rear = _size - 1;
            empty = true;
            size = _size;
            array = new Item<T>[_size];
        }
        ~Queue(){
            delete[] array;
        }
                
        void enqueue(const T& value, int priority);
        int top();
        T dequeue();        
        bool isFull();
};

template <typename T>
void Queue<T>::enqueue(const T& value, int priority){
    Item<T> newitem;
    newitem.value = value;
    newitem.priority = priority;
    if (isFull() == false)
    {
        rear = (rear + 1) % size;
        array[rear] = newitem;
    }

    else if (isFull()) {
        size *= 2;
        Item<T> *bigarray = new Item<T>[size];
        for (int i = 0; i < size; i++) {
            bigarray[i] = array[(front + i) % (size / 2)];
           
        }
        front = 0;
        rear = size / 2;
        delete array;
        array = bigarray;
        array[rear] = newitem;

    }
    empty = false;
    return;
}

template <typename T>
int Queue<T>::top(){
    //TODO
    //returning the array index of the highest priority item
    //exception is empty
    int i = 0;
    int index = 0;
    int p = array[front].priority;
   
    do {
        if (p < array[(front+i)%size].priority) {
            p = array[(front + i) % size].priority;
          
            index = i;
       }
        i++;
    } while ((front + i - 1) % size != rear);
    return (front + index) % size;
}

template <typename T>
T Queue<T>::dequeue(){
    int index = top();
    T deqval = array[top()].value;
    if (index > front) {
        for (int i = 0; i < index - front; i++) {
            *(array + index-i) = *(array + index-i - 1);
        }
    }
    else if (index == front) {
    }
    else {
        for (int i = index+size-front; i >0; i--) {
            *(array + (front + i) % size) = *(array + (front + i - 1) % size);
        }
    }

    front = (front + 1) % size;

    return deqval;
}

template <typename T>
bool Queue<T>::isFull() {
    if (!empty) {
        if (rear - front == -1 || rear - front == size - 1) {
            return true;
        }
        else return false;
    }
    else return false;
}