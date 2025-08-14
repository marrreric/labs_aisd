#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <cctype>
#include <unordered_set>
#include <cstdlib>

using namespace std;

// Динамический массив
class DynamicArray {
private:
    string* data;
    size_t capacity;
    size_t size;

    void resize() {
        capacity *= 2;
        string* newData = new string[capacity];
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    DynamicArray() : capacity(2), size(0) {
        data = new string[capacity];
    }

    ~DynamicArray() {
        delete[] data;
    }

    void append(const string& value) {
        if (size == capacity) {
            resize();
        }
        data[size++] = value;
    }

    string get(size_t index) const {
        if (index < size) {
            return data[index];
        }
        throw out_of_range("Index out of range");
    }

    size_t getSize() const {
        return size;
    }
};

// Структура узла для двусвязного списка
struct Node {
    string data;
    Node* prev;
    Node* next;

    Node(const string& data) : data(data), prev(nullptr), next(nullptr) {}
};

// Двусвязный список
class DoublyLinkedList {
private:
    Node* head;
    Node* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void append(const string& data) {
        Node* newNode = new Node(data);
        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void removeLast() {
        if (!tail) {
            cout << "Список пустой" << endl;
            return;
        }

        Node* toDelete = tail;
        tail = tail->prev;

        if (tail) {
            tail->next = nullptr;
        }
        else {
            head = nullptr; 
        }

        delete toDelete;
    }

    string getLast() const {
        return tail ? tail->data : "";
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    void clear() {
        while (!isEmpty()) {
            removeLast();
        }
    }

    ~DoublyLinkedList() {
        clear();
    }
};

// Класс стека, использующий DoublyLinkedList
class Stack {
private:
    DoublyLinkedList list;

public:
    void push(const string& data) {
        list.append(data);
    }

    string pop() {
        if (list.isEmpty()) {
            cout << "Стек пустой" << endl;
            return "";
        }
        string data = list.getLast();
        list.removeLast();
        return data;
    }

    string peek() const {
        return list.getLast();
    }

    bool isEmpty() const {
        return list.isEmpty();
    }
};

// Функция для определения приоритета операторов
int precedence(const string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    return 0;
}

// Функция для проверки, является ли токен оператором
bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

// Функция для проверки, является ли токен функцией
bool isFunction(const string& token) {
    return token == "cos" || token == "sin";
}

// Функция для преобразования инфиксного выражения в постфиксное
DynamicArray infixToPostfix(const string& expression) {
    Stack operatorStack;
    DynamicArray output;//рез
    stringstream ss(expression);//для разбора на токены(чис, оп, ф)
    string token;

    while (ss >> token) {
        if (isdigit(token[0]) || (token.size() > 1 && token[0] == '-' && isdigit(token[1]))) {
            output.append(token);
        }
        else if (isFunction(token)) {
            operatorStack.push(token);
        }
        else if (token == "(") {
            operatorStack.push(token);
        }
        else if (token == ")") {
            while (!operatorStack.isEmpty() && operatorStack.peek() != "(") {
                output.append(operatorStack.pop());
            }
            operatorStack.pop(); 
            
            if (!operatorStack.isEmpty() && isFunction(operatorStack.peek())) {
                output.append(operatorStack.pop());
            }
        }
        else if (isOperator(token)) {
            while (!operatorStack.isEmpty() &&
                precedence(operatorStack.peek()) >= precedence(token)) {//оп по приоритету
                output.append(operatorStack.pop());
            }
            operatorStack.push(token);
        }
    }

    while (!operatorStack.isEmpty()) {
        output.append(operatorStack.pop());
    }

    return output;
}

//// Функция для вычисления результата постфиксного выражения
//double evaluatePostfix(const DynamicArray& postfix) {
//    Stack valueStack;
//
//    for (size_t i = 0; i < postfix.getSize(); ++i) {
//        string token = postfix.get(i);
//
//        if (isdigit(token[0]) || (token.size() > 1 && token[0] == '-' && isdigit(token[1]))) {
//            valueStack.push(token);
//        }
//        else if (isOperator(token)) {
//            double rightOperand = stod(valueStack.pop());
//            double leftOperand = stod(valueStack.pop());
//            double result;
//
//            if (token == "+") result = leftOperand + rightOperand;
//            else if (token == "-") result = leftOperand - rightOperand;
//            else if (token == "*") result = leftOperand * rightOperand;
//            else if (token == "/") {
//                
//                if (rightOperand == 0) {
//                    cout << "Ошибка: Деление на ноль" << endl;
//                    return 0; 
//                }
//                result = leftOperand / rightOperand;
//            }
//            else if (token == "^") result = pow(leftOperand, rightOperand);
//            valueStack.push(to_string(result));
//        }
//        else if (isFunction(token)) {
//            double operand = stod(valueStack.pop());
//            double result;
//
//            if (token == "cos") result = cos(operand);
//            else if (token == "sin") result = sin(operand);
//
//            valueStack.push(to_string(result));
//        }
//    }
//
//    return stod(valueStack.pop());
//}

// Функция для проверки вложенности и наличия недопустимых символов
bool isValidInput(const string& expression) {
    unordered_set<char> validChars = { '+', '-', '*', '/', '^', '(', ')', ' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                        's', 'i', 'n', 'c', 'o' }; 
    for (char ch : expression) {
        if (validChars.find(ch) == validChars.end() && !isalpha(ch)) {
            return false;
        }
    }
    return true;
}

// Функция для вывода разрешенных символов
void printAllowedCharacters() {
    cout << "Разрешенные символы: +, -, *, /, ^, sin, cos, (, ), 0-9" << endl;
}

// Главная функция
int main() {
    setlocale(LC_ALL, "rus");
    string expression;
    printAllowedCharacters();
    while (true) {
        cout << "\nВведите выражение: ";
        getline(cin, expression);
        if (!isValidInput(expression)) {
            cout << "Некорректный ввод. Пожалуйста, используйте разрешенные символы." << endl;
            continue;
        }

        DynamicArray postfix = infixToPostfix(expression);
        cout << "Постфиксное выражение: ";
        for (size_t i = 0; i < postfix.getSize(); ++i) {
            cout << postfix.get(i) << " ";
        }

        //double result = evaluatePostfix(postfix);
        //cout << "\nРезультат: " << result << endl;

        cin.get(); 
        system("cls"); 
        printAllowedCharacters();
    }

    return 0;
}


