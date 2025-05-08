#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

const int MAXN = 100005;
vector<int> adj[MAXN];
bool visited[MAXN];

void parallel_bfs(int start) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int level_size = q.size();
        vector<int> current_level;

        // Extract current level
        for (int i = 0; i < level_size; i++) {
            int v = q.front();
            q.pop();
            cout << v << " ";
            current_level.push_back(v);
        }

        vector<int> next_level;

        // Parallelize over current level nodes
        #pragma omp parallel for
        for (int i = 0; i < current_level.size(); i++) {
            int v = current_level[i];
            for (int u : adj[v]) {
                // Critical section to avoid race on visited and next_level
                #pragma omp critical
                {
                    if (!visited[u]) {
                        visited[u] = true;
                        next_level.push_back(u);
                    }
                }
            }
        }

        for (int u : next_level)
            q.push(u);
    }
}

void parallel_dfs(int start) {
    stack<int> s;
    s.push(start);
    visited[start] = true;

    while (!s.empty()) {
        int v = s.top();
        s.pop();
        cout << v << " ";

        vector<int> neighbors;

        // Collect unvisited neighbors
        #pragma omp parallel for
        for (int i = 0; i < adj[v].size(); i++) {
            int u = adj[v][i];
            bool needs_push = false;

            #pragma omp critical
            {
                if (!visited[u]) {
                    visited[u] = true;
                    needs_push = true;
                }
            }

            if (needs_push) {
                #pragma omp critical
                neighbors.push_back(u);
            }
        }

        for (int u : neighbors)
            s.push(u);
    }
}

void reset_visited(int n) {
    for (int i = 1; i <= n; i++)
        visited[i] = false;
}

int main() {
    int n, m;
    cout << "Enter number of nodes and edges: ";
    cin >> n >> m;

    if (m > n * (n - 1) / 2) {
        cout << "Error: Too many edges.\n";
        return 1;
    }

    cout << "Enter edges:\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected graph
    }

    while (true) {
        cout << "\nChoose an option:\n1. Parallel BFS\n2. Parallel DFS\n3. Exit\nEnter your choice: ";
        int choice;
        cin >> choice;
        if (choice == 3) break;

        cout << "Enter starting node: ";
        int start;
        cin >> start;

        reset_visited(n);
        if (choice == 1) {
            cout << "Running Parallel BFS...\nVisited nodes: ";
            parallel_bfs(start);
        } else if (choice == 2) {
            cout << "Running Parallel DFS...\nVisited nodes: ";
            parallel_dfs(start);
        }

        for (int i = 1; i <= n; i++) {
            if (!visited[i] && !adj[i].empty()) {
                cout << "\nGraph has disconnected components. Running again from node: " << i << endl;
                if (choice == 1)
                    parallel_bfs(i);
                else
                    parallel_dfs(i);
            }
        }
        cout << endl;
    }

    return 0;
}
