#README

##Les différentes fonctions :

1. void Ensemble(double ** pred,int nombreAlgo,int NombreAuteur,int k,int* AuteurK,double* ProbaK)

-pred est un tableau à deux dimensions contenant le résultat des differents algorithmes(ligne) pour les differents auteurs (colonnes)

Il stock la proba et le nom de K meilleurs dans ProbaK et AuteurK.

2. void ExecutionML(string apprentissage,string aClassifier,int NombreAuteur,double* res)

-cette algorithme lis les données, les stocks sous le bon format et utilise l’algorithme de prédiction.
Il stock le résultat dans la variable res.

3. Mat KNNClassifier(Mat & X_train,Mat & y_train,Mat & X_test,Mat & y_test)

-Applique l’algorithme des KNN et retourne le résultat.
