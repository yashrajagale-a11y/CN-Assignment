#include <iostream>
#include <string>

using namespace std;

const int N = 4;
const int INF = 999;

int main() {
    int cost[N][N];
    int dist[N][N];
    int nextHop[N][N];

    cout << "Enter the cost matrix (" << N << "x" << N << "):\n";
    cout << "(You can type numbers or 'INF' for no direct connection)\n";

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            string input;
            cin >> input;

            if (input == "INF" || input == "inf") {
                cost[i][j] = INF;
            } else {
                cost[i][j] = stoi(input); 
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = cost[i][j];

            if (i == j) {
                nextHop[i][j] = i;
            } else if (cost[i][j] != INF) {
                nextHop[i][j] = j;
            } else {
                nextHop[i][j] = -1;
            }
        }
    }

    int updated;
    do {
        updated = 0;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    if (dist[i][k] != INF && dist[k][j] != INF) {
                        if (dist[i][j] > dist[i][k] + dist[k][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            nextHop[i][j] = nextHop[i][k];
                            updated = 1;
                        }
                    }
                }
            }
        }
    } while (updated != 0);

    // Individual Routing Tables
    cout << "\n================ ROUTING TABLES ================\n";
    for (int i = 0; i < N; i++) {
        cout << "\nRouting Table for Router r" << i << ":\n";
        cout << "Destination\tCost\tNext Hop\n";
        cout << "-------------------------------------\n";

        for (int j = 0; j < N; j++) {
            cout << "r" << j << "\t\t";
            
            if (dist[i][j] == INF) {
                cout << "INF\t-\n";
            } else {
                cout << dist[i][j] << "\tr" << nextHop[i][j] << "\n";
            }
        }
    }

    // Query Source and Destination Path
    int src, dest;
    cout << "\n================ PATH QUERY ================\n";
    cout << "Enter source router (0 to " << N - 1 << "): ";
    cin >> src;
    cout << "Enter destination router (0 to " << N - 1 << "): ";
    cin >> dest;

    if (src < 0 || src >= N || dest < 0 || dest >= N) {
        cout << "Invalid router indices provided.\n";
    } else if (dist[src][dest] == INF) {
        cout << "\nNo path exists from r" << src << " to r" << dest << ".\n";
    } else {
        cout << "\nTotal Cost: " << dist[src][dest] << "\n";
        cout << "Path: r" << src;
        
        int current = src;
        while (current != dest) {
            current = nextHop[current][dest];
            cout << " -> r" << current;
        }
        cout << "\n";
    }

    return 0;
}
