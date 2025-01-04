#include <iostream>
#include <iomanip>

using namespace std;

class Matrix
{
public:
    //----De/Constructor----//
    Matrix();
    Matrix(int _mass) { mass = _mass, Init(); };
    ~Matrix();

    //----IO----//
    int CreateMatrix();
    int Output();

    //----Lab1----//
    Matrix& operator +(Matrix& obj);
    Matrix& operator *(Matrix& obj);
    Matrix& operator xor(Matrix& obj);

    int vertexDegree(int vertex);
    int deleteVertex(int vertex);
    int deleteEdge(int vertex1, int vertex2);
    void vertexClosure(int vertex1, int vertex2);
    void edgeTighter(int vertex1, int vertex2);

    //----Lab2----//
    int SearchInWidth(int startVert, int endVert);
    int SearchInHeight(int* activatedVertices, int endVert, int startVert);

    //----Lab3----//
    int Deixtra(int* activatedVertices, int endVert, int startVert);
    int Deixtra_(int* activatedVertices, int endVert, int startVert);
    int BellmanFord(int* activatedVertices, int endVert, int startVert);
    
    //----Support----//
    int* OfflineVertices(int* vertices, int* activatedVertices);
    int* ActiveVertices();

    friend int GetVertex();

private:
    int** data;
    int mass;
    int Input();
    int Delete();
    int Init();
    int* Copy(int* get);
};

//----De/Constructor----//
Matrix::Matrix()
{
    data = NULL;
    mass = 0;
}

Matrix::~Matrix()
{
    Delete();
}

//----IO----//
int Matrix::Output()
{
    cout << "__|  ";
    for (int i = 0; i < mass; i++)
        cout << "V" << i + 1 << "  ";
    cout << endl;


    for (int i = 0; i < mass; i++)
    {
        cout << "V" << i + 1 << "|";
        for (int j = 0; j < mass; j++)
            cout << setw(4) << data[i][j];
        cout << endl;
    }
    cout << endl;

    return 0;
}

int Matrix::CreateMatrix()
{
    Delete();

    Init();

    Input();

    return 1;
}

int Matrix::Input()
{
    for (int i = 0; i < mass; i++)
    {
        for (int j = 0; j < mass; j++)
        {
            if (j == i) continue;
            cout << "Input mass of edge (" << i + 1 << ", " << j + 1 << ") - ";
            cin >> data[i][j];
        }
    }
    return 1;
}

int Matrix::Init()
{
    data = new int* [mass];
    for (int i = 0; i < mass; i++)
    {
        data[i] = new int[mass];
        for (int j = 0; j < mass; j++)
            data[i][j] = 0;
    }
    return 0;
}

int Matrix::Delete()
{
    if (!data) return 0;
    for (int i = 0; i < mass; i++)
        delete data[i];

    delete data;
    return 0;
}

int* Matrix::Copy(int* get)
{
    int* vertex = new int[mass];

    for (int i = 0; i < mass; i++)
        vertex[i] = get[i];

    return vertex;
}

