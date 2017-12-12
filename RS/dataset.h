#pragma once
#include <math.h>
#include <string>
#include <map>
#include <vector>
#include <string.h>

#include "column.h"
#include "matrix.h"
#include "probamap.h"

// dire au compilateur d'affecter l'adresse 0 pour les champs null
#define null(type) reinterpret_cast<type*>(0)

class DatasetSplitPair;


// Représenter des jeux de données avec des fonctionnalités utiles pour le ml
class Dataset
{

  public:

    // Défini une liste de clés
    typedef std::map<std::string, unsigned int> KeyList;

    // Défini une vecteur de seuils pour chaque feature
    typedef std::vector<double> ThreshVector;

    // constructeur par défaut
    Dataset( unsigned int rows, unsigned int columns );

    // construit à partir d'un autre dataset
    Dataset( Dataset & reference, unsigned int rows );

    // destructeur
    ~Dataset( void );

    /* regarde toutes les valeurs de la colonne column
     * et les sépare selon le seuil threshold (<=, >)
     * en comptant dans la proba map la fréquence de
     * <= et de > dans la colonne
     * les clés des valeurs inf au threshold
     * sont LE pour 'less or equal' et G pour 'greater'
     */
    ProbaMap count_thresh(unsigned int column, double threshold );

    /* retourne liste des seuils à tester en évaluant column
     * il s'agit des valeurs de la colonne
     */
    ThreshVector get_thresh(unsigned int column );

    /* divise effectivement le dataset selon les valeurs de la colonne
     * column avec le threshold
     * renvoie un array de deux datasets
     * le premier contenant les valeurs <= au threshold
     * le deuxième contenant les valeurs > au threshold
     */
    DatasetSplitPair split(
      unsigned int column, double threshold );

    /* détermine IG (information gain) d'une feature (colonne)
     * pour percevoir le gain d'information que la division
     * selon tous les thresholds
     * sur la feature attribute_column apporte
     * par rapport à une colonne de décision/classification ie. d'étiquettes
     * La fonction sélectionne le meilleur seuil et
     * threshold pointe vers lui à la fin, et retourne le gain associé
     */
    double IG(
      unsigned int decision_column,
      unsigned int attribute_column,
      double & threshold );

    /* génère un bootstrap sample à partir du dataset
     * (sampling avec remplacement)
     * sélectionne aléatoirement un sample de taille sample_size
     * qui peut contenir plusieurs fois une même instance
     * ces samples seront alors affecté chacun à un decision tree
     * différent pour lancer notre random forest
     */
    Dataset bootstrap_sample( unsigned int sample_size );


    // renvoie la matrice de données
    RealMatrix & data_matrix( void )
    {
      return *data;
    }

    /* pour utiliser les opérateurs naturels sur les datasets
     * renvoie un pointeur sur le début de la ligne row dans data
     * à partir des références aux lignes
     */

    double * operator[]( const unsigned int row )
    {
      unsigned int data_index = data_ref[row];
      return (*data)[data_index];
    }

    // retourne le nombre de lignes
    unsigned int get_row( void )
    {
      return data_ref.get_row();
    }

    /* redimensionne le set de référence
     * et non pas le dataset en lui même
     */
    void resize( unsigned int rows )
    {
      data_ref.resize( rows );
    }

    // renvoie l'array de référence aux lignes
    IntegerCol & data_reference( void )
    {
      return data_ref;
    }

    // accès aux lignes
    unsigned int & operator()( const unsigned int row )
    {
      return data_ref[row];
    }

    // renvoie liste des clé
    KeyList & get_keys( void )
    {
      return keys;
    }

    friend class test; // pour les tests

  private:
    KeyList                   keys;
    IntegerCol             data_ref;
    RealMatrix * const        data;

    // selon si on utilise la référence ou pas
    const bool                usingRef;


};


// on définit une paire de datasets générés après division
class DatasetSplitPair
{
  public:

    /* constructeur prenant en compte le maximum de lignes contenus
     * dans les datasets résultants (garder espace mémoire)
    */
    DatasetSplitPair( Dataset & ref_dataset, const unsigned int max_rows ) :
      ds_le( ref_dataset, max_rows ),
      ds_g( ref_dataset, max_rows )
    { }

    Dataset ds_le;  // éléments <= threshold
    Dataset ds_g;   // éléments > threshold
};



