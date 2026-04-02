// ============================================================
// main.cpp — Smart Transport & Navigation System
// ============================================================
// Menu-driven interactive program demonstrating:
//
// OOP Concepts:
//   ✓ Inheritance    (City, Stop → Location)
//   ✓ Polymorphism   (DijkstraFinder, BFSFinder → RouteFinder)
//   ✓ Encapsulation  (private data, public getters/setters)
//   ✓ Abstraction    (pure virtual methods in Location, RouteFinder)
//
// DSA Concepts:
//   ✓ Weighted Graph (Adjacency List) — TransportNetwork
//   ✓ Dijkstra's Algorithm            — shortest path by distance
//   ✓ BFS                             — minimum stops path
//   ✓ Min-Heap (Priority Queue)       — used in Dijkstra
//   ✓ Trie (Prefix Tree)             — city name autocomplete
//   ✓ Stack (LIFO)                    — recent search history
//   ✓ HashMap (Hash Table)            — fare lookup by route
// ============================================================

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

// --- Project Headers ---
#include "Location.h"
#include "City.h"
#include "Stop.h"
#include "TransportNetwork.h"
#include "RouteFinder.h"
#include "DijkstraFinder.h"
#include "BFSFinder.h"
#include "Trie.h"
#include "Stack.h"
#include "HashMap.h"

using namespace std;

// ============================================================
// ANSI Color Codes for beautiful terminal output
// ============================================================
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BG_BLUE "\033[44m"

// ============================================================
// Helper: Clear input buffer
// ============================================================
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============================================================
// Helper: Display a path with total cost
// ============================================================
void displayPath(const vector<string>& path, const TransportNetwork& network,
                 const string& algoName) {
    if (path.empty()) {
        cout << RED << "  ✗ No path found!" << RESET << endl;
        return;
    }

    cout << GREEN << "\n  ✓ Path found using " << algoName << ":" << RESET << endl;
    cout << "  ─────────────────────────────────────────────" << endl;

    // Display path with arrows and segment distances
    cout << CYAN << "  Route: " << RESET;
    for (size_t i = 0; i < path.size(); i++) {
        cout << BOLD << path[i] << RESET;
        if (i < path.size() - 1) {
            int dist = network.getDistance(path[i], path[i + 1]);
            cout << YELLOW << " ──(" << dist << " km)──▶ " << RESET;
        }
    }
    cout << endl;

    // Statistics
    int totalCost = network.calculatePathCost(path);
    int stops = path.size() - 1;
    cout << "\n  📊 Statistics:" << endl;
    cout << "     Total Distance : " << GREEN << totalCost << " km" << RESET << endl;
    cout << "     Number of Stops: " << GREEN << stops << RESET << endl;
    cout << "  ─────────────────────────────────────────────" << endl;
}

// ============================================================
// displayBanner — Show the main title banner
// ============================================================
void displayBanner() {
    cout << CYAN << BOLD;
    cout << "\n  ╔══════════════════════════════════════════════════════════╗" << endl;
    cout << "  ║                                                          ║" << endl;
    cout << "  ║     🚂  SMART TRANSPORT & NAVIGATION SYSTEM  🚂        ║" << endl;
    cout << "  ║                                                          ║" << endl;
    cout << "  ║     DSA + OOP Project | B.Tech Final Year               ║" << endl;
    cout << "  ║                                                          ║" << endl;
    cout << "  ╚══════════════════════════════════════════════════════════╝" << endl;
    cout << RESET << endl;
}

