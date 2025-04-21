#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;


/**
* @author Tanner Hunt
* @date 4/20/2025
* Note:
* normally I would put all my classes in different files with associated header
* files, but the project instructions wants everything in one .cpp file.
*/
/**
* a helper class that splits a line into individual words
*/
class Tokenizer {
public:
	/**
	* breaks a string into substrings
	* @param t_string the string to be broken up
	* @param delimiter what to break the string up by
	* @return a vector of substrings, omitting the character delimiter
	*/
	static vector<string> tokenize(string t_string, char delimiter) {
		vector<string> t_vector(0);
		unsigned int index = 0;
		string word = "";

		//iterate over each character in the string, breaking it into substrings by a delimiter
		while (index < t_string.length()) {
			char character = t_string.at(index);

			//grow the word if the character does not match the delimiter
			if (character != delimiter) {
				word = word + character;
			}

			//add the word to the vector and start a new word if the character matches the delimiter
			else {
				t_vector.push_back(word);
				word = "";
			}
			index++;
		}
		t_vector.push_back(word);
		return t_vector;
	}

};

struct Course {
	Course(string id, string name) {
		courseID = id;
		courseName = name;
		prerequisites.resize(0);
	}

	string courseID;
	string courseName;
	vector<string> prerequisites;
};

class Node {
public:
	//constructor
	Node(Course* course) {
		this->left = nullptr;
		this->right = nullptr;
		this->parent = nullptr;
		this->course = course;
	}

	//destructor
	~Node() {
		delete this->course;
	}

	/**
	@return the ID of this nodes associated course
	*/
	string getId() {
		return this->course->courseID;
	}

	/**
	@return the data member of this node
	*/
	Course* getCourse() {
		return this->course;
	}

	/**
	* @return a vector of strings representing the prerequisite course ID's
	*/
	vector<string> getPrerequisites() {
		return this->course->prerequisites;
	}

	/**
	@return A string representing this nodes associated course
	*/
	string toString() {
		//add the coures id and name
		string id = this->course->courseID;
		string name = this->course->courseName;
		string t_string = id + " " + name;

		//add prerequisite course information
		if (!this->course->prerequisites.empty()) {
			t_string = t_string + "\n\t" + to_string(this->course->prerequisites.size()) + " prerequisite";
			//differentiate between "prerequisite:" and "prerequisites:"
			if (this->course->prerequisites.size() > 1) {
				t_string = t_string + "s:";
			}
			else {
				t_string = t_string + ":";
			}

			//add all prerequisites
			for (string course : this->course->prerequisites) {
				t_string = t_string + "\n\t\t" + course;
			}
			t_string = t_string + "\n";
		}

		//notify user if this course has no prerequisites
		else {
			t_string = t_string + "\n\t no prerequisites\n";
		}

		return t_string;
	}

	/**
	@return this nodes left child
	*/
	Node* getLeft() {
		return this->left;
	}

	/**
	* @return this nodes right child pointer
	*/
	Node* getRight() {
		return this->right;
	}

	/**
	* @return this nodes parent pointer
	*/
	Node* getParent() {
		return this->parent;
	}

	/**
	*@return the node that should replace this one should it be deleted.  This may be a nullpointer, its only child, or its right childs leftmost grandchild
	*/
	Node* getSuccessorNode() {
		//case where node is a leaf node
		if(this->getLeft() == nullptr && this->getRight() == nullptr){
			return nullptr;
		}

		//case where node has only left child
		else if(this->getLeft() != nullptr && this->getRight() == nullptr){
			return this->getLeft();
		}

		//case where node has only right child
		else if (this->getLeft() == nullptr && this->getRight() != nullptr) {
			return this->getRight();
		}

		//case where node has both left and right child.  Find the right childs left-most node
		else{
			Node* t_node = this->getRight();
			//move down the left subtree of this nodes right child
			while (t_node->getLeft() != nullptr) {
				t_node = t_node->getLeft();
			}
			return t_node;
		}
	}

