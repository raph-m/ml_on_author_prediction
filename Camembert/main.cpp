//Gnener les resultats sous forme de camembert

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <iostream>
#include <sstream> //Pour convertir en string
#include <bits/stdc++.h>
#include <functiontest.cpp>
using namespace std;


QT_CHARTS_USE_NAMESPACE

//Importer le tableau de Hugo : Il retourne un tableau content les ID des auteurs et les probabilités correspondantes
//Importer les fonction de Vardon : Il retourne le nom de l'auteur à partir de son ID

int main(int argc, char *argv[])
{
    float res[] = {0.1, 0.5, 0.07, 0.100, 0.090, 0.070, 0.08, 0.09, 0.20, 0.02};
    float v1 = 0, v2 = 0, v3 = 0, v4 = 0;
    int first_author, second_author, third_author, fourth_author;
    //Generer le tableau res
    //Copier ce tableau
    float auxiliaire[10];
    for (int i =0; i<10; i++){
        auxiliaire[i] = res[i];
    }

    sort(res, res+10, greater<int>()); //Trie le tableau
    v1 = res[0];
    v2 = res[1];
    v3 = res[2];
    v4 = res[3];

    for (int i =0; i<10; i++){
        if (auxiliaire[i] == v1)
        first_author = i;
        if (auxiliaire[i] == v2)
        second_author = i;
        if (auxiliaire[i] == v3)
        third_author = i;
        if (auxiliaire[i] == v4)
        fourth_author = i;
    }
    cout<< "The first author is"<< first_author;
    cout<< "The second author is"<< second_author;
    cout<< "The third author is"<< third_author;
    cout<< "The fourth author is"<< fourth_author;

    QApplication a(argc, argv);
    //Remplacer par les variables representatnt les bons pourcentages et les noms des auteurs correspondants
    QPieSeries *series = new QPieSeries();
    series->append("first_author", (int) 100*v1);
    series->append("second_author", (int) 100*v2);
    series->append("third_author", (int) 100*v3);
    series->append("fourth_author", (int) 100*v4);
    series->append("Others", (int) 100-100*(v1+v2+v3+v4));

    QPieSlice *slice = series->slices().at(1);
    slice->setExploded();
    slice->setLabelVisible();
    slice->setPen(QPen(Qt::darkGreen, 2));
    slice->setBrush(Qt::green);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple piechart example");
    //chart->legend()->hide();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();



    return a.exec();
}
