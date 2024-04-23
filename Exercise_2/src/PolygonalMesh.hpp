#pragma once

#include <iostream>
#include <vector>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

//per dire che entrambe le cose nei due file header fanno parte di un'unità logica più grande
namespace PolygonalMeshLibrary{
struct PolygonalMesh{

    unsigned int NumberOfCell0Ds;
    vector<unsigned int> IdCell0Ds;
    vector<Vector2d> CoordinatesCell0Ds; //ci metto dentro un vector per farci operazioni matematiche
    vector<unsigned int> MarkerCell0Ds;

    unsigned int NumberOfCell1Ds;
    vector<unsigned int> IdCell1Ds;
    vector<array<unsigned int, 2>> VerticesCell1Ds; //sono id, non serve fare operazioni matematiche
    vector<unsigned int> MarkerCell1Ds;

    unsigned int NumberOfCell2Ds;
    vector<unsigned int> IdCell2Ds;
    vector<vector<unsigned int>> VerticesCell2Ds; //non so a priori quanti sono
    vector<vector<unsigned int>> EdgesCell2Ds;

    //ora vorrei memorizzare tutti gli elementi con un certo marker (diverso da 0). Per questi non so a priori quanti sono (tranne quelli ai vertici del dominio)
    //e non mi interessa l'accesso randomico -> uso un dizionario con chiave=marker e valore=lista degli id degli elementi con quel marker
    map<unsigned int, list <unsigned int>> VerticesMarker;
    map<unsigned int, list <unsigned int>> EdgesMarker;
    //non servono i marker per le 2D (sono tutti 0)

    //per la tolleranza in 2D si fa il max tra quella di default, una scelta dall'utente e quella derivante dalla tolleranza in 1D
};
}
//per leggere la mesh, ha senso metterla fuori (perché non dipende dai valori degli attributi della mesh)