	/**
	* sets this nodes left child to the node
	*/
	void setLeft(Node* node) {
		this->left = node;
	}

	/**
	* sets this nodes right pointer
	*/
	void setRight(Node* node) {
		this->right = node;
	}

	/**
	* sets this nodes parent pointer
	*/
	void setParent(Node* node) {
		this->parent = node;
	}

	/**
	* makes this nodes parent point to this nodes successor node
	*/
	void updateParent() {
		//case where this node has no parent
		if (this->getParent() == nullptr) {
			return;
		}

		//case where this is the parents left child
		else if (this->getParent()->getLeft() == this) {
			parent->setLeft(this->getSuccessorNode());
		}

		//case where this is the parents right child
		else {
			parent->setRight(this->getSuccessorNode());
		}
	}
private:
	Course* course;
	Node* left;
	Node* right;
	Node* parent;
};

/**
* an unbalanced binary search tree that stores Course objects.  Must be accessed as a singleton
* object through the getInstance() method
*/
class BSTree {
public:
	/**
	* singleton access method
	*/
	static BSTree* getInstance() {
		if (instance == nullptr) {
			instance = new BSTree();
		}
		return instance;
	}

	~BSTree() {
		//recursively delete the root, emptying the tree
		while (root != nullptr) {
			remove(root);
		}
	}
	
	/**
	* adds a course to the BSTree.  Returns a Node* for faster deletions when courses are validated.
	* @param course: a pointer to the Course object which will be the Nodes data member
	* @return the node that was created by this operation
	*/
	Node* add(Course* course){
		Node* node = new Node(course);

		//case where tree is empty
		if (root == nullptr) {
			root = node;
			return root;
		}

		Node* curNode = root;
		while (curNode != nullptr) {
			//case where node should go left, but node already has child
			if (node->getId() <= curNode->getId() && curNode->getLeft() != nullptr) {
				curNode = curNode->getLeft();
			}

			//case where node should go right, but node already has a child
			else if (node->getId() > curNode->getId() && curNode->getRight() != nullptr) {
				curNode = curNode->getRight();
			}

			//case where node should go left and slot is available
			else if (node->getId() <= curNode->getId() && curNode->getLeft() == nullptr) {
				curNode->setLeft(node);
				node->setParent(curNode);
				//escape while-loop and return the created node
				curNode = nullptr;
			}
			//case where node should go right and slot is available
			else {
				curNode->setRight(node);
				node->setParent(curNode);
				//escape while-loop and return the created node
				curNode = nullptr;
			}
		}

		//update the size of the tree
		size++;
		return node;
	}

	/**
	* deletes a specified node and glues its parents and children together
	*/
	void remove(Node* node){
		//caching variables for readability.  Lines get long with a lot of nested pointers without this.
		Node* parent = node->getParent();
		Node* left = node->getLeft();
		Node* right = node->getRight();
		Node* successorNode = node->getSuccessorNode();

		//update the parents child pointer to the successor node
		node->updateParent();

		//update the successor nodes connections
		if (successorNode != nullptr) {

			successorNode->setParent(parent);
			//update the successor nodes children pointers.
			//this doesn't need to be done if the node only has one child
			if (left != nullptr && right != nullptr) {
				successorNode->setLeft(left);
				//make sure the successor node won't be pointing at itself
				if (node->getRight() != successorNode) {
					successorNode->setRight(right);
				}
			}
		}
		
		//update the size of the tree
		size--;
		delete node;
	}

	/**
	* returns an individual courses details in the BST along with its prerequisites
	*/
	string getDetails(string courseId) {
		Node* curNode = root;

		//iterate down the tree until the correct node is found
		while (curNode != nullptr) {
			//case where course is found
			if (curNode->getId() == courseId) {
				return curNode->toString();
			}

			//case where courseId may be in left subtree
			else if (courseId < curNode->getId()) {
				curNode = curNode->getLeft();
			}

			//case where courseId may be in right subtree
			else {
				curNode = curNode->getRight();
			}
		}

		//tree empty and/or course not found.
		return "Course not found\n";
	}

