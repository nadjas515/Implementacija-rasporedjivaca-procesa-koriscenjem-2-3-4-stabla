#include "Tree.h"

Tree::Tree(int increment) :root(nullptr), increment(increment) {}

Tree::Tree(Key* key, int increment) :root(new Node(key, nullptr)), increment(increment) {}

Tree::Tree(Node* root, int increment) :root(root), increment(increment) {}

void Tree::insertKey(Key* key) {
	if (!root) {
		root = new Node(key, nullptr, black);
		return;
	}
	Node* newNode = findKey(key)->insertKey(key);
	if (newNode) {
		balanceTree(newNode);
	}
}

void Tree::levelOrder(std::ostream& output) {
	if (!root)
		return;
	std::queue<Node*> q;
	Node* p = root;
	q.push(p);
	while (!q.empty()) {
		p = q.front();
		p->print(output);
		q.pop();
		if (!p->isLeaf()) {
			for (int i = 0; i < 4; i++) {
				if (p->getNode(i)) q.push(p->getNode(i));
			}
		}
		output << std::endl;
	}
}

void Tree::inOrder(std::ostream& output) {
	resetIters();
	if (!root)
		return;
	std::stack<Node*> s;
	s.push(root);
	Node* p = root;
	int i;
	while (!s.empty()) {
		while (!p->isLeaf()) {
			i = p->getIter();
			if (p->end()) s.pop();
			s.push(p->getNode(i));
			p = p->getNode(i);
		}
		if (p->isLeaf()) {
			p->printAllKeys(output);
			s.pop();
			if (s.empty()) break;
			p = s.top();
			while (p->end()) {
				s.pop();
				p = s.top();
			}
			if (!p->end()) {
				i = p->getIter();
				p->getKey(i)->print(output);
				output << std::endl;
				p->next();
			}
		}

	}

}

void Tree::balanceTree(Node* newNode) {
	Node* parentNode = newNode->getParent();
	while (parentNode->numOfKeys() == 3) {
		parentNode->breakNode(newNode);
		if (parentNode->getParent()) {
			newNode = parentNode;
			parentNode = parentNode->getParent();
		}
		else {
			root = parentNode;
			return;
		}
	}
	parentNode->adoptNode(newNode);
}

Node* Tree::findKey(Key* key) {
	if (!root)
		return nullptr;
	Node* temp = root;
	while (!temp->isLeaf()) {
		if (temp->findKey(key) >= 0) return temp;
		if (*temp->getKey(1) < *key) {
			if (!temp->getKey(0) || *temp->getKey(0) > *key)
				temp = temp->getNode(1);
			else
				temp = temp->getNode(0);
		}
		else {
			if (!temp->getKey(2) || *temp->getKey(2) < *key)
				temp = temp->getNode(2);
			else
				temp = temp->getNode(3);
		}
	}
	return temp;
}

Key* Tree::findKeyByTime(int w, int e) {
	if (!root)
		return nullptr;
	std::queue<Node*> q;
	Node* p = root;
	q.push(p);
	while (!q.empty()) {
		p = q.front();
		q.pop();
		for (int i = 0; i < 3; i++) {
			if (p->getKey(i) && p->getKey(i)->getWaitingTime() == w && p->getKey(i)->getExecutionTime() == e)
				return p->getKey(i);
		}
		if (!p->isLeaf()) {
			for (int i = 0; i < 4; i++) {
				if (p->getNode(i)) q.push(p->getNode(i));
			}
		}
	}
	return nullptr;
}

Node* Tree::findKeyByName(Key* key) {
	if (!root)
		return nullptr;
	std::queue<Node*> q;
	Node* p = root;
	q.push(p);
	while (!q.empty()) {
		p = q.front();
		q.pop();
		if (p->findKey(key) >= 0) return p;
		if (!p->isLeaf()) {
			for (int i = 0; i < 4; i++) {
				if (p->getNode(i)) q.push(p->getNode(i));
			}
		}
	}
	return nullptr;
}

void Tree::deleteKey(Key* key) {
	if (!root)
		return;
	Node* node = findKeyByName(key);
	if (!node)
		return;
	if (!node->isLeaf()) {
		int index = node->findKey(key);
		Node* pred = node->findPred(index);
		int predKey = pred->firstKey();
		Node::exchangeKeys(node, index, pred, predKey);
		node = pred;
	}
	node->deleteKey(key);
	while (node && node->numOfKeys() == 0) {
		Node* parent = node->getParent();
		if (!parent) {
			parent = node;
			node = node->firstSubTree();
			parent->resetNode();
			delete parent;

			root = node;
			if (node)
				node->setParent(nullptr);
			return;
		}
		parent->sortKids(node);
		node = parent;
	}
}

