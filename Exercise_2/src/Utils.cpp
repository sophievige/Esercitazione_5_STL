#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

namespace PolygonalMeshLibrary{
bool importMesh(const string& path, PolygonalMesh& mesh){
    if (!importCell0Ds(path+"./Cell0Ds.csv", mesh))
        return false;
    else{ //test per vedere se i marker delle celle 0D sono corretti
        for (auto& el: mesh.VerticesMarker){
            cout << "key" << el.first << endl;
            for (auto& id: el.second){
                cout << id << " ";
            }
            cout << endl;
        }
    }
    if (!importCell1Ds(path+"./Cell1Ds.csv", mesh))
        return false;
    else{ //test per vedere se i marker delle celle 1D sono corretti
        for (auto& el: mesh.EdgesMarker){
            cout << "key" << el.first << endl;
            for (auto& id: el.second){
                cout << id << " ";
            }
            cout << endl;
        }
    }
    if (!importCell2Ds(path+"./Cell2Ds.csv", mesh))
        return false;
    else{ //test per vedere se i numeri di lati e vertici delle celle 2D coincidono
        for (unsigned int i=0; i<mesh.NumberOfCell2Ds; i++){
            cout << "id: " << mesh.IdCell2Ds[i] << " numero vertici " << (mesh.VerticesCell2Ds[i]).size() << " numero lati " << (mesh.EdgesCell2Ds[i]).size() << endl;
        }
    }
    return true;
}

bool importCell0Ds(const string& fileName, PolygonalMesh& mesh){
    ifstream file(fileName);
    if(file.fail())
        return false;
    string header;
    getline(file, header);
    list <string> lines;
    string line;
    while(getline(file, line))
        lines.push_back(line);
    unsigned int n=lines.size(); //numero di righe=numero di nodi
    mesh.NumberOfCell0Ds=n;
    mesh.CoordinatesCell0Ds.reserve(n); //modifico la capacità, non la dimensione(altrimenti poi facendo il push back lascio liberi tutti i primi n)
    mesh.IdCell0Ds.reserve(n);
    mesh.MarkerCell0Ds.reserve(n);

    for (const string& line : lines){
        istringstream convert(line);
        unsigned int id;
        unsigned int marker;
        char c;
        Vector2d coordinates;
        convert >> id >> c >> marker >> c >>coordinates(0)>> c >>coordinates(1);
        mesh.CoordinatesCell0Ds.push_back(coordinates);
        mesh.IdCell0Ds.push_back(id);
        mesh.MarkerCell0Ds.push_back(marker);

        if(marker!=0){
            auto ret = mesh.VerticesMarker.insert({marker,{id}}); //verifica se ho già inserito il marker come chiave
            if (!ret.second) //il secondo valore restituito è un booleano che dice se c'era già o no
                ((ret.first))->second.push_back(id); //se c'era già la chiave aggiungo alla lista
        }

    }
    file.close();
    return true;}

bool importCell1Ds(const string& fileName, PolygonalMesh& mesh){
    ifstream file(fileName);
    if(file.fail())
        return false;
    string header;
    getline(file, header);
    list <string> lines;
    string line;
    while(getline(file, line))
        lines.push_back(line);
    unsigned int n=lines.size();
    mesh.NumberOfCell1Ds=n;
    mesh.VerticesCell1Ds.reserve(n); //modifico la capacità, non la dimensione(altrimenti poi facendo il push back lascio liberi tutti i primi n)
    mesh.IdCell1Ds.reserve(n);
    mesh.MarkerCell1Ds.reserve(n);

    for (const string& line : lines){
        istringstream convert(line);
        unsigned int id;
        unsigned int marker;
        char c;
        array<unsigned int, 2> extremities;
        convert >> id >> c >> marker >> c >> extremities[0] >> c >> extremities[1];
        mesh.VerticesCell1Ds.push_back(extremities);
        mesh.IdCell1Ds.push_back(id);
        mesh.MarkerCell1Ds.push_back(marker);

        if(marker!=0){
            auto ret = mesh.EdgesMarker.insert({marker,{id}}); //verifica se ho già inserito il marker
            if (!ret.second)
                (ret.first)->second.push_back(id); //se c'era già la chiave aggiungo alla lista
        }

    }
    file.close();
    return true;
}

bool importCell2Ds(const string& fileName, PolygonalMesh& mesh){
    ifstream file(fileName);
    if(file.fail())
        return false;
    string header;
    getline(file, header);
    list <string> lines;
    string line;
    while(getline(file, line))
        lines.push_back(line);
    unsigned int n=lines.size();
    mesh.NumberOfCell2Ds=n;
    mesh.VerticesCell2Ds.reserve(n); //modifico la capacità, non la dimensione(altrimenti poi facendo il push back lascio liberi tutti i primi n)
    mesh.EdgesCell2Ds.reserve(n);
    mesh.IdCell2Ds.reserve(n);

    for (const string& line : lines){
        istringstream convert(line);
        unsigned int id;
        vector<unsigned int> vertices;
        char c;
        unsigned int marker; //lo butto via, tanto so che sono tutti 0
        unsigned int numVertices;
        convert >> id >> c >> marker >> c >> numVertices;
        vertices.resize(numVertices);
        for (unsigned int i=0; i<numVertices; i++){
            convert >> c >> vertices[i];
        }

        vector<unsigned int> edges;
        unsigned int numEdges;
        convert >> c >> numEdges;
        edges.resize(numEdges);
        for (unsigned int i=0; i<numEdges; i++){
            convert >> c >> edges[i];
        }

        mesh.VerticesCell2Ds.push_back(vertices);
        mesh.EdgesCell2Ds.push_back(edges);
        mesh.IdCell2Ds.push_back(id);

    }
    file.close();
    return true;
}


double setTol1D(){
    double tol1D;
    double tolDefault1D = 10* numeric_limits<double>::epsilon();
    double tolUtente1D;
    cout << "Inserire tolleranza 1D desiderata per la mesh: ";
    cin >> tolUtente1D;
    tol1D = max(tolUtente1D,tolDefault1D);
    return tol1D;
}

double setTol2D(const double tol1D){ //non serve passare per referenza i double
    double tol2D;
    double tolDefault2D = 10* numeric_limits<double>::epsilon();
    double tolUtente2D;
    cout << "Inserire tolleranza 2D desiderata per la mesh: ";
    cin >> tolUtente2D;
    tol2D = max(tolUtente2D, tolDefault2D);
    return max(tol2D,(sqrt(3)/4)*pow(tol1D, 2));
}

bool testLengthEdges (const double& tol1D, PolygonalMesh& mesh){
    bool flag=true;
    for (unsigned int& id:mesh.IdCell1Ds){
        auto vertices = mesh.VerticesCell1Ds[id];//vertices contiene gli id degli estremi del segmento
        Vector2d begin = mesh.CoordinatesCell0Ds[vertices[0]];
        Vector2d end = mesh.CoordinatesCell0Ds[vertices[1]];
        if (sqrt(pow(begin[0]-end[0],2)+pow(begin[1]-end[1],2))<tol1D){
            cerr << "Il lato " << id << " ha lunghezza nulla in precisione finita." << endl;
            flag=false;}
    }
    return flag;
}

bool testAreaCells2D (const double& tol2D, PolygonalMesh& mesh){
    bool flag=true;
    for (unsigned int& id:mesh.IdCell2Ds){
        auto vertices = mesh.VerticesCell2Ds[id];
        unsigned int numVertices = vertices.size();
        double area=abs(prodottoVettoriale(mesh.CoordinatesCell0Ds[vertices[0]], mesh.CoordinatesCell0Ds[vertices[numVertices-1]]));
        for (unsigned int i=0; i<(numVertices-1); i++){
            area=area + abs(prodottoVettoriale(mesh.CoordinatesCell0Ds[vertices[i]], mesh.CoordinatesCell0Ds[vertices[i+1]]));
        }
        area=area/2;
        if (area < tol2D){
            cerr << "La cella 2D " << id << " ha area nulla in precisione finita." << endl;
            flag=false;
        }
    }
    return flag;
}

double prodottoVettoriale (const Vector2d& v1, const Vector2d& v2){
    return v1[0]*v2[1]-v1[1]*v2[0];
}


}
