# book-library-cpp
This app is all about implemetation of library system in school. It is written in cpp and it working very well!

Overall working of this game:

>A class Book contains: 
ISBN : string
Title: string
Author: string
Copyright date: Integer
Checked_out: bool
Genre:{fiction, nonfiction ,periodical, biography, children}
The appropriate constructors
Appropriate getter functions
A function to check out a book
A function to check in a book
Overload the == operator ( two books will be deemed equal if they have the same ISBN)
Overload the != operator
Overload the << operator to display title, author , ISBN and genre on separate lines.

>A Class Patron  that contains:
User name: string
Card number :string
Owed fees: integer
Getter functions that return these data members
Function that returns true if the user owes fees to the library
Setter function for the fees

>Create a Transaction  class that contains:
A Book
A Patron
Activity ( check out or check in a book)
A Date

>A class Library that contains:
A vector of Books
A vector of Patrons 
A vector of Transactions
A function to add a book to the library
A function to check out  a Book : Whenever a user checks out a book, have the library make sure that both the user and the book are in the library. If they aren’t, report an error. Then check to make sure that the user owes no fees. If the user does, report an error. If not, create a Transaction, and place it in the vector of Transactions. Also write a function that will return a vector that contains the names of all Patrons who owe fees.
