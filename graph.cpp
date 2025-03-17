#include "graph.h"

Graph::Graph()
{

}

int Graph::addEdge(int x, int y, int weight)
{
    if(x > graphMatrix.size() || y > graphMatrix.size())
    {
        return 1; // Если номер хотя бы одной из вершин больше порядка матрицы, то возвращаем 1
    }
    else if(x == y)
    {
        return 2; // Возвращаем 2, если пытаемся добавить ребро на главной диагонали матрицы
    }
    else
    {
        //Если всё ок, то добавляем ребро
        graphMatrix[x-1][y-1] = weight;
        graphMatrix[y-1][x-1] = weight;

        //Чистим MSTedges, если он не пуст

        if(!MSTedges.isEmpty())
            MSTedges.clear();
        return 0; //возвращаем 0 в случае успеха
    }
}

int Graph::deleteEdge(int x, int y)
{
    if(x > graphMatrix.size() || y > graphMatrix.size())
    {
        return 1; // Если номер хотя бы одной из вершин больше порядка матрицы, то возвращаем 1
    }
    else if(x == y)
    {
        return 2; // Возвращаем 2, если пытаемся удалить ребро на главной диагонали матрицы
    }
    else if(graphMatrix[x-1][y-1] == 0)
    {
        return 3; // Возвращаем 3, если ребра не существует
    }
    else
    {
        graphMatrix[x-1][y-1] = 0;
        graphMatrix[y-1][x-1] = 0;

        //Чистим MSTedges, если он не пуст

        if(!MSTedges.isEmpty())
            MSTedges.clear();
        return 0; // Обнуляем ребро и возвращаем 0 в случае успеха
    }
}

void Graph::addVertex()
{
    int nSize = graphMatrix.size() + 1;

    graphMatrix.resize(nSize); // Увеличиваем количество строк

    for(int i = 0; i < nSize; i++)
    {
        graphMatrix[i].resize(nSize); // В каждой строке увеличиваем количество столбцов на 1
    }

    graphMatrix[nSize - 1][nSize - 1] = -1; // На главную диагональ ставим -1

    //Чистим MSTedges, если он не пуст

    if(!MSTedges.isEmpty())
        MSTedges.clear();
}

void Graph::deleteVertex()
{
    if(graphMatrix.size() == 0)
        return;

    int size = graphMatrix.size();

    //Чистим MSTedges, если он не пуст

    if(!MSTedges.isEmpty())
        MSTedges.clear();

    for(int i = 0; i < size; i++)
    {
        graphMatrix[i].pop_back(); // В каждой строке удаляем последний столбец
    }

    graphMatrix.pop_back(); // Удаляем последнюю строку
}

int Graph::getSize()
{
    return graphMatrix.size(); // Возвращаем размер матрицы
}

int Graph::getWeight(int x, int y)
{
    return graphMatrix[x][y]; // Передаём вершины с нуля, поэтому не надо отнимать 1
}

