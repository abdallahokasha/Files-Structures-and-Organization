//
//  Book.hpp
//  BookStore
//
//  Created by Abdallah Okasha on 1/28/21.
//  Copyright © 2021 Abdallah Okasha. All rights reserved.
//

#ifndef Book_hpp
#define Book_hpp

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    string description;
    Book(string title, string author, string description);
    Book(int id, string title, string author, string description);
    ~Book();
};

#endif /* Book_hpp */
