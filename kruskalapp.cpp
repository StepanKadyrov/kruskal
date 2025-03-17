#include "kruskalapp.h"

KruskalApp::KruskalApp(QWidget *parent)
    : QMainWindow(parent)
{
    // Задаём название окна, размер и шрифт

    this->setWindowTitle("Алгоритм Краскала");
    this->setMinimumSize(1300, 650);
    this->setStyleSheet("*{font-family: Comic Sans MS; font-size: 14pt; font-weight: Normal;}");

    // Создаём главный виджет и делаем его центральным для окна

    QWidget *mainWid = new QWidget();
    this->setCentralWidget(mainWid);

    // Создаём меню

    QMenu *menu = new QMenu("&Меню", mainWid);
    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->setStyleSheet("background:transparent");
    menu->addAction("&Загрузить файл", this, SLOT(downloadFile()));
    menu->addAction("&Сохранить результат", this, SLOT(saveFile()));
    menu->addAction("&Выход", this, SLOT(close()));
    menuBar->addMenu(menu);
    this->setMenuWidget(menuBar);

    //SpinBox для задания количества вершин графа и коннектим сигнал изменения значения в нём со слотом

    vertexNumber = new QSpinBox();
    vertexNumber->setMinimum(0);
    connect(vertexNumber, SIGNAL(valueChanged(int)), SLOT(vertexNumberChanged(int)));

    // Все лейблы. 1 - для спинбокса, 2 - для разделения LineEdit'ов, 3 - для поля весе ребра

    QLabel *lbl1 = new QLabel("Количество вершин:");

    QLabel *lbl2 = new QLabel("-");
    lbl2->setAlignment(Qt::AlignCenter);
    lbl2->setFixedSize(50, 50);

    QLabel *lbl3 = new QLabel("Вес: ");

    // Для полей установим валидацию, чтобы можно было вводить только числа и установим фиксированный размер

    lEdit1 = new QLineEdit();
    lEdit2 = new QLineEdit();
    weightLEdit = new QLineEdit();

    lEdit1->setFixedSize(50, 50);
    lEdit2->setFixedSize(50, 50);

    lEdit1->setValidator(new QIntValidator());
    lEdit2->setValidator(new QIntValidator());
    weightLEdit->setValidator(new QIntValidator());

    // Кнопка для запуска алгоритма поиска остова

    QPushButton *startBtn = new QPushButton("Найти остов");
    connect(startBtn, SIGNAL(clicked()), SLOT(startSlot()));
    startBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    // Кнопки для создания и удаления рёбер и коннектим их со слотами

    QPushButton *makeBtn = new QPushButton("Создать ребро");
    QPushButton *deleteBtn= new QPushButton("Удалить ребро");

    connect(makeBtn, SIGNAL(clicked()), SLOT(makeEdgeSlot()));
    connect(deleteBtn, SIGNAL(clicked()), SLOT(deleteEdgeSlot()));

    // Создаём таблицу для вывода связей и настраиваем её ячейки

    tblEdges = new QTableWidget();
    tblEdges->setEditTriggers(QAbstractItemView::NoEditTriggers); //Запрещаем редактирование таблицы
    tblEdges->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tblEdges->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Компоновщик, в него добавляем все виджеты

    QGridLayout *topLayout = new QGridLayout();
    topLayout->addItem(new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 0);
    topLayout->addWidget(tblEdges, 0, 4, 7, 1);
    topLayout->addWidget(lbl1, 1, 0, 1, 4);
    topLayout->addWidget(vertexNumber, 2, 0, 1, 1);
    topLayout->addWidget(startBtn, 3, 0, 3, 1);
    topLayout->addWidget(lEdit1, 2, 1, 1, 1);
    topLayout->addWidget(lbl2, 2, 2, 1, 1);
    topLayout->addWidget(lEdit2, 2, 3, 1, 1);
    topLayout->addWidget(lbl3, 3, 1, 1, 1);
    topLayout->addWidget(weightLEdit, 3, 2, 1, 2);
    topLayout->addWidget(makeBtn, 4, 1, 1, 3);
    topLayout->addWidget(deleteBtn, 5, 1, 1, 3);

    // Устанавливаем компоновщик на главный виджет

    mainWid->setLayout(topLayout);
}

KruskalApp::~KruskalApp()
{
}

