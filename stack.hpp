#include <iostream>
#include <utility>
#include <vector>
#include <string>

using namespace std;

template <typename T>
class Stack{
    private: 
        int current;
        int size;
        T * array;

    public:
        Stack<T> (int _size = 16)
        {   
            current = -1;
            size = _size;
            array = new T[_size];
        }
        ~Stack()
        {
            delete[] array;
        }    

        bool isEmpty() const;
        bool isFull() const;
        T top() const;
        T pop();
        void push(const T& item);
};

template <typename T>
bool Stack<T>::isEmpty() const{
        return (current == -1) ;
}

template <typename T>
bool Stack<T>::isFull() const{
    return (current == size-1);
}

template <typename T>
T Stack<T>::top() const{
    //TODO
    return array[current];
}

template <typename T>
T Stack<T>::pop(){
    T temp = top();
    current -= 1;
    return temp;
}

template <typename T>
void Stack<T>::push(const T& item){
    if (!isFull()) {
        current += 1;
        array[current] = item;
        return;
    }
    else {
        size *= 2;
        current += 1;
        array[current] = item;
        return;
    }
}

bool checkParentheses(const string& line, const vector<pair<char,char>>& pairs);
float calculate(const string& line);