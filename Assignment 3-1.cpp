#include <iostream>
#include <string>
using namespace std;

int main()
{
    string ip;

    cout << "Enter IPv4 Address: ";
    cin >> ip;

    string f = "";
    int index = 0;

    while (index < ip.length() && ip[index] != '.')
    {
        f.push_back(ip[index]);
        index++;
    }

    if (f == "")
    {
        cout << "Invalid IP Address";
        return 0;
    }

    int first = stoi(f);

    if (first < 0 || first > 255)
    {
        cout << "Invalid IP Address";
        return 0;
    }

    cout << "First Octet = " << first << endl;

    cout << "\nSubnet Definition:\n";
    cout << "A subnet is a smaller logical division of a network.\n";

    cout << "\nSubnet Mask Definition:\n";
    cout << "A subnet mask separates the Network ID from the Host ID.\n";

    if (first >= 1 && first <= 126)
    {
        cout << "\nIP Class : A\n";
        cout << "Default Subnet Mask : 255.0.0.0\n";
        cout << "IP Range : 1.0.0.0 - 126.255.255.255\n";
        cout << "Hosts Per Network : 2^24 - 2\n";
        cout << "Number of Networks : 2^7 - 2\n";
    }
    else if (first >= 128 && first <= 191)
    {
        cout << "\nIP Class : B\n";
        cout << "Default Subnet Mask : 255.255.0.0\n";
        cout << "IP Range : 128.0.0.0 - 191.255.255.255\n";
        cout << "Hosts Per Network : 2^16 - 2\n";
        cout << "Number of Networks : 2^14\n";
    }
    else if (first >= 192 && first <= 223)
    {
        cout << "\nIP Class : C\n";
        cout << "Default Subnet Mask : 255.255.255.0\n";
        cout << "IP Range : 192.0.0.0 - 223.255.255.255\n";
        cout << "Hosts Per Network : 2^8 - 2\n";
        cout << "Number of Networks : 2^21\n";
    }
    else if (first >= 224 && first <= 239)
    {
        cout << "\nIP Class : D\n";
        cout << "Purpose : Multicast\n";
        cout << "IP Range : 224.0.0.0 - 239.255.255.255\n";
    }
    else if (first >= 240 && first <= 255)
    {
        cout << "\nIP Class : E\n";
        cout << "Purpose : Experimental / Reserved\n";
        cout << "IP Range : 240.0.0.0 - 255.255.255.255\n";
    }

    cout << "\nReason for '-2' in Hosts Per Network:\n";
    cout << "One address is reserved as the Network Address.\n";
    cout << "One address is reserved as the Broadcast Address.\n";

    return 0;
}