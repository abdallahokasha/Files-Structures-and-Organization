//
//  Library.cpp
//  BookStore
//
//  Created by Abdallah Okasha on 1/28/21.
//  Copyright © 2021 Abdallah Okasha. All rights reserved.
//

#include "Library.hpp"
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

bool Library::AddBook(Book book, vector<Book> &booksList){
    // check if book id is already exist
    for (int i = 0; i < (int)booksList.size(); ++i){
        if (book.id == booksList[i].id){
            cout << "Error Invalid book id\n";
            return false;
        }
    }
    booksList.push_back(book);
    return true;
}

bool Library::EditBook(int bookId, Book book, vector<Book> &booksList){
    for (int i=0; i < (int)booksList.size(); ++i){
        if (bookId == booksList[i].id){
            booksList[i].title = book.title;
            booksList[i].author = book.author;
            booksList[i].description = book.description;
            return true;
        }
    }
    return false;
}

void Library::ListBooks(vector<Book> &booksList) const {
    cout << "\n*** Books List ***\n";
    for (int i = 0; i < (int)booksList.size(); ++i){
        cout << "Id: " << booksList[i].id << "\n";
        cout << "Title: "<< booksList[i].title << "\n";
        cout << "Author: "<< booksList[i].author << "\n";
        cout << "Description: " << booksList[i].description << "\n";
        if (i < booksList.size()-1) cout << "    -------    \n";
    }
    cout << "\n";
}

void Library::SearchForBook(int bookId, vector<Book> booksList) const{
    for (int i=0; i < (int)booksList.size(); ++i){
        if (bookId == booksList[i].id){
            cout << "Book details: \n";
            cout << "Id: " << booksList[i].id << "\n";
            cout << "Title: " << booksList[i].title << "\n";
            cout << "Author: " << booksList[i].author << "\n";
            cout << "Description: " << booksList[i].description << "\n";
        }
    }
}

bool Library::DeleteBook (int bookId, vector<Book> &booksList){
    for (auto it = booksList.begin(); it != booksList.end(); ++it){
        if (it->id == bookId){
            booksList.erase(it);
            return true;
        }
    }
    cout << "Error! can't find book with this id\n";
    return false;
}

// helper function to split strings
vector<string> SplitString (string str, char delimiter){
    // str = regex_replace(str, regex("(^[ ]+)|([ ]+$)"),""); // trim spaces
    string temp = "";
    vector<string> tokens;
    for (auto i = 0; i < str.size(); ++i){
        if (str[i] == delimiter){
            tokens.push_back(temp);
            temp = "";
        }
        else temp += str[i];
    }
    return tokens;
}

bool Library::SaveBooks(vector<Book> &booksList) const{
    ofstream myfile;
    myfile.open ("books.txt", ios::trunc);
    for (auto i = 0; i < (int)booksList.size(); ++i){
        // cout << "description: " << booksList[i].description << "\n";
        myfile << booksList[i].id << "|" <<  booksList[i].title << "|" <<  booksList[i].author << "|" <<  booksList[i].description << "|#";
    }
    myfile.close();
    cout << "Saving is Done!\n";
    return true;
}

vector<Book> Library::RetrieveBooks() const{
    string line;
    ifstream myfile;
    myfile.open("books.txt");
    vector<Book> booksList;
    if (myfile.is_open())
    {
        string allBooks = "";
        while (getline (myfile, line))
        {
            // cout << line << '\n';
            allBooks += line;
        }
        myfile.close();
        vector<string> bookStringsList = SplitString(allBooks, '#');
        for (int i = 0; i < (int)bookStringsList.size(); ++i){
            // cout << "book record: " << bookStringsList[i] << "\n";
            vector <string> bookFields = SplitString(bookStringsList[i], '|');
            int bookId = stoi(bookFields[0]);
            Book newBook(bookId, bookFields[1], bookFields[2], bookFields[3]);
            booksList.push_back(newBook);
        }
    }
    else cout << "Unable to open file\n";
    return booksList;
}

Library::Library()
{
}

Library::~Library()
{
    
}