void Tree::inputTree(std::istream& input) {
	std::string line;
	while (std::getline(input, line)) {
		if (!line.size()) return;
		std::istringstream ss(line);
		std::string name;
		int complete, max;
		ss >> name >> complete >> max;
		if (name.size() > 256) {
			std::cout << "Predugacko ime, mora biti ispod 256 znakova" << std::endl;
			return;
		}
		Key* key = new Key(name, complete, max);
		insertKey(key);

	}
}

void Tree::process(std::ostream& output) {
	while (root) {
		inOrder(output);
		output << std::endl;
		Node* current = root;
		while (current->firstSubTree()) {
			current = current->firstSubTree();
		}
		Key* currentProcess = current->getKey(current->firstKey());

		int inc = currentProcess->incrementExecutionTime(increment);
		currentProcess->incrementWaitingTime(inc);
		if (currentProcess->isWaitingTimeExceeded()) {
			currentProcess->resetWaitingTime();
		}
		Key* temp = new Key(currentProcess);
		output << "Izvrsava se proces" << std::endl;
		temp->print(output);
		output << std::endl;
		deleteKey(currentProcess);
		incrementProcesses(output, inc);
		if (!temp->isDone())
			insertKey(temp);
		else
			delete temp;
		output << "-----------------" << std::endl;
	}

}

void Tree::incrementProcesses(std::ostream& output, int inc) {
	resetIters();
	if (!root)
		return;
	std::queue<Key*> q;
	std::stack<Node*> s;
	s.push(root);
	Node* p = root;
	int i;
	while (!s.empty()) {
		while (!p->isLeaf()) {
			i = p->getIter();
			if (p->end()) s.pop();
			s.push(p->getNode(i));
			p = p->getNode(i);
		}
		if (p->isLeaf()) {
			for (int j = 0; j < 3; j++) {
				if (p->getKey(j)) {
					p->getKey(j)->incrementWaitingTime(inc);
					if (p->getKey(j)->isWaitingTimeExceeded()) {
						q.push(p->getKey(j));
					}
				}
			}
			s.pop();
			if (s.empty()) break;
			p = s.top();
			while (p->end()) {
				s.pop();
				p = s.top();
			}
			if (!p->end()) {
				i = p->getIter();
				if (p->getKey(i)) {
					p->getKey(i)->incrementWaitingTime(inc);
					if (p->getKey(i)->isWaitingTimeExceeded()) {
						q.push(p->getKey(i));
					}
				}
				p->next();
			}
		}
	}
	if (!q.empty()) {
		output << std::endl << "Izgled stabla prije izbacivanja procesa koji su prekoracili vrijeme cekanja" << std::endl;
		inOrder(output);
	}
	while (!q.empty()) {
		Key* k = q.front();
		k->resetWaitingTime();
		Key* temp = new Key(k);
		output << "Resetovalo se vrijeme procesa: ";
		k->print(output);
		output << std::endl;
		deleteKey(k);
		insertKey(temp);
		q.pop();
	}
}

void Tree::resetIters() {
	if (!root)
		return;
	std::queue<Node*> q;
	Node* p = root;
	q.push(p);
	while (!q.empty()) {
		p = q.front();
		p->begin();
		q.pop();
		if (!p->isLeaf()) {
			for (int i = 0; i < 4; i++) {
				if (p->getNode(i)) q.push(p->getNode(i));
			}
		}
	}
}

void Tree::redBlackTree(std::ostream& output) {
	if (!root)
		return;
	std::queue<Node*> q;
	Node* p = root;
	q.push(p);
	while (!q.empty()) {
		p = q.front();
		if (p->getKey(1)) {
			Node* parent = p->getParent();
			if (parent) {
				output << "parent:";
				int index = parent->findChild(p) / 2 * 2;
				if (parent->getKey(index))
					parent->getKey(index)->print(output);
				else
					parent->getKey(1)->print(output);
				output << std::endl;
			}
			p->getKey(1)->print(output);
			output << std::endl;
			if (p->getKey(0)) {
				output << "parent:";
				p->getKey(1)->print(output);
				output << std::endl;
				p->getKey(0)->print(output);
				output << std::endl;
			}
			if (p->getKey(2)) {
				output << "parent:";
				p->getKey(1)->print(output);
				output << std::endl;
				p->getKey(2)->print(output);
				output << std::endl;
			}
		}
		q.pop();
		if (!p->isLeaf()) {
			for (int i = 0; i < 4; i++) {
				if (p->getNode(i)) q.push(p->getNode(i));
			}
		}
		output << std::endl;
	}
}

Tree::~Tree() {
	if (!root) return;
	std::stack<Node*> q;
	Node* p = root;
	q.push(p);
	while (!q.empty()) {
		p = q.top();
		q.pop();
		if (!p->isLeaf()) {
			for (int i = 0; i < 4; i++) {
				if (p->getNode(i)) q.push(p->getNode(i));
			}
		}
		delete p;
	}
}