#include "dataset.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------

//syntaxe appelant les constructeurs prédéfinis pour chaque élément après :
Dataset::Dataset( unsigned int rows, unsigned int columns ) :
  data_ref(rows),
  data( new RealMatrix(rows, columns) ),
  usingRef(false)
{
  // initialise les pointeurs vers les éléments de data (0.0)
  for ( unsigned int row = 0; row < rows; ++row )
  {
    data_ref[row] = row;
  }
}

//------------------------------------------------------------------------------

Dataset::Dataset( Dataset & reference, unsigned int rows ) :
  data_ref(rows),
  data(&reference.data_matrix()),
  usingRef(true)
{}

//------------------------------------------------------------------------------

Dataset::~Dataset( void )
{
  // détruit le dataset si ce n'est pas une référence
  if ( !usingRef )
  {
    delete data;
  }
}

//------------------------------------------------------------------------------

ProbaMap Dataset::count_thresh(
        unsigned int column, double threshold )
{
  ProbaMap pmap;
  //on rappelle que pmap est de type std::map<std::string, double>
  pmap["LE"]  = 0.0;
  pmap["G"]   = 0.0;

  unsigned int rows = data_ref.get_row();
  if ( rows > 0 )
  {
    // parcourt la colonne pour séparer les élements selon le seuil
    for ( unsigned int row = 0; row < rows; ++row )
    {
      // voir redéfinition de l'opérateur en dataset.h
      if ( operator[](row)[column] > threshold )
      {
        pmap["G"] += 1.0;
      }
      else
      {
        pmap["LE"] += 1.0;
      }
    }

    // normalisation, on retient les fréquences
    pmap["LE"] /= rows;
    pmap["G"] /= rows;
  }

  return pmap;
}

//------------------------------------------------------------------------------

Dataset::ThreshVector Dataset::get_thresh( unsigned int column )
{
  ThreshVector thresholds;
  for ( unsigned int row = 0; row < get_row(); ++row )
  {
    thresholds.push_back( operator[](row)[column] );
  }
  return thresholds;
}

//------------------------------------------------------------------------------

DatasetSplitPair Dataset::split(
        unsigned int column, double threshold )
{

  /* création d'un objet DatasetSplitPair
   * séparation et allocation mémoire
   */
  DatasetSplitPair split( *this, get_row() );
  unsigned int ds_le_elements = 0;
  unsigned int ds_g_elements = 0;

  // partition des données
  for ( unsigned int row = 0; row < get_row(); ++row )
  {
    if ( operator[](row)[column] > threshold )
    {
      split.ds_g(ds_g_elements) = operator()(row);
      ++ds_g_elements;
    }
    else
    {
      split.ds_le(ds_le_elements) = operator()(row);
      ++ds_le_elements;
    }
  }

  // on ajuste la taille des données pour économiser espace mémoire
  split.ds_le.resize( ds_le_elements );
  split.ds_g.resize( ds_g_elements );

  return split;
}

//------------------------------------------------------------------------------

double Dataset::IG(
  unsigned int decision_column,
  unsigned int attribute_column,
  // référence qui va contenir le meilleur seuil
  double & threshold )
{
  // calcul de l'entropie de catégorisation selon la decision_column
  ProbaMap enum_dc = count_thresh( decision_column, threshold );
  double class_entropy = enum_dc.entropy();


  // itération sur tous les seuils possibles
  ThreshVector thresholds = get_thresh( attribute_column );
  double best_split_threshold = -1e199;
  double best_split_ig        = -1000.0;
  for ( ThreshVector::const_iterator iter = thresholds.begin();
    iter != thresholds.end(); ++iter ) {

    double split_ig = class_entropy;
    double threshold = *iter;

    // Enumerate at threshold.
    DatasetSplitPair split_pair = split( attribute_column, threshold );
    // transforme get_row() en double
    double split_prob_le = split_pair.ds_le.get_row() / static_cast<double>(get_row());
    double split_prob_g = split_pair.ds_g.get_row() / static_cast<double>(get_row());

    // Calcul de l'entropie de la partie LE
    ProbaMap enum_dc_le = split_pair.ds_le.count_thresh( decision_column, 0.0 );
    double class_entropy_le = enum_dc_le.entropy();
    split_ig = split_ig - split_prob_le * class_entropy_le;

    // Calcul de l'entropie de la partie G
    ProbaMap enum_dc_g = split_pair.ds_g.count_thresh( decision_column, 0.0 );
    double class_entropy_g = enum_dc_g.entropy();
    split_ig = split_ig - split_prob_g * class_entropy_g;

    // gain d'information? si oui ce nouveau threshold devient la star à comparer
    if ( split_ig > best_split_ig ) {
      best_split_ig = split_ig;
      best_split_threshold = threshold;
    }

  }

  /* on a trouvé le meilleur seuil de séparation
   * et le pointeur threshold pointe vers cette valeur après l'appel de IG
   */

  threshold = best_split_threshold;
  return best_split_ig;
}

//------------------------------------------------------------------------------

Dataset Dataset::bootstrap_sample( unsigned int sample_size )
{
  Dataset sample( *this, sample_size );
  unsigned int rows = get_row();
  for ( unsigned int row = 0; row < sample_size; ++row )
  {
    sample(row) = static_cast<unsigned int>(rand()) % rows;
  }
  return sample;
}

