#include <vector>
#include <cmath>
#include "Key.h"

class Node
{
public:
	Node();
	Node(Key* key, Node* parent = nullptr, colour c = red);
	Node* insertKey(Key* key);
	int size();
	bool isLeaf();
	bool isEmpty();
	void print(std::ostream& output);
	void printAllKeys(std::ostream& output);
	Key* getKey(int index);
	Node* getParent();
	Node* getNode(int index);
	Node* firstSubTree();
	int firstKey();
	int findChild(Node* child);
	int findKey(Key* key);
	void adoptNode(Node* newNode);
	void breakNode(Node* newNode);
	int numOfKeys();
	void deleteKey(Key* key);
	void sortKids(Node* node);
	Node* findPred(int index);
	void setKey(int index, Key* other);
	static void exchangeKeys(Node* n1, int i, Node* n2, int j);
	void shiftLeft();
	void shiftRight();
	void borrowOneKey(int empty, int brother);
	void takeOneKey(int empty, int brother);
	void colourNode();
	void begin();
	void next();
	bool end();
	int getIter();
	void resetNode();
	void setParent(Node* p);
	~Node();
private:
	Key* keys[3];
	Node* nodes[4];
	Node* parent;
	int iter;
};

inline bool Node::isLeaf()
{
	return size() == 0;
}

inline Key* Node::getKey(int index) {
	return keys[index];
}

inline Node* Node::getParent() {
	return parent;
}

inline Node* Node::getNode(int index) {
	return nodes[index];
}

inline void Node::setKey(int index, Key* other) {
	keys[index] = other;
}

inline void Node::begin() {
	iter = firstKey();
}

inline void Node::next() {
	iter++;
}

inline bool Node::end() {
	int last = -1;
	for (int i = 3; i >= 0; i--) {
		if (nodes[i]) {
			last = i;
			break;
		}
	}
	return last == iter;
}

inline int Node::getIter() {
	return iter;
}

inline void Node::setParent(Node* p) {
	parent = p;
}
