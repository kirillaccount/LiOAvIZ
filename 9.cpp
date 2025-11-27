#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <random>
#include <iomanip>
#include <limits>
#include <chrono>
using namespace std;
using namespace std::chrono;
class Graph {
private:
    vector<vector<int>> adjacencyMatrix;
    vector<vector<int>> adjacencyList;
    int vertices;

    void buildAdjacencyListFromMatrix() {
        adjacencyList.clear();
        adjacencyList.resize(vertices);
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (adjacencyMatrix[i][j] == 1) {
                    adjacencyList[i].push_back(j);
                }
            }
        }
    }
public:
    Graph(int n) : vertices(n) {
        generateRandomGraph();
    }
    void generateRandomGraph() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);
        adjacencyMatrix.resize(vertices, vector<int>(vertices, 0));
        for (int i = 0; i < vertices; i++) {
            for (int j = i + 1; j < vertices; j++) {
                int edge = dis(gen);
                adjacencyMatrix[i][j] = edge;
                adjacencyMatrix[j][i] = edge;
            }
        }
        buildAdjacencyListFromMatrix();
    }
    void printAdjacencyMatrix() {
        cout << "\nМатрица смежности:\n";
        cout << "   ";
        for (int i = 0; i < vertices; i++) {
            cout << setw(3) << i;
        }
        cout << "\n";
        for (int i = 0; i < vertices; i++) {
            cout << setw(3) << i;
            for (int j = 0; j < vertices; j++) {
                cout << setw(3) << adjacencyMatrix[i][j];
            }
            cout << "\n";
        }
    }
    void printAdjacencyList() {
        cout << "\nСписки смежности:\n";
        for (int i = 0; i < vertices; i++) {
            cout << i << ": ";
            for (int neighbor : adjacencyList[i]) {
                cout << neighbor << " ";
            }
            cout << "\n";
        }
    }
    vector<int> bfsDistanceMatrix(int startVertex) {
        auto start = high_resolution_clock::now();
        vector<int> dist(vertices, -1);
        queue<int> q;
        dist[startVertex] = 0;
        q.push(startVertex);
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            for (int i = 0; i < vertices; i++) {
                if (adjacencyMatrix[current][i] == 1 && dist[i] == -1) {
                    dist[i] = dist[current] + 1;
                    q.push(i);
                }
            }
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "Время выполнения BFS (матрица): " << duration.count() << " мкс\n";
        return dist;
    }
    vector<int> bfsDistanceList(int startVertex) {
        auto start = high_resolution_clock::now();
        vector<int> dist(vertices, -1);
        queue<int> q;
        dist[startVertex] = 0;
        q.push(startVertex);
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            for (int neighbor : adjacencyList[current]) {
                if (dist[neighbor] == -1) {
                    dist[neighbor] = dist[current] + 1;
                    q.push(neighbor);
                }
            }
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "Время выполнения BFS (списки): " << duration.count() << " мкс\n";
        return dist;
    }
    vector<int> dfsDistanceMatrix(int startVertex) {
        auto start = high_resolution_clock::now();
        vector<int> dist(vertices, -1);
        stack<int> s;
        dist[startVertex] = 0;
        s.push(startVertex);
        while (!s.empty()) {
            int current = s.top();
            s.pop();
            for (int i = 0; i < vertices; i++) {
                if (adjacencyMatrix[current][i] == 1 && dist[i] == -1) {
                    dist[i] = dist[current] + 1;
                    s.push(i);
                }
            }
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "Время выполнения DFS (матрица): " << duration.count() << " мкс\n";
        return dist;
    }

    vector<int> dfsDistanceList(int startVertex) {
        auto start = high_resolution_clock::now();
        vector<int> dist(vertices, -1);
        stack<int> s;
        dist[startVertex] = 0;
        s.push(startVertex);
        while (!s.empty()) {
            int current = s.top();
            s.pop();
            for (int neighbor : adjacencyList[current]) {
                if (dist[neighbor] == -1) {
                    dist[neighbor] = dist[current] + 1;
                    s.push(neighbor);
                }
            }
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "Время выполнения DFS (списки): " << duration.count() << " мкс\n";
        return dist;
    }
    void printDistances(const vector<int>& dist, int startVertex) {
        cout << "\nРасстояния от вершины " << startVertex << ":\n";
        for (int i = 0; i < vertices; i++) {
            cout << "Вершина " << i << ": ";
            if (dist[i] == -1) {
                cout << "недостижима";
            }
            else {
                cout << dist[i];
            }
            cout << "\n";
        }
    }
};
int main() {
    setlocale(LC_ALL, "Russian");
    int vertices;
    cout << "Введите количество вершин графа: ";
    cin >> vertices;
    if (vertices <= 0) {
        cout << "Неверное количество вершин! Установлено значение по умолчанию: 5" << endl;
        vertices = 5;
    }
    Graph graph(vertices);
    int choice;
    do
    {
        cout << "\n1. Установить количество вершин";
        cout << "\n2. Показать матрицу смежности";
        cout << "\n3. Показать списки смежности";
        cout << "\n4. BFS - расстояния (матрица смежности)";
        cout << "\n5. BFS - расстояния (списки смежности)";
        cout << "\n6. DFS - расстояния (матрица смежности)";
        cout << "\n7. DFS - расстояния (списки смежности)";
        cout << "\n0. Выход";
        cout << "\nВыберите операцию: ";
        cin >> choice;
        switch (choice) {
        case 1: {
            cout << "Введите количество вершин: ";
            cin >> vertices;
            if (vertices > 0) {
                graph = Graph(vertices);
            }
            else {
                cout << "Неверное количество вершин!\n";
            }
            break;
        }
        case 2: {
            graph.printAdjacencyMatrix();
            break;
        }
        case 3: {
            graph.printAdjacencyList();
            break;
        }
        case 4: {
            int startVertex;
            cout << "Введите начальную вершину: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < vertices) {
                vector<int> dist = graph.bfsDistanceMatrix(startVertex);
                graph.printDistances(dist, startVertex);
            }
            else {
                cout << "Неверная вершина!\n";
            }
            break;
        }
        case 5: {
            int startVertex;
            cout << "Введите начальную вершину: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < vertices) {
                vector<int> dist = graph.bfsDistanceList(startVertex);
                graph.printDistances(dist, startVertex);
            }
            else {
                cout << "Неверная вершина!\n";
            }
            break;
        }
        case 6: {
            int startVertex;
            cout << "Введите начальную вершину: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < vertices) {
                vector<int> dist = graph.dfsDistanceMatrix(startVertex);
                graph.printDistances(dist, startVertex);
            }
            else {
                cout << "Неверная вершина!\n";
            }
            break;
        }
        case 7: {
            int startVertex;
            cout << "Введите начальную вершину: ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < vertices) {
                vector<int> dist = graph.dfsDistanceList(startVertex);
                graph.printDistances(dist, startVertex);
            }
            else {
                cout << "Неверная вершина!\n";
            }
            break;
        }
        case 0: {
            break;
        }
        default: {
            cout << "Неверный выбор!\n";
            break;
        }
        }
    } while (choice != 0);
    return 0;
}