	/**
	* print the courses in ascending order in the BSTree
	*/
	void print() {
		if (root == nullptr) {
			return;
		}

		//passing the root to the recursive version of this function
		printPreorder(root);
	}

	int getSize() {
		return this->size;
	}

private:
	Node* root;
	static BSTree* instance;
	int size = 0;
	
	//private constructor forces methods to use getInstance() method
	BSTree() {
		root = nullptr;
	};

	/**
	* A recursive function that prints the courses in the BST in ascending order
	*/
	void printPreorder(Node* node) {

		//to print in ascending order, call on left nodes first, print the node, then call on right nodes
		if (node->getLeft() != nullptr) {
			printPreorder(node->getLeft());
		}

		//print the nodes course
		cout << node->toString() << endl;

		if (node->getRight() != nullptr) {
			printPreorder(node->getRight());
		}
	}

};



//initialize the static member variable in BSTree... outside the class... why is C++ like this?
BSTree* BSTree::instance = nullptr;

class FileParser {
public:
	//singleton access method
	static FileParser* getInstance() {
		if (instance == nullptr) {
			instance = new FileParser();
		}
		return instance;
	}

	void readFile() {
		ifstream file("CS 300 ABCU_Advising_Program_Input.csv");
		string line = "";

		//turn each line into a course and add to BSTree
		while (getline(file, line)) {
			Course* course = toCourse(line);
			if (course != nullptr) {
				addCourseToTree(course);
			}
		}

		validateCourses();
	}



private:
	static FileParser* instance;
	queue<Node*> coursesToValidate;
	//private constructor forces methods to use getInstance() method
	FileParser() {};

	/**
	* convert a string into a course object.  Validate the line is valid.
	*/
	Course* toCourse(string line) {
		vector<string> inputs = Tokenizer::tokenize(line, ',');

		//lines without both a courseID and courseName
		if (inputs.size() < 2) {
			return nullptr;
		}
		
		string id = inputs.at(0);
		string name = inputs.at(1);

		Course* course = new Course(id, name);

		//add prerequisite courses.  Prerequisites start at the third token
		for (int i = 2; i < inputs.size(); i++) {
			//don't add blank prerequisite courses
			if (inputs.at(i).empty()) {
				continue;
			}
			course->prerequisites.push_back(inputs.at(i));
		}

		return course;
	}

	/**
	* add a course to the binary search tree.  Also add the course to the queue of courses
	* to be validated if it has any prerequisites
	*/
	void addCourseToTree(Course* course) {
		BSTree* tree = BSTree::getInstance();
		if (course->prerequisites.size() > 0) {
			coursesToValidate.push(tree->add(course));
		}
		else {
			tree->add(course);
		}
	}

	void validateCourses() {
		BSTree* tree = BSTree::getInstance();

		//check that every course added to the queue has valid prerequisite courses
		while(!coursesToValidate.empty()){
			Node* node = coursesToValidate.front();
			vector<string> prerequisites = node->getPrerequisites();
			
			//search through this nodes prerequisite courses
			for (unsigned int i = 0; i < prerequisites.size(); i++) {
				string prereqCourse = prerequisites.at(i);
				coursesToValidate.pop();
				//if tree does not contain this class ID, delete the node and skip to the next node
				if (tree->getDetails(prereqCourse) == "Course not found\n") {
					tree->remove(node);
					break;
				}
			}

			//remove this course from the queue
		}
	
	}
};

//initialize the static member variable in FileParser
FileParser* FileParser::instance = nullptr;

/**
* a base class for menu options that will prompt the users for a valid input, what that input does
* and activates code if a valid input is given
*/
class AbstractOption {
public:

	/**
	* the description that will be printed when an option is printed to a menu
	*/
	void setDescription(string desc) {
		this->description = desc;
	}

	/**
	* prints what input calls this option followed by what this option does
	*/
	void printDescription() {
		cout << this->validInput << ": " << this->description;
	}

	/**
	* sets what user inputs will call this menu option
	*/
	void setInput(string input) {
		this->validInput = input;
	}

