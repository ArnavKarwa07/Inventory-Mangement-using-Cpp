#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// Product class to store product details
class Product
{
private:
    int productId;
    string productName;
    double productPrice;
    int productQuantity;

public:
    // Constructor
    
    Product(int id, string name, double price, int quantity)
        : productId(id), productName(name), productPrice(price), productQuantity(quantity) {}

    // Accessors
    int getId() const { return productId; }
    string getName() const { return productName; }
    double getPrice() const { return productPrice; }
    int getQuantity() const { return productQuantity; }

    // Mutators
    void setPrice(double price) { productPrice = price; }
    void setQuantity(int quantity) { productQuantity = quantity; }

    // Display product details
    void display() const
    {
        cout << "Product ID: " << productId << endl;
        cout << "Name: " << productName << endl;
        cout << "Price: ₹" << productPrice << endl;
        cout << "Quantity: " << productQuantity << endl;
    }
};

// Inventory class to manage products
class Inventory
{
private:
    vector<Product> products;

public:
    // Add a new product
    void addProduct(const Product &product)
    {
        products.push_back(product);
    }

    // Display all products
    void displayAll() const
    {
        if (products.empty())
        {
            cout << "No products available in inventory." << endl;
        }
        else
        {
            for (const auto &product : products)
            {
                product.display();
                cout << endl;
            }
        }
    }

    // Search product by ID
    Product *findProductById(int id)
    {
        auto it = find_if(products.begin(), products.end(), [id](const Product &p)
                          { return p.getId() == id; });
        if (it != products.end())
        {
            return &(*it);
        }
        return nullptr;
    }

    // Update product details
    bool updateProduct(int id, double price, int quantity)
    {
        Product *product = findProductById(id);
        if (product)
        {
            product->setPrice(price);
            product->setQuantity(quantity);
            return true;
        }
        return false;
    }

    // Delete product by ID
    bool deleteProduct(int id)
    {
        auto it = remove_if(products.begin(), products.end(), [id](const Product &p)
                            { return p.getId() == id; });
        if (it != products.end())
        {
            products.erase(it, products.end());
            return true;
        }
        return false;
    }

    // Save inventory to file
    void saveToFile(const string &filename)
    {
        ofstream outFile(filename);
        if (outFile.is_open())
        {
            for (const auto &product : products)
            {
                outFile << product.getId() << "," << product.getName() << "," << product.getPrice() << "," << product.getQuantity() << endl;
            }
            outFile.close();
        }
        else
        {
            cout << "Error: Unable to save to file " << filename << endl;
        }
    }

    // Load inventory from file
    void loadFromFile(const string &filename)
    {
        ifstream inFile(filename);
        if (!inFile)
        {
            cout << "File not found. Starting with an empty inventory." << endl;
            return;
        }

        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string idStr, name, priceStr, quantityStr;

            // Use getline to parse the values separated by commas
            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, priceStr, ',');
            getline(ss, quantityStr, ',');

            // Convert strings to appropriate types
            int id = stoi(idStr);
            double price = stod(priceStr);
            int quantity = stoi(quantityStr);

            // Add the product to the inventory
            products.emplace_back(id, name, price, quantity);
        }
        inFile.close();
    }

    // Generate report of low stock items
    void generateLowStockReport(int threshold) const
    {
        cout << "Low Stock Report (Below " << threshold << " items):" << endl;
        for (const auto &product : products)
        {
            if (product.getQuantity() < threshold)
            {
                product.display();
                cout << endl;
            }
        }
    }
};

int main()
{
    Inventory inventory;
    int choice;
    string filename = "inventory.txt";

    // Load initial inventory from file
    inventory.loadFromFile(filename);

    do
    {
        cout << "\nInventory Management System Menu:" << endl;
        cout << "1. Add Product" << endl;
        cout << "2. Display All Products" << endl;
        cout << "3. Update Product" << endl;
        cout << "4. Delete Product" << endl;
        cout << "5. Generate Low Stock Report" << endl;
        cout << "6. Save and Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int id, quantity;
            string name;
            double price;
            cout << "Enter product ID: ";
            cin >> id;
            cout << "Enter product name: ";
            cin.ignore();       // To ignore the leftover newline from previous input
            getline(cin, name); // Allow spaces in the name
            cout << "Enter product price: ";
            cin >> price;
            cout << "Enter product quantity: ";
            cin >> quantity;
            inventory.addProduct(Product(id, name, price, quantity));
            break;
        }
        case 2:
            inventory.displayAll();
            break;
        case 3:
        {
            int id, quantity;
            double price;
            cout << "Enter product ID to update: ";
            cin >> id;
            cout << "Enter new price: ";
            cin >> price;
            cout << "Enter new quantity: ";
            cin >> quantity;
            if (inventory.updateProduct(id, price, quantity))
            {
                cout << "Product updated successfully." << endl;
            }
            else
            {
                cout << "Product not found." << endl;
            }
            break;
        }
        case 4:
        {
            int id;
            cout << "Enter product ID to delete: ";
            cin >> id;
            if (inventory.deleteProduct(id))
            {
                cout << "Product deleted successfully." << endl;
            }
            else
            {
                cout << "Product not found." << endl;
            }
            break;
        }
        case 5:
        {
            int threshold;
            cout << "Enter stock threshold for report: ";
            cin >> threshold;
            inventory.generateLowStockReport(threshold);
            break;
        }
        case 6:
            inventory.saveToFile(filename);
            cout << "Inventory saved to file. Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);
    return 0;
}
