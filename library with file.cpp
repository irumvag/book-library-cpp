#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h> // For debugging the current working directory

using namespace std;

enum class Genre { fiction, nonfiction, periodical, biography, children };

string get_genre_to_string(Genre g) {
    switch (g) {
        case Genre::fiction:
            return "Fiction";
        case Genre::nonfiction:
            return "Nonfiction";
        case Genre::periodical:
            return "Periodical";
        case Genre::biography:
            return "Biography";
        case Genre::children:
            return "Children";
        default:
            return "Unknown Genre";
    }
}

Genre string_to_genre(const string &genre_str) {
    if (genre_str == "Fiction") return Genre::fiction;
    if (genre_str == "Nonfiction") return Genre::nonfiction;
    if (genre_str == "Periodical") return Genre::periodical;
    if (genre_str == "Biography") return Genre::biography;
    if (genre_str == "Children") return Genre::children;
    throw invalid_argument("Invalid genre string");
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
    Book(string isbn, string t, string a, int date, Genre g) : ISBN(isbn), title(t), author(a), copyright_date(date), genre(g), checked_out(false) {}
    Book() : copyright_date(0), checked_out(false), genre(Genre::fiction) {}

    string get_ISBN() const { return ISBN; }
    string get_title() const { return title; }
    string get_author() const { return author; }
    int get_copyright_date() const { return copyright_date; }
    Genre get_genre() const { return genre; }
    bool is_checked_out() const { return checked_out; }

    void check_out() { checked_out = true; }
    void check_in() { checked_out = false; }

    friend ostream &operator<<(ostream &os, const Book &b) {
        os << b.ISBN << ',' << b.title << ',' << b.author << ',' << b.copyright_date << ',' << get_genre_to_string(b.genre) << ',' << b.checked_out;
        return os;
    }

    friend istream &operator>>(istream &is, Book &b) {
        string genre_str;
        is >> b.ISBN >> ws;
        getline(is, b.title, ',');
        getline(is, b.author, ',');
        is >> b.copyright_date >> ws;
        getline(is, genre_str, ',');
        b.genre = string_to_genre(genre_str);
        is >> b.checked_out;
        return is;
    }
};

class Patron {
private:
    string user_name;
    string card_number;
    int owed_fees;

public:
    Patron(string name, string card, int fees = 0) : user_name(name), card_number(card), owed_fees(fees) {}
    Patron() : owed_fees(0) {}

    string get_user_name() const { return user_name; }
    string get_card_number() const { return card_number; }
    int get_owed_fees() const { return owed_fees; }

    void set_owed_fees(int fees) { owed_fees = fees; }

    bool owes_fees() const { return owed_fees > 0; }

    friend ostream &operator<<(ostream &os, const Patron &p) {
        os << p.user_name << ',' << p.card_number << ',' << p.owed_fees;
        return os;
    }

    friend istream &operator>>(istream &is, Patron &p) {
        getline(is, p.user_name, ',');
        getline(is, p.card_number, ',');
        is >> p.owed_fees;
        return is;
    }
};

class Library {
private:
    vector<Book> books;
    vector<Patron> patrons;

    const string books_file = "books.txt";
    const string patrons_file = "patrons.txt";

public:
    Library() {
        load_books();
        load_patrons();
    }

    ~Library() {
        save_books();
        save_patrons();
    }

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

    void display_books() {
        for (const auto &b : books)
            cout << b.get_title() << " by " << b.get_author() << "\n";
    }

    void display_patrons() {
        for (const auto &p : patrons)
            cout << p.get_user_name() << " (" << p.get_card_number() << ")\n";
    }

    vector<string> patrons_with_fees() const {
        vector<string> result;
        for (const auto &p : patrons) {
            if (p.owes_fees())
                result.push_back(p.get_user_name());
        }
        return result;
    }

    void save_books() {
        ofstream ofs(books_file);
        if (ofs.is_open()) {
            for (const auto &b : books)
                ofs << b << '\n';
            cout << "Books saved to file.\n"; // Debugging message
        } else {
            cout << "Failed to open books file for writing.\n";
        }
    }

    void load_books() {
        ifstream ifs(books_file);
        if (ifs.is_open()) {
            Book b;
            while (ifs >> b) {
                books.push_back(b);
            }
        } else {
            cerr << "Failed to open books file for reading.\n";
        }
    }

    void save_patrons() {
        ofstream ofs(patrons_file);
        if (ofs.is_open()) {
            for (const auto &p : patrons)
                ofs << p << '\n';
            cout << "Patrons saved to file.\n"; // Debugging message
        } else {
            cout << "Failed to open patrons file for writing.\n";
        }
    }

    void load_patrons() {
        ifstream ifs(patrons_file);
        if (ifs.is_open()) {
            Patron p;
            while (ifs >> p) {
                patrons.push_back(p);
            }
        } else {
            cerr << "Failed to open patrons file for reading.\n";
        }
    }
};

int main() {
    // Debugging current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("getcwd() error");
    }

    Library library;

    string choice;
    while (true) {
        cout << "\nLibrary Menu:\n";
        cout << "1. Add a Book\n";
        cout << "2. List Books\n";
        cout << "3. Add a Patron\n";
        cout << "4. List Patrons\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        try {
            switch (stoi(choice)) {
                case 1: {
                    string isbn, title, author;
                    int year, genre_choice;
                    cout << "Enter ISBN: ";
                    cin >> isbn;
                    cin.ignore();
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
                case 2:
                    library.display_books();
                    break;
                case 3: {
                    string name, card;
                    int fees;
                    cout << "Enter Patron Name: ";
                    cin >> ws;  // To discard leading whitespaces
                    getline(cin, name);
                    cout << "Enter Card Number: ";
                    getline(cin, card);
                    cout << "Enter Fees Owed: ";
                    cin >> fees;
                    library.add_patron(Patron(name, card, fees));
                    cout << "Patron added successfully.\n";
                    break;
                }
                case 4:
                    library.display_patrons();
                    break;
                case 5:
                    cout << "Exiting program.\n";
                    return 0;
                default:
                    cout << "Invalid choice.\n";
                    break;
            }
        } catch (const exception &e) {
            cout << "Invalid input. Please try again.\n";
        }
    }

    return 0;
}
6q123QWAQZdc| dreszwd221    1q21    2q1`1q1w21q1`1qacq  d212w2112   `1q1w21q1qaQA                                                                                                                       q22W22222222222222222222222222222222222222rq\aewdswe23`1`   111111111111111111111111111991  1Q12Q2322   1q1w21q12122111111111111111111111111111111111111111111111111111q    1qwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwa2wq2319qrqw\z vzxcggsa vv 3q    13WAWQQW2Q2Q11                                                                                                                                                                                                                                                                                                                                                                                      |Z 