    
/*Name: Tshepang Malakalaka
Student Number: 24612391
Module: COS2611
Assignment: Assessment 3
*/
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

class Graph
{
private:
    vector<string> parks;

    /*
     * The adjacency list represents the graph.
     *
     * Each index in adjacencyList represents one conservation area.
     * Each pair<int, int> stores:
     *   - first: the index of a neighbouring conservation area
     *   - second: the distance to that area in kilometres
     *
     * For example:
     * adjacencyList[0] = { {1, 120} }
     *
     * means that park 0 is connected to park 1 by a 120 km corridor.
     *
     * The graph is undirected, so each corridor is stored in both
     * directions.
     */
    vector<vector<pair<int, int>>> adjacencyList;

public:

    /*
     * Constructor
     *
     * Creates an empty adjacency list with one position for
     * each conservation area.
     */
    Graph(int numberOfParks)
    {
        adjacencyList.resize(numberOfParks);
    }

    /*
     * Adds a conservation area to the parks vector.
     *
     * The position of the park in the vector becomes its index.
     * For example:
     *
     * index 0 = Kruger National Park
     * index 1 = Limpopo National Park
     */
    void addPark(string parkName)
    {
        parks.push_back(parkName);
    }

    /*
     * Displays the numbered list of conservation areas.
     *
     * This allows the user to select a park by entering its index.
     */
    void displayParks()
    {
        cout << "\nConservation Areas:\n";

        for (int i = 0; i < parks.size(); i++)
        {
            cout << i << ". " << parks[i] << endl;
        }
    }

    /*
     * Adds a weighted corridor between two conservation areas.
     *
     * source and destination are the indexes of the two parks.
     * distance stores the corridor distance in kilometres.
     *
     * Because the graph is undirected, the corridor is added in
     * both directions.
     */
    void addCorridor(int source, int destination, int distance)
    {
        adjacencyList[source].push_back({destination, distance});
        adjacencyList[destination].push_back({source, distance});
    }

    /*
     * Displays the graph using its adjacency list.
     *
     * Each conservation area is displayed together with all its
     * neighbouring areas and the distance to each neighbour.
     */
    void displayNetwork()
    {
        cout << "\nSouthern Africa Wildlife Corridor Network\n";
        cout << "------------------------------------------\n";

        for (int i = 0; i < parks.size(); i++)
        {
            cout << parks[i] << ":\n";

            for (pair<int, int> neighbour : adjacencyList[i])
            {
                int neighbourIndex = neighbour.first;
                int distance = neighbour.second;

                cout << "  -> " << parks[neighbourIndex]
                     << " (" << distance << " km)" << endl;
            }

            cout << endl;
        }
    }

    /*
     * Generates and displays the adjacency matrix.
     *
     * The rows and columns represent conservation areas.
     * A value of 0 means there is no direct corridor between
     * two areas.
     *
     * A non-zero value represents the distance of the corridor
     * in kilometres.
     */
    void displayAdjacencyMatrix()
    {
        int numberOfParks = parks.size();

        vector<vector<int>> matrix(
            numberOfParks,
            vector<int>(numberOfParks, 0)
        );

        /*
         * Convert the adjacency list into a matrix.
         *
         * For every neighbour stored in the adjacency list,
         * the corresponding matrix position receives the distance.
         */
        for (int i = 0; i < numberOfParks; i++)
        {
            for (pair<int, int> neighbour : adjacencyList[i])
            {
                int neighbourIndex = neighbour.first;
                int distance = neighbour.second;

                matrix[i][neighbourIndex] = distance;
            }
        }

        cout << "\nAdjacency Matrix (Distances in km)\n";
        cout << "----------------------------------\n\n";

        cout << "\t";

        for (int i = 0; i < numberOfParks; i++)
        {
            cout << i << "\t";
        }

        cout << endl;

        for (int i = 0; i < numberOfParks; i++)
        {
            cout << i << "\t";

            for (int j = 0; j < numberOfParks; j++)
            {
                cout << matrix[i][j] << "\t";
            }

            cout << endl;
        }

        cout << "\nKey:\n";

        for (int i = 0; i < numberOfParks; i++)
        {
            cout << i << " = " << parks[i] << endl;
        }
    }

