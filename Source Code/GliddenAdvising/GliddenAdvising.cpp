
// Written by Nick Glidden | nicholas.glidden@snhu.edu | Dec 2022
// Project Two - CS300 Data Structures and Algorithms

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <map>
#include <algorithm>

using namespace std;

// course
struct course {
	string courseNumber;
	string courseName;
	vector<string> preReqs;
};

// hash table
class HashTable {

	private:

		struct HashNode {

			unsigned int key;
			course theCourse;
			HashNode* next;

			// construct an empty node
			HashNode() {
				key = UINT_MAX;
				next = nullptr;
			}

			// construt a node with course data
			HashNode(course aCourse) : HashNode() {
				theCourse = aCourse;
			}

			// construct a node with course data, and a key value
			HashNode(course aCourse, unsigned int aKey) : HashNode(aCourse) {
				key = aKey;
			}
		};

		// our vector of nodes (used for all searching)
		vector<HashNode> nodes;
	
		// the number of buckets
		unsigned int tableSize = 180;

		// the func that will assign buckets to new nodes
		unsigned int hash(string key);

	public:
		HashTable();
		virtual ~HashTable();
		void Insert(course course);
		void findCourse(string courseNumber);
		void PrintAll();
};

// constrcuctor
HashTable::HashTable() {
	// create a node for each bucket in the hash table
	nodes.resize(tableSize);
}

// deconstructor
HashTable::~HashTable() {
	// Erase all nodes in hash table
	nodes.erase(nodes.begin());
}

// hashing algorithm
unsigned int HashTable::hash(string key) {
	
	// sum of ascii value for each char in string key
	int output = 0;
	for (int i = 0; i < key.size(); i++) {
		output += int(key[i]);
	}

	// use modolo operator to ensure bucket is within tablesize
	return output % tableSize;
}


// insert - used for loading bids
void HashTable::Insert(course course) {

	// generate key to place node (a bucket)
	unsigned key = hash(course.courseNumber);

	// see what else is in that bucket
	HashNode* nodeInBucket = &(nodes.at(key));

	// if bucket is empty
	if (nodeInBucket == nullptr) {
		HashNode* newNode = new HashNode(course, key);
		nodes.insert(nodes.begin() + key, (*newNode));
	}

	// if bucket has nodes
	else {

		// if it has an empty node, populate that node with course object data
		if (nodeInBucket->key == UINT_MAX) {
			nodeInBucket->key = key;
			nodeInBucket->theCourse = course;
			nodeInBucket->next = nullptr;

		}

		// if it has one or more populated nodes 
		else {
		
			// find the last node
			// while the node we're at, has a NEXT node, go to the next one
			while (nodeInBucket->next != nullptr) {
				nodeInBucket = nodeInBucket->next;
			}

			// create node object to be inserted
			HashNode* newNode = new HashNode(course, key);

			// insert node object as the next node to our last
			nodeInBucket->next = newNode;

		}
	}
}


// load - menu option #1
void loadCourses(string filePath, HashTable* listCourses) {

	// used for iterating the file contents by line
	ifstream infile(filePath);
	string line;

	// iterate over each line
	while (getline(infile, line)) {

		// create a stream with the contents of the line (for parsing)
		istringstream stringstream(line);

		// used for parsing and assmebling process
		string piece;
		vector<string> assembler;
		course individualCourse;

		// parse line by ',' and push each piece to assembler
		while (getline(stringstream, piece, ',')) {
			assembler.push_back(piece);
		}

		// use each piece in assembler to correctly populate our course object
		for (int i = 0; i < assembler.size(); i++) {

			// course number
			if (i == 0) {
				individualCourse.courseNumber = assembler[i];
			}

			// course name
			else if (i == 1) {
				individualCourse.courseName = assembler[i];
			}

			// prereqs
			else {
				individualCourse.preReqs.push_back(assembler[i]);
			}
		}

		// insert our course object into our hash table
		listCourses->Insert(individualCourse);
	}
}