	/**
	* decides whether a users input should call this menu options associated function
	*/
	virtual bool input(string input) {
		if (input == validInput) {
			this->onSelect();
			return true;
		}
		return false;
	}

private:
	/**
	* what the option should do  if the method is called
	*/
	virtual void onSelect() = 0;
	string description;
	string validInput;

};

/**
* loads data from a CSV into a BSTree if the user enters an appropriate key,
* then validates the courses
*/
class LoadDataOption : public AbstractOption {
public:
	/**
	* constructor.  Set the description for this menu option.
	*/
	LoadDataOption() {
		AbstractOption::setDescription("Loads course data from a CSV file");
		AbstractOption::setInput("1");
	}
	
	void onSelect() override{
		FileParser::getInstance()->readFile();
		cout << "Loaded " << BSTree::getInstance()->getSize() << " courses" << endl;
	}
};

/**
* prints all the data stored in the BSTree
*/
class PrintDataOption : public AbstractOption {
public:
	/**
	* constructor.  Set the description and valid inputs for this menu option.
	*/
	PrintDataOption() {
		AbstractOption::setDescription("Prints an alphanumeric list of all courses in the Computer Science Department");
		AbstractOption::setInput("2");
	}

	void onSelect() override {
		BSTree::getInstance()->print();
	}
};

/**
* finds an individual course in the BSTree and prints its details
*/
class PrintCourseOption : public AbstractOption {
public:
	/**
	* constructor.  Set the description and valid inputs for this menu option.
	*/
	PrintCourseOption() {
		AbstractOption::setDescription("Prints an individual course and it's prerequisites");
		AbstractOption::setInput("3 + Course ID");
	}
	
	void onSelect() override {
		cout << BSTree::getInstance()->getDetails(courseId);
	}

	bool input(string input) override {
		vector<string> inputs = Tokenizer::tokenize(input, ' ');

		//this option requires two input words: a 3 and a course ID
		if(inputs.size() >= 2 && inputs.at(0) == "3") {
			courseId = inputs.at(1);
			onSelect();
			return true;
		}
		return false;
	}

private:
	string courseId;
};

/**
* closes the console
*/
class ExitProgramOption : public AbstractOption {
public:
	/**
	* constructor.  Set the description and valid inputs for this menu option.
	*/
	ExitProgramOption() {
		AbstractOption::setDescription("Close the program");
		AbstractOption::setInput("9");
	}

	/**
	* close the program
	*/
	void onSelect() override {
		exit(0);
	}
};

/**
* prompts the user for input and acts as a mediator to activate the appropriate
* code based on that input
*/
class Menu {
public:
	//constructor. Adds all menu options
	Menu() {
		menuOptions.resize(0);
		
		menuOptions.push_back(new LoadDataOption());
		menuOptions.push_back(new PrintDataOption());
		menuOptions.push_back(new PrintCourseOption());
		menuOptions.push_back(new ExitProgramOption());
	}

	//destructor
	~Menu() {
		for (unsigned int i = 0; i < menuOptions.size(); i++) {
			delete menuOptions.at(i);
		}
	}

	/**
	* output all valid inputs and their behaviors
	*/
	void printMenu() {
		for (unsigned int i = 0; i < menuOptions.size(); i++) {
			menuOptions.at(i)->printDescription();
			cout << endl;
		}
	}

	/**
	* activate the menu option for the users input.  Prompts the user to enter a valid input if an incorrect input is entered.
	*/
	void tryInput(string input) {
		for (unsigned int i = 0; i < menuOptions.size(); i++) {
			if (menuOptions.at(i)->input(input)) {
				return;
			}

		}
		cout << "invalid input.  Please enter a valid input key, e.g. \"1\", \"3 CSCI100\"" << endl;
	}

private:
	vector<AbstractOption*> menuOptions;

};

int main()
{
	Menu* menu = new Menu();

	//repeatedly print the menu and take the users input
	while (true) {
		string input = "";
		menu->printMenu();
		getline(cin, input);
		menu->tryInput(input);
	}

}