void KruskalApp::paintEvent(QPaintEvent *)
{
    // Создаём объект рисования и задаёт кисть с цветом для заливки вершин графа

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(QColor(0, 255, 0)));

    // Запрашиваем количество вершин графа и меняем размер вектора с координатами центров вершин

    int n = graph.getSize();
    vertexCoord.resize(n);

    // Генерируем координаты центров вершин и добавляем их в вектор

    for(int i = 0; i < n; i++)
    {
        qreal fAngle = 2 * 3.14 * i / n;
        qreal x = this->width()/5 + qCos(fAngle) * 150;
        qreal y = this->height()/3 + qSin(fAngle) * 150;
        vertexCoord[i] = QPoint(x, y);
    }

    const QVector<QPair<int, int>> MSTedges = graph.getMSTedges();

    // Рисуем рёбра между графами

    for(int i = 0; i < n; i++)
    {
        for(int j = i; j < n; j++)
        {
            if(graph.getWeight(i, j) != -1 && graph.getWeight(i, j) != 0)
            {
                if(MSTedges.contains(qMakePair(i, j)) || MSTedges.contains(qMakePair(j, i)))
                {
                    painter.setPen(QColor(Qt::red));
                    painter.drawText(QPoint((vertexCoord[i].x() + vertexCoord[j].x())/2, (vertexCoord[i].y() + vertexCoord[j].y())/2), QString::number(graph.getWeight(i, j)));
                }
                painter.drawLine(vertexCoord[i], vertexCoord[j]);
                painter.setPen(QColor(Qt::black));
            }
        }
    }

    // Рисуем вершины и подписываем их

    for(int i = 0; i < n; i++)
    {
        painter.drawEllipse(vertexCoord[i], 20, 20);
        painter.drawText(QRect(vertexCoord[i].x() - 20, vertexCoord[i].y() - 20, 40, 40), Qt::AlignCenter, QString::number(i + 1));
    }

}

void KruskalApp::downloadFile()
{
    // Выводим инструкцию о формате матрицы графа

    QMessageBox::information(this, "Алгоритм Краскала", "Инструкция:\n"
                                                        "1) Матрица должна быть симметрична\n"
                                                        "2) На главной диагонали должны стоять -1\n"
                                                        "3) Если между вершинами нет ребёр, то должен стоять 0\n"
                                                        "4) Матрица должна быть записана с первой строки файла\n"
                                                        "В ином случае файл не сможет загрузиться");

    QString filename = QFileDialog::getOpenFileName(this, "Файл с матрицей смежности графа", "", "Text file (*.txt)");
    if(!filename.isEmpty())
    {
        if(graph.setGraphFile(filename))
        {
            vertexNumber->setValue(graph.getSize());
            fillTable();
        }
        else
        {
            QMessageBox::critical(this, "Алгоритм Краскала", "Введите корректную матрицу смежности графа"); // Если матрица некорректная, то выводим ошибку
        }
    }
}

void KruskalApp::saveFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "Файл сохранения", "", "*.txt");
    if(!filename.isEmpty())
    {
        graph.saveGraph(filename);
    }
}

void KruskalApp::startSlot()
{
    if(graph.getSize() > 1)
    {
        if(graph.isConnected())
        {
            QMessageBox::information(this, "Алгоритм Краскала", "Вес минимального остовного дерева: " + QString::number(graph.kruskalAlg()) + "\nРёбра остовного дерева выделены красным");
        }
        else
        {
            QMessageBox::critical(this, "Алгоритм Краскала", "Граф должен быть связным"); // Если граф не связан, то выводим предупреждение
        }
    }
    else
    {
        QMessageBox::warning(this, "Алгоритм Краскала", "Добавьте в граф хотя бы 2 связные вершины");
    }
}

void KruskalApp::vertexNumberChanged(int n)
{
    // Проверяем входное значение, если больше, чем текущий размер графа, то добавляем строку и столбец в таблицу, а вершину в граф. В ином случае всё наоборот
    if(n > graph.getSize())
    {
        // В цикле добаляем нужное количество вершин

        for(int i = graph.getSize(); i < n; i++)
        {
            graph.addVertex();
        }
    }
    else
    {
        // В цикле удаляем нужное количество вершин

        for(int i = graph.getSize(); i > n; i--)
        {
            graph.deleteVertex();
        }
    }

    // Меняем размер таблицы и заполняем её

    tblEdges->setRowCount(n);
    tblEdges->setColumnCount(n);

    fillTable();

    // Обновляем экран для перерисовки

    update();

}

