#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <string>
#include <cstring>
using namespace std;

class WeightedGraph {
private:
    vector<vector<int>> adjacencyMatrix;
    int vertices;
    bool isDirected;
    bool isWeighted;
    
    vector<int> bfsDistances(int startVertex) {
        if (startVertex < 0 || startVertex >= vertices) {
            return vector<int>(vertices, -1);
        }
        
        vector<int> dist(vertices, -1);
        queue<int> q;
        dist[startVertex] = 0;
        q.push(startVertex);
        
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            
            for (int i = 0; i < vertices; i++) {
                if (adjacencyMatrix[current][i] > 0 && dist[i] == -1) {
                    if (isWeighted) {
                        dist[i] = dist[current] + adjacencyMatrix[current][i];
                    } else {
                        dist[i] = dist[current] + 1;
                    }
                    q.push(i);
                }
            }
        }
        return dist;
    }
    
public:
    WeightedGraph(int n, bool directed = false, bool weighted = true) 
        : vertices(n), isDirected(directed), isWeighted(weighted) {
        generateRandomGraph();
    }
    
    void generateRandomGraph() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> weightDis(1, 10);
        uniform_int_distribution<> edgeDis(0, 2);
        
        adjacencyMatrix.clear();
        adjacencyMatrix.resize(vertices, vector<int>(vertices, 0));
        
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (i == j) continue;
                
                if (!isDirected && i < j) {
                    if (edgeDis(gen) > 0) {
                        if (isWeighted) {
                            int weight = weightDis(gen);
                            adjacencyMatrix[i][j] = weight;
                            adjacencyMatrix[j][i] = weight;
                        } else {
                            adjacencyMatrix[i][j] = 1;
                            adjacencyMatrix[j][i] = 1;
                        }
                    }
                }
                else if (isDirected) {
                    if (edgeDis(gen) > 0) {
                        if (isWeighted) {
                            adjacencyMatrix[i][j] = weightDis(gen);
                        } else {
                            adjacencyMatrix[i][j] = 1;
                        }
                    }
                }
            }
        }
    }
    
    void printAdjacencyMatrix() {
        cout << "\nМатрица смежности " 
             << (isDirected ? "ориентированного" : "неориентированного") 
             << (isWeighted ? " взвешенного" : " невзвешенного") 
             << " графа:\n";
        cout << "   ";
        for (int i = 0; i < vertices; i++) {
            cout << setw(4) << i;
        }
        cout << "\n";
        
        for (int i = 0; i < vertices; i++) {
            cout << setw(3) << i;
            for (int j = 0; j < vertices; j++) {
                cout << setw(4) << adjacencyMatrix[i][j];
            }
            cout << "\n";
        }
    }
    
    void findDistances(int startVertex) {
        vector<int> dist = bfsDistances(startVertex);
        cout << "Расстояния от вершины " << startVertex << ":\n";
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
    
    int calculateEccentricity(int vertex) {
        if (vertex < 0 || vertex >= vertices) {
            return -1;
        }
        
        vector<int> dist = bfsDistances(vertex);
        int eccentricity = -1;
        for (int i = 0; i < vertices; i++) {
            if (dist[i] != -1 && dist[i] > eccentricity) {
                eccentricity = dist[i];
            }
        }
        return eccentricity;
    }
    
    void analyzeGraph() {
        vector<int> eccentricities(vertices);
        int radius = numeric_limits<int>::max();
        int diameter = -1;
        
        for (int i = 0; i < vertices; i++) {
            eccentricities[i] = calculateEccentricity(i);
            if (eccentricities[i] != -1) {
                radius = min(radius, eccentricities[i]);
                diameter = max(diameter, eccentricities[i]);
            }
        }
        
        vector<int> centralVertices, peripheralVertices;
        for (int i = 0; i < vertices; i++) {
            if (eccentricities[i] == radius) centralVertices.push_back(i);
            if (eccentricities[i] == diameter) peripheralVertices.push_back(i);
        }
        
        cout << "Эксцентриситеты вершин:\n";
        for (int i = 0; i < vertices; i++) {
            cout << "Вершина " << i << ": " << eccentricities[i] << "\n";
        }
        cout << "\nРадиус графа: " << radius << endl;
        cout << "Диаметр графа: " << diameter << endl;
        cout << "Центральные вершины: ";
        for (int v : centralVertices) cout << v << " ";
        cout << endl;
        cout << "Периферийные вершины: ";
        for (int v : peripheralVertices) cout << v << " ";
        cout << endl;
    }
    
    int getVerticesCount() const { return vertices; }
    bool getIsDirected() const { return isDirected; }
    bool getIsWeighted() const { return isWeighted; }
    
    void setVertices(int n) { 
        vertices = n; 
        generateRandomGraph(); 
    }
    
    void setDirected(bool directed) { 
        isDirected = directed; 
        generateRandomGraph(); 
    }
    
    void setWeighted(bool weighted) { 
        isWeighted = weighted; 
        generateRandomGraph(); 
    }
    
    void regenerateGraph() { 
        generateRandomGraph(); 
    }
};

