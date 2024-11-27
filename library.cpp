#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include<algorithm>
using namespace std;
enum class Genre { fiction, nonfiction, periodical, biography, children };
class Book {
private:
    string ISBN;
    string title;
    string author;
    int copyright_date;
    bool checked_out;
    Genre genre;

public:
    Book(string isbn, string t, string a, int date, Genre g)
        : ISBN(isbn), title(t), author(a), copyright_date(date), checked_out(false), genre(g) {}
    string get_ISBN() const { return ISBN; }
    string get_title() const { return title; }
    string get_author() const { return author; }
    int get_copyright_date() const { return copyright_date; }
    Genre get_genre() const { return genre; }
    bool is_checked_out() const { return checked_out; }

    void check_out() {
        if (checked_out)
            throw runtime_error("Book is already checked out.");
        checked_out = true;
    }

    void check_in() {
        if (!checked_out)
            throw runtime_error("Book is already checked in.");
        checked_out = false;
    }

    bool operator==(const Book &other) const { return ISBN == other.ISBN; }
    bool operator!=(const Book &other) const { return !(*this == other); }

    friend ostream &operator<<(ostream &os, const Book &b) {
        os << "Title: " << b.title << "\nAuthor: " << b.author
           << "\nISBN: " << b.ISBN << "\nGenre: " << static_cast<int>(b.genre) << "\n";
        return os;
    }
};

class Patron {
private:
    string user_name;
    string card_number;
    int owed_fees;

public:
    Patron(string name, string card, int fees = 0)
        : user_name(name), card_number(card), owed_fees(fees) {}

    string get_user_name() const { return user_name; }
    string get_card_number() const { return card_number; }
    int get_owed_fees() const { return owed_fees; }

    void set_owed_fees(int fees) { owed_fees = fees; }

    bool owes_fees() const { return owed_fees > 0; }
};

struct Transaction {
    Book book;
    Patron patron;
    string activity; 
    string date;

    Transaction(const Book &b, const Patron &p, const string &act, const string &d)
        : book(b), patron(p), activity(act), date(d) {}
};

class Library {
private:
    vector<Book> books;
    vector<Patron> patrons;
    vector<Transaction> transactions;

public:
    void add_book(const Book &b) { books.push_back(b); }

    void add_patron(const Patron &p) { patrons.push_back(p); }

    void check_out_book(const string &isbn, const string &card_number, const string &date) {
        auto book_it = find_if(books.begin(), books.end(), [&isbn](const Book &b) { return b.get_ISBN() == isbn; });
        if (book_it == books.end())
            throw runtime_error("Book not found in the library.");

        auto patron_it = find_if(patrons.begin(), patrons.end(), [&card_number](const Patron &p) { return p.get_card_number() == card_number; });
        if (patron_it == patrons.end())
            throw runtime_error("Patron not found in the library.");

        if (patron_it->owes_fees())
            throw runtime_error("Patron owes fees to the library.");

        book_it->check_out();
        transactions.emplace_back(*book_it, *patron_it, "check out", date);
    }

    void check_in_book(const string &isbn, const string &card_number, const string &date) {
        auto book_it = find_if(books.begin(), books.end(), [&isbn](const Book &b) { return b.get_ISBN() == isbn; });
        if (book_it == books.end())
            throw runtime_error("Book not found in the library.");

        auto patron_it = find_if(patrons.begin(), patrons.end(), [&card_number](const Patron &p) { return p.get_card_number() == card_number; });
        if (patron_it == patrons.end())
            throw runtime_error("Patron not found in the library.");

        book_it->check_in();
        transactions.emplace_back(*book_it, *patron_it, "check in", date);
    }

    vector<string> patrons_with_fees() const {
        vector<string> result;
        for (const auto &p : patrons) {
            if (p.owes_fees()) {
                result.push_back(p.get_user_name());
            }
        }
        return result;
    }
};

int main() {
    try {
        Book book1("123-456-789", "C++ Primer", "Stanley Lippman", 2013, Genre::nonfiction);
        Book book2("987-654-321", "Harry Potter", "J.K. Rowling", 1997, Genre::fiction);
        cout<<book1;
        cout<<book2;
        Patron patron1("Kevia", "592", 0);
        Patron patron2("Gad", "471", 10);
        Library library;
        library.add_book(book1);
        library.add_book(book2);
        library.add_patron(patron1);
        library.add_patron(patron2);
        library.check_out_book("123-456-789", "001", "2024-11-27");
        auto fees_owing = library.patrons_with_fees();
        for (const auto &name : fees_owing) {
            cout << name << " owes fees.\n";
        }

    } catch (const std::exception &e) {
        cerr << e.what() << '\n';
    }

    return 0;
}
