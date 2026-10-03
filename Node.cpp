#include "Node.h"

Node::Node() :parent(nullptr), iter(-1) {
	for (int i = 0; i < 3; i++) {
		keys[i] = nullptr;
		nodes[i] = nullptr;
	}
	nodes[3] = nullptr;
}

Node::Node(Key* key, Node* parent, colour c) :parent(parent), iter(-1) {
	keys[0] = keys[2] = nullptr;
	keys[1] = key;
	if (c == black)
		keys[1]->turnBlack();
	for (int i = 0; i < 4; i++) {
		nodes[i] = nullptr;
	}
}

Node* Node::insertKey(Key* key) {
	switch (numOfKeys()) {
	case 0:
		keys[1] = key;
		break;
	case 1:
		if (*keys[1] < *key)
			keys[0] = key;
		else
			keys[2] = key;
		break;
	case 2:
		if (!keys[0]) {
			for (int i = 0; i < 2; i++) {
				if (*(keys[i + 1]) < *key) {
					keys[i] = key;
					return nullptr;
				}
				keys[i] = keys[i + 1];
			}
			keys[2] = key;
		}
		else if (!keys[2]) {
			for (int i = 2; i > 0; i--) {
				if (*(keys[i - 1]) > *key) {
					keys[i] = key;
					return nullptr;
				}
				keys[i] = keys[i - 1];
			}
			keys[0] = key;
		}
		break;
	case 3:
		for (int i = 0; i < 3; i++) {
			if (*key > *keys[i]) {
				nodes[i] = new Node(key, this);
				colourNode();
				return nodes[i];
			}
		}
		nodes[3] = new Node(key, this);
		colourNode();
		return nodes[3];
		break;
	}
	colourNode();
	return nullptr;
}

int Node::size() {
	int size = 0;
	for (int i = 0; i < 4; i++) {
		if (nodes[i]) size++;
	}
	return size;
}

bool Node::isEmpty() {
	for (int i = 0; i < 4; i++) {
		if (!keys[i]) return false;
	}
	return true;
}

void Node::print(std::ostream& output) {
	if (parent) {
		output << "parent:\t";
		parent->getKey(1)->print(output);
		output << std::endl;
	}
	output << "[";
	for (int i = 0; i < 3; i++) {
		if (i > 0)
			output << "|";
		if (!keys[i]) output << "NONE";
		else keys[i]->print(output);

	}
	output << "]" << std::endl;
}

void Node::printAllKeys(std::ostream& output) {
	for (int i = 0; i < 3; i++) {
		if (keys[i]) {
			keys[i]->print(output);
			output << std::endl;
		}

	}
}

Node* Node::firstSubTree() {
	for (int i = 0; i < 4; i++) {
		if (nodes[i]) return nodes[i];
	}
	return nullptr;
}

int Node::firstKey() {
	for (int i = 0; i < 4; i++) {
		if (keys[i]) return i;
	}
	return -1;
}

int Node::findChild(Node* child) {
	for (int i = 0; i < 4; i++) {
		if (nodes[i] == child) return i;
	}
	return -1;
}

int Node::findKey(Key* key) {
	for (int i = 0; i < 3; i++) {
		if (keys[i] && *(keys[i]) == *key)
			return i;
	}
	return -1;
}

void Node::adoptNode(Node* newNode) {
	int num = findChild(newNode);
	int index = (num / 2) * 2;
	if (!keys[index]) {
		keys[index] = newNode->keys[1];
		keys[index]->turnRed();
		nodes[index] = newNode->nodes[1];
		nodes[index + 1] = newNode->nodes[2];
	}
	else {
		if (!index) {
			for (int i = 2; i > num; i--) {
				nodes[i + 1] = nodes[i];
				keys[i] = keys[i - 1];
			}
			keys[num] = newNode->keys[1];
			nodes[num] = newNode->nodes[1];
			nodes[num + 1] = newNode->nodes[2];
		}
		else {
			for (int i = 0; i < num - 1; i++) {
				nodes[i] = nodes[i + 1];
				keys[i] = keys[i + 1];
			}
			keys[num - 1] = newNode->keys[1];
			nodes[num - 1] = newNode->nodes[1];
			nodes[num] = newNode->nodes[2];
		}
		colourNode();
	}
	newNode->nodes[1]->parent = newNode->nodes[2]->parent = this;
	newNode->resetNode();
	delete newNode;
}



