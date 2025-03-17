#ifndef KRUSKALAPP_H
#define KRUSKALAPP_H

#include <QtMath>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QSpacerItem>
#include <QTableWidget>
#include <QHeaderView>
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>

#include "graph.h"

class KruskalApp : public QMainWindow
{
    Q_OBJECT

public:
    KruskalApp(QWidget *parent = nullptr);
    ~KruskalApp();
private:
    QLineEdit *lEdit1; // Поле ввода для первой вершины
    QLineEdit *lEdit2; // Поле для ввода для второй вершины
    QLineEdit *weightLEdit; // Поле для ввода веса ребра
    QTableWidget *tblEdges; // Таблица для представления графа
    QSpinBox *vertexNumber; //Spinbox для изменения количества вершин
    Graph graph; // Сам граф
    void fillTable();
    void fillTableItem(int, int);
    QVector<QPoint> vertexCoord; // Вектор для хранения центров вершин графа
protected:
    void paintEvent(QPaintEvent*); //Перегружаем событие отрисовки
private slots:
    void downloadFile();
    void saveFile();
    void startSlot();
    void vertexNumberChanged(int); // Слот, реагирующий на изменение количества вершин
    void makeEdgeSlot();
    void deleteEdgeSlot();
};
#endif // KRUSKALAPP_H
