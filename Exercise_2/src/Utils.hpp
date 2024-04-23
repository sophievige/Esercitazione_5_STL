#pragma once //per evitare definizioni multiple (se ho più file header)

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;
namespace PolygonalMeshLibrary{
bool importMesh(const string& path, PolygonalMesh& mesh);

bool importCell0Ds(const string& fileName, PolygonalMesh& mesh);

bool importCell1Ds(const string& fileName, PolygonalMesh& mesh);

bool importCell2Ds(const string& fileName, PolygonalMesh& mesh);

double setTol1D();

double setTol2D(double tol1D);

bool testLengthEdges (const double& tol1D, PolygonalMesh& mesh);

bool testAreaCells2D (const double& tol2D, PolygonalMesh& mesh);

double prodottoVettoriale (const Vector2d& v1, const Vector2d& v2);
}
