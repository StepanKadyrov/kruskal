#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QFile>
#include <QObject>
#include <QDebug>

struct DisjointSets
{
    int *parent, *rnk;
    int n;

    DisjointSets(int n)
    {
        this->n = n;
        parent = new int[n+1];
        rnk = new int[n+1];

        for (int i = 0; i <= n; i++)
        {
            rnk[i] = 0;
            parent[i] = i;
        }
    }

    int find(int u)
    {
        /* Make the parent of the nodes in the path
        from u--> parent[u] point to parent[u] */
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    // Union by rank
    void merge(int x, int y)
    {
        x = find(x), y = find(y);
        /* Make tree with smaller height
        a subtree of the other tree */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // If rnk[x] <= rnk[y]
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

class Graph
{
public:
    Graph();
    int addEdge(int x, int y, int weight);
    int deleteEdge(int x, int y);
    void addVertex();
    void deleteVertex();
    int getSize();
    int getWeight(int x, int y);
    bool setGraphFile(QString filename);
    bool isConnected();
    int kruskalAlg();
    void saveGraph(QString filename);
    const QVector<QPair<int, int>>& getMSTedges();
private:
    QVector<QVector<int>> graphMatrix;
    QVector<QPair<int, int>> MSTedges;
};

#endif // GRAPH_H
