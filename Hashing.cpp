/*
 * Hashing.cpp
 *
 *  Created on: May 6, 2016
 *      Author: okasha
 */
/*
 * book.cpp

 *
 *  Created on: Aug 7, 2015
 *      Author: abdallah
 */
/*
 * Abdallah Mohamed Okasha 20110230 G3
 * Eslam WallyEldeen Mohamed 20110490 G1
 */
#include <bits/stdc++.h>
using namespace std;
vector<int> allList;
string path;
int listHead;
int choice, method, numberOfrecords, searchSteps;
vector<vector<pair<string, int> > > table1(101, vector<pair<string, int> >(3));
vector<vector<pair<string, int> > > table2(101, vector<pair<string, int> >(3));
struct Book {
public:
	char ISBN[30]; // unique, like the ID for a student
	char Title[50];
	char Author[50];
	float Price;
	char publisher[50];
	short pages;
};

const int maxBufferSize = 2000;
/* Hashing Functions */
void insert_m1(char ISBN[], int offset);
void delete_m1(char ISBN[], int offset);
void search_m1(char ISBN[], int offset);

void insert_m2(char ISBN[], int offset);
void delete_m2(char ISBN[], int offset);
void search_m2(char ISBN[], int offset);

void insert_m3(char ISBN[], int offset);
void delete_m3(char ISBN[], int offset);
void search_m3(char ISBN[], int offset);

void constructTable1();
void constructTable2();

void printTable1();
void printTable2();

void writeTable1();
void writeTable2();

void readTable1();
void readTable2();

