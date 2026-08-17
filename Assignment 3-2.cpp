#include <iostream>
#include <string>
using namespace std;

void parseIP(string ip, int addr[]) {
    int index = 0;
    int num = 0;

    for (int i = 0; i <= ip.length(); i++) {
        if (ip[i] == '.' || ip[i] == '\0') {
            addr[index] = num;
            index++;
            num = 0;
        } else {
            num = num * 10 + (ip[i] - '0');
        }
    }
}

void printIP(unsigned int ip) {
    cout << ((ip >> 24) & 255) << "."
         << ((ip >> 16) & 255) << "."
         << ((ip >> 8) & 255) << "."
         << (ip & 255);
}

int main() {

    string ip;
    int requiredSubnets;
    int addr[4];

    cout << "Enter IP address: ";
    cin >> ip;

    cout << "Enter number of required subnets: ";
    cin >> requiredSubnets;

    // Step 1: Parse IP
    parseIP(ip, addr);

    // Validate IP
    for (int i = 0; i < 4; i++) {
        if (addr[i] < 0 || addr[i] > 255) {
            cout << "Invalid IP address." << endl;
            return 0;
        }
    }

    // Step 2: Find IP Class
    char ipClass;
    int defaultBits;

    if (addr[0] >= 1 && addr[0] <= 126) {
        ipClass = 'A';
        defaultBits = 8;
    }
    else if (addr[0] >= 128 && addr[0] <= 191) {
        ipClass = 'B';
        defaultBits = 16;
    }
    else if (addr[0] >= 192 && addr[0] <= 223) {
        ipClass = 'C';
        defaultBits = 24;
    }
    else {
        cout << "Class D/E address. Not supported." << endl;
        return 0;
    }

    cout << "\nIP Class: " << ipClass << endl;

    // Step 3: Calculate borrowed bits
    int borrowedBits = 0;
    int possibleSubnets = 1;

    while (possibleSubnets < requiredSubnets) {
        borrowedBits++;
        possibleSubnets *= 2;
    }

    // Check whether borrowing is possible
    int maximumBorrowableBits = 32 - defaultBits - 2;

    if (borrowedBits > maximumBorrowableBits) {
        cout << "Required number of subnets is not possible." << endl;
        return 0;
    }

    cout << "Borrowed bits: " << borrowedBits << endl;
    cout << "Actual number of subnets: "
         << possibleSubnets << endl;

    // Step 4: Calculate host bits
    int totalNetworkBits = defaultBits + borrowedBits;
    int hostBits = 32 - totalNetworkBits;

    unsigned int addressesPerSubnet = 1U << hostBits;

    cout << "Subnet prefix: /"
         << totalNetworkBits << endl;

    cout << "Addresses per subnet: "
         << addressesPerSubnet << endl;

    cout << "Usable IP addresses per subnet: "
         << addressesPerSubnet - 2 << endl;

    // Step 5: Create subnet mask
    unsigned int mask;

    if (totalNetworkBits == 32)
        mask = 0xFFFFFFFF;
    else
        mask = 0xFFFFFFFF << (32 - totalNetworkBits);

    cout << "Subnet Mask: ";
    printIP(mask);
    cout << endl;

    // Step 6: Convert IP to 32-bit number
    unsigned int ipValue =
        ((unsigned int)addr[0] << 24) |
        ((unsigned int)addr[1] << 16) |
        ((unsigned int)addr[2] << 8) |
        (unsigned int)addr[3];

    // Find network address
    unsigned int network = ipValue & mask;

    // Step 7: Print subnet details
    cout << "\n-----------------------------------------------\n";

    for (int i = 0; i < possibleSubnets; i++) {

        unsigned int subnetNetwork =
            network + (i * addressesPerSubnet);

        unsigned int broadcast =
            subnetNetwork + addressesPerSubnet - 1;

        unsigned int firstIP = subnetNetwork + 1;
        unsigned int lastIP = broadcast - 1;

        cout << "\nSubnet " << i + 1 << endl;

        cout << "Network Address : ";
        printIP(subnetNetwork);

        cout << "\nFirst IP        : ";
        printIP(firstIP);

        cout << "\nLast IP         : ";
        printIP(lastIP);

        cout << "\nBroadcast       : ";
        printIP(broadcast);

        cout << "\n-----------------------------------------------\n";
    }

    return 0;
}