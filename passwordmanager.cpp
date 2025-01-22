#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// XOR-based encryption and decryption function
string xorEncryptDecrypt(const string& data, const string& key) {
    string output = data;
    for (size_t i = 0; i < data.size(); ++i) {
        output[i] ^= key[i % key.size()];
    }
    return output;
}

// Struct to hold credential data
struct Credential {
    string username;
    string encryptedPassword;
};

class PasswordManager {
private:
    vector<Credential> credentials;
    string key;

    // Save credentials to a file
    void saveCredentialsToFile() {
        ofstream file("credentials.txt");
        for (const auto& cred : credentials) {
            file << cred.username << " " << cred.encryptedPassword << endl;
        }
        file.close();
    }

    // Load credentials from a file
    void loadCredentialsFromFile() {
        ifstream file("credentials.txt");
        string username, encryptedPassword;
        while (file >> username >> encryptedPassword) {
            credentials.push_back({username, encryptedPassword});
        }
        file.close();
    }

public:
    PasswordManager(const string& encryptionKey) : key(encryptionKey) {
        loadCredentialsFromFile();
    }

    // Add a new credential
    void addCredential(const string& username, const string& password) {
        string encryptedPassword = xorEncryptDecrypt(password, key);
        credentials.push_back({username, encryptedPassword});
        saveCredentialsToFile();
        cout << "Credential added successfully!" << endl;
    }

    // View all stored credentials
    void viewCredentials() {
        for (const auto& cred : credentials) {
            string decryptedPassword = xorEncryptDecrypt(cred.encryptedPassword, key);
            cout << "Username: " << cred.username << ", Password: " << decryptedPassword << endl;
        }
    }
};

int main() {
    string masterPassword;
    string key = "my_secret_key"; // Use your own secure key

    cout << "Welcome to the Password Manager!" << endl;
    cout << "Enter master password: ";
    cin >> masterPassword;

    if (masterPassword != "master_password") { // Replace with your master password
        cout << "Incorrect master password. Exiting..." << endl;
        return 1;
    }

    PasswordManager manager(key);
    int choice;

    do {
        cout << "\nMenu:\n";
        cout << "1. Add Credential\n";
        cout << "2. View Credentials\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            manager.addCredential(username, password);
            break;
        }
        case 2:
            manager.viewCredentials();
            break;
        case 3:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 3);

    return 0;
}