// ============================================================
// displayMenu — Show the main menu options
// ============================================================
void displayMenu() {
    cout << YELLOW << "\n  ┌──────────────────────────────────────────────┐" << RESET << endl;
    cout << YELLOW << "  │" << WHITE << BOLD << "              MAIN MENU                       " << RESET << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  ├──────────────────────────────────────────────┤" << RESET << endl;
    cout << YELLOW << "  │" << CYAN << "  [1]  " << WHITE << "Add City                            " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << CYAN << "  [2]  " << WHITE << "Add Route (Road)                    " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << CYAN << "  [3]  " << WHITE << "Remove City                         " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << CYAN << "  [4]  " << WHITE << "Remove Route                        " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  ├──────────────────────────────────────────────┤" << RESET << endl;
    cout << YELLOW << "  │" << CYAN << "  [5]  " << WHITE << "Block a Road                        " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << CYAN << "  [6]  " << WHITE << "Unblock a Road                      " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  ├──────────────────────────────────────────────┤" << RESET << endl;
    cout << YELLOW << "  │" << GREEN << "  [7]  " << WHITE << "Shortest Path (Dijkstra)            " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << GREEN << "  [8]  " << WHITE << "Minimum Stops Path (BFS)            " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << GREEN << "  [9]  " << WHITE << "Compare Dijkstra vs BFS             " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  ├──────────────────────────────────────────────┤" << RESET << endl;
    cout << YELLOW << "  │" << MAGENTA << "  [10] " << WHITE << "Search City by Prefix (Trie)       " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << MAGENTA << "  [11] " << WHITE << "View Recent Searches (Stack)       " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  ├──────────────────────────────────────────────┤" << RESET << endl;
    cout << YELLOW << "  │" << BLUE << "  [12] " << WHITE << "Add / Update Fare (HashMap)         " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << BLUE << "  [13] " << WHITE << "Lookup Fare (HashMap)               " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << BLUE << "  [14] " << WHITE << "Display All Fares                   " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  ├──────────────────────────────────────────────┤" << RESET << endl;
    cout << YELLOW << "  │" << DIM << "  [15] " << WHITE << "Display Network Map                 " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << DIM << "  [16] " << WHITE << "Display Blocked Routes              " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << DIM << "  [17] " << WHITE << "Display All Cities (Trie)           " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << DIM << "  [18] " << WHITE << "Show OOP Demo (Polymorphism)        " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  ├──────────────────────────────────────────────┤" << RESET << endl;
    cout << YELLOW << "  │" << GREEN << BOLD << "  [19] " << WHITE << "Load Sample Network                " << RESET << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  │" << RED <<  "  [0]  " << WHITE << "Exit                                " << YELLOW << "│" << RESET << endl;
    cout << YELLOW << "  └──────────────────────────────────────────────┘" << RESET << endl;
    cout << CYAN << "\n  Enter choice: " << RESET;
}