    /*
     * Breadth-First Search (BFS)
     *
     * BFS explores a graph level by level.
     *
     * A queue is used to store conservation areas that have
     * been discovered but have not yet been fully explored.
     *
     * The visited vector prevents the same conservation area
     * from being visited repeatedly.
     */
    void BFS(int start)
    {
        vector<bool> visited(parks.size(), false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "\nBFS Traversal starting from "
             << parks[start] << ":\n";

        bool first = true;

        while (!q.empty())
        {
            int current = q.front();
            q.pop();

            if (!first)
            {
                cout << " -> ";
            }

            cout << parks[current];
            first = false;

            /*
             * Examine every neighbour of the current park.
             * If a neighbour has not been visited, mark it as
             * visited and add it to the queue.
             */
            for (pair<int, int> neighbour : adjacencyList[current])
            {
                int neighbourIndex = neighbour.first;

                if (!visited[neighbourIndex])
                {
                    visited[neighbourIndex] = true;
                    q.push(neighbourIndex);
                }
            }
        }

        cout << endl;
    }

    /*
     * Dijkstra's Algorithm
     *
     * Dijkstra calculates the shortest distance from a starting
     * conservation area to all other areas.
     *
     * The distance vector stores the shortest known distance
     * to each park.
     *
     * The previous vector stores the park that came immediately
     * before each park on the shortest route. This allows the
     * final route to be reconstructed.
     */
    void dijkstra(int start, int destination)
    {
        int numberOfParks = parks.size();

        const int INF = numeric_limits<int>::max();

        vector<int> distance(numberOfParks, INF);
        vector<int> previous(numberOfParks, -1);

        /*
         * The priority queue stores:
         *
         * pair<distance, park index>
         *
         * greater<pair<int, int>> makes the smallest distance
         * appear at the top of the priority queue.
         */
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        distance[start] = 0;
        pq.push({0, start});

        while (!pq.empty())
        {
            int currentDistance = pq.top().first;
            int current = pq.top().second;

            pq.pop();

            /*
             * If this queue entry is not the most recent shortest
             * distance, ignore it.
             */
            if (currentDistance > distance[current])
            {
                continue;
            }

            /*
             * Examine every neighbouring conservation area.
             */
            for (pair<int, int> neighbour : adjacencyList[current])
            {
                int next = neighbour.first;
                int edgeDistance = neighbour.second;

                int newDistance =
                    distance[current] + edgeDistance;

                /*
                 * Relaxation:
                 *
                 * If travelling through the current park produces
                 * a shorter route to the neighbouring park, update
                 * the shortest distance and remember the current
                 * park as the previous park.
                 */
                if (newDistance < distance[next])
                {
                    distance[next] = newDistance;
                    previous[next] = current;

                    pq.push({distance[next], next});
                }
            }
        }

        if (distance[destination] == INF)
        {
            cout << "\nNo route exists between "
                 << parks[start] << " and "
                 << parks[destination] << ".\n";

            return;
        }

        /*
         * Reconstruct the shortest route by following the previous
         * park from the destination back to the starting park.
         */
        vector<int> path;

        int current = destination;

        while (current != -1)
        {
            path.push_back(current);
            current = previous[current];
        }

        reverse(path.begin(), path.end());

        cout << "\nShortest Path from "
             << parks[start] << " to "
             << parks[destination] << ":\n\n";

        for (int i = 0; i < path.size(); i++)
        {
            cout << parks[path[i]];

            if (i < path.size() - 1)
            {
                cout << " -> ";
            }
        }

        cout << "\n\nTotal distance: "
             << distance[destination] << " km\n";
    }

    /*
     * Returns the number of conservation areas in the graph.
     */
    int getNumberOfParks()
    {
        return parks.size();
    }
};


int main()
{
    Graph network(6);

    /*
     * Add the six conservation areas required by the project.
     */
    network.addPark("Kruger National Park");
    network.addPark("Limpopo National Park");
    network.addPark("Chobe National Park");
    network.addPark("Hwange National Park");
    network.addPark("Etosha National Park");
    network.addPark("Kgalagadi Transfrontier Park");

    /*
     * Add weighted, undirected wildlife corridors.
     */
    network.addCorridor(0, 1, 120);
    network.addCorridor(1, 2, 290);
    network.addCorridor(2, 3, 90);
    network.addCorridor(4, 5, 850);

    int choice;

    do
    {
        cout << "\n\n";
        cout << "Southern Africa Wildlife Corridor Network\n";
        cout << "------------------------------------------\n";
        cout << "1. Display conservation network\n";
        cout << "2. Display adjacency matrix\n";
        cout << "3. Perform BFS traversal\n";
        cout << "4. Find shortest path between parks\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                network.displayNetwork();
                break;
            }

            case 2:
            {
                network.displayAdjacencyMatrix();
                break;
            }

            case 3:
            {
                int start;

                network.displayParks();

                cout << "\nEnter the number of the starting park: ";
                cin >> start;

                if (start >= 0 &&
                    start < network.getNumberOfParks())
                {
                    network.BFS(start);
                }
                else
                {
                    cout << "Invalid park selection.\n";
                }

                break;
            }

            case 4:
            {
                int start;
                int destination;

                network.displayParks();

                cout << "\nEnter the number of the starting park: ";
                cin >> start;

                cout << "Enter the number of the destination park: ";
                cin >> destination;

                if (start >= 0 &&
                    start < network.getNumberOfParks() &&
                    destination >= 0 &&
                    destination < network.getNumberOfParks())
                {
                    network.dijkstra(start, destination);
                }
                else
                {
                    cout << "Invalid park selection.\n";
                }

                break;
            }

            case 5:
            {
                cout << "\nExiting program...\n";
                break;
            }

            default:
            {
                cout << "\nInvalid choice. Please try again.\n";
            }
        }

    } while (choice != 5);

    return 0;
}