void updateAvailList(int offset) {
	vector<int> tmp;
	for (int i = 0; i < (int) allList.size(); i++) {
		if (allList[i] != offset)
			tmp.push_back(allList[i]);
	}
	allList.resize(0);
	allList = tmp;
	fstream f;
	f.open("book.txt", ios::binary | ios::in | ios::out);

	int x = -1;
	if ((int) allList.size() == 0) {
		f.seekp(0, ios::beg);
		f.write((char*) &x, sizeof(int));
	} else {
		f.seekp(0, ios::beg);
		f.write((char*) &allList[0], sizeof(int));
	}

	for (int i = 1; i < (int) allList.size(); i++) {

		f.seekp(allList[i - 1] + sizeof(int), ios::beg);
		f.put('*');
		f.write((char*) &allList[i], sizeof(int));
	}
	f.close();
}
void getDeletedList() {
	fstream f;
	f.open("book.txt", ios::in | ios::app | ios::binary | ios::out);
	int head;

	char ch[1];
	f.read((char*) &head, sizeof(head));
	allList.resize(0);
	allList.push_back(head);
	while (head != -1) {
		if (head == -1)
			break;
		f.seekg(head + sizeof(int), ios::beg);
		f.read(ch, 1);
		if (ch[0] != '*')
			break;
		else {
			f.read((char*) &head, sizeof(head));
			//cout << "->" << head << "\n";
		}
		allList.push_back(head);
	}
	f.close();
	cout << "Deleted records:\n";
	cout << (int) allList.size() << "\n";
	for (int i = 0; i < (int) allList.size(); i++)
		cout << allList[i] << " ";
	cout << "\n";
}
void addBook() {
	char ch;
	Book b;
	int flag = 0, head, length, offset;
	while (cin >> ch && ch == 'y') {
		flag = 0;
		cin.ignore();
		cout << "Enter book ISBN\n";
		cin.getline(b.ISBN, 30);
		cout << b.ISBN << "\n";

		cout << "Enter book Title\n";
		cin.getline(b.Title, 50);
		cout << b.Title << "\n";

		cout << "Enter book Author\n";
		cin.getline(b.Author, 50);
		cout << b.Author << "\n";

		cout << "Enter book Price\n";
		cin >> b.Price;
		cout << b.Price << "\n";

		cin.ignore();
		cout << "Enter book publisher\n";
		cin.getline(b.publisher, 50);
		cout << b.publisher << "\n";

		cout << "Enter book pages\n";
		cin >> b.pages;
		cout << b.pages << "\n";

		fstream f;
		f.open("book.txt", ios::out | ios::binary | ios::in);

		char buffer[maxBufferSize];
		strcpy(buffer, b.ISBN);
		strcat(buffer, "|");
		strcat(buffer, b.Title);
		strcat(buffer, "|");

		strcat(buffer, b.Author);
		strcat(buffer, "|");

		char tmp[50];
		sprintf(tmp, "%f", b.Price);
		strcat(buffer, tmp);
		strcat(buffer, "|");

		strcat(buffer, b.publisher);
		strcat(buffer, "|");

		sprintf(tmp, "%d", b.pages);
		strcat(buffer, tmp);

		int len = strlen(buffer);
		// searching for suitable size in deleted records
		f.seekg(0, ios::beg);
		f.read((char*) &head, sizeof(head));
		cout << head << "\n";
		f.seekg(f.tellg() + 1, ios::beg);
		while (!f.eof()) {
			if (f.eof())
				break;
			f.read((char*) &length, sizeof(len));
			cout << length << "\n";

			char *buf = new char[length];
			f.read(buf, length);
			int isDeleted = 0;
			for (int i = 0; i < len; i++) {
				if (buf[i] == '*') {
					isDeleted = 1;
					break;
				}
			}
			cout << length << " " << len << "\n";
			if (length > len && isDeleted) {
				flag = 1;
				f.seekp(f.tellp() - length - sizeof(length), ios::beg);
				offset = f.tellp();
				break;
			}
		}
		f.close();
		//if there is no suitable size
		if (flag) {
			getDeletedList();
			f.open("book.txt", ios::out | ios::binary | ios::in);
			cout << "add on deleted record!\n";
			cout << offset << "\n";
			f.seekp(offset + sizeof(int), ios::beg);
			//f.write((char*) &len, sizeof(len));
			f.write(buffer, len);
			f.close();
			//update AVAIl list if needed
			updateAvailList(offset);
		} else {
			f.open("book.txt", ios::out | ios::binary | ios::in | ios::app);
			f.write((char*) &len, sizeof(len));
			f.write(buffer, len);
			f.close();
			cout << "\n" << len << "\n" << buffer << "\n";
		}
	}
	if (method == 1)
		insert_m1(b.ISBN, offset), printTable1();
	else if (method == 2)
		insert_m2(b.ISBN, offset);
	else if (method == 3)
		insert_m3(b.ISBN, offset);
}
void writeListHead(int listHead) {
	fstream f;
	f.open("book.txt", ios::out | ios::binary | ios::app | ios::in);
	f.seekp(0, ios::beg);
	f.write((char*) &listHead, sizeof(listHead));
	f.put('|');
	f.close();
}
void update(string isbn) {
	fstream f;
	f.open("book.txt", ios::in | ios::binary | ios::out);

	Book b;
	int len, head, flag = 0;
	f.read((char*) &head, sizeof(head));
	f.seekg(f.tellg() + 1, ios::beg);
	cout << head << "\n";
	while (!f.eof()) {
		f.read((char*) &len, sizeof(len));
		cout << len << "\n";
		char *buf = new char[len];
		f.read(buf, len);

		istringstream strbuf(buf);
		strbuf.getline(b.ISBN, 30, '|');

		//find this record
		if (isbn == b.ISBN) {
			cout << "Record Founded: " << buf << "\n";
			for (int i = 0; i < (int) strlen(buf); i++)
				if (buf[i] == '*') {
					cout << "Record already deleted!\n";
					return;
				}
			flag = 1;
			break;
		}
	}
	if (!flag) {
		cout << "this book not founded!\n";
		return;
	} else {
		//write new book new
		cout << "Enter book ISBN\n";
		cin.getline(b.ISBN, 30);
		cout << b.ISBN << "\n";

		cout << "Enter book Title\n";
		cin.getline(b.Title, 50);
		cout << b.Title << "\n";

		cout << "Enter book Author\n";
		cin.getline(b.Author, 50);
		cout << b.Author << "\n";

		cout << "Enter book Price\n";
		cin >> b.Price;
		cout << b.Price << "\n";

		cin.ignore();
		cout << "Enter book publisher\n";
		cin.getline(b.publisher, 50);
		cout << b.publisher << "\n";

		cout << "Enter book pages\n";
		cin >> b.pages;
		cout << b.pages << "\n";

		char buffer[maxBufferSize];
		strcpy(buffer, b.ISBN);
		strcat(buffer, "|");
		strcat(buffer, b.Title);
		strcat(buffer, "|");

		strcat(buffer, b.Author);
		strcat(buffer, "|");

		char tmp[50];
		sprintf(tmp, "%f", b.Price);
		strcat(buffer, tmp);
		strcat(buffer, "|");

		strcat(buffer, b.publisher);
		strcat(buffer, "|");

		sprintf(tmp, "%d", b.pages);
		strcat(buffer, tmp);

		int newLen = strlen(buffer);
		f.seekp(f.tellp() - len, ios::beg);
		f.write(buffer, len);
	}
	f.close();
}