void KruskalApp::fillTable()
{
    // Заполняем таблицу

    int n = graph.getSize();

    QTableWidgetItem *ptwi;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            int weight = graph.getWeight(i, j); // Запрашиваем вес ребра

            if(weight == -1)
            {
                ptwi = new QTableWidgetItem("-"); // Если вес -1, то это главная диагональ и ставим прочерк
            }
            else if(weight == 0)
            {
                ptwi = new QTableWidgetItem(""); // Если вес 0, то ребра нет и ничего не ставим
            }
            else
            {
                ptwi = new QTableWidgetItem(QString::number(weight));
            }

            ptwi->setTextAlignment(Qt::AlignCenter);

            tblEdges->setItem(i, j, ptwi);
        }
    }
}

void KruskalApp::makeEdgeSlot()
{
    if(lEdit1->text().isEmpty() || lEdit2->text().isEmpty())
    {
        QMessageBox::warning(this, "Алгоритм Краскала", "Введите две вершины"); // Если введены не 2 вершины, то выводим сообщение
    }
    else if(lEdit1->text().toInt() < 1 || lEdit2->text().toInt() < 1)
    {
        QMessageBox::warning(this, "Алгоритм Краскала", "Введите корректные вершины"); // Если введены не 2 вершины, то выводим сообщение
    }
    else if(weightLEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Алгоритм Краскала", "Введите вес"); // Если не введён вес, то выводим сообщение
    }
    else if(weightLEdit->text().toInt() <= 0)
    {
        QMessageBox::warning(this, "Алгоритм Краскала", "Введите корректный вес ребра"); // Если введён не положительный вес, то выводим сообщение
        weightLEdit->setText(""); // Очищаем поле с весом ребра
    }
    else
    {
        // Если всё корректно, то считываем значения из полей, чистим поля и пытаемся добавить ребро

        int x = lEdit1->text().toInt();
        int y = lEdit2->text().toInt();
        int weight = weightLEdit->text().toInt();

        lEdit1->setText("");
        lEdit2->setText("");
        weightLEdit->setText("");

        switch(graph.addEdge(x, y, weight))
        {
            case 1:
                QMessageBox::warning(this, "Алгоритм Краскала", "Введите существующие вершины для добавления ребра"); // Если введены вершины, не существующие в графе, то выводим сообщение
                break;
            case 2:
                QMessageBox::warning(this, "Алгоритм Краскала", "Введите разные вершины для добавления ребра"); // Если введена одна и та же вершина, то выводим сообщение
                break;
            case 0:
                tblEdges->item(x - 1, y - 1)->setText(QString::number(weight)); // В случае успеха заполняем ячейки
                tblEdges->item(y - 1, x - 1)->setText(QString::number(weight));
                update();
                break;
        }
    }
}

void KruskalApp::deleteEdgeSlot()
{
    if(lEdit1->text().isEmpty() || lEdit1->text().isEmpty())
    {
        QMessageBox::warning(this, "Алгоритм Краскала", "Введите две вершины для удаления ребра"); // Если введены не 2 вершины, то выводим сообщение
    }
    else
    {
        // Если всё корректно, то считываем значения из полей, чистим поля и пытаемся удалить ребро

        int x = lEdit1->text().toInt();
        int y = lEdit2->text().toInt();

        lEdit1->setText("");
        lEdit2->setText("");

        switch(graph.deleteEdge(x, y))
        {
            case 1:
                QMessageBox::warning(this, "Алгоритм Краскала", "Введите существующие вершины для удаления ребра"); // Если введены вершины, не существующие в графе, то выводим сообщение
                break;
            case 2:
                QMessageBox::warning(this, "Алгоритм Краскала", "Введите разные вершины для удаления ребра"); // Если введена одна и та же вершина, то выводим сообщение
                break;
            case 3:
                QMessageBox::warning(this, "Алгоритм Краскала", "Введите вершины, между которыми существует ребро"); // Если между вершинами не существует ребра, то выводим сообщение
                break;
            case 0:
                tblEdges->item(x - 1, y - 1)->setText(""); // В случае успеха удаляем значение из ячеек
                tblEdges->item(y - 1, x - 1)->setText("");
                update();
                break;
        }
    }
}

