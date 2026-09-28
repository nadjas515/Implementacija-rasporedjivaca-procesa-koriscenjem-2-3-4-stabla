#include "Node.h"
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Tree
{
public:
	Tree(int increment);
	Tree(Key* key, int increment);
	Tree(Node* root, int increment);
	void insertKey(Key* key);
	void levelOrder(std::ostream& output);
	void inOrder(std::ostream& output);
	void balanceTree(Node* newNode);
	Node* findKey(Key* key);
	Node* findKeyByName(Key* key);
	Key* findKeyByTime(int w, int e);
	void deleteKey(Key* key);
	void inputTree(std::istream& input);
	void process(std::ostream& output);
	void incrementProcesses(std::ostream& output, int inc);
	void resetIters();
	void redBlackTree(std::ostream& output);
	~Tree();
private:
	Node* root;
	int increment;
};