void searchByAuthor(string author) {
	Book b;
	fstream f;
	int flag = 0, len, head;
	if (author == "") {
		cout << "No ISBN Entered !! \n";
		return;
	}
	f.open("book.txt", ios::in | ios::binary | ios::out);
	f.seekg(0, ios::beg);
	f.read((char*) &head, sizeof(head));
	cout << head << "\n";
	f.seekg(f.tellg() + 1, ios::beg);
	while (!f.eof()) {
		f.read((char*) &len, sizeof(len));
		cout << len << "\n";

		char *buf = new char[len];
		f.read(buf, len);

		istringstream strbuf(buf);
		strbuf.getline(b.ISBN, 30, '|');
		strbuf.getline(b.Title, 50, '|');
		strbuf.getline(b.Author, 50, '|');

		if (author == b.Author) {
			cout << "Record Founded: " << buf << "\n";
			flag = 1;
		}

		char strPrice[30];
		strbuf.getline(strPrice, '|');
		b.Price = atof(strPrice);
		//cout << b.Price << "\n";

		strbuf.getline(b.publisher, 50, '|');

		char strPages[30];
		strbuf.getline(strPages, '|');
		b.Price = atoi(strPages);
	}
	if (!flag)
		cout << "this book not founded!\n";
	f.close();

}
void searchByISBN(string isbn) {
	Book b;
	fstream f;
	int flag = 0, len, head;
	if (isbn == "") {
		cout << "No ISBN Entered !! \n";
		return;
	}
	f.open("book.txt", ios::in | ios::binary | ios::out);
	f.seekg(0, ios::beg);
	f.read((char*) &head, sizeof(head));
	cout << head << "\n";
	f.seekg(f.tellg() + 1, ios::beg);
	while (!f.eof()) {
		f.read((char*) &len, sizeof(len));
		cout << len << "\n";

		char *buf = new char[len];
		f.read(buf, len);

		istringstream strbuf(buf);
		strbuf.getline(b.ISBN, 30, '|');
		strbuf.getline(b.Title, 50, '|');
		strbuf.getline(b.Author, 50, '|');

		if (isbn == b.ISBN) {
			cout << "Record Founded: " << buf << "\n";
			flag = 1;
			break;
		}

		char strPrice[30];
		strbuf.getline(strPrice, '|');
		b.Price = atof(strPrice);
		//cout << b.Price << "\n";

		strbuf.getline(b.publisher, 50, '|');

		char strPages[30];
		strbuf.getline(strPages, '|');
		b.Price = atoi(strPages);
	}
	if (!flag)
		cout << "this book not founded!\n";
	f.close();
}

void deleteBook(string isbn) {
	fstream f;
	f.open("book.txt", ios::in | ios::binary | ios::out);

	Book b;
	int offset;
	int len, head, flag = 0;
	f.read((char*) &head, sizeof(head));
	f.seekg(f.tellg() + 1, ios::beg);
	cout << head << "\n";
	while (!f.eof()) {
		f.read((char*) &len, sizeof(len));
		cout << len << "\n";
		char *buf = new char[len];
		f.read(buf, len);

		istringstream strbuf(buf);
		strbuf.getline(b.ISBN, 30, '|');

		//find this record
		if (isbn == b.ISBN) {
			cout << "Record Founded: " << buf << "\n";
			for (int i = 0; i < (int) strlen(buf); i++)
				if (buf[i] == '*') {
					cout << "Record already deleted!\n";
					return;
				}
			flag = 1;
			break;
		}
	}
	if (!flag) {
		cout << "this book not founded!\n";
		return;
	} else {
		f.seekp(f.tellp() - len - sizeof(len), ios::beg);
		offset = f.tellp();
		cout << offset << endl;
		f.seekp(f.tellp() + sizeof(len), ios::beg);
		f.put('*');
		f.write((char*) &head, sizeof(head));
		f.seekp(0, ios::beg);
		f.write((char*) &offset, sizeof(offset));
	}
	f.close();
	if (method == 1)
		delete_m1(b.ISBN, offset);
	else if (method == 2)
		delete_m2(b.ISBN, offset);
	else if (method == 3)
		delete_m3(b.ISBN, offset);
}
void compactFile() {
	int len, head;
	fstream f;
	f.open("book.txt", ios::binary | ios::in | ios::out);

	f.read((char*) &head, sizeof(head));
	f.seekg(f.tellg() + 1, ios::beg);
	int i = 0;
	char buffer[10000];
	while (!f.eof()) {
		f.read((char*) &len, sizeof(len));
		char *buf = new char[len];
		f.read(buf, len);
		int flag = 1;
		for (int j = 0; j < (int) strlen(buf); j++) {
			if (buf[i] == '*')
				flag = 0;
			break;
		}

		if (flag) {
			if (!i) {
				char tmp[50];
				sprintf(tmp, "%d", len);
				strcpy(buffer, tmp);
				strcat(buffer, buf);
			} else {
				char tmp[50];
				sprintf(tmp, "%d", len);
				strcat(buffer, tmp);
				strcat(buffer, buf);
			}
			i++;
		}
	}
	f.close();

	ofstream of;
	of.open("book.txt", ios::trunc);
	of.close();

	f.open("book.txt", ios::binary | ios::app | ios::out);
	len = strlen(buffer);
	f.write(buffer, len);
	cout << "records not deleted\n";
	cout << buffer << "\n" << len << "\n";
	f.close();
}

