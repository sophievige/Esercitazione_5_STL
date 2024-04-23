#include <iostream>
#include "Utils.hpp"
#include "PolygonalMesh.hpp"

using namespace std;
using namespace PolygonalMeshLibrary;
int main()
{
    //setto le tolleranze per tutto il programma
    double tol1D = setTol1D();
    double tol2D = setTol2D(tol1D);

    PolygonalMesh mesh;
    string path="./PolygonalMesh";
    if (!importMesh(path, mesh))
        return 1; //se qualcosa va storto

    testLengthEdges(tol1D,mesh);
    testAreaCells2D(tol2D, mesh);

  return 0;
}