// ============================================================
// loadSampleNetwork — Preload cities and routes for testing
// ============================================================
// Creates a sample network of Indian cities with realistic
// distances, designed so that Dijkstra and BFS give different
// results (demonstrating the difference between the algorithms).
// ============================================================
void loadSampleNetwork(TransportNetwork& network, Trie& trie,
                       HashMap<int>& fareMap, vector<Location*>& locations) {

    cout << YELLOW << "\n  Loading sample network..." << RESET << endl;

    // --- Create City objects (Inheritance + Encapsulation demo) ---
    locations.push_back(new City("Delhi",     28.6139, 77.2090, 19000000, true));
    locations.push_back(new City("Mumbai",    19.0760, 72.8777, 21000000, true));
    locations.push_back(new City("Kolkata",   22.5726, 88.3639, 15000000, true));
    locations.push_back(new City("Chennai",   13.0827, 80.2707, 10000000, true));
    locations.push_back(new City("Bangalore", 12.9716, 77.5946, 12000000, true));
    locations.push_back(new City("Hyderabad", 17.3850, 78.4867, 10000000, true));
    locations.push_back(new City("Jaipur",    26.9124, 75.7873,  4000000, false));
    locations.push_back(new City("Lucknow",   26.8467, 80.9462,  3500000, false));
    locations.push_back(new City("Pune",      18.5204, 73.8567,  7000000, false));
    locations.push_back(new City("Ahmedabad", 23.0225, 72.5714,  8000000, true));

    // --- Create Stop objects (Inheritance demo) ---
    locations.push_back(new Stop("Nagpur", 21.1458, 79.0882, "Railway", "NH-44"));
    locations.push_back(new Stop("Bhopal", 23.2599, 77.4126, "Railway", "NH-46"));

    // --- Add cities to network and trie ---
    vector<string> cityNames = {
        "Delhi", "Mumbai", "Kolkata", "Chennai", "Bangalore",
        "Hyderabad", "Jaipur", "Lucknow", "Pune", "Ahmedabad",
        "Nagpur", "Bhopal"
    };

    for (const string& name : cityNames) {
        network.addCity(name);
        trie.insert(name);
    }

    // ============================================================
    // Add Routes (Weighted Edges)
    // ============================================================
    // The distances are designed so that:
    //   - Direct routes exist but may be expensive (high distance)
    //   - Indirect routes via multiple cities may be shorter in total
    //   - This creates scenarios where Dijkstra ≠ BFS results
    //
    // Example: Delhi → Bangalore
    //   BFS path (2 stops):  Delhi → Bangalore (direct, 2150 km)
    //   Dijkstra path (5 stops): Delhi → Jaipur → Ahmedabad →
    //                            Mumbai → Pune → Bangalore (1960 km)
    // ============================================================

    // Major highway routes
    network.addRoute("Delhi", "Jaipur", 280);          // NH-48
    network.addRoute("Jaipur", "Ahmedabad", 660);      // NH-48
    network.addRoute("Ahmedabad", "Mumbai", 530);       // NH-48
    network.addRoute("Mumbai", "Pune", 150);             // Mumbai-Pune Expressway
    network.addRoute("Pune", "Bangalore", 840);          // NH-48
    network.addRoute("Bangalore", "Chennai", 350);       // NH-44
    network.addRoute("Chennai", "Kolkata", 1660);        // NH-16 (East Coast)
    network.addRoute("Delhi", "Lucknow", 550);           // NH-2
    network.addRoute("Lucknow", "Kolkata", 990);         // NH-2
    network.addRoute("Hyderabad", "Bangalore", 570);     // NH-44
    network.addRoute("Mumbai", "Hyderabad", 710);        // NH-65
    network.addRoute("Delhi", "Nagpur", 1100);           // NH-44
    network.addRoute("Nagpur", "Hyderabad", 500);        // NH-44
    network.addRoute("Delhi", "Bhopal", 780);            // NH-46
    network.addRoute("Bhopal", "Mumbai", 780);           // NH-3
    network.addRoute("Nagpur", "Pune", 720);             // NH-53

    // Direct but EXPENSIVE routes (key for Dijkstra vs BFS comparison)
    // These are longer than the sum of indirect routes, so:
    //   - BFS picks these (fewest stops/edges)
    //   - Dijkstra avoids these (finds shorter total distance via multiple hops)
    network.addRoute("Delhi", "Bangalore", 2700);        // Direct but expensive
    network.addRoute("Delhi", "Mumbai", 1500);           // Direct NH-48
    network.addRoute("Delhi", "Chennai", 2800);          // Direct but expensive
    network.addRoute("Kolkata", "Bangalore", 2500);      // Direct but expensive

    // --- Add fares to HashMap ---
    fareMap.insert("Delhi->Jaipur", 450);
    fareMap.insert("Jaipur->Ahmedabad", 800);
    fareMap.insert("Ahmedabad->Mumbai", 650);
    fareMap.insert("Mumbai->Pune", 200);
    fareMap.insert("Pune->Bangalore", 1100);
    fareMap.insert("Bangalore->Chennai", 500);
    fareMap.insert("Chennai->Kolkata", 2000);
    fareMap.insert("Delhi->Lucknow", 700);
    fareMap.insert("Lucknow->Kolkata", 1200);
    fareMap.insert("Hyderabad->Bangalore", 750);
    fareMap.insert("Mumbai->Hyderabad", 900);
    fareMap.insert("Delhi->Bangalore", 3500);
    fareMap.insert("Delhi->Mumbai", 1800);
    fareMap.insert("Delhi->Nagpur", 1500);
    fareMap.insert("Nagpur->Hyderabad", 600);
    fareMap.insert("Delhi->Bhopal", 1000);
    fareMap.insert("Bhopal->Mumbai", 1000);
    fareMap.insert("Nagpur->Pune", 950);
    fareMap.insert("Delhi->Chennai", 3600);
    fareMap.insert("Kolkata->Bangalore", 3000);

    cout << GREEN << "  ✓ Sample network loaded successfully!" << RESET << endl;
    cout << "    • " << network.getCityCount() << " cities added" << endl;
    cout << "    • " << network.getRouteCount() << " routes added" << endl;
    cout << "    • " << fareMap.getSize() << " fares configured" << endl;
    cout << "    • All city names indexed in Trie" << endl;
}