int main() {
	choice = 0;
	while (choice != 9) {
		cout << "Enter your Choice: \n";
		cout << "1- add new book\n";
		cout << "2- update\n";
		cout << "3- delete book\n";
		cout << "4- searchByISBN\n";
		cout << "5- searchByAuthor\n";
		cout << "6- compactFile\n";
		cout << "7- write list head\n";
		cout << "8- getDeletedList\n";
		cout << "9- Exit\n";

		cout << "\n >> Enter Choice:\n";
		cin >> choice;
		cout << "\n >> Enter ur Method:\n";
		cin >> method;
		if (method == 1)
			constructTable1();
		if (method == 2)
			constructTable2();
		if (choice == 1) {
			cout << "1- add new book\n";
			addBook();
		} else if (choice == 2) {
			string isbn;
			cout << "2- update , Enter book ISBN \n";
			cin.ignore();
			getline(cin, isbn);
			update(isbn);
		} else if (choice == 3) {
			string isbn;
			cout << "3- Delete , Enter book ISBN \n";
			cin.ignore();
			getline(cin, isbn);
			deleteBook(isbn);
		} else if (choice == 4) {
			string isbn;
			cout << "4- searchByISBN , Enter book ISBN \n";
			cin.ignore();
			getline(cin, isbn);
			searchByISBN(isbn);
		} else if (choice == 5) {
			string author;
			cout << "5- searchByAuthor , Enter book Author \n";
			cin.ignore();
			getline(cin, author);
			searchByAuthor(author);
		} else if (choice == 6) {
			cout << "compact file!\n";
			compactFile();
		} else if (choice == 7) {
			cout << "Write List Head\n";
			writeListHead(-1);
		} else if (choice == 8) {
			cout << "getDeletedList\n";
			getDeletedList();
		} else
			break;
	}
	return 0;
}
void insert_m1(char ISBN[], int offset) {
	int flag = 0;
	string isbn(ISBN);
	int hashValue = (int(ISBN[0]) * int(ISBN[1])) % 101;
	for (int i = hashValue; i < 101; ++i) {
		vector<pair<string, int> > tmp = table1[i];
		for (int j = 0; j < 3; ++j) {
			if (tmp[j].first != "######") {
				tmp[j].first = isbn;
				tmp[j].second = offset;
				flag = 1;
			}
			if (flag)
				break;
		}
		table1[i] = tmp;
		if (flag)
			break;
	}
	if (!flag) {
		for (int i = 0; i < 101; ++i) {
			vector<pair<string, int> > tmp = table1[i];
			for (int j = 0; j < 3; ++j) {
				if (tmp[j].first != "######") {
					tmp[j].first = isbn;
					tmp[j].second = offset;
					flag = 1;
				}
				if (flag)
					break;
			}
			table1[i] = tmp;
			if (flag)
				break;
		}
	}
	if (!flag)
		cout << "\n Can't insert in hash index\n";
}
void delete_m1(char ISBN[], int offset) {
	int flag = 0;
	string isbn(ISBN);
	int hashValue = (int(ISBN[0]) * int(ISBN[1])) % 101;
	for (int i = hashValue; i < 101; ++i) {
		vector<pair<string, int> > tmp = table1[i];
		for (int j = 0; j < 3; ++j) {
			if (tmp[j].first == isbn) {
				tmp[j].first = "######";
				flag = 1;
			}
			if (flag)
				break;
		}
		table1[i] = tmp;
		if (flag)
			break;
	}
	if (!flag) {
		for (int i = 0; i < 101; ++i) {
			vector<pair<string, int> > tmp = table1[i];
			for (int j = 0; j < 3; ++j) {
				if (tmp[j].first == isbn) {
					tmp[j].first = "######";
					flag = 1;
				}
				if (flag)
					break;
			}
			table1[i] = tmp;
			if (flag)
				break;
		}
	}
	if (!flag)
		cout << "\n Can't find in hash index\n";
}