/*
========================================================
GENAI PROMPTS USED
========================================================

1. "Explain the requirements of this wildlife conservation
   corridor graph project and help me break it into manageable
   parts."

2. "What is the significance of BFS and Dijkstra's algorithm
   in a weighted graph?"

3. "Help me understand how to represent an undirected weighted
   graph using an adjacency list in C++."

4. "Help me implement a graph using an adjacency list where
   each edge stores a neighbouring conservation area and a
   distance in kilometres."

5. "Explain how to generate and display an adjacency matrix
   from a weighted adjacency list."

6. "Explain BFS step by step and how a queue and visited array
   are used."

7. "Explain Dijkstra's shortest path algorithm and how to
   reconstruct the shortest route using a previous array."

8. "Help me create a menu-driven C++ program that displays a
   graph, adjacency matrix, performs BFS, and finds shortest
   paths using Dijkstra's algorithm."

9. "Review and explain my C++ graph code and identify errors
   in my adjacency list structure."

========================================================
REFLECTION
========================================================

Generative AI helped me understand how to structure a weighted,
undirected graph using an adjacency list. It was particularly
useful for explaining how the adjacency list stores neighbouring
parks and corridor distances. The explanations of BFS and
Dijkstra's algorithm also helped me understand how queues,
priority queues, distances, and previous nodes are used.

One difficulty was that the initial code structure contained
errors, such as using the wrong type for the adjacency list and
calling functions that had not yet been defined. I corrected
these issues by changing the adjacency list to a vector of
vectors containing pairs and by adding the required graph
functions. I also adapted the program to include an adjacency
matrix, a menu system, input validation, and shortest-path
reconstruction. I tested and reviewed the generated code so that
I could understand the purpose of each data structure and
algorithm rather than simply copying the solution.
*/
