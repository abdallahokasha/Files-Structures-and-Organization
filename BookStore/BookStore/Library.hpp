//
//  Library.hpp
//  BookStore
//
//  Created by Abdallah Okasha on 1/28/21.
//  Copyright © 2021 Abdallah Okasha. All rights reserved.
//

#ifndef Library_hpp
#define Library_hpp

#include "Book.hpp"
#include <stdio.h>

class Library {
public:
    vector <Book> Books;
    bool AddBook (Book book, vector<Book> &booksList);
    bool EditBook (int bookId, Book book, vector<Book> &booksList);
    void ListBooks (vector<Book> &booksList) const;
    bool DeleteBook (int bookId, vector<Book> &booksList);
    bool SaveBooks(vector<Book> &booksList) const;
    vector<Book> RetrieveBooks()const;
    void SearchForBook(int bookId, vector<Book> booksList) const;
    Library();
    ~Library();
};

#endif /* Library_hpp */
