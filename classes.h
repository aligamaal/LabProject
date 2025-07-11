#ifndef CLASSES_H
#define CLASSES_H
#include <cstddef>
#include <QString>
#include <iostream>
#include "classes.h"

// Forward declaration of QMessageBox in case needed elsewhere
class QMessageBox;

// -------------------------
// Abstract Base Class: User
// -------------------------
class User {
protected:
    QString username;
    QString password;
    QString role;

public:
    User(const QString& uname, const QString& pass, const QString& r);

    bool login(const QString& uname, const QString& pass);

    QString getUsername() const;
    QString getPassword() const;
    QString getRole() const;

    void setPassword(const QString& newPass);

    virtual void displayMenu() const = 0; // Pure virtual
    virtual ~User();
};

// -----------------
// Admin Class
// -----------------
class Admin : public User {
public:
    Admin(const QString& uname, const QString& pass);
    void displayMenu() const override;
};

// -----------------
// Manager Class
// -----------------
class Manager : public User {
public:
    Manager(const QString& uname, const QString& pass);
    void displayMenu() const override;
};

// -----------------
// Staff Class
// -----------------
class Staff : public User {
public:
    Staff(const QString& uname, const QString& pass);
    void displayMenu() const override;
};

// Add after the Staff class
// -------------------------
// Abstract Base Class: Item
// -------------------------
class Item {
protected:
    QString itemID;
    QString name;
    QString description;
    int quantity;
    double price;
    int threshold;

public:
    Item(const QString& id, const QString& n, const QString& desc, int qty, double pr);
    virtual ~Item() = default;

    // Existing methods...
    virtual QString getType() const = 0;
    QString getItemID() const;
    QString getName() const;
    QString getDescription() const;
    int getQuantity() const;
    double getPrice() const;
    int getThreshold() const;

    void setQuantity(int qty);
    void setPrice(double pr);
    void setThreshold(int thr) { threshold = thr; }
    // Add low stock detection
    bool isLowStock() const { return quantity < threshold; }

    virtual QString display() const = 0;
};

// -----------------
// Product Class
// -----------------
class Product : public Item {
    QString brand;
    QString category;

public:
    Product(const QString& id, const QString& n, const QString& desc,
            int qty, double pr, const QString& br, const QString& cat);

    QString getType() const override;
    QString getBrand() const { return brand; }
    QString getCategory() const { return category; }

    QString display() const override;
};

// -----------------
// Supply Class
// -----------------
class Supply : public Item {
    QString supplier;
    bool consumable;

public:
    Supply(const QString& id, const QString& n, const QString& desc,
           int qty, double pr, const QString& sup, bool cons);

    QString getType() const override;
    QString getSupplier() const { return supplier; }
    bool isConsumable() const { return consumable; }

    QString display() const override;
};


#endif // CLASSES_H
