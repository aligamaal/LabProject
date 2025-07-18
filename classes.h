#ifndef CLASSES_H
#define CLASSES_H

#include <QString>
#include <vector>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QBrush>
#include <QColor>
#include <QAbstractItemView>

// ==================== USER CLASSES ====================

// Base User class
class User {
protected:
    QString username;
    QString password;
    QString role;

public:


    User(const QString& uname, const QString& pass, const QString& r);
    virtual ~User();

    // Login verification
    bool login(const QString& uname, const QString& pass);
<<<<<<< HEAD

    // Getters
=======
// getters and setters
>>>>>>> 966a455586751fd469cb8ea97e80e7f8237c781e
    QString getUsername() const;
    QString getPassword() const;
    QString getRole() const;

    // Setters
    void setPassword(const QString& newPass);

    // Virtual methods
    virtual void displayMenu() const = 0;  // Pure virtual
};

// Admin class - Full system access, user management
class Admin : public User {
public:
    Admin(const QString& uname, const QString& pass);
    void displayMenu() const override;
};

// Manager class - Inventory management, reports, pricing
class Manager : public User {
public:
    Manager(const QString& uname, const QString& pass);
    void displayMenu() const override;
};

// Staff class - Basic inventory operations
class Staff : public User {
public:
    Staff(const QString& uname, const QString& pass);
    void displayMenu() const override;
};

// Role constants
const QString ADMIN = "Admin";
const QString MANAGER = "Manager";
const QString STAFF = "Staff";

// ==================== ITEM CLASSES ====================

// Base Item class
class Item {
private:
    QString name;
    int quantity;
    double price;
    int lowStockThreshold;
    QString category;
    QString supplier;    // Add threshold for low stock warning

public:
<<<<<<< HEAD
    Item(const QString& n, int qty, double pr,const QString& category_ , const QString& supplier_);
    ~Item();
    void display() const;
    static void populateTable(QTableWidget* table, const std::vector<Item*>& itemList);
    // Getters
=======
    Item(const QString& id, const QString& n, const QString& desc, int qty, double pr);
    virtual ~Item() = default;

    // getters and setters
    virtual QString getType() const = 0;
    QString getItemID() const;
>>>>>>> 966a455586751fd469cb8ea97e80e7f8237c781e
    QString getName() const;
    int getQuantity() const;
    double getPrice() const;
    int getLowStockThreshold() const;
    QString getCategory() const;
    QString getSupplier() const;



    // Setters
    void setName(QString n);
    void setCategory(QString category_);
    void setQuantity(int qty);
    void setPrice(double pr);
    void setLowStockThreshold(int threshold);
    void setSupplier(QString supplier_);

    // Stock management
    bool isLowStock() const;
};

<<<<<<< HEAD
=======
// -----------------
// Product Class
// -----------------
class Product : public Item {
    QString brand;
    QString category;

public:
    Product(const QString& id, const QString& n, const QString& desc,
            int qty, double pr, const QString& br, const QString& cat);

// getters and setters
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

//getters and setters
    QString getType() const override;
    QString getSupplier() const { return supplier; }
    bool isConsumable() const { return consumable; }

    QString display() const override;
};


>>>>>>> 966a455586751fd469cb8ea97e80e7f8237c781e
#endif // CLASSES_H