// ============================================================
// MAIN FUNCTION — Entry Point
// ============================================================
int main() {
    // --- Initialize all data structures ---
    TransportNetwork network;                // Weighted Graph (Adjacency List)
    Trie trie;                               // Prefix Tree for city search
    Stack<string> recentSearches(5);         // Stack for last 5 searches
    HashMap<int> fareMap;                    // Hash Map for fare lookup

    // --- Create pathfinding objects (Polymorphism) ---
    // Both are RouteFinder pointers — the actual algorithm depends
    // on the concrete class (runtime polymorphism via vtable)
    RouteFinder* dijkstra = new DijkstraFinder();
    RouteFinder* bfs = new BFSFinder();

    // --- Store Location objects for OOP demo ---
    vector<Location*> locations;

    int choice;
    string city1, city2, prefix;
    int distance, fare;
    bool networkLoaded = false;

    // Display title banner
    displayBanner();

    // Auto-load sample network by default for a better demo experience
    loadSampleNetwork(network, trie, fareMap, locations);
    networkLoaded = true;

    // ============================================================
    // Main Menu Loop
    // ============================================================
    do {
        displayMenu();
        if (!(cin >> choice)) {
            clearInput(); // Clear error flags and ignore bad input
            choice = -1;  // Set invalid choice to trigger default bad-input case
        }

        switch (choice) {

        // ════════════════════════════════════════════════
        // CASE 1: Add City
        // ════════════════════════════════════════════════
        case 1: {
            cout << CYAN << "\n  ━━━ ADD CITY ━━━" << RESET << endl;
            cout << "  Enter city name: ";
            cin >> city1;

            if (network.hasCity(city1)) {
                cout << YELLOW << "  ⚠ City '" << city1 << "' already exists!" << RESET << endl;
            } else {
                network.addCity(city1);
                trie.insert(city1);

                // Also create a City object for OOP demo
                float lat, lon;
                int pop;
                char metro;
                cout << "  Enter latitude: "; cin >> lat;
                cout << "  Enter longitude: "; cin >> lon;
                cout << "  Enter population: "; cin >> pop;
                cout << "  Is it a metro city? (y/n): "; cin >> metro;
                locations.push_back(new City(city1, lat, lon, pop, (metro == 'y' || metro == 'Y')));

                cout << GREEN << "  ✓ City '" << city1 << "' added successfully!" << RESET << endl;
            }
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 2: Add Route
        // ════════════════════════════════════════════════
        case 2: {
            cout << CYAN << "\n  ━━━ ADD ROUTE ━━━" << RESET << endl;
            cout << "  Enter source city: "; cin >> city1;
            cout << "  Enter destination city: "; cin >> city2;
            cout << "  Enter distance (km): "; cin >> distance;

            if (distance <= 0) {
                cout << RED << "  ✗ Distance must be positive!" << RESET << endl;
            } else {
                network.addRoute(city1, city2, distance);
                // Auto-add to trie if new
                trie.insert(city1);
                trie.insert(city2);
                cout << GREEN << "  ✓ Route added: " << city1 << " ↔ " << city2
                     << " (" << distance << " km)" << RESET << endl;
            }
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 3: Remove City
        // ════════════════════════════════════════════════
        case 3: {
            cout << CYAN << "\n  ━━━ REMOVE CITY ━━━" << RESET << endl;
            cout << "  Enter city name: "; cin >> city1;

            if (!network.hasCity(city1)) {
                cout << RED << "  ✗ City '" << city1 << "' not found!" << RESET << endl;
            } else {
                network.removeCity(city1);
                trie.remove(city1);
                cout << GREEN << "  ✓ City '" << city1 << "' removed (along with all its routes)." << RESET << endl;
            }
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 4: Remove Route
        // ════════════════════════════════════════════════
        case 4: {
            cout << CYAN << "\n  ━━━ REMOVE ROUTE ━━━" << RESET << endl;
            cout << "  Enter source city: "; cin >> city1;
            cout << "  Enter destination city: "; cin >> city2;

            if (!network.hasRoute(city1, city2)) {
                cout << RED << "  ✗ No route exists between '" << city1 << "' and '" << city2 << "'!" << RESET << endl;
            } else {
                network.removeRoute(city1, city2);
                cout << GREEN << "  ✓ Route removed: " << city1 << " ↔ " << city2 << RESET << endl;
            }
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 5: Block Road (Dynamic Rerouting)
        // ════════════════════════════════════════════════
        case 5: {
            cout << CYAN << "\n  ━━━ BLOCK ROAD ━━━" << RESET << endl;
            cout << "  Enter source city: "; cin >> city1;
            cout << "  Enter destination city: "; cin >> city2;

            if (!network.hasRoute(city1, city2)) {
                cout << RED << "  ✗ No route exists between '" << city1 << "' and '" << city2 << "'!" << RESET << endl;
            } else if (network.isRouteBlocked(city1, city2)) {
                cout << YELLOW << "  ⚠ Route already blocked!" << RESET << endl;
            } else {
                network.blockRoute(city1, city2);
                cout << RED << "  🚧 Road BLOCKED: " << city1 << " ↔ " << city2 << RESET << endl;
                cout << DIM << "     Pathfinding algorithms will now reroute around this road." << RESET << endl;
            }
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 6: Unblock Road
        // ════════════════════════════════════════════════
        case 6: {
            cout << CYAN << "\n  ━━━ UNBLOCK ROAD ━━━" << RESET << endl;
            cout << "  Enter source city: "; cin >> city1;
            cout << "  Enter destination city: "; cin >> city2;

            if (!network.isRouteBlocked(city1, city2)) {
                cout << YELLOW << "  ⚠ Route is not currently blocked!" << RESET << endl;
            } else {
                network.unblockRoute(city1, city2);
                cout << GREEN << "  ✓ Road UNBLOCKED: " << city1 << " ↔ " << city2 << RESET << endl;
            }
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 7: Shortest Path (Dijkstra)
        // ════════════════════════════════════════════════
        case 7: {
            cout << CYAN << "\n  ━━━ SHORTEST PATH (DIJKSTRA) ━━━" << RESET << endl;
            cout << "  Enter source city: "; cin >> city1;
            cout << "  Enter destination city: "; cin >> city2;

            if (!network.hasCity(city1)) {
                cout << RED << "  ✗ Source city '" << city1 << "' not found!" << RESET << endl;
                break;
            }
            if (!network.hasCity(city2)) {
                cout << RED << "  ✗ Destination city '" << city2 << "' not found!" << RESET << endl;
                break;
            }

            // Use polymorphism: dijkstra is a RouteFinder*
            // but at runtime, DijkstraFinder::findPath() is called
            vector<string> path = dijkstra->findPath(network, city1, city2);
            displayPath(path, network, dijkstra->getAlgorithmName());

            // Push to search history stack
            string searchEntry = city1 + " -> " + city2 + " [Dijkstra]";
            recentSearches.push(searchEntry);
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 8: Minimum Stops Path (BFS)
        // ════════════════════════════════════════════════
        case 8: {
            cout << CYAN << "\n  ━━━ MINIMUM STOPS PATH (BFS) ━━━" << RESET << endl;
            cout << "  Enter source city: "; cin >> city1;
            cout << "  Enter destination city: "; cin >> city2;

            if (!network.hasCity(city1)) {
                cout << RED << "  ✗ Source city '" << city1 << "' not found!" << RESET << endl;
                break;
            }
            if (!network.hasCity(city2)) {
                cout << RED << "  ✗ Destination city '" << city2 << "' not found!" << RESET << endl;
                break;
            }

            // Polymorphism: bfs is a RouteFinder* but BFSFinder::findPath() runs
            vector<string> path = bfs->findPath(network, city1, city2);
            displayPath(path, network, bfs->getAlgorithmName());

            // Push to history
            string searchEntry = city1 + " -> " + city2 + " [BFS]";
            recentSearches.push(searchEntry);
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 9: Compare Dijkstra vs BFS
        // ════════════════════════════════════════════════
        case 9: {
            cout << CYAN << "\n  ━━━ COMPARE: DIJKSTRA vs BFS ━━━" << RESET << endl;
            cout << "  Enter source city: "; cin >> city1;
            cout << "  Enter destination city: "; cin >> city2;

            if (!network.hasCity(city1) || !network.hasCity(city2)) {
                cout << RED << "  ✗ One or both cities not found!" << RESET << endl;
                break;
            }

            // Run BOTH algorithms
            vector<string> dijkstraPath = dijkstra->findPath(network, city1, city2);
            vector<string> bfsPath = bfs->findPath(network, city1, city2);

            // Display Dijkstra result
            cout << MAGENTA << "\n  ╔══════════════════════════════════════════╗" << endl;
            cout << "  ║   ALGORITHM COMPARISON: " << city1 << " → " << city2 << endl;
            cout << "  ╚══════════════════════════════════════════╝" << RESET << endl;

            cout << BLUE << "\n  ▶ " << dijkstra->getAlgorithmName() << ":" << RESET << endl;
            if (dijkstraPath.empty()) {
                cout << RED << "     No path found." << RESET << endl;
            } else {
                cout << "     Path: ";
                for (size_t i = 0; i < dijkstraPath.size(); i++) {
                    cout << dijkstraPath[i];
                    if (i < dijkstraPath.size() - 1) cout << " → ";
                }
                cout << endl;
                int dCost = network.calculatePathCost(dijkstraPath);
                cout << "     Distance : " << GREEN << dCost << " km" << RESET << endl;
                cout << "     Stops    : " << (dijkstraPath.size() - 1) << endl;
            }

            cout << BLUE << "\n  ▶ " << bfs->getAlgorithmName() << ":" << RESET << endl;
            if (bfsPath.empty()) {
                cout << RED << "     No path found." << RESET << endl;
            } else {
                cout << "     Path: ";
                for (size_t i = 0; i < bfsPath.size(); i++) {
                    cout << bfsPath[i];
                    if (i < bfsPath.size() - 1) cout << " → ";
                }
                cout << endl;
                int bCost = network.calculatePathCost(bfsPath);
                cout << "     Distance : " << GREEN << bCost << " km" << RESET << endl;
                cout << "     Stops    : " << (bfsPath.size() - 1) << endl;
            }

            // Comparison summary
            if (!dijkstraPath.empty() && !bfsPath.empty()) {
                int dCost = network.calculatePathCost(dijkstraPath);
                int bCost = network.calculatePathCost(bfsPath);
                int dStops = dijkstraPath.size() - 1;
                int bStops = bfsPath.size() - 1;

                cout << YELLOW << "\n  ━━━ VERDICT ━━━" << RESET << endl;
                if (dCost < bCost) {
                    cout << GREEN << "  ✓ Dijkstra saves " << (bCost - dCost)
                         << " km in distance!" << RESET << endl;
                }
                if (bStops < dStops) {
                    cout << GREEN << "  ✓ BFS saves " << (dStops - bStops)
                         << " stop(s)!" << RESET << endl;
                }
                if (dCost == bCost && dStops == bStops) {
                    cout << GREEN << "  ✓ Both algorithms found the same optimal path!" << RESET << endl;
                }
            }

            // Push to history
            string searchEntry = city1 + " -> " + city2 + " [Compare]";
            recentSearches.push(searchEntry);
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 10: Search City by Prefix (Trie)
        // ════════════════════════════════════════════════
        case 10: {
            cout << CYAN << "\n  ━━━ SEARCH CITY BY PREFIX (TRIE) ━━━" << RESET << endl;
            cout << "  Enter prefix: "; cin >> prefix;

            vector<string> results = trie.searchByPrefix(prefix);
            if (results.empty()) {
                cout << YELLOW << "  No cities found with prefix '" << prefix << "'" << RESET << endl;
            } else {
                cout << GREEN << "  Found " << results.size() << " match(es):" << RESET << endl;
                for (const string& r : results) {
                    cout << "    • " << r << endl;
                }
            }
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 11: View Recent Searches (Stack)
        // ════════════════════════════════════════════════
        case 11: {
            cout << CYAN << "\n  ━━━ RECENT SEARCHES (STACK) ━━━" << RESET << endl;
            recentSearches.display();
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 12: Add/Update Fare (HashMap)
        // ════════════════════════════════════════════════
        case 12: {
            cout << CYAN << "\n  ━━━ ADD/UPDATE FARE (HASHMAP) ━━━" << RESET << endl;
            cout << "  Enter source city: "; cin >> city1;
            cout << "  Enter destination city: "; cin >> city2;
            cout << "  Enter fare (Rs.): "; cin >> fare;

            // Create consistent key (alphabetical)
            string key = (city1 < city2) ? city1 + "->" + city2 : city2 + "->" + city1;
            fareMap.insert(key, fare);
            cout << GREEN << "  ✓ Fare updated: " << key << " = Rs." << fare << RESET << endl;
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 13: Lookup Fare (HashMap)
        // ════════════════════════════════════════════════
        case 13: {
            cout << CYAN << "\n  ━━━ LOOKUP FARE (HASHMAP) ━━━" << RESET << endl;
            cout << "  Enter source city: "; cin >> city1;
            cout << "  Enter destination city: "; cin >> city2;

            string key = (city1 < city2) ? city1 + "->" + city2 : city2 + "->" + city1;
            int foundFare;
            if (fareMap.get(key, foundFare)) {
                cout << GREEN << "  ✓ Fare for " << city1 << " ↔ " << city2
                     << " : Rs." << foundFare << RESET << endl;
            } else {
                cout << YELLOW << "  ⚠ No fare configured for this route." << RESET << endl;
            }
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 14: Display All Fares
        // ════════════════════════════════════════════════
        case 14: {
            cout << CYAN << "\n  ━━━ ALL FARES (HASHMAP) ━━━" << RESET << endl;
            fareMap.display();
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 15: Display Network Map
        // ════════════════════════════════════════════════
        case 15: {
            cout << CYAN << "\n  ━━━ NETWORK MAP ━━━" << RESET << endl;
            network.displayNetwork();
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 16: Display Blocked Routes
        // ════════════════════════════════════════════════
        case 16: {
            cout << CYAN << "\n  ━━━ BLOCKED ROUTES ━━━" << RESET << endl;
            network.displayBlockedRoutes();
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 17: Display All Cities in Trie
        // ════════════════════════════════════════════════
        case 17: {
            cout << CYAN << "\n  ━━━ ALL CITIES IN TRIE ━━━" << RESET << endl;
            trie.displayAll();
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 18: OOP Demo — Polymorphism in Action
        // ════════════════════════════════════════════════
        case 18: {
            cout << MAGENTA << "\n  ━━━ OOP DEMO: POLYMORPHISM IN ACTION ━━━" << RESET << endl;

            if (locations.empty()) {
                cout << YELLOW << "  ⚠ No locations loaded. Use option 19 to load sample network first." << RESET << endl;
                break;
            }

            cout << "\n  Demonstrating Runtime Polymorphism:" << endl;
            cout << "  Each object is called through a Location* pointer," << endl;
            cout << "  but the CORRECT derived class display() runs.\n" << endl;

            for (Location* loc : locations) {
                // This is RUNTIME POLYMORPHISM:
                // Even though 'loc' is a Location* pointer, the
                // correct derived class display() is called via vtable
                cout << "    Type: " << CYAN << loc->getType() << RESET << " → ";
                loc->display();
            }

            // Also demo RouteFinder polymorphism
            cout << "\n  RouteFinder Polymorphism:" << endl;
            cout << "    dijkstra->getAlgorithmName() = "
                 << CYAN << dijkstra->getAlgorithmName() << RESET << endl;
            cout << "    bfs->getAlgorithmName()      = "
                 << CYAN << bfs->getAlgorithmName() << RESET << endl;
            cout << DIM << "\n  Both are RouteFinder* pointers with different behavior!" << RESET << endl;
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 19: Load Sample Network
        // ════════════════════════════════════════════════
        case 19: {
            if (networkLoaded) {
                cout << YELLOW << "  ⚠ Sample network is already loaded!" << RESET << endl;
            } else {
                loadSampleNetwork(network, trie, fareMap, locations);
                networkLoaded = true;
            }
            break;
        }

        // ════════════════════════════════════════════════
        // CASE 0: Exit
        // ════════════════════════════════════════════════
        case 0: {
            cout << CYAN << BOLD;
            cout << "\n  ╔══════════════════════════════════════════════╗" << endl;
            cout << "  ║     Thank you for using Smart Transport!     ║" << endl;
            cout << "  ║           Have a safe journey! 🚂            ║" << endl;
            cout << "  ╚══════════════════════════════════════════════╝" << endl;
            cout << RESET << endl;
            break;
        }

        default:
            cout << RED << "  ✗ Invalid choice! Please try again." << RESET << endl;
        }

    } while (choice != 0);

    // ============================================================
    // Cleanup — Free dynamically allocated memory
    // ============================================================
    delete dijkstra;
    delete bfs;
    for (Location* loc : locations) {
        delete loc;
    }

    return 0;
}
