
// Core classes

class Item {
    
    
};

class Product : public Item {
    
    
};

class Supply : public Item {
    
};

// User classes

class User {
                // Login/Logout operations, password management, profile management, system interaction
};

class Admin : public User {         // Add/remove items, manage users, view all reports, system backup.
                                            
};

class Manager : public User {                // add/update items, generate reports, monitor stock levels, approve orders
    
};

class Employee : public User {
                                        // view items, update quanities, search inventory, generate basic reports.
};



// Maestro class

class InventorySystem {
    
};
