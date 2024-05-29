#include <iostream>

struct Node
{
    int data;
    Node* left;
    Node* right;

    // Конструктор по умолчанию
    Node() : data(0), left(nullptr), right(nullptr) {}

    // Конструктор с параметрами
    Node(int data, Node* left = nullptr, Node* right = nullptr) :
        data(data), left(left), right(right) {}

    // Конструктор копирования
    Node(const Node& other) : data(other.data), left(other.left ? new Node(*other.left) : nullptr), right(other.right ? new Node(*other.right) : nullptr) {}

    // Конструктор перемещения
    Node(Node&& other) noexcept : data(other.data), left(other.left), right(other.right)
    {
        other.data = 0;
        other.left = nullptr;
        other.right = nullptr;
    }

    // Деструктор
    ~Node()
    {
        data = 0;
        left = nullptr;
        right = nullptr;
    }

    // Операторы сравнения для Node
    bool operator==(const Node& other) 
    {
        return data == other.data; 
    }
    bool operator!=(const Node& other) 
    { 
        return data != other.data;
    }
    bool operator<(const Node& other)  
    {
        return data < other.data;
    }
    bool operator>(const Node& other)
    {
        return data > other.data; 
    }
    bool operator<=(const Node& other)
    {
        return data <= other.data; 
    }
    bool operator>=(const Node& other) 
    {
        return data >= other.data;
    }

    friend std::ostream& operator<<(std::ostream& stream, Node*& node)
    {
        stream << node->data;
        return stream;
    }
};

class BTree
{
public:
    BTree() : root(nullptr) {}
    BTree(Node* rt) : root(rt) {}

    // Конструктор копирования
    BTree(const BTree& other) : root(copy(other.root)) {}

    // Конструктор перемещения
    BTree(BTree&& other) noexcept : root(other.root)
    {
        other.root = nullptr;
    }

    // Оператор присваивания копированием
    BTree& operator=(const BTree& other)
    {
        if (this == &other) return *this;
        dispose(root);
        root = copy(other.root);
        return *this;
    }

    // Оператор присваивания перемещением
    BTree& operator=(BTree&& other) noexcept
    {
        if (this == &other) return *this;
        dispose(root);
        root = other.root;
        other.root = nullptr;
        return *this;
    }

    ~BTree()
    {
        dispose(root);
    }

    void Add(int data)
    {
        add(root, data);
    }

    void Remove(int data)
    {
        Node* node = extractNode(searchNode(root, data));
        if (node != nullptr)
        {
            delete node;
        }
    }

    void RotateLeft(int data)
    {
        rotateLeft(searchNode(root, data));
    }

    void RotateRight(int data)
    {
        rotateRight(searchNode(root, data));
    }

    // Операторы сравнения для BTree
    bool operator==(const BTree& other)  
    {
        return compareTrees(root, other.root);
    }
    bool operator!=(const BTree& other)
    {
        return !compareTrees(root, other.root);
    }

    friend std::ostream& operator<<(std::ostream& stream, BTree& tree)
    {
        stream << "__________TREE START___________" << std::endl;
        tree.printLineTree(stream, tree.root);
        stream << std::endl;
        stream << "_______________________________" << std::endl;
        tree.printVerticalTree(stream, tree.root);
        stream << "__________TREE  END ___________" << std::endl;
        return stream;
    }

private:
    Node* root;

    void dispose(Node*& node)
    {
        if (node != nullptr)
        {
            dispose(node->left);
            dispose(node->right);
            delete node;
        }
    }

    Node* copy(Node* node)
    {
        if (node == nullptr) return nullptr;
        return new Node(node->data, copy(node->left), copy(node->right));
    }

    void add(Node*& node, int data)
    {
        if (node == nullptr)
        {
            node = new Node(data);
        }
        else if (data < node->data)
        {
            add(node->left, data);
        }
        else if (data > node->data)
        {
            add(node->right, data);
        }
    }

