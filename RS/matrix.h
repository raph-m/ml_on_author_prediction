#pragma once

#include "column.h"



// défini matrice réelle
class RealMatrix {

private:

  unsigned int rows;
  unsigned int columns;
  double * data;

public:

    // constructeur
    RealMatrix( unsigned int rows, unsigned int columns ) :
      rows(rows), columns(columns)
    {
      // allocation de la matrice des données
      data = new double[rows*columns];
      memset( data, 0, sizeof(*data)*rows*columns );
    }

    // destructeur
    ~RealMatrix( void )
    {
      delete [] data;
    }

    // retourne rows
    unsigned int get_row( void )
    {
      return rows;
    }

    // retourne colonne
    unsigned int column_count( void )
    {
      return columns;
    }

    /* pour rendre utilisation plus naturelle
     * retourne un pointeur au début de la ligne row
     */
    double * operator[]( unsigned int row )
    {
      return &data[row*columns];
    }


};