// print - menu option #2
void HashTable::PrintAll() {

	// logic to see if there are any courses in existance
	bool empty = true;
	for (auto i : nodes) {
		if (i.key != UINT_MAX) {
			empty = false;
		}
	}

	// if no courses exist
	if (empty) {
		cout << "No courses exist." << endl;
	}

	// if some courses exist, start printing logic
	else {

		//printing header
		cout << "Here is a sample schedule:" << endl << endl;

		// used for sorting the results at the end of collection
		map<string, string> numName;
		vector<string> nums;

		// iterate nodes
		for (auto i : nodes) {

			// if node is not empty
			if (i.key != UINT_MAX) {

				// get all nodes at this key
				HashNode* nodeInBucket = &(nodes.at(i.key));

				// if only one node exists, push data to our sorting variables (nums & numname)
				if (nodeInBucket->next == nullptr) {
					numName.emplace(nodeInBucket->theCourse.courseNumber, nodeInBucket->theCourse.courseName);
					nums.push_back(nodeInBucket->theCourse.courseNumber);
				}

				// if more than one node exists
				else {

					// iterate through each node, push data to our sorting variables (nums & numname)
					while (nodeInBucket != nullptr) {
						numName.emplace(nodeInBucket->theCourse.courseNumber, nodeInBucket->theCourse.courseName);
						nums.push_back(nodeInBucket->theCourse.courseNumber);
						nodeInBucket = nodeInBucket->next;
					}

				}
			}
		}

		// sort 'nums' - a vector of all the course numbers
		sort(nums.begin(), nums.end());

		// for each item in sorted nums, print formatted data
		for (string i : nums) {
			cout << i << ", " << numName[i] << endl;
		}

	}
}


// find - menu option #3
void HashTable::findCourse(string courseNumber) {

	// logic to see if there are any courses in existance
	bool empty = true;
	for (auto i : nodes) {
		if (i.key != UINT_MAX) {
			empty = false;
		}
	}

	// if no courses exist
	if (empty) {
		cout << "No courses exist." << endl;
	}

	// if some courses exist, start finding logic
	else {

		// get respective hash, return whats in that hash (bucket)
		unsigned key = hash(courseNumber);
		HashNode* nodeInBucket = &(nodes.at(key));

		// if theres no bucket
		if (nodeInBucket == nullptr) {
			cout << "Course number '" << courseNumber << "' was not found. Please try again." << endl;
		}

		// if there is a bucket
		else {

			// bucket has: one, empty node
			if (nodeInBucket->key == UINT_MAX) {
				cout << "Course number '" << courseNumber << "' was not found. Please try again." << endl;
			}

			// bucket has at least one populated node already
			else {

				// if there's more than one node, lets iterate through until we find either:
				// the node we're looking for: print success messsage
				// the last one -> result: no node found
				while (nodeInBucket != nullptr) {

					// found the node we're looking for
					if (nodeInBucket->theCourse.courseNumber == courseNumber) {

						// print node number and name
						cout << nodeInBucket->theCourse.courseNumber << ", " << nodeInBucket->theCourse.courseName << endl;

						// print all the prereqs, if there are any
						if (nodeInBucket->theCourse.preReqs.size() != 0) {
							cout << "Prerequisites: ";
							int comma = 0;
							for (string e : nodeInBucket->theCourse.preReqs) {
								if (comma == 0) {
									cout << e;
								}
								else {
									cout << ", " << e;
								}
								comma++;
							}
							cout << endl;
						}

						break;
					}

					// this is the node we need, lets check out the next node
					else {

						// next node is empty - course doesn't exist 
						if (nodeInBucket->next == nullptr) {
							cout << "Course number '" << courseNumber << "' was not found. Please try again." << endl;
							break;
						}

						// there's a node there. let's move to it, and run this while statement again
						else {
							nodeInBucket = nodeInBucket->next;
						}

					}
				}
			}
		}
	}
}


// program main
int main() {

	// will point to our hashtable
	HashTable* listCourses;

	// comnstruct the hashtable
	listCourses = new HashTable();

	// variables for Menu
	clock_t timer;
	string command = "";
	string fileChoice = "";
	string courseChoice = "";

	// program start welcome
	cout << endl << "Welcome to the couse planner" << endl;

	// menu loop
	while (command != "4") {

		cout << endl << "1. Load Data Structure" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course" << endl;
		cout << "4. Exit" << endl << endl;
		cout << "What would you like to do? ";
		cin >> command;

		// load bids
		if (command == "1") {

			cout << "What is the name of the file you would like to load? (input.txt): ";
			cin >> fileChoice;

			// try to load the file
			try {
				ifstream ifs(fileChoice);
				if (ifs.is_open()) {
					timer = clock();
					loadCourses(fileChoice, listCourses);
					timer = clock() - timer;
					cout << "Finished. Total Time: " << timer << "ms." << endl;
				}
				else {
					throw (fileChoice);
				}
			}

			// if load fails, inform the user
			catch (string badFile) {
				cout << "Unfortunately, there was an issue processing the file named: " << badFile << "." << endl;
				cout << "Please try again." << endl;
			}

		}

		// print sample schedule
		else if (command == "2") {
			listCourses->PrintAll();
		}

		// find information by courseNumber
		else if (command == "3") {
			cout << "What course do you want to know about? ";
			cin >> courseChoice;
			listCourses->findCourse(courseChoice);
		}

		// quit program
		else if (command == "4") {
			cout << "Thank you for using the course planner!" << endl;
		}

		/// inproper menu command handler
		else {
			cout << command << " is not a valid option." << endl;
		}
	}
}