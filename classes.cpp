
// Core classes

class Item {
    
    
};

class Product : public Item {
    
    
};

class RawMaterial : public Item {
    
};

class Equipment : public Item {
    
};

class Supplier {
    
};

class Transaction {
    
};

// User classes

class Admin {                           // add/remove/modify all items, manage user                                     accounts (create, delete, modify                                                permissions), generate reports, configure                                       system settings, backup and restore data,                                       view audit logs, etc.
                                            
};

class InventoryManager {                // Add/update/remove inventory items,                                           process stock adjustments, generate                                             inventory reports, monitor stock levels and                                     reorder alerts, manage supplier                                                 information, process incoming/outgoing                                          transactions, conduct stock audits.
    
};

class StockClerk {
                                        // Update item quanitites, record stock movements, search and view inventory items, generate basic reports, process receiving/shipping transactions, update item locations
};

class Viewer {
                                        // View inventory items and quanities, search inventory database, generate read-only reports, view transaction history, monitor stock levels.
};

// Maestro class

class InventorySystem {
    
};