    void printVerticalTree(std::ostream& stream, Node*& node, int depth = 0)
    {
        if (node == nullptr)
        {
            return;
        }
        printVerticalTree(stream, node->left, depth + 1);
        for (int i = 0; i < depth; ++i)
        {
            stream << "\t";
        }
        stream << node << std::endl;
        printVerticalTree(stream, node->right, depth + 1);
    }

    void printLineTree(std::ostream& stream, Node*& node)
    {
        if (node == nullptr)
        {
            return;
        }
        printLineTree(stream, node->left);
        stream << node << " ";
        printLineTree(stream, node->right);
    }

    void rotateLeft(Node*& node)
    {
        if (node == nullptr || node->right == nullptr)
        {
            return;
        }
        Node* temp = node->right;
        node->right = node->right->left;
        temp->left = node;
        node = temp;
    }

    void rotateRight(Node*& node)
    {
        if (node == nullptr || node->left == nullptr)
        {
            return;
        }
        Node* temp = node->left;
        node->left = node->left->right;
        temp->right = node;
        node = temp;
    }

    Node*& searchNode(Node*& node, int data)
    {
        if (node == nullptr || node->data == data)
        {
            return node;
        }
        if (data < node->data)
        {
            return searchNode(node->left, data);
        }
        else
        {
            return searchNode(node->right, data);
        }
    }

    Node* extractNode(Node*& node)
    {
        if (node == nullptr)
        {
            return node;
        }
        if (node->left == nullptr && node->right == nullptr)
        {
            Node* temp = node;
            node = nullptr;
            return temp;
        }
        if (node->left == nullptr)
        {
            Node* temp = node;
            node = node->right;
            temp->right = temp->left = nullptr;
            return temp;
        }
        if (node->right == nullptr)
        {
            Node* temp = node;
            node = node->left;
            temp->right = temp->left = nullptr;
            return temp;
        }
        Node* leftMostRight = nullptr;
        if (node->right->left == nullptr)
        {
            leftMostRight = extractNode(node->right);
        }
        else
        {
            leftMostRight = node->right;
            while (leftMostRight->left->left != nullptr)
            {
                leftMostRight = leftMostRight->left;
            }
            leftMostRight = extractNode(leftMostRight->left);
        }
        Node* res = node;
        leftMostRight->left = node->left;
        leftMostRight->right = node->right;
        node = leftMostRight;
        return res;
    }

    bool compareTrees(Node* a, Node* b) const
    {
        if (a == nullptr && b == nullptr)
        {
            return true;
        }
        if (a == nullptr || b == nullptr)
        {
            return false;
        }
        return a->data == b->data && compareTrees(a->left, b->left) && compareTrees(a->right, b->right);
    }
};

int main(int argc, char* argv[])
{
    BTree tree;
    tree.Add(16);
    tree.Add(8);
    tree.Add(4);
    tree.Add(2);
    tree.Add(1);
    tree.Add(3);
    tree.Add(5);
    tree.Add(6);
    tree.Add(7);
    tree.Add(32);
    tree.Add(20);
    tree.Add(21);
    tree.Add(22);
    tree.Add(25);
    tree.Add(23);
    tree.Add(24);
    tree.Add(28);
    tree.Add(12);
    tree.Add(9);
    tree.Add(10);
    tree.Add(14);
    tree.Add(15);
    tree.Add(13);
    tree.Add(11);
    std::cout << tree << std::endl;
    BTree tr2 = tree;
    bool f = tree == tr2;
    std::cout << f << std::endl;

    std::cout << tr2 << std::endl;
    tree.RotateLeft(2);
    std::cout << tree << std::endl;
    tree.RotateRight(2);
    tree.Remove(50);
    std::cout << tree << std::endl;
    tree.Remove(24);
    std::cout << tree << std::endl;
    tree.Remove(9);
    std::cout << tree << std::endl;
    tree.Remove(3);
    tree.Remove(5);
    std::cout << tree << std::endl;
    tree.Remove(4);
    std::cout << tree << std::endl;
    tree.Remove(8);
    std::cout << tree << std::endl;
    tree.Remove(16);
    std::cout << tree << std::endl;

    tree.RotateRight(20);
    std::cout << tree << std::endl;

    return EXIT_SUCCESS;
}