//----Lab1----//
Matrix& Matrix::operator + (Matrix& obj)
{
    for (int i = 0; i < mass; i++)
    {
        for (int j = 0; j < mass; j++)
        {
            if ((data[i][j] && obj.data[i][j]) || (data[i][j] < obj.data[i][j]))
                data[i][j] = obj.data[i][j];

            else if (obj.data[i][j])
                data[i][j] += obj.data[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator * (Matrix& obj)
{
    for (int i = 0; i < mass; i++)
    {
        for (int j = 0; j < mass; j++)
        {
            if ((data[i][j] && obj.data[i][j]) || (data[i][j] > obj.data[i][j]))
                data[i][j] = obj.data[i][j];

            else
                data[i][j] = 0;
        }
    }
    return *this;
}

Matrix& Matrix::operator xor (Matrix& obj)
{
    for (int i = 0; i < mass; i++)
    {
        for (int j = i; j < mass; j++)
        {
            data[i][j] -= obj.data[i][j];
            data[i][j] = abs(data[i][j]);
            data[j][i] -= obj.data[j][i];
            data[j][i] = abs(data[j][i]);
        }
    }
    return *this;
}

int Matrix::vertexDegree(int vertex)
{
    int counter = 0;

    if (vertex > mass) return counter;

    for (int i = 0; i < mass; i++)
    {
        counter += data[vertex][i];
    }

    return counter;
}

int Matrix::deleteVertex(int vertex)
{
    for (int i = 0; i < mass; i++)
    {
        for (int j = 0; j < mass; j++)
        {
            if (i == vertex || j == vertex) data[i][j] = 0;
        }
    }
    return 1;
}

int Matrix::deleteEdge(int vertex1, int vertex2)
{
    for (int i = 0; i < mass; i++)
    {
        for (int j = 0; j < mass; j++)
        {
            if (i == vertex1 && j == vertex2)
            {
                data[i][j] = 0;
                data[j][i] = 0;
            }
        }
    }
    return 1;
}

void Matrix::vertexClosure(int vertex1, int vertex2)
{
    int globCounter = 0;
    Matrix matrix = { mass - 1 };
    for (int i = 0; i < mass; i++)
    {
        data[vertex1][i] += data[vertex2][i];
        data[vertex2][i] = 0;
    }
    for (int i = 0; i < mass; i++)
    {
        data[i][vertex1] += data[i][vertex2];
        data[i][vertex2] = 0;
    }
    for (int i = 0; i < mass; i++)
    {
        int counter = 0;
        if (i == vertex2)
        {
            globCounter += 1;
            continue;
        }
        for (int j = 0; j < mass; j++)
        {
            if (j == vertex2)
            {
                counter += 1;
                continue;
            }
            matrix.data[i - globCounter][j - counter] = data[i][j];
        }
    }
    data = matrix.data;
    mass = matrix.mass;
}

void Matrix::edgeTighter(int vertex1, int vertex2)
{
    data[vertex1][vertex2] = 0;
    data[vertex2][vertex1] = 0;
    vertexClosure(vertex1, vertex2);
}

//----Lab2----//
int Matrix::SearchInWidth(int startVert, int endVert)
{
    int counter = 1;
    int* vertices = Copy(data[startVert]);

    while (!vertices[endVert] && counter < mass + 1)
    {
        for (int i = 0; i < mass; i++)
        {
            if (vertices[i])
            {
                for (int j = 0; j < mass; j++)
                {
                    vertices[j] += data[i][j];
                }
            }
        }
        counter++;
    }

    if (counter >= mass)
        return 0;

    return counter;
}

int Matrix::SearchInHeight(int* activatedVertices, int endVert, int startVert)
{
    int temp = 0;
    int min = mass;

    activatedVertices[startVert] = 1;

    int* incidence = OfflineVertices(Copy(data[startVert]), activatedVertices);

    for (int i = 0; i < mass; i++)
    {
        if (incidence[endVert]) return 1;

        if (incidence[i])
        {
            temp = SearchInHeight(Copy(activatedVertices), endVert, i);
            if (temp < min) min = temp;
        }
    }

    return min + 1;
}

//----Lab3----//
int Matrix::Deixtra_(int* activatedVertices, int endVert, int startVert)
{
    int temp = 0;
    int min = 100000000;

    activatedVertices[startVert] = 1;

    int* incidence = OfflineVertices(Copy(data[startVert]), activatedVertices);

    if (incidence[endVert]) return incidence[endVert];

    for (int i = 0; i < mass; i++)
    {
        if (incidence[i])
        {
            temp = Deixtra_(Copy(activatedVertices), endVert, i);
            if (!min) min = temp;
            else if (temp < min) min = incidence[i] + temp;
        }
    }

    return min;
}

int Matrix::Deixtra(int* activatedVertices, int endVert, int startVert)
{
    int temp = 0;
    int min = 100000000;

    activatedVertices[startVert] = 1;

    int* incidence = OfflineVertices(Copy(data[startVert]), activatedVertices);

    if (incidence[endVert]) min = incidence[endVert];

    for (int i = 0; i < mass; i++)
    {
        if (incidence[i])
        {
            temp = Deixtra_(Copy(activatedVertices), endVert, i);
            if (!min) min = temp;
            else if (temp < min) min = incidence[i] + temp;
        }
    }

    return min;
}

int Matrix::BellmanFord(int* activatedVertices, int endVert, int startVert) // скажи нет рекурсиям (они очень сильно вредят логике программы)
{
    for (int i = 0; i < mass; i++)
    {
        if (data[startVert][i])
        {
            if (!activatedVertices[i] && i != startVert)
                activatedVertices[i] = data[startVert][i];
            else
            {
                if (activatedVertices[i] > data[startVert][i] + activatedVertices[startVert])
                    activatedVertices[i] = data[startVert][i] + activatedVertices[startVert];
            }
        }
    }
    for (int k = 0; k < mass; k++)
    {
        for (int i = 0; i < mass; i++)
        {
            if (activatedVertices[i])
            {
                for (int j = 0; j < mass; j++)
                {
                    if (data[i][j])
                    {
                        if (!activatedVertices[j])
                            activatedVertices[j] = data[i][j] + activatedVertices[i];
                        else if (activatedVertices[j] > data[i][j] + activatedVertices[i])
                            activatedVertices[j] = data[i][j] + activatedVertices[i];
                    }
                }
            }
        }
    }

    return activatedVertices[endVert];
}

//----Support----//
int* Matrix::OfflineVertices(int* vertices, int* activatedVertices)
{
    for (int i = 0; i < mass; i++)
        if (activatedVertices[i]) vertices[i] = 0;

    return vertices;
}

int* Matrix::ActiveVertices()
{
    int* activatedVertices = new int[mass];

    for (int i = 0; i < mass; i++)
    {
        activatedVertices[i] = 0;
    }

    return activatedVertices;
}

int GetVertex()
{
    int vertex;

    cout << "Input vertex: ";
    cin >> vertex;

    vertex -= 1;

    return vertex;
}

//----Main----//
int main()
{
    Matrix matrix;
    int i;

    while (1)
    {
        cout << endl;
        cout << "1. Create Matrix" << endl
            << "2. Get adjustment matrix of graph" << endl
            << "3. Search in Height" << endl
            << "4. Search in Width" << endl
            << "5. Deixtras algorithm" << endl
            << "6. Belman-Ford algorithm" << endl

            << "0. Exit" << endl << endl;
        cin >> i;
        switch (i)
        {
        case 1: matrix.CreateMatrix(); break;
        case 2: matrix.Output(); break;
        case 3: cout << matrix.SearchInHeight(matrix.ActiveVertices(), GetVertex(), GetVertex()); break;
        case 4: cout << matrix.SearchInWidth(GetVertex(), GetVertex()); break;
        case 5: cout << matrix.Deixtra(matrix.ActiveVertices(), GetVertex(), GetVertex()); break;
        case 6: cout << matrix.BellmanFord(matrix.ActiveVertices(), GetVertex(), GetVertex()); break;
        case 0: return 1;
        default: return 0;
        }
    }
    return 1;
}
