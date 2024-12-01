#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

enum class Genre { fiction, nonfiction, periodical, biography, children };

string get_genre_to_string(Genre g){
    switch(g){
        case Genre::fiction:
        return "Fiction";
        case Genre::nonfiction:
        return "nonfiction";
        case Genre::periodical:
        return "periodical";
        case Genre::biography:
        return "biography";
        case Genre::children:
        return "children";
        default:
        return "Genre not found";
    }
}
class Book {
private:
    string ISBN;
    string title;
    string author;
    int copyright_date;
    bool checked_out;
    Genre genre;

public:
    Book(string isbn, string t, string a, int date, Genre g) {
        ISBN = isbn;
        title = t;
        author = a;
        copyright_date = date;
        checked_out = false;
        genre = g;
    }

    string get_ISBN() const { return ISBN; }
    string get_title() const { return title; }
    string get_author() const { return author; }
    int get_copyright_date() const { return copyright_date; }
    Genre get_genre() const { return genre; }
    bool is_checked_out() const { return checked_out; }

    void check_out() { checked_out = true; }
    void check_in() { checked_out = false; }

    friend ostream &operator<<(ostream &os, const Book &b) {
        os << "\n\nTitle: " << b.title << "\nAuthor: " << b.author
           << "\nISBN: " << b.ISBN << "\nGenre: " << get_genre_to_string(b.genre) << "\n\n";
        return os;
    }
};

class Patron {
private:
    string user_name;
    string card_number;
    int owed_fees;

public:
    Patron(string name, string card, int fees = 0) {
        user_name = name;
        card_number = card;
        owed_fees = fees;
    }

    string get_user_name() const { return user_name; }
    string get_card_number() const { return card_number; }
    int get_owed_fees() const { return owed_fees; }

    void set_owed_fees(int fees) { owed_fees = fees; }

    bool owes_fees() const { return owed_fees > 0; }
};

class Library {
private:
    vector<Book> books;
    vector<Patron> patrons;

public:
    void add_book(const Book &b) { books.push_back(b); }
    void add_patron(const Patron &p) { patrons.push_back(p); }

    bool check_out_book(const string &isbn, const string &card_number) {
        auto book_it = find_if(books.begin(), books.end(), [&isbn](const Book &b) { return b.get_ISBN() == isbn; });
        if (book_it == books.end() || book_it->is_checked_out())
            return false;

        auto patron_it = find_if(patrons.begin(), patrons.end(), [&card_number](const Patron &p) { return p.get_card_number() == card_number; });
        if (patron_it == patrons.end() || patron_it->owes_fees())
            return false;

        book_it->check_out();
        return true;
    }

    bool check_in_book(const string &isbn) {
        auto book_it = find_if(books.begin(), books.end(), [&isbn](const Book &b) { return b.get_ISBN() == isbn; });
        if (book_it == books.end() || !book_it->is_checked_out())
            return false;

        book_it->check_in();
        return true;
    }
    void display(){
        for(auto w:books)
        cout<<w;
    }

    vector<string> patrons_with_fees() const {
        vector<string> result;
        for (const auto &p : patrons) {
            if (p.owes_fees())
                result.push_back(p.get_user_name());
        }
        return result;
    }
};

int main() {
    Library library;

    library.add_book(Book("123-456-789", "C++ Primer", "Stanley Lippman", 2013, Genre::nonfiction));
    library.add_book(Book("987-654-321", "Harry Potter", "J.K. Rowling", 1997, Genre::fiction));
    library.add_patron(Patron("Kevia", "223"));
    library.add_patron(Patron("Gad", "224", 10));

    string choice;
    while (true) {
        cout << "\nLibrary Menu:\n";
        cout <<"0. List all books\n";
        cout << "1. Add a Book\n";
        cout << "2. Check Out Book\n";
        cout << "3. Check In Book\n";
        cout << "4. List Patrons with Fees\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (stoi(choice)) {
            case 0:{
                library.display();
                break;
            }
            case 1: {
                string isbn, title, author;
                int year, genre_choice;
                cout << "Enter ISBN: ";
                cin >> isbn;
                cin.ignore(); // Clear input buffer
                cout << "Enter Title: ";
                getline(cin, title);
                cout << "Enter Author: ";
                getline(cin, author);
                cout << "Enter Copyright Year: ";
                cin >> year;
                cout << "Enter Genre (0: Fiction, 1: Nonfiction, 2: Periodical, 3: Biography, 4: Children): ";
                cin >> genre_choice;

                if (genre_choice < 0 || genre_choice > 4) {
                    cout << "Invalid genre choice.\n";
                } else {
                    library.add_book(Book(isbn, title, author, year, static_cast<Genre>(genre_choice)));
                    cout << "Book added successfully.\n";
                }
                break;
            }
            case 2: {
                string isbn, card_number;
                cout << "Enter ISBN: ";
                cin >> isbn;
                cout << "Enter Patron Card Number: ";
                cin >> card_number;

                if (library.check_out_book(isbn, card_number))
                    cout << "Book checked out successfully.\n";
                else
                    cout << "Failed to check out book. Ensure ISBN and Patron Card Number are valid and no fees are owed.\n";
                break;
            }
            case 3: {
                string isbn;
                cout << "Enter ISBN: ";
                cin >> isbn;

                if (library.check_in_book(isbn))
                    cout << "Book checked in successfully.\n";
                else
                    cout << "Failed to check in book. Ensure the book is checked out. \n";
                break;
            }
            case 4: {
                auto fees_owing = library.patrons_with_fees();
                if (fees_owing.empty())
                    cout << "No patrons owe fees.\n";
                else
                    for (const auto &name : fees_owing)
                        cout << name << " owes fees.\n";
                break;
            }
            case 5: {
                cout << "Exiting. Goodbye!\n";
                return 0;
            }
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
