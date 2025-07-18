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

    // Getters
    QString getUsername() const;
    QString getPassword() const;
    QString getRole() const;

    // Setters
    void setPassword(const QString& newPass);

    virtual void displayMenu() const = 0;  // Pure virtual
};

class Admin : public User {
public:
    Admin(const QString& uname, const QString& pass);
    void displayMenu() const override;
};

class Manager : public User {
public:
    Manager(const QString& uname, const QString& pass);
    void displayMenu() const override;
};

class Staff : public User {
public:
    Staff(const QString& uname, const QString& pass);
    void displayMenu() const override;
};

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
    QString supplier;

public:
    Item(const QString& n, int qty, double pr,const QString& category_ , const QString& supplier_);
    ~Item();
    void display() const;
    static void populateTable(QTableWidget* table, const std::vector<Item*>& itemList);
    // Getters
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

#endif // CLASSES_H
