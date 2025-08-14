#include <iostream>
#include <sstream>
#include <stack>
#include <fstream>
#include <stdexcept>
#include <queue>
#include <functional>

class Node {
public:
    int data;
    Node* left;
    Node* right;
    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

class BinaryTree {
public:
    Node* root;

    BinaryTree() : root(nullptr) {}

    void insert(int value) {
        root = insertRec(root, value);
    }

    

    void preOrderTraversal(Node* node) {
        if (node != nullptr) {
            std::cout << node->data << " ";
            preOrderTraversal(node->left);
            preOrderTraversal(node->right);
        }
    }


    Node* getRoot() { return root; }

private:
    Node* insertRec(Node* node, int value) {
        if (node == nullptr) {
            return new Node(value);
        }
        if (value < node->data) {
            node->left = insertRec(node->left, value);
        }
        else {
            node->right = insertRec(node->right, value);
        }
        return node;
    }
};


Node* parseTree(const std::string& str, int& index) {
    if (index >= str.length()) return nullptr;

    if (str[index] == '(') {
        index++;
        std::string number = "";
        while (index < str.length() && isdigit(str[index])) {
            number += str[index++];
        }
        if (number.empty()) {
            throw std::runtime_error("Ошибка: неверный формат.");
        }
        int value = std::stoi(number);
        Node* node = new Node(value);
        node->left = parseTree(str, index);
        if (index < str.length() && str[index] == ')') index++;
        node->right = parseTree(str, index);
        if (index < str.length() && str[index] == ')') index++;
        return node;
    }

    return nullptr; 
}


BinaryTree createTreeFromFile(const std::string& filename) {
    BinaryTree tree;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Ошибка: не удалось открыть файл " + filename);
    }

    std::string line;
    if (getline(file, line)) {
        int idx = 0;
        tree.root = parseTree(line, idx);
        if (idx != line.length()) {
            throw std::runtime_error("Ошибка парсинга дерева: неверный формат.");
        }
    }
    else {
        throw std::runtime_error("Ошибка: файл пуст.");
    }

    return tree;
}



class AVLNode {
public:
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int value) : key(value), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
public:
    AVLNode* root;

    AVLTree() : root(nullptr) {}

    int height(AVLNode* node) {
        return node ? node->height : 0;
    }


    int balanceFactor(AVLNode* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(AVLNode* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    AVLNode* rightRotate(AVLNode* y) { //если левое поддерево слишком высокое(левый случай)
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

    AVLNode* insert(AVLNode* node, int key) {
        if (!node) return new AVLNode(key);

        if (key < node->key) {
            node->left = insert(node->left, key);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key);
        }
        else {
            return node;
        }

        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = balanceFactor(node);

        if (balance > 1 && key < node->left->key) {
            return rightRotate(node);
        }
        if (balance < -1 && key > node->right->key) {
            return leftRotate(node);
        }
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    void levelOrder() {
        if (root == nullptr) return;
        std::queue<AVLNode*> q;
        q.push(root);
        while (!q.empty()) {
            AVLNode* node = q.front();
            q.pop();
            std::cout << node->key << " ";
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }

    void preOrder(AVLNode* node) {
        if (node != nullptr) {
            std::cout << node->key << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void inOrder(AVLNode* node) {
        if (node != nullptr) {
            inOrder(node->left);
            std::cout << node->key << " ";
            inOrder(node->right);
        }
    }

    void postOrder(AVLNode* node) {
        if (node != nullptr) {
            postOrder(node->left);
            postOrder(node->right);
            std::cout << node->key << " ";
        }
    }

    AVLNode* getRoot() { return root; }
};

int main() {
    setlocale(LC_ALL, "Russian"); 
    try {
        BinaryTree tree = createTreeFromFile("..\\tree.txt");
        std::cout << "Обход обычного двоичного дерева (прямой): ";
        tree.preOrderTraversal(tree.getRoot()); 
        std::cout << std::endl;

        AVLTree avlTree;
        std::function<void(Node*)> insertToAVL = [&](Node* node) {
            if (node != nullptr) {
                avlTree.root = avlTree.insert(avlTree.root, node->data);
                insertToAVL(node->left);
                insertToAVL(node->right);
            }
            };
        insertToAVL(tree.getRoot());

        std::cout << "Обход АВЛ дерева (уровневый): ";
        avlTree.levelOrder();
        std::cout << std::endl;

        std::cout << "Прямой обход АВЛ дерева (префикс): ";
        avlTree.preOrder(avlTree.getRoot());
        std::cout << std::endl;

        std::cout << "Центральный обход АВЛ дерева (инордер): ";
        avlTree.inOrder(avlTree.getRoot());
        std::cout << std::endl;

        std::cout << "Обратный обход АВЛ дерева (постфикс): ";
        avlTree.postOrder(avlTree.getRoot());
        std::cout << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}


