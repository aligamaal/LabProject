**The American University in Cairo**   
**School of Sciences and Eng.**

**CSCE 1102**   
**Dept. of Computer Science  Summer 2025** 

**(Fundamentals of Computing II \- Lab)**   
**\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ Project** 

**\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ Project Milestones and Deliverables** 

**Milestones Summary** 

| *Milestone*  | *%*  | *Due* |
| :---: | :---: | :---: |
| **Milestone 0**  | **Project Team Formation** | **Check Due dates  Here** |
| **Milestone 1**  | **5% (Team grade)** |  |
| **Milestone 2**  | **10% (4% forms, 4% user handling, 2%github)** |  |
| **Milestone 3**  | **10% Demo+Pres (Team grade)  15% (12% Code, 3% github)** |  |

**\* *Project Individual grade is calculated as a percentage of team grade relative to  individual’s share, code quality, and understanding*** 

**Total Project Grade \= 40%**   
**Milestone 1 \- Initial Design Report** 

**1\. GitHub Repo** 

● Create a github repo for your project and make it **private** 

● Add an info text file listing all team members, their IDs, and contact info. ● Add your teammates as collaborators 

● Add the following accounts as collaborator: **mohismailAUC, Mina-Sawers 2\. Project Classes** 

● List the classes of your project as shown below 

● You should include ***core classes*** (e.g. Book, LibraryMemebr,...etc), ***users  classes*** (e..g Admin, Librarian,.. etc) and ***maestro class(es)*** that should  manage the system operation as a whole (e.g. Library class) 

● For each ***user class***, list the functionality that should be supported by the  application for this user.  

**Deliverables:** 

● Each team prepares a design report formatted as shown below: ● The report should be  

○ submitted to a form (link will be provided) 

○ uploaded on github 

**The American University in Cairo Summer 2025**  
**Fundamentals of Computing II \- LAB** 

**CSCE 1102- Project**  

**Milestone 1 \- Design Report** 

**Team\#:** 1						

**Team Email: quickstockcs2@gmail.com**

**Members’ Info:** 

| Member Name  | ID  | Email |
| ----- | ----- | ----- |
| Salma Gomaa | **900241457** | **[salmagomaa@aucegypt.edu](mailto:salmagomaa@aucegypt.edu)**  |
| **Omar mubarak** | **900243846** | **[Mubarak@aucegypt.edu](mailto:Mubarak@aucegypt.edu)**  |
| **Nour elmasry** | **900223003** | **[nourelmasry04@aucegpt.edu](mailto:nourelmasry04@aucegpt.edu)**  |
| **Ali gamal** | **900222145** | **[aligamal04@aucegypt.edu](mailto:aligamal04@aucegypt.edu)**  |

**GitHub link: [https://github.com/aligamaal/Group-Project.git](https://github.com/aligamaal/Group-Project.git)** 

**Core classes**  

| Class Name /Short  description | Data members  | Member Functions |
| :---- | :---- | :---- |
| **Item**  | **String itemID, string name, string description, int quantity, double price, bool isActive** | **Public: // Pure virtual functions virtual void display() const \= 0; virtual double calculateTotalValue() const \= 0; // common functions void updateQuantity(int newQuantity); void addStock(int amount); void removeStock(int amount); bool isInStock() const; bool isLowStock(int threshold) const; // getters string getItemID() const; string getName() const; int getQuantity() const; double getPrice() const; // setters void setName(const string& name); void setPrice(double price);** |
| **Product : Item** | **String category, string brand, double weight, string dimensions, string barcode** | **public: void display() const override; double calculateTotalValue() const override; void setCategory(const string& category); void setBarcode(const string& barcode); string getCategory() const;  string getBarcode() const;** |
| **Supply : Item** | **String supplierName, string supplierContact, int reorderLevel, string lastOrderDate, bool isConsumable** | **public: void display() const override; double calculateTotalValue() const override; void setSupplier(const string& supplier); string getSupplier() const; bool needsReorder() const;** |
|  |  |  |

**Users classes and functionality of each user** 

| Class Name /Short  description | Data members  | Member Functions |
| :---- | :---- | :---- |
| **User** | **String userId, string username, string password, string email, string role, string firstName, string lastName, string dateCreated, bool isActive** | **public:    User(string id, string user, string pass, string mail, string r); // Authentication     bool login(string user, string pass);     void logout();          // Getters     string getUserId() const;     string getUsername() const;     string getEmail() const;     string getRole() const;     string getFirstName() const;     string getLastName() const;     string getDateCreated() const;     bool getIsActive() const;          // Setters     void setUsername(string user);     void setPassword(string pass);     void setEmail(string mail);     void setFirstName(string fname);     void setLastName(string lname);     void setIsActive(bool active);          // Utility functions     void displayInfo() const;     void changePassword(string newPass);          // Pure virtual function     virtual void displayMenu() const \= 0;**  |
| **Admin : User** | **String adminLevel, int maxUsers Vector \<String\> systemPermissions** | **Public: // User management     void createUser(const User& user);     void deleteUser(const string& userID);     void modifyUserPermissions(const string& userID);     void viewAllUsers() const;          // Item management     void addItem(const Item& item);     void removeItem(const std::string& itemID);     void modifyItem(const std::string& itemID);          // System management     void generateFullReport() const;     void backupSystem();     void restoreSystem();     void viewSystemLogs() const;     void configureSystem();** |
| **Manager : User** | **String department, double budgetLimit, string supervisorID, Int approvalLevel** | **public:     // Item management     void addItem(const Item& item);     void updateItem(const string& itemID);     void viewAllItems() const;          // Stock management     void updateStock(const string& itemID, int quantity);     void checkLowStock() const;     void approveReorder(const string& itemID);          // Reporting     void generateInventoryReport() const;     void generateStockReport() const;     void generateValueReport() const;     void viewItemHistory(const string& itemID) const;** |
| **Employee : User** | **String employeeID, string shift, string workstation, string managerID, vector \<string\> assignedTasks** | **// Basic item operations     void searchItem(const string& keyword) const;     void viewItem(const string& itemID) const;     void viewAllItems() const;          // Stock updates     void updateQuantity(const string& itemID, int newQuantity);     void recordStockMovement(const string& itemID, int amount, const string& type);          // Basic reporting     void generateSimpleReport() const;     void checkItemAvailability(const string& itemID) const;     void viewLowStockItems() const;** |

**Maestro class(es)**

| List Name  | Selected DS  | Reasons for selecting this DS |
| :---- | :---- | :---- |
| **InventorySystem** | **Stack**  | **Inventory system is managed using a LIFO system (last in first out) which is represented by the stack.** |