void Node::breakNode(Node* newNode) {
	int num = findChild(newNode);
	int index = (num / 2) * 2;
	if (num % 2) {
		newNode->keys[2] = newNode->keys[1];
		newNode->keys[2]->turnRed();
	}
	else {
		newNode->keys[0] = newNode->keys[1];
		newNode->keys[0]->turnRed();
	}
	if (num % 2) {
		newNode->nodes[3] = newNode->nodes[2];
		newNode->nodes[2] = newNode->nodes[1];
		newNode->nodes[1] = nodes[index];
		if (nodes[index])
			nodes[index]->parent = newNode;
	}
	else {
		newNode->nodes[0] = newNode->nodes[1];
		newNode->nodes[1] = newNode->nodes[2];
		newNode->nodes[2] = nodes[index + 1];
		if (nodes[index + 1])
			nodes[index + 1]->parent = newNode;
	}
	newNode->keys[1] = keys[index];
	newNode->keys[1]->turnBlack();
	keys[index] = nullptr;
	Node* otherChild = new Node(keys[2 - index], this, black);
	if (index) {
		otherChild->nodes[1] = nodes[0];
		otherChild->nodes[2] = nodes[1];
		if (nodes[0])
			nodes[0]->parent = otherChild;
		if (nodes[1])
			nodes[1]->parent = otherChild;
	}
	else {
		otherChild->nodes[1] = nodes[2];
		otherChild->nodes[2] = nodes[3];
		if (nodes[2])
			nodes[2]->parent = otherChild;
		if (nodes[3])
			nodes[3]->parent = otherChild;
	}
	keys[2 - index] = nullptr;
	nodes[0] = nodes[3] = nullptr;
	if (index) {
		nodes[2] = newNode;
		nodes[1] = otherChild;
	}
	else {
		nodes[2] = otherChild;
		nodes[1] = newNode;
	}
}

int Node::numOfKeys() {
	int size = 0;
	for (int i = 0; i < 3; i++) {
		if (keys[i]) size++;
	}
	return size;
}

void Node::deleteKey(Key* key) {
	int index = findKey(key);
	delete keys[index];
	keys[index] = nullptr;
	if (index != 1) return;
	if (numOfKeys() > 0) {
		if (keys[0]) {
			keys[1] = keys[0];
			keys[0] = nullptr;
		}
		else if (keys[2]) {
			keys[1] = keys[2];
			keys[2] = nullptr;
		}
		keys[1]->turnBlack();
		return;
	}
}

void Node::sortKids(Node* node) {
	int index = findChild(node);
	int brother;
	int p = index / 2 * 2;
	if (numOfKeys() > 1) {
		if (index % 2) brother = index - 1;
		else brother = index + 1;
		if (!nodes[brother]) {
			brother = index;
			if (index == 1) {
				shiftLeft();
				index--;
			}
			else {
				shiftRight();
				index++;
			}
		}
	}
	else {
		p = 1;
		brother = 3 - index;
	}
	if (brother > index && node->nodes[2]) {
		node->nodes[1] = node->nodes[2];
		node->nodes[2] = nullptr;
	}
	else if (brother < index && node->nodes[1]) {
		node->nodes[2] = node->nodes[1];
		node->nodes[1] = nullptr;
	}
	switch (nodes[brother]->numOfKeys()) {
	case 3:
		takeOneKey(index, brother);
	case 2:
		borrowOneKey(index, brother);
		nodes[brother]->colourNode();
		break;
	case 1:
		node->keys[1] = keys[p];
		keys[p] = nullptr;
		int children = 0;
		if (brother > index) children = 2;
		node->keys[children] = nodes[brother]->keys[1];
		nodes[brother]->keys[1] = nullptr;
		node->nodes[children] = nodes[brother]->nodes[1];
		node->nodes[children + 1] = nodes[brother]->nodes[2];
		if (node->nodes[children])
			node->nodes[children]->parent = node;
		if (node->nodes[children + 1])
			node->nodes[children + 1]->parent = node;
		nodes[brother]->nodes[1] = nullptr;
		nodes[brother]->nodes[2] = nullptr;
		nodes[brother]->resetNode();
		delete nodes[brother];
		if (index == 0 || index == 3) {
			nodes[abs(index - 1)] = node;
			nodes[index] = nullptr;
		}
		else {
			nodes[brother] = nullptr;
		}
		break;
	}
	node->colourNode();
	colourNode();
}

