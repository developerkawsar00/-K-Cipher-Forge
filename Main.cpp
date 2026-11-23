#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include <sstream>

using namespace std;

// ==================== BASE64 ====================
const string BASE64_CHARS =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

string base64Encode(const string& input) {
    string output;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            output.push_back(BASE64_CHARS[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) {
        output.push_back(BASE64_CHARS[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    while (output.size() % 4) {
        output.push_back('=');
    }
    return output;
}

string base64Decode(const string& input, bool& ok) {
    ok = true;
    vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) {
        T[static_cast<unsigned char>(BASE64_CHARS[i])] = i;
    }

    string clean;
    for (char c : input) {
        if (!isspace(static_cast<unsigned char>(c))) {
            clean.push_back(c);
        }
    }

    if (clean.empty()) return "";
    if (clean.size() % 4 != 0) {
        ok = false;
        return "";
    }

    string output;
    int val = 0, valb = -8;
    for (size_t i = 0; i < clean.size(); i++) {
        char c = clean[i];
        if (c == '=') {
            if (i < clean.size() - 2) {
                ok = false;
                return "";
            }
            break;
        }
        int d = T[static_cast<unsigned char>(c)];
        if (d == -1) {
            ok = false;
            return "";
        }
        val = (val << 6) + d;
        valb += 6;
        if (valb >= 0) {
            output.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return output;
}

// ==================== HEX ====================
string hexEncode(const string& input) {
    ostringstream oss;
    oss << hex << uppercase << setfill('0');
    for (unsigned char c : input) {
        oss << setw(2) << static_cast<int>(c);
    }
    return oss.str();
}

int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
}

string hexDecode(const string& input, bool& ok) {
    ok = true;
    string clean;
    for (char c : input) {
        if (!isspace(static_cast<unsigned char>(c))) {
            clean.push_back(c);
        }
    }

    if (clean.size() % 2 != 0) {
        ok = false;
        return "";
    }

    string output;
    for (size_t i = 0; i < clean.size(); i += 2) {
        int hi = hexCharToInt(clean[i]);
        int lo = hexCharToInt(clean[i + 1]);
        if (hi == -1 || lo == -1) {
            ok = false;
            return "";
        }
        output.push_back(char((hi << 4) | lo));
    }
    return output;
}

// ==================== CAESAR CIPHER ====================
string caesarCipher(const string& input, int shift) {
    shift = ((shift % 26) + 26) % 26;
    string output = input;
    for (char& c : output) {
        if (c >= 'a' && c <= 'z') {
            c = char('a' + (c - 'a' + shift) % 26);
        } else if (c >= 'A' && c <= 'Z') {
            c = char('A' + (c - 'A' + shift) % 26);
        }
    }
    return output;
}

// ==================== XOR CIPHER ====================
string xorCipher(const string& input, const string& key, bool& ok) {
    if (key.empty()) {
        ok = false;
        return "";
    }
    ok = true;
    string output = input;
    for (size_t i = 0; i < input.size(); i++) {
        output[i] = char(input[i] ^ key[i % key.size()]);
    }
    return output;
}

// ==================== MENU & CLI ====================
void showMenu() {
    cout << "\n====================================\n";
    cout << "       K-CODEC (C++ CLI Tool)\n";
    cout << "====================================\n";
    cout << "1. Base64 Encode\n";
    cout << "2. Base64 Decode\n";
    cout << "3. Hex Encode\n";
    cout << "4. Hex Decode\n";
    cout << "5. Caesar Cipher Encode\n";
    cout << "6. Caesar Cipher Decode\n";
    cout << "7. XOR Cipher (Encode / Decode)\n";
    cout << "0. Exit\n";
    cout << "------------------------------------\n";
    cout << "Choose an option: ";
}

int main() {
    while (true) {
        showMenu();
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[!] Invalid input. Enter a number.\n";
            continue;
        }
        cin.ignore(10000, '\n');

        if (choice == 0) {
            cout << "Bye! Happy coding.\n";
            break;
        }

        string text;
        cout << "Enter text: ";
        getline(cin, text);

        bool ok = true;
        if (choice == 1) {
            cout << "Result: " << base64Encode(text) << "\n";
        } else if (choice == 2) {
            string res = base64Decode(text, ok);
            if (!ok) cout << "[!] Invalid Base64 input.\n";
            else cout << "Result: " << res << "\n";
        } else if (choice == 3) {
            cout << "Result: " << hexEncode(text) << "\n";
        } else if (choice == 4) {
            string res = hexDecode(text, ok);
            if (!ok) cout << "[!] Invalid Hex input.\n";
            else cout << "Result: " << res << "\n";
        } else if (choice == 5) {
            int shift;
            cout << "Enter shift (e.g. 3): ";
            cin >> shift;
            cin.ignore(10000, '\n');
            cout << "Result: " << caesarCipher(text, shift) << "\n";
        } else if (choice == 6) {
            int shift;
            cout << "Enter shift (e.g. 3): ";
            cin >> shift;
            cin.ignore(10000, '\n');
            cout << "Result: " << caesarCipher(text, -shift) << "\n";
        } else if (choice == 7) {
            string key;
            cout << "Enter key: ";
            getline(cin, key);
            string res = xorCipher(text, key, ok);
            if (!ok) {
                cout << "[!] Key cannot be empty.\n";
            } else {
                cout << "Result (Raw): " << res << "\n";
                cout << "Result (Hex - safe to copy): " << hexEncode(res) << "\n";
            }
        } else {
            cout << "[!] Invalid option.\n";
        }
    }
    return 0;
}