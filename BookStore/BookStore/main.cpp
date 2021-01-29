//
//  main.cpp
//  BookStore
//
//  Created by Abdallah Okasha on 1/28/21.
//  Copyright © 2021 Abdallah Okasha. All rights reserved.
//

#include <iostream>
#include "Library.hpp"

using namespace std;

int main() {
    Library lib;
    lib.Books = lib.RetrieveBooks();
    // Adminstration/commands Menu
    while (true){
        int choice;
        cout << "==== Book Manager ====\n";
        cout << "\t1) View all books\n";
        cout << "\t2) Add a book\n";
        cout << "\t3) Edit a book\n";
        cout << "\t4) Search for a book\n";
        cout << "\t5) Save and exit\n";
        cout << "Choose[1-5]: ";
        cin >> choice;
        if (choice == 1){
            // View All Books
            lib.ListBooks(lib.Books);
        }
        else if (choice == 2){
            // Add Book
            string title, author, description;
            cout << "Please enter the following info: \n";
            cout << "Title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Author: ";
            getline(cin, author);
            cout << "Description: ";
            getline(cin, description);
            bool bookIsAdded = false;
            Book bk((int)lib.Books.size()+1, title, author, description);
            bookIsAdded = lib.AddBook(bk, lib.Books);
            if (bookIsAdded)
                cout << "Book [" << lib.Books.size() << "] is saved\n";
        }
        else if (choice == 3){
            // Edit Book
            int bookId;
            cout << "Please enter book id to edit: ";
            cin >> bookId;
            
            string title, author, description;
            cout << "Please enter the new info: \n";
            cin.ignore();
            cout << "Title: ";
            getline(cin, title);
            cout << "Author: ";
            getline(cin, author);
            cout << "Description: ";
            getline(cin, description);
            Book editedBook(bookId, title, author, description);
            
            bool bookIsUpdated = false;
            bookIsUpdated = lib.EditBook(bookId, editedBook, lib.Books);
            
            if (bookIsUpdated)
                cout << "Book [" << bookId << "] is updated\n";
        }
        else if (choice == 4){
            // Search book
            int bookId;
            cout << "Book search, Please enter book id to: ";
            cin >> bookId;
            cout << "\n";
            lib.SearchForBook(bookId, lib.Books);
            cout << "\n";
        }
        else {
            // Save & Exit
            lib.SaveBooks(lib.Books);
            break;
        }
    }
    return 0;
}
