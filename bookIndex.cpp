/*
 * bookIndex.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: abdallah
 */
/*
 * Abdallah Mohamed Okasha 20110230
 * Eslam Wally-Elden Mohamed 20110490
 * Alaa Afifi kamal 20110238
 */
#include <bits/stdc++.h>
using namespace std;
vector<int> allList;
string path;
int listHead;
struct Book {
public:
	char ISBN[30]; // unique
	char Title[50];
	char Author[50];
	float Price;
	char publisher[50];
	short pages;
};
map<string, vector<string> > invertList;
const int maxBufferSize = 2000;

struct primaryIndex {
public:
	char ISBN[30];
	int offset;
};
struct secondaryIndex {
public:
	char Author[50];
	char ISBN[30];
};

vector<primaryIndex> pIndex;
vector<secondaryIndex> sIndex;
void sortPrimaryIndex() {

	int flag;
	while (1) {
		flag = 1;
		for (int i = 0; i < (int) pIndex.size() - 2; i++) {
			//if (pIndex[i + 1].ISBN < pIndex[i].ISBN)
			if (strcmp(pIndex[i + 1].ISBN, pIndex[i].ISBN) < 0)
				swap(pIndex[i + 1], pIndex[i]), flag = 0;
			//cout << "Swap \n";

		}
		if (flag)
			break;
	}
}
void sortSecondaryIndex() {
	int flag;
	while (1) {
		flag = 1;
		for (int i = 0; i < (int) sIndex.size() - 2; i++) {
			//if (sIndex[i + 1].Author < sIndex[i].Author)
			if (strcmp(sIndex[i + 1].Author, sIndex[i].Author) < 0)
				swap(sIndex[i + 1], sIndex[i]), flag = 0;
			//cout << "Swap \n";
		}
		if (flag)
			break;
	}
}
void showPrimaryIndex() {
	for (int i = 0; i < (int) pIndex.size() - 1; i++)
		cout << pIndex[i].ISBN << " " << pIndex[i].offset << "\n";
	cout << "\n";
}
void showSecondaryIndex() {
	for (int i = 0; i < (int) sIndex.size() - 1; i++)
		cout << sIndex[i].Author << " " << sIndex[i].ISBN << "\n";
	cout << "\n";
}
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
void readPrimaryIndex() {
	int len, head;
	Book b;
	fstream f;
	f.open("book.txt", ios::in | ios::binary);
	pIndex.resize(0);
	f.read((char*) &head, sizeof(head));
	f.seekg(f.tellg() + 1, ios::beg);
	while (!f.eof()) {

		if (f.eof())
			break;
		f.read((char*) &len, sizeof(len));
		char *buf = new char[len];
		f.read(buf, len);

		int flag = 1;
		for (int i = 0; i < (int) strlen(buf); i++) {
			if (buf[i] == '*') {
				flag = 0;
				break;
			}
		}
		if (!flag)
			continue;

		primaryIndex p;
		istringstream strbuf(buf);
		strbuf.getline(p.ISBN, 30, '|');

		p.offset = f.tellg() - len - sizeof(len);

		pIndex.push_back(p);
	}
	f.close();
	sortPrimaryIndex();
	for (int i = 0; i < (int) pIndex.size() - 1; i++)
		cout << pIndex[i].ISBN << " " << pIndex[i].offset << "\n";
	cout << "\n";
}
void readSecondaryIndex() {
	int len, head;
	Book b;
	fstream f;
	f.open("book.txt", ios::in | ios::binary);
	sIndex.resize(0);
	f.read((char*) &head, sizeof(head));
	f.seekg(f.tellg() + 1, ios::beg);
	while (!f.eof()) {
		f.read((char*) &len, sizeof(len));
		char *buf = new char[len];
		f.read(buf, len);
		int flag = 1;
		for (int i = 0; i < (int) strlen(buf); i++) {
			if (buf[i] == '*') {
				flag = 0;
				break;
			}
		}
		if (!flag)
			continue;

		secondaryIndex s;
		istringstream strbuf(buf);
		strbuf.getline(s.ISBN, 30, '|');
		strbuf.getline(b.Title, 50, '|');
		strbuf.getline(s.Author, 50, '|');

		sIndex.push_back(s);
		if (f.eof())
			break;
	}
	f.close();
	sortSecondaryIndex();
	for (int i = 0; i < (int) sIndex.size() - 1; i++)
		cout << sIndex[i].Author << " " << sIndex[i].ISBN << "\n";
	cout << "\n";
}
void invertedList() {
	invertList.clear();

	for (int i = 0; i < (int) sIndex.size() - 1; i++) {
		vector<string> list;
		cout << sIndex[i].Author << "\n";
		for (int j = 0; j < (int) sIndex.size() - 1; j++) {
			if (strcmp(sIndex[j].Author, sIndex[i].Author) == 0) {
				string tmp(sIndex[j].ISBN);
				list.push_back(tmp);
			}
		}
		string tmp(sIndex[i].Author);
		invertList[tmp] = list;
	}
	for (int i = 0; i < (int) sIndex.size() - 1; i++) {
		vector<string> list;
		string tmp(sIndex[i].Author);
		list = invertList[tmp];

		cout << "\n" << tmp << ":\n";
		for (int j = 0; j < (int) list.size(); j++)
			cout << list[j] << "\n";
		cout << "--------------\n";
	}
}
void addBook() {
	char ch;
	int flag = 0, head, length, offset;
	while (cin >> ch && ch == 'y') {
		flag = 0;
		Book b;
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
		//if there is suitable size
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
	readPrimaryIndex();
	readSecondaryIndex();
	invertedList();
}
void writeListHead(int listHead) {
	fstream f;
	f.open("book.txt", ios::out | ios::binary | ios::app | ios::in);
	f.seekp(0, ios::beg);
	f.write((char*) &listHead, sizeof(listHead));
	f.put('|');
	f.close();
	cout << "\n";
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
	readPrimaryIndex();
	readSecondaryIndex();
	invertedList();
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
		int offset = f.tellp();
		cout << offset << endl;
		f.seekp(f.tellp() + sizeof(len), ios::beg);
		f.put('*');
		f.write((char*) &head, sizeof(head));
		f.seekp(0, ios::beg);
		f.write((char*) &offset, sizeof(offset));
	}
	f.close();
	readPrimaryIndex();
	readSecondaryIndex();
	invertedList();
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
				strcat(buffer, tmp);
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
void writePrimaryIndex() {

	ofstream of;
	of.open("sindex.txt", ios::trunc);
	of.close();

	ofstream f;
	f.open("pindex.txt", ios::out | ios::binary);
	for (int i = 0; i < (int) pIndex.size() - 1; i++) {

		char buffer[maxBufferSize];
		strcpy(buffer, pIndex[i].ISBN);
		strcat(buffer, "|");
		char tmp[30];
		sprintf(tmp, "%d", pIndex[i].offset);
		strcat(buffer, tmp);

		int len = strlen(buffer);
		f.write((char*) &len, sizeof(len));
		f.write(buffer, len);
		cout << len << "\n" << buffer << "\n\n";
	}
	f.close();
}
void writeSecondaryIndex() {

	ofstream of;
	of.open("sindex.txt", ios::trunc);
	of.close();

	fstream f;
	f.open("sindex.txt", ios::out | ios::binary | ios::app);

	for (int i = 0; i < (int) sIndex.size() - 1; i++) {

			char buffer[maxBufferSize];
			strcpy(buffer, sIndex[i].ISBN);
			strcat(buffer, "|");
			strcat(buffer, sIndex[i].Author);

			int len = strlen(buffer);
			f.write((char*) &len, sizeof(len));
			f.write(buffer, len);
			cout << len << "\n" << buffer << "\n\n";
		}
	f.close();
}
void searchISBNByIndex(string isbn) {
	readPrimaryIndex();
	readSecondaryIndex();
	invertedList();

	int first = 0, last = (int) pIndex.size() - 2;
	int mid = (first + last) / 2;

	int offset = -1;
	while (first <= last) {
		string tmp(pIndex[mid].ISBN);
		if (tmp < isbn)
			first = mid + 1;
		else if (pIndex[mid].ISBN == isbn) {
			cout << "Book found\n";
			offset = pIndex[mid].offset;
			break;
		} else
			last = mid - 1;

		mid = (first + last) / 2;
	}
	if (first > last) {
		cout << "Book not found\n";
		return;
	}
	int len;
	fstream f;
	f.open("book.txt", ios::in | ios::binary);
	f.seekg(offset, ios::beg);
	f.read((char*) &len, sizeof(len));
	char *buf = new char[len];
	f.read(buf, len);
	f.close();
	cout << len << " " << buf << "\n\n";
}
void searchAuthorByIndex(string author) {
	readPrimaryIndex();
	readSecondaryIndex();
	invertedList();
	int len;
	vector<string> list;
	list = invertList[author];
	for (int i = 0; i < (int) list.size(); i++) {
		for (int j = 0; j < (int) pIndex.size() - 1; j++) {
			string tmp(pIndex[j].ISBN);
			if (list[i] == tmp) {
				fstream f;
				f.open("book.txt", ios::in | ios::binary);
				f.seekg(pIndex[j].offset, ios::beg);
				f.read((char*) &len, sizeof(len));
				char *buf = new char[len];
				f.read(buf, len);
				f.close();
				cout << len << " " << buf << "\n\n";
			}
		}
	}
}
void clearFile() {
	ofstream of;
	of.open("book.txt", ios::trunc);
	of.close();
	cout << "\n";
}
int main() {
	int choice;
	choice = 0;
	while (1) {
		cout << "Enter your Choice: \n";
		cout << "1- add new book\n";
		cout << "2- update\n";
		cout << "3- delete book\n";
		cout << "4- searchByISBN\n";
		cout << "5- searchByAuthor\n";
		cout << "6- compactFile\n";
		cout << "7- write list head\n";
		cout << "8- getDeletedList\n";
		cout << "9- readPrimaryIndex\n";
		cout << "10- readSecondaryIndex\n";
		cout << "11- writePrimaryIndex\n";
		cout << "12- writeSecondaryIndex\n";
		cout << "13- showPrimaryIndex\n";
		cout << "14- showSecondaryIndex\n";
		cout << "15- invertedList\n";
		cout << "16- Search ISBN by index\n";
		cout << "17- Search Author by index\n";
		cout << "18- Clear file\n";
		cout << "19- Exit\n";

		cout << "\n>> Enter Choice:\n";
		cin >> choice;
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
			cout << "3- Delete, Enter book ISBN \n";
			cin.ignore();
			getline(cin, isbn);
			deleteBook(isbn);
		} else if (choice == 4) {
			string isbn;
			cout << "4- searchByISBN, Enter book ISBN \n";
			cin.ignore();
			getline(cin, isbn);
			searchByISBN(isbn);
		} else if (choice == 5) {
			string author;
			cout << "5- searchByAuthor, Enter book Author \n";
			cin.ignore();
			getline(cin, author);
			searchByAuthor(author);
		} else if (choice == 6) {
			cout << "6- compact file!\n";
			compactFile();
		} else if (choice == 7) {
			cout << "7- Write List Head\n";
			writeListHead(-1);
		} else if (choice == 8) {
			cout << "8- getDeletedList\n";
			getDeletedList();
		} else if (choice == 9) {
			cout << "9- Read Primary index\n";
			readPrimaryIndex();
		} else if (choice == 10) {
			cout << "10- Read Secondary index\n";
			readSecondaryIndex();
		} else if (choice == 11) {
			cout << "11- Write Primary Index\n";
			writePrimaryIndex();
		} else if (choice == 12) {
			cout << "12- Write Secondary Index\n";
		} else if (choice == 13) {
			cout << "13- show Primary Index\n";
			showPrimaryIndex();
		} else if (choice == 14) {
			cout << "14- show Secondary Index\n";
			showSecondaryIndex();
		} else if (choice == 15) {
			cout << "15- inverted List\n";
			invertedList();
		} else if (choice == 16) {
			string isbn;
			cout << "16- search by ISBN index, Enter ISBN\n";
			cin.ignore();
			getline(cin, isbn);
			searchISBNByIndex(isbn);
		} else if (choice == 17) {
			string author;
			cout << "17- search by Author index, Enter Author\n";
			cin.ignore();
			getline(cin, author);
			searchAuthorByIndex(author);
		} else if (choice == 18) {
			cout << "18- Clear File\n";
			clearFile();
		} else
			break;
	}
	return 0;
}
