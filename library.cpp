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

// ---------- Library (STL map keeps records in memory) ----------
class Library {
    map<int, unique_ptr<LibraryItem>> items;
    LibraryItem& find(int id) {
        auto it = items.find(id);
        if (it == items.end()) throw ItemNotFound(id);
        return *it->second;
    }
public:
    void add(unique_ptr<LibraryItem> item) {
        int id = item->getId();
        if (items.count(id)) throw LibraryError("ID " + to_string(id) + " already exists");
        items[id] = move(item);
    }
    void list() const {
        cout << left << setw(5) << "ID" << setw(11) << "Type" << setw(32) << "Title" << setw(20) << "Author/Publisher" << "Status\n";
        for (const auto& [id, it] : items)
            cout << setw(5) << id << setw(11) << it->type() << setw(32) << it->getTitle() << setw(20) << it->creator()
                 << (it->isAvailable() ? "Available" : "Issued") << "\n";
    }
    void search(const string& word) const {
        bool any = false;
        for (const auto& [id, it] : items)
            if (it->getTitle().find(word) != string::npos) {
                cout << "  Found: [" << id << "] " << it->getTitle() << " (" << it->type() << ")\n"; any = true;
            }
        if (!any) cout << "  No title contains \"" << word << "\"\n";
    }
    void issue(int id)  { auto& it = find(id); it.issue();    cout << "  Issued '" << it.getTitle() << "' for " << it.loanDays() << " days\n"; }
    void giveBack(int id) { auto& it = find(id); it.giveBack(); cout << "  Returned '" << it.getTitle() << "'\n"; }

    void save(const string& file) const {
        ofstream out(file);
        if (!out) throw LibraryError("Cannot open " + file + " for writing");
        for (const auto& [id, it] : items) out << it->toCsv() << "\n";
        cout << "  Saved " << items.size() << " records to " << file << "\n";
    }
    void load(const string& file) {
        ifstream in(file);
        if (!in) { cout << "  No saved data (" << file << ") - starting with an empty library\n"; return; }
        string line; int n = 0, bad = 0;
        while (getline(in, line)) {
            stringstream ss(line);
            string type, id, title, who, av;
            if (!getline(ss, type, ',') || !getline(ss, id, ',') || !getline(ss, title, ',') ||
                !getline(ss, who, ',') || !getline(ss, av)) { ++bad; continue; }
            try {
                if (type == "Book") add(make_unique<Book>(stoi(id), title, who, av == "1"));
                else if (type == "Magazine") add(make_unique<Magazine>(stoi(id), title, who, av == "1"));
                else { ++bad; continue; }
                ++n;
            } catch (const exception&) { ++bad; }
        }
        cout << "  Loaded " << n << " records from " << file << (bad ? " (" + to_string(bad) + " bad lines skipped)" : "") << "\n";
    }
};

int main() {
    cout << "Name: Prasad Wakhare" << endl;
    Library lib;
    lib.load("library.csv");
    lib.list();
    try { lib.add(make_unique<Book>(101, "Let Us C++", "Yashavant Kanetkar")); } catch (const LibraryError& e) { cout << "Note: " << e.what() << "\n"; }
    lib.save("library.csv");
    return 0;
}