bool Graph::setGraphFile(QString filename)
{
    QFile inputFile(filename);
    if(inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream fileStream(&inputFile);
        QString line = fileStream.readLine(); // Считываем первую строку
        QStringList splitLine = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts); // Разделяем первую строку
        int sizeFirst = splitLine.size(); //Размер первой строки
        int size = 0; // Размер матрицы

        //Если размер первой строки не нулевой, то увеличиваем размер считываемой матрицы на 1, в ином случае закрываем файл и возвращаем false

        if(sizeFirst)
        {
            size++;
        }
        else
        {
            inputFile.close();
            return false;
        }

        if(splitLine[0].toInt() != -1)
        {
            inputFile.close();
            return false;
        }

        // Просматриваем файл до конца, считывая его по строчке и определяя количество элементов в ней

        int i = 1;

        while(!fileStream.atEnd())
        {
            line = fileStream.readLine();
            splitLine = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
            if(sizeFirst == splitLine.size())
            {
                size++; //Если размер считанной строки равен размеру первой строки, то увеличиваем размер считываемой матрицы

                if(splitLine.size() > i && splitLine[i].toInt() != -1)
                {
                    inputFile.close(); // Если на главной диагонали не -1, то закрываем файл и возвращаем false
                    return false;
                }

            }
            else
            {
                inputFile.close(); // В ином случае закрываем файл и возвращаем false
                return false;
            }
            i++;
        }

        // Если количество строк не совпало с количеством столбцов, то закрываем файл и возвращаем false

        if(size != sizeFirst)
        {
            inputFile.close();
            return false;
        }

        inputFile.reset(); // Возвращаем поток в начальное положение

        // Создаём временную матрицу и заполняем её

        QVector<QVector<int>> temp(size);

        for(int i = 0; i < size; i ++)
        {
            line = fileStream.readLine();
            splitLine = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
            for(int j = 0; j < size; j++)
            {
                temp[i].push_back(splitLine[j].toInt());
            }
        }

        inputFile.close(); // Закрываем файл, далее он больше не нужен

        // Проверяем временную матрицу на симметричность, если не симметрична, то возвращаем false

        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                if(temp[i][j] != temp[j][i])
                {
                    return false;
                }
            }
        }

        // Находим разницу между новым графом и уже имеющимся и в зависимости от этого добавляем или удаляем вершины

        int sub = size - this->getSize();

        if(sub > 0)
        {
            for(int i = 0; i < sub; i++)
            {
                this->addVertex();
            }
        }
        else
        {
            for(int i = 0; i < -sub; i++)
            {
                this->deleteVertex();
            }
        }

        // Добавляем рёбра в граф

        for(int i = 0; i < size; i++)
        {
            for(int j = i + 1; j < size; j++)
            {
                this->addEdge(i + 1, j + 1, temp[i][j]);
            }
        }

        return true; // Возвращаем true в случае успеха
    }

    return false;//Возвращаем false в случае неудачного открытия файла
}

bool Graph::isConnected()
{
    int check = 0;

    for(int i = 0; i < this->getSize(); i++)
    {
        for(int j = 0; j < this->getSize(); j++)
        {
            if(graphMatrix[i][j] != 0 && graphMatrix[i][j] != -1)
            {
                check++;
                break;
            }
        }
    }

    return (check == this->getSize() ? true : false);
}

int Graph::kruskalAlg()
{
    //Создаём вектор рёбер и заполняем его

    QVector<QPair<int, QPair<int, int>>> edges;

    for(int i = 0; i < this->getSize(); i++)
    {
        for(int j = i + 1; j < this->getSize(); j++)
        {
            if(graphMatrix[i][j] != 0)
            {
                edges.push_back({graphMatrix[i][j], {i, j}});
            }
        }
    }

    std::sort(edges.begin(), edges.end()); //Сортируем рёбра по возрастанию

    int length = 0;

    DisjointSets ds(this->getSize()); // Создаём экземляр структуры, отвечающий за компоненты связности

    QVector<QPair<int, QPair<int, int>>>::iterator it;

    // Очищаем старое остовное дерево

    MSTedges.clear();

    // Ищем дерево

    for (it=edges.begin(); it!=edges.end(); it++)
    {
        int u = it->second.first;
        int v = it->second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        // Если вершины принадлежат разных компонентах связности, то добавляем вес ребра в вес минимального дерева и объединяем множества
        if (set_u != set_v)
        {
            length += it->first;
            MSTedges.push_back(qMakePair(it->second.first, it->second.second));
            ds.merge(set_u, set_v);
        }
    }

    delete [] ds.parent; // Чистим память
    delete [] ds.rnk;

    return length; // Возвращаем длину остовного дерева
}

void Graph::saveGraph(QString filename)
{
    QFile outputFile(filename);
    if(outputFile.open(QIODevice::Append))
    {
        QTextStream fileStream(&outputFile);

        for(int i = 0; i < this->getSize(); i++)
        {
            for(int j = 0; j < this->getSize(); j++)
            {
                fileStream << graphMatrix[i][j] << " ";
            }

            fileStream << "\n";
        }

        //Если MSTedges не пустое, то его тоже записываем в файл

        if(!MSTedges.isEmpty())
        {
            fileStream << QString(("Минимальное остовное дерево:\n")).toUtf8();

            QVector<QPair<int, int>>::iterator it;

            for (it=MSTedges.begin(); it!=MSTedges.end(); it++)
            {
                fileStream << it->first + 1 << " - " << it->second + 1 << QString(" Вес: ").toUtf8() << graphMatrix[it->first][it->second] << "\n";
            }
        }
        else
        {
            fileStream << QString("Минимальное остовное дерево отсутствует\n").toUtf8() ;
        }

        outputFile.close();
    }

}

const QVector<QPair<int, int> >& Graph::getMSTedges()
{
    return MSTedges;
}
