#include "classes.h"

// ---- User ----
User::User(const QString& uname, const QString& pass, const QString& r)
    : username(uname), password(pass), role(r) {}

bool User::login(const QString& uname, const QString& pass) {
    return (username == uname && password == pass);
}

QString User::getUsername() const {
    return username;
}

QString User::getPassword() const { // ✅ This was missing!
    return password;
}

QString User::getRole() const {
    return role;
}

void User::setPassword(const QString& newPass) {
    password = newPass;
}

User::~User() {}  // Virtual destructor definition

// ---- Admin ----
Admin::Admin(const QString& uname, const QString& pass)
    : User(uname, pass, "admin") {}

void Admin::displayMenu() const {
    std::cout << "Admin Menu\n";
}

// ---- Manager ----
Manager::Manager(const QString& uname, const QString& pass)
    : User(uname, pass, "manager") {}

void Manager::displayMenu() const {
    std::cout << "Manager Menu\n";
}

// ---- Staff ----
Staff::Staff(const QString& uname, const QString& pass)
    : User(uname, pass, "staff") {}

void Staff::displayMenu() const {
    std::cout << "Staff Menu\n";
}


// ---- Item Base Class ----
Item::Item(const QString& id, const QString& n, const QString& desc, int qty, double pr)
    : itemID(id), name(n), description(desc), quantity(qty), price(pr) {}


QString Item::getItemID() const { return itemID; }
QString Item::getName() const { return name; }
QString Item::getDescription() const { return description; }
int Item::getQuantity() const { return quantity; }
double Item::getPrice() const { return price; }
void Item::setQuantity(int qty) { quantity = qty; }
void Item::setPrice(double pr) { price = pr; }

// ---- Product ----
Product::Product(const QString& id, const QString& n, const QString& desc, int qty, double pr,
                 const QString& brand_, const QString& category_)
    : Item(id, n, desc, qty, pr), brand(brand_), category(category_) {}

QString Product::display() const {
    return QString("Product | ID: %1 | Name: %2 | Qty: %3 | Price: %4 | Brand: %5 | Category: %6")
    .arg(itemID)
        .arg(name)
        .arg(quantity)
        .arg(price)
        .arg(brand)
        .arg(category);

}
QString Product::getType() const { return "Product"; }
// ---- Supply ----
Supply::Supply(const QString& id, const QString& n, const QString& desc, int qty, double pr,
               const QString& supplier_, bool consumable_)
    : Item(id, n, desc, qty, pr), supplier(supplier_), consumable(consumable_) {}

QString Supply::display() const {
    return QString("Supply | ID: %1 | Name: %2 | Qty: %3 | Price: %4 | Supplier: %5 | Consumable: %6")
    .arg(itemID)
        .arg(name)
        .arg(quantity)
        .arg(price)
        .arg(supplier)
        .arg(consumable ? "Yes" : "No");
}
QString Supply::getType() const
{ return "Supply"; }