Node* Node::findPred(int index) {
	Node* temp = nodes[index + 1];
	while (temp->firstSubTree()) {
		temp = temp->firstSubTree();
	}
	return temp;
}

void Node::exchangeKeys(Node* n1, int i, Node* n2, int j) {
	Key* temp = n1->keys[i];
	n1->keys[i] = n2->keys[j];
	n2->keys[j] = temp;
	if (i == 1)
		n1->keys[i]->turnBlack();
	else
		n1->keys[i]->turnRed();
	if (j == 1)
		n2->keys[j]->turnBlack();
	else
		n2->keys[j]->turnRed();
}

void Node::shiftLeft() {
	if (!keys[0]) {
		for (int i = 0; i < 2; i++) {
			keys[i] = keys[i + 1];
			nodes[i] = nodes[i + 1];
		}
		keys[2] = nullptr;
		nodes[2] = nodes[3];
		nodes[3] = nullptr;
		colourNode();
	}
}

void Node::shiftRight() {
	if (!keys[2]) {
		nodes[3] = nodes[2];
		for (int i = 2; i > 0; i--) {
			keys[i] = keys[i - 1];
			nodes[i] = nodes[i - 1];
		}
		keys[0] = nullptr;
		nodes[0] = nullptr;
		colourNode();
	}
}

void Node::borrowOneKey(int empty, int brother) {
	int parentIndex = empty / 2 * 2;
	if (empty + brother == 3)
		parentIndex = 1;
	nodes[empty]->keys[1] = keys[parentIndex];
	if (brother > empty) {
		nodes[brother]->shiftLeft();
		keys[parentIndex] = nodes[brother]->keys[0];
		nodes[brother]->keys[0] = nullptr;
		if (!nodes[empty]->nodes[2]) {
			nodes[empty]->nodes[2] = nodes[brother]->nodes[0];
			if (nodes[empty]->nodes[2])
				nodes[empty]->nodes[2]->parent = nodes[empty];
		}
		else {
			nodes[empty]->nodes[3] = nodes[brother]->nodes[0];
			if (nodes[empty]->nodes[3])
				nodes[empty]->nodes[3]->parent = nodes[empty];
		}
		nodes[brother]->nodes[0] = nullptr;

	}
	else {
		nodes[brother]->shiftRight();
		keys[parentIndex] = nodes[brother]->keys[2];
		nodes[brother]->keys[2] = nullptr;
		if (!nodes[empty]->nodes[1]) {
			nodes[empty]->nodes[1] = nodes[brother]->nodes[3];
			if (nodes[empty]->nodes[1])
				nodes[empty]->nodes[1]->parent = nodes[empty];
		}
		else {
			nodes[empty]->nodes[0] = nodes[brother]->nodes[3];
			if (nodes[empty]->nodes[0])
				nodes[empty]->nodes[0]->parent = nodes[empty];
		}
		nodes[brother]->nodes[3] = nullptr;
	}
}

void Node::takeOneKey(int empty, int brother) {
	if (brother > empty) {
		nodes[empty]->keys[2] = nodes[brother]->keys[0];
		nodes[brother]->keys[0] = nullptr;
		nodes[empty]->nodes[2] = nodes[brother]->nodes[0];
		nodes[brother]->nodes[0] = nullptr;
		if (nodes[empty]->nodes[2])
			nodes[empty]->nodes[2]->parent = nodes[empty];
	}
	else {
		nodes[empty]->keys[0] = nodes[brother]->keys[2];
		nodes[brother]->keys[2] = nullptr;
		nodes[empty]->nodes[1] = nodes[brother]->nodes[3];
		nodes[brother]->nodes[3] = nullptr;
		if (nodes[empty]->nodes[1])
			nodes[empty]->nodes[1]->parent = nodes[empty];
	}
}

void Node::colourNode() {
	if (keys[0])
		keys[0]->turnRed();
	if (keys[1])
		keys[1]->turnBlack();
	if (keys[2])
		keys[2]->turnRed();
}

void Node::resetNode() {
	for (int i = 0; i < 3; i++) {
		keys[i] = nullptr;
		nodes[i] = nullptr;
	}
	nodes[3] = nullptr;
	parent = nullptr;
	iter = -1;
}

Node::~Node() {
	for (int i = 0; i < 3; i++) {
		if (keys[i])
			delete keys[i];
	}
}