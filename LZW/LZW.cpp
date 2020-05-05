#include <iostream>
#pragma warning(disable : 4996)
struct node {
	const char* value;
	char ch;
	node* child;
	node* side;
	uint8_t num;
};
class Tree {
	uint8_t count;
	node* find(const char* key);
	void insert(const char* key, std::string& nstr);
public:
	node* root;
	Tree() :count('0'), root(nullptr) {}
	void printTree(node* root_, size_t level);
	void decoder(node* root_, char* str, uint8_t level);
	friend Tree coder(const char* str, std::string& nstr);
};
Tree coder(const char* str, std::string& nstr) {
	Tree tree;
	char* temp1 = nullptr;
	char* temp = new char[2];
	node* prev;
	size_t lenght = strlen(str);
	for (; *str != '\0'; ++str) {
		temp = new char[2];
		*temp = *str;
		*(temp + 1) = '\0';
		if (tree.find(temp) == nullptr) {
			tree.insert(temp, nstr);
		}
	}
	str -= lenght;
	temp = new char[2];
	*temp = *str;
	*(temp + 1) = '\0';
	uint8_t prev_n = '0';
	while (*str != '\0') {
		if ((prev = tree.find(temp)) == nullptr) {
			nstr += prev_n;
			tree.insert(temp, nstr);
			temp = new char[2];
			*temp = *str;
			*(temp + 1) = '\0';
		}
		else {
			prev_n = prev->num;
			delete[] temp1;
			char* temp1 = new char[strlen(temp) + 1];
			strcpy(temp1, temp);
			delete[] temp;
			if (*(++str) == '\0') { nstr += prev_n; break; }
			temp = new char[strlen(temp1) + 2];
			strcpy(temp, temp1);
			strncat(temp, str, 1);
			*(temp + strlen(temp) + 1) = '\0';
		}
	}
	return tree;
}
int main()
{
	system("chcp 1251->LZW");
	Tree tree;
	std::string newCoderLine;
	char* enterLine = new char[10];
	uint8_t choose;
	std::cout << "Enter line" << std::endl;
	std::cin >> enterLine;
	tree = coder(enterLine, newCoderLine);
	char* newLine = new char[newCoderLine.length() + 1];
	for (size_t i = 0; i < newCoderLine.length(); ++i)
		*(newLine + i) = newCoderLine[i];
	while (1) {
		std::cout << "\n\t1-Print compressed line\n";
		std::cout << "\t2-Print prefix tree\n";
		std::cout << "\t3-Decoder line\n";
		std::cin >> choose;
		if (choose == '1')
			std::cout << newCoderLine << std::endl;
		else if (choose == '2')
			tree.printTree(tree.root, 0);
		else if (choose == '3')
			tree.decoder(tree.root, newLine, '1');
		else break;
	}
	return 0;
}

node* Tree::find(const char* key)
{
	node* temp = nullptr, * list = nullptr;
	for (list = root; *key != '\0'; ++key) {
		for (temp = list; temp != nullptr; temp = temp->side) {
			if (temp->ch == *key)
				break;
		}
		if (temp != nullptr)
			list = temp->child;
		else
			return nullptr;
	}
	if (temp == nullptr)
		return nullptr;
	return temp;
}

void Tree::insert(const char* key, std::string& nstr)
{
	const char* value = key;
	node* temp = nullptr, * parent, * list;
	parent = nullptr;
	list = root;
	while (*key != '\0') {
		for (temp = list; temp != nullptr; temp = temp->side) {
			if (temp->ch == *key)
				break;
		}
		if (temp == nullptr) {
			temp = new node;
			temp->ch = *key;
			temp->value = value;
			temp->num = count;
			temp->side = list;
			temp->child = nullptr;
			if (parent != nullptr) {
				parent->child = temp;
			}
			else
				root = temp;
			list = nullptr;
		}
		else {
			list = temp->child;
		}
		parent = temp;
		++key;
	}
	++count;
}

void Tree::printTree(node* root_, size_t level)
{
	node* temp;
	size_t i;
	for (temp = root_; temp != nullptr; temp = temp->side) {
		for (i = 0; i < level; i++)
			std::cout << " ";
		if (temp->value != nullptr)
			std::cout << temp->ch << " " << temp->value << " " << temp->num << std::endl;
		else
			std::cout << temp->ch << std::endl;
		if (temp->child != nullptr)
			printTree(temp->child, level + 1);
	}
}

void Tree::decoder(node* root_, char* nstr, uint8_t level)
{
	node* temp;
	for (temp = root_; temp != nullptr; temp = temp->side) {
		if (*nstr == temp->num) {
			std::cout << temp->value;
			if (*(++nstr) == '\0') {
				exit(0);
			}
			decoder(this->root, nstr, level + 1);
		}
		if (*(nstr) == '\0') {
			return;
		}

		if ((temp != nullptr) && (temp->child != nullptr))
			decoder(temp->child, nstr, level);
	}
}
