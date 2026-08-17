#include <iostream>
#include <string>
using namespace std;

int main() {
    int d1, d2, d3, d4;
    int h[8]; 
    int s1, s2, s3, error;
    string data, received;

    
    cout << "========== Sender Side ==========\n";

    cout << "Enter 4-bit data (e.g., 1011): ";
    cin >> data;

    // Validate input
    if (data.length() != 4) {
        cout << "Invalid input! Enter exactly 4 bits.\n";
        return 0;
    }

    for (char c : data) {
        if (c != '0' && c != '1') {
            cout << "Invalid input! Only 0 and 1 are allowed.\n";
            return 0;
        }
    }

    d4 = data[0] - '0';
    d3 = data[1] - '0';
    d2 = data[2] - '0';
    d1 = data[3] - '0';

    // Generate parity bits (Even Parity)
    h[1] = d1 ^ d2 ^ d4; // P1
    h[2] = d1 ^ d3 ^ d4; // P2
    h[3] = d1;
    h[4] = d2 ^ d3 ^ d4; // P3
    h[5] = d2;
    h[6] = d3;
    h[7] = d4;

    cout << "\nGenerated Hamming Code: ";
    for (int i = 7; i >= 1; i--)
        cout << h[i];
    cout << endl;

    // ================= Receiver Side =================
    cout << "\n========== Receiver Side ==========\n";

    cout << "Enter received 7-bit Hamming code: ";
    cin >> received;

    // Validate input
    if (received.length() != 7) {
        cout << "Invalid input! Enter exactly 7 bits.\n";
        return 0;
    }

    for (char c : received) {
        if (c != '0' && c != '1') {
            cout << "Invalid input! Only 0 and 1 are allowed.\n";
            return 0;
        }
    }

    // Store received bits
    for (int i = 0; i < 7; i++)
        h[7 - i] = received[i] - '0';

    // Calculate syndrome bits
    s1 = h[1] ^ h[3] ^ h[5] ^ h[7];
    s2 = h[2] ^ h[3] ^ h[6] ^ h[7];
    s3 = h[4] ^ h[5] ^ h[6] ^ h[7];

    // Find error position
    error = s1 + (2 * s2) + (4 * s3);

    if (error == 0) {
        cout << "\nNo Error Detected.\n";
    } else {
        cout << "\nError Detected at Bit Position: " << error << endl;

        // Correct the error
        h[error] ^= 1;

        cout << "Corrected Hamming Code: ";
        for (int i = 7; i >= 1; i--)
            cout << h[i];
        cout << endl;
    }

    // Display original data bits
    cout << "\nOriginal Data Bits: ";
    cout << h[7] << h[6] << h[5] << h[3] << endl;

    return 0;
}