#include "Tree.h"

int main() {
	int a;
	int increment;
	int w, e;
	Tree* t = nullptr;
	Key* key;
	std::string name;
	int index;
	Node* node;
	while (1) {
		std::cout << "Meni:" << std::endl\
			<< "1. Stvaranje praznog stabla" << std::endl\
			<< "2. Pretrazivanje stabla po trenutnom vremenu cekanja i izvrsavanja procesa" << std::endl\
			<< "3. Umetanje novih procesa u stablo sa standardnog ulaza" << std::endl\
			<< "4. Ucitavanje novih procesa iz datoteke" << std::endl\
			<< "5. Citanje procesa" << std::endl\
			<< "6. Brisanje procesa iz stabla" << std::endl\
			<< "7. Ispis stabla" << std::endl\
			<< "8. Simulacija rasporedjivaca procesa" << std::endl\
			<< "9. Inorder obilazak" << std::endl
			<< "10. Unistavanje stabla" << std::endl\
			<< "11. Zavrsetak programa" << std::endl;
		std::cin >> a;
		switch (a) {
		case 1:
			if (!t) {
				std::cout << "Unesite inkrement vremena za svaki proces" << std::endl;
				std::cin >> increment;
				t = new Tree(increment);
			}
			else {
				std::cout << "Stablo vec postoji, samo unesite procese koje zelite" << std::endl;
			}
			break;
		case 2:
			if (!t) {
				std::cout << "Stablo ne postoji, prvo ga napravite" << std::endl;
				break;
			}
			std::cout << "Unesite trenutno vrijeme cekanja i izvrsavanja procesa."\
				<< std::endl << "Ako ih ima vise istih ispisace se prvi" << std::endl;
			std::cin >> w >> e;
			key = t->findKeyByTime(w, e);
			if (key) {
				key->print(std::cout);
				std::cout << std::endl;
			}
			else {
				std::cout << "Proces ne postoji" << std::endl;
			}
			break;
		case 3:
			if (!t) {
				std::cout << "Stablo ne postoji, prvo ga napravite" << std::endl;
				break;
			}
			std::cout << "Za prekid unosa unesite prazan red" << std::endl;
			std::cin.ignore(32677, '\n');
			t->inputTree(std::cin);
			break;
		case 4:
			if (!t) {
				std::cout << "Stablo ne postoji, prvo ga napravite" << std::endl;
				break;
			}
			{
				std::cout << "Unesite naziv datoteke" << std::endl;
				std::cin >> name;
				std::ifstream file(name);
				if (!file) {
					std::cout << "Datoteka ne postoji" << std::endl;
					break;
				}
				t->inputTree(file);
				file.close();
				break;

			}

		case 5:
			if (!t) {
				std::cout << "Stablo ne postoji, prvo ga napravite" << std::endl;
				break;
			}
			std::cout << "Unesite ime, trenutno vrijeme cekanja i izvrsavanja procesa." << std::endl;
			std::cin >> name >> w >> e;
			key = new Key(name, w, e);
			node = t->findKeyByName(key);
			if (node) {
				index = node->findKey(key);
				delete key;
				key = node->getKey(index);
				key->print(std::cout);
				std::cout << std::endl;
			}
			else {
				std::cout << "Proces ne postoji" << std::endl;
				delete key;
			}
			break;
		case 6:
			if (!t) {
				std::cout << "Stablo ne postoji, prvo ga napravite" << std::endl;
				break;
			}
			std::cout << "Unesite ime, trenutno vrijeme cekanja i izvrsavanja procesa." << std::endl;
			std::cin >> name >> w >> e;
			key = new Key(name, w, e);
			t->deleteKey(key);
			delete key;
			break;
		case 7:
			if (!t) {
				std::cout << "Stablo ne postoji, prvo ga napravite" << std::endl;
				break;
			}
			std::cout << "Unesite 1 za 2-3-4 stablo ili 2 za crno-crno stablo" << std::endl;
			std::cin >> w;
			std::cout << "Unesite 1 za ispis na standardnom izlazu ili 2 za ispis u datoteci" << std::endl;
			std::cin >> e;
			if (w == 1) {
				if (e == 1)
					t->levelOrder(std::cout);
				else if (e == 2) {
					std::cout << "Unesite naziv datoteke" << std::endl;
					std::cin >> name;
					std::ofstream file(name);
					t->levelOrder(file);
					file.close();
				}
			}
			else if (w == 2) {
				if (e == 1)
					t->redBlackTree(std::cout);
				else if (e == 2) {
					std::cout << "Unesite naziv datoteke" << std::endl;
					std::cin >> name;
					std::ofstream file(name);
					t->redBlackTree(file);
					file.close();
				}
				else {
					std::cout << "Pogresan unos" << std::endl;
					break;
				}
			}
			else {
				std::cout << "Pogresan unos" << std::endl;
				break;
			}
			break;
		case 8:
			if (!t) {
				std::cout << "Stablo ne postoji, prvo ga napravite" << std::endl;
				break;
			}
			std::cout << "Unesite 1 za ispis na standardnom izlazu ili 2 za ispis u datoteci" << std::endl;
			std::cin >> e;
			if (e == 1)
				t->process(std::cout);
			else if (e == 2) {
				std::cout << "Unesite naziv datoteke" << std::endl;
				std::cin >> name;
				std::ofstream file(name);
				t->process(file);
				file.close();
			}
			break;
		case 9:
			if (!t) {
				std::cout << "Stablo ne postoji, prvo ga napravite" << std::endl;
				break;
			}
			std::cout << "Unesite 1 za ispis na standardnom izlazu ili 2 za ispis u datoteci" << std::endl;
			std::cin >> e;
			if (e == 1)
				t->inOrder(std::cout);
			else if (e == 2) {
				std::cout << "Unesite naziv datoteke" << std::endl;
				std::cin >> name;
				std::ofstream file(name);
				t->inOrder(file);
				file.close();
			}
			break;
		case 10:
			if (!t) {
				std::cout << "Stablo ne postoji, prvo ga napravite" << std::endl;
				break;
			}
			delete t;
			t = nullptr;
			break;
		case 11:
			delete t;
			return 0;
		}
	}
	return 0;
}