#include<iostream>

struct Node
{
	int data;
	Node* next;

	Node();
	Node(int data, Node* next = nullptr);
	~Node();

	friend std::ostream& operator<<(std::ostream& stream, Node& node)
	{
		stream << node.data;
		return stream;
	}
};

class LinkedList
{
public:
	LinkedList();
	~LinkedList();

	int Length();
	bool IsEmpty();

	void PushHead(int data);
	void Insert(int index, int data);

	void PushTail(int data);
	int PopHead();

	int Extract(int index);
	int PopTail();

	int Data(int index);
	void swap(int ia, int ib);
	void sort();

	friend std::ostream& operator<<(std::ostream& stream, LinkedList& list);

private:
	void dispose();

	int PopData(Node* node);
	void InsertNode(int index, Node* node);
	Node* ExtractNode(int index);
	bool IndexValid(int index);

	Node* head;
};

int main(int argc, char* argv[]) {
	LinkedList list;
	std::cout << list << std::endl;
	list.PushHead(1);
	std::cout << list << std::endl;
	list.PushTail(3);
	std::cout << list << std::endl;
	list.Insert(1, 2);
	std::cout << list << std::endl;
	list.Insert(3, 4);
	std::cout << list << std::endl << std::endl;
	std::cout << list << std::endl;
	std::cout << list.PopHead() << " ";
	std::cout << list.Extract(1) << " ";
	std::cout << list.PopTail() << " ";
	std::cout << list.Extract(0) << std::endl;
	std::cout << list << std::endl << std::endl;

	list.PushHead(3); list.PushHead(4); list.PushHead(1); list.PushHead(2);
	list.PushHead(7); list.PushHead(5); list.PushHead(8); list.PushHead(6);
	std::cout << list << std::endl;
	list.sort();
	std::cout << list << std::endl;
	return EXIT_SUCCESS;
}


Node::Node() : data(0), next(nullptr)
{
}
Node::Node(int data, Node* next) : data(data), next(next)
{
}
Node::~Node()
{
	data = 0; next = nullptr;
}

LinkedList::LinkedList() : head(nullptr)
{
}

LinkedList::~LinkedList()
{
	dispose();
}

int LinkedList::Length()
{
	int len = 0;
	Node* newNd = head;
	while (newNd != nullptr)
	{
		newNd = newNd->next;
		++len;
	}

	return len;
}

bool LinkedList::IsEmpty()
{
	return head == nullptr;
}

void LinkedList::PushHead(int data)
{
	Insert(0, data);
}

void LinkedList::Insert(int index, int data)
{
	Node* nd = new Node(data);
	InsertNode(index, nd);
}

void LinkedList::PushTail(int data)
{
	Insert(Length(), data);
}

int LinkedList::PopHead()
{
	return PopData(ExtractNode(0));
}

int LinkedList::Extract(int index)
{
	return PopData(ExtractNode(index));
}

int LinkedList::PopTail()
{
	return PopData(ExtractNode(Length() - 1));
}

int LinkedList::Data(int index)
{
	if (!IndexValid(index) || (Length() - 1 < index))
	{
		return -1;
	}

	Node* nd = head;
	while (index > 0)
	{
		nd = nd->next;
		index = index - 1;
	}

	return nd->data;
}

void LinkedList::swap(int ia, int ib)
{
	if (!IndexValid(ia) || !IndexValid(ib) || ia == ib)
	{
		return;
	}

	if (ia > ib)
	{
		int c = ib;
		ib = ia;
		ia = c;
	}

	Node* extractedNodeB = ExtractNode(ib);
	Node* extractedNodeA = ExtractNode(ia);

	if (extractedNodeA == nullptr || extractedNodeB == nullptr)
	{
		return;
	}

	InsertNode(ia, extractedNodeB);
	InsertNode(ib, extractedNodeA);
}

void LinkedList::sort()
{
	for (int i = 0; i < Length(); i++)
	{
		for (int j = 0; j < Length() - 1; j++)
		{
			if (Data(j) > Data(j + 1))
			{
				swap(j, j + 1);
			}
		}
	}
}

void LinkedList::dispose()
{
	while (head != nullptr)
	{
		Node* nd = head;
		head = head->next;
		delete nd;
	}
}

int LinkedList::PopData(Node* node)
{
	if (node == nullptr)
	{
		return 0;
	}

	int res = node->data;
	delete node;

	return res;
}

void LinkedList::InsertNode(int index, Node* node)
{
	if (!IndexValid(index))
	{
		return;
	}

	Node* nd = head;
	if (index == 0)
	{
		node->next = head;
		head = node;
	}
	else
	{
		for (int i = 0; i < index - 1 && nd->next != nullptr; i++)
		{
			nd = nd->next;
		}

		node->next = nd->next;
		nd->next = node;
	}
}

Node* LinkedList::ExtractNode(int index)
{
	if (!IndexValid(index))
	{
		return nullptr;
	}

	Node* nd = head;
	if (index == 0)
	{
		head = head->next;
	}
	else
	{
		Node* prev = head;
		for (int i = 0; i < index - 1 && prev->next != nullptr; i++)
		{
			prev = prev->next;
		}

		if (prev->next == nullptr)
		{
			return nullptr;
		}

		nd = prev->next;
		prev->next = prev->next->next;
	}

	return nd;
}

bool LinkedList::IndexValid(int index)
{
	return (index >= 0 && index <= Length());
}

std::ostream& operator<<(std::ostream& stream, LinkedList& list)
{
	Node* nd = list.head;
	while (nd != nullptr)
	{
		stream << *nd << " ";
		nd = nd->next;
	}

	return stream;
}