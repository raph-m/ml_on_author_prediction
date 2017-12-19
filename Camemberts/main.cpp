//Generer les resultats sous forme de camembert

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <iostream>
#include <sstream> //Pour convertir en string
#include <bits/stdc++.h>

using namespace std;


QT_CHARTS_USE_NAMESPACE

// Dans la suite res representera le tableau des probabilité
//generé par les algorithmes de prédictions.

//On effectuera un tri pour récupérer les valeurs maximales du tableau
//et les indices de ces valeurs font référence au auteurs prédits

int main(int argc, char *argv[])
{
    float res[] = {0.02, 0.05, 0.07, 0.06, 0.090, 0.070, 0.5, 0.09, 0.01, 0.04}; //Tableau des proba
    float v1, v2, v3, v4; // valeurs maximales
    int first_author, second_author, third_author, fourth_author; // A tirer du tableau des proba
                                                                  // ce sont les indices


    //sort(res, res+10, greater<int>()); //Trie le tableau


    // Initialisation
    v1 = res[0];
    v2 = res[0];
    v3 = res[0];
    v4 = res[0];

    // Trouver les valeurs maximales
    for(int i=0; i<10; i++){
        if(res[i]>v1){
            v1 = res[i];
            first_author = i;
        }
    }

    for(int i=0; i<10; i++){
         if(res[i]>v2)
            if (res[i]<v1){
               v2 = res[i];
               second_author = i;
            }
        }
    for(int i=0; i<10; i++){
         if(res[i]>v3)
            if (res[i]<v2){
               v3 = res[i];
               third_author = i;
            }
        }
    for(int i=0; i<10; i++){
         if(res[i]>v4)
            if (res[i]<v3){
               v4 = res[i];
               fourth_author = i;
            }
    }



//    for (int i =0; i<10; i++){
//        if (auxiliaire[i] == v1)
//        first_author = i;
//        if (auxiliaire[i] == v2)
//        second_author = i;
//        if (auxiliaire[i] == v3)
//        third_author = i;
//        if (auxiliaire[i] == v4)
//        fourth_author = i;
//    }
//    cout<< "The first author is"<< first_author;
//    cout<< "The second author is"<< second_author;
//    cout<< "The third author is"<< third_author;
//    cout<< "The fourth author is"<< fourth_author;

    cout<< "The first author is "<< first_author << " With probability " << v1 <<"\n";
    cout<< "The second author is "<< second_author<< " With probability " << v2<<"\n";
    cout<< "The third author is "<< third_author<< " With probability " << v3 <<"\n";
    cout<< "The fourth author is "<< fourth_author<< " With probability " << v4<<"\n";

    QApplication a(argc, argv);
    //Remplacer par les variables representatnt les bons pourcentages et les noms des auteurs correspondants
    QPieSeries *series = new QPieSeries();
    series->append("first_author", v1);
    series->append("second_author", v2);
    series->append("third_author", v3);
    series->append("fourth_author", v4);
    series->append("Others", 1 - (v1+v2+v3+v4));

    QPieSlice *slice = series->slices().at(1);
    slice->setExploded();
    slice->setLabelVisible();
    slice->setPen(QPen(Qt::darkGreen, 2));
    slice->setBrush(Qt::green);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Probabilités d'appartenance aux auteurs");


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();



    return a.exec();
}
