//
//  Book.cpp
//  BookStore
//
//  Created by Abdallah Okasha on 1/28/21.
//  Copyright © 2021 Abdallah Okasha. All rights reserved.
//

#include "Book.hpp"

Book::Book(string title, string author, string description){
    this->title = title;
    this->author = author;
    this->description = description;
}
Book::Book(int id, string title, string author, string description){
    this->id = id;
    this->title = title;
    this->author = author;
    this->description = description;
}

Book::~Book(){}
