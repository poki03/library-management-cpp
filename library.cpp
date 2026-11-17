// Practical 12: Mini project - Library Management System (console)
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <iomanip>
using namespace std;

// ---------- Exceptions ----------
class LibraryError : public runtime_error {
public: explicit LibraryError(const string& m) : runtime_error(m) {}
};
class ItemNotFound : public LibraryError {
public: explicit ItemNotFound(int id) : LibraryError("No item with ID " + to_string(id)) {}
};
class ItemUnavailable : public LibraryError {
public: ItemUnavailable(const string& t, const string& why) : LibraryError("'" + t + "' " + why) {}
};

// ---------- Class hierarchy (inheritance + polymorphism) ----------
class LibraryItem {
protected:
    int id; string title; bool available;
public:
    LibraryItem(int i, string t, bool a = true) : id(i), title(move(t)), available(a) {}
    virtual ~LibraryItem() = default;
    int getId() const { return id; }
    const string& getTitle() const { return title; }
    bool isAvailable() const { return available; }
    void issue()      { if (!available) throw ItemUnavailable(title, "is already issued"); available = false; }
    void giveBack()   { if (available)  throw ItemUnavailable(title, "was not issued"); available = true; }
    virtual string type() const = 0;
    virtual string creator() const = 0;
    virtual int loanDays() const = 0;
    string toCsv() const { return type() + "," + to_string(id) + "," + title + "," + creator() + "," + (available ? "1" : "0"); }
};
class Book : public LibraryItem {
    string author;
public:
    Book(int i, string t, string a, bool av = true) : LibraryItem(i, move(t), av), author(move(a)) {}
    string type() const override { return "Book"; }
    string creator() const override { return author; }
    int loanDays() const override { return 14; }
};
class Magazine : public LibraryItem {
    string publisher;
public:
    Magazine(int i, string t, string p, bool av = true) : LibraryItem(i, move(t), av), publisher(move(p)) {}
    string type() const override { return "Magazine"; }
    string creator() const override { return publisher; }
    int loanDays() const override { return 7; }
};

int main() {
    cout << "Name: Prasad Wakhare" << endl;
    Book b(101, "Let Us C++", "Yashavant Kanetkar");
    cout << b.toCsv() << "\n";
    b.issue();
    try { b.issue(); } catch (const LibraryError& e) { cout << "Error: " << e.what() << "\n"; }
    return 0;
}