void search_m1(char ISBN[], int offset) {

	int flag = 0, found = -1;
	string isbn(ISBN);
	int hashValue = (int(ISBN[0]) * int(ISBN[1])) % 101;
	for (int i = hashValue; i < 101; ++i) {
		vector<pair<string, int> > tmp = table1[i];
		for (int j = 0; j < 3; ++j) {
			if (tmp[j].first == isbn) {
				cout << "Found @ " << j << "\n";
				found = j;
				flag = 1;
				++numberOfrecords;
				searchSteps += i - hashValue;
			}
			if (flag)
				break;
		}
		if (flag)
			break;
	}
	if (!flag) {
		for (int i = 0; i < 101; ++i) {
			vector<pair<string, int> > tmp = table1[i];
			for (int j = 0; j < 3; ++j) {
				if (tmp[j].first == isbn) {
					cout << "Found @ " << j << "\n";
					found = j;
					flag = 1;
					++numberOfrecords;
					searchSteps += i - hashValue;
				}
				if (flag)
					break;
			}
			if (flag)
				break;
		}
	}

	if (!flag)
		cout << "\n Can't find in hash index\n";
	cout << "Method#1: Avg search so far: " << searchSteps / numberOfrecords
			<< "\n";
}
//======
void insert_m2(char ISBN[], int offset) {

}
void delete_m2(char ISBN[], int offset) {

}
void search_m2(char ISBN[], int offset) {

}
//=======
void insert_m3(char ISBN[], int offset) {

}
void delete_m3(char ISBN[], int offset) {

}
void search_m3(char ISBN[], int offset) {

}

void constructTable1() {
	Book b;
	fstream f;
	int len, head, offset;

	f.open("book.txt", ios::in | ios::binary | ios::out);
	f.seekg(0, ios::beg);
	f.read((char*) &head, sizeof(head));
	cout << head << "\n";
	f.seekg(f.tellg() + 1, ios::beg);
	while (!f.eof()) {
		offset = f.tellg();
		f.read((char*) &len, sizeof(len));
		cout << len << "\n";

		char *buf = new char[len];
		f.read(buf, len);

		istringstream strbuf(buf);
		strbuf.getline(b.ISBN, 30, '|');
		strbuf.getline(b.Title, 50, '|');
		strbuf.getline(b.Author, 50, '|');
		insert_m1(b.ISBN, offset);
	}
	f.close();
}
void printTable1() {
	for (int i = 0; i < 101; ++i) {
		vector<pair<string, int> > tmp = table1[i];
		for (int j = 0; j < 3; ++j)
			cout << "(" << tmp[j].first << ", " << tmp[j].second << ")     ";
		cout << "\n";
	}
}
void writeTable1() {
	fstream f;
	f.open("table1.txt", ios::out | ios::binary | ios::in | ios::app);
	for (int i = 0; i < 101; ++i) {
		vector<pair<string, int> > tmp = table1[i];
		for (int j = 0; j < 3; ++j) {
			char buffer[maxBufferSize];
			string pk = tmp[j].first;
			strcpy(buffer, pk.c_str());
			strcat(buffer, "|");

			char off[50];
			sprintf(off, "%d", tmp[j].second);
			strcat(buffer, off);
			int len = strlen(buffer);
			f.write((char*) &len, sizeof(len));
			f.write(buffer, len);
		}
	}
	f.close();
}
void readTable1() {
	int len;
	fstream f;
	f.open("table1.txt", ios::in | ios::binary | ios::out);
	while (!f.eof()) {
		f.read((char*) &len, sizeof(len));
		cout << len << "\n";
		char *buf = new char[len];
		f.read(buf, len);

		istringstream strbuf(buf);
		char pk[30];
		char off[50];
		strbuf.getline(pk, 30, '|');
		strbuf.getline(off, 50, '|');
		int offset = atoi(off);
		insert_m1(pk, offset);
	}
	f.close();
}

void constructTable2() {

}

void printTable2() {

}