void processCommandLine(int argc, char* argv[], int& vertices, bool& isDirected, bool& isWeighted, bool& doAnalysis, int& startVertex, bool& interactiveMode) {
    vertices = 6;
    isDirected = false;
    isWeighted = true;
    doAnalysis = false;
    startVertex = -1;
    interactiveMode = true;
    
    if (argc == 1) {
        return;
    }
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 && i + 1 < argc) {
            vertices = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-d") == 0) {
            isDirected = true;
        }
        else if (strcmp(argv[i], "-u") == 0) {
            isDirected = false;
        }
        else if (strcmp(argv[i], "-w") == 0) {
            isWeighted = true;
        }
        else if (strcmp(argv[i], "-uw") == 0) {
            isWeighted = false;
        }
        else if (strcmp(argv[i], "-a") == 0) {
            doAnalysis = true;
            interactiveMode = false;
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            startVertex = atoi(argv[i + 1]);
            interactiveMode = false;
        }
    }
    
    if (doAnalysis || startVertex != -1) {
        interactiveMode = false;
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    
    int vertices;
    bool isDirected;
    bool isWeighted;
    bool doAnalysis;
    int startVertex;
    bool interactiveMode;
    
    processCommandLine(argc, argv, vertices, isDirected, isWeighted, doAnalysis, startVertex, interactiveMode);
    
    if (vertices <= 0) {
        cout << "Неверное количество вершин! Установлено значение по умолчанию: 6" << endl;
        vertices = 6;
    }
    
    WeightedGraph graph(vertices, isDirected, isWeighted);
    
    if (!interactiveMode) {
        graph.printAdjacencyMatrix();
        if (doAnalysis) {
            graph.analyzeGraph();
        }
        if (startVertex != -1) {
            if (startVertex >= 0 && startVertex < graph.getVerticesCount()) {
                graph.findDistances(startVertex);
            }
            else {
                cout << "Неверная стартовая вершина: " << startVertex << endl;
            }
        }
        return 0;
    }
    
    int choice;
    do {
        cout << "\n=== Меню работы с графами ===";
        cout << "\n1. Неориентированный граф";
        cout << "\n2. Ориентированный граф";
        cout << "\n3. Переключить взвешенность (текущее: " 
             << (graph.getIsWeighted() ? "взвешенный" : "невзвешенный") << ")";
        cout << "\n4. Анализ графа";
        cout << "\n5. Изменить количество вершин";
        cout << "\n6. Регенерировать граф";
        cout << "\n7. Найти расстояния от вершины";
        cout << "\n0. Выход";
        cout << "\nВыберите операцию: ";
        cin >> choice;
        
        int newVertices;
        
        switch (choice) {
        case 1:
            if (graph.getIsDirected()) {
                graph.setDirected(false);
                cout << "Граф установлен как неориентированный" << endl;
            }
            graph.printAdjacencyMatrix();
            break;
            
        case 2:
            if (!graph.getIsDirected()) {
                graph.setDirected(true);
                cout << "Граф установлен как ориентированный" << endl;
            }
            graph.printAdjacencyMatrix();
            break;
            
        case 3:
            graph.setWeighted(!graph.getIsWeighted());
            cout << "Граф установлен как " 
                 << (graph.getIsWeighted() ? "взвешенный" : "невзвешенный") << endl;
            graph.printAdjacencyMatrix();
            break;
            
        case 4:
            cout << "\nАнализ текущего графа:" << endl;
            graph.printAdjacencyMatrix();
            graph.analyzeGraph();
            break;
            
        case 5:
            cout << "Введите новое количество вершин: ";
            cin >> newVertices;
            if (newVertices > 0) {
                graph.setVertices(newVertices);
                graph.printAdjacencyMatrix();
            }
            else {
                cout << "Неверное количество вершин!" << endl;
            }
            break;
            
        case 6:
            graph.regenerateGraph();
            cout << "Граф регенерирован" << endl;
            graph.printAdjacencyMatrix();
            break;
            
        case 7:
            cout << "Введите стартовую вершину для поиска расстояний (0-" 
                 << graph.getVerticesCount() - 1 << "): ";
            cin >> startVertex;
            if (startVertex >= 0 && startVertex < graph.getVerticesCount()) {
                graph.findDistances(startVertex);
            }
            else {
                cout << "Неверная вершина! Допустимый диапазон: 0-" 
                     << graph.getVerticesCount() - 1 << endl;
            }
            break;
            
        case 0:
            cout << "Выход из программы..." << endl;
            break;
            
        default:
            cout << "Неверный выбор!" << endl;
        }
        
    } while (choice != 0);
    
    return 0;
}