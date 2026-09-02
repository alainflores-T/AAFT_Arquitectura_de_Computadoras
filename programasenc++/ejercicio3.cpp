#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main()
{
    // 1. Crear archivo con 1000 enteros
   /* int datos[1000];

    for (int i = 0; i < 1000; i++)
        datos[i] = i;

    ofstream salida("datos.bin", ios::binary);

    salida.write(
        reinterpret_cast<char*>(datos),
        sizeof(datos)
    );

    salida.close();

    cout << "Archivo creado." << endl;*/


    // 2. Leer el archivo
    int arreglo1[1000];

    ifstream entrada("datos.bin", ios::binary);

    entrada.read(
        reinterpret_cast<char*>(arreglo1),
        sizeof(arreglo1)
    );

    entrada.close();


    // 3. Segundo arreglo
    int arreglo2[100];

    // Copiar 500-599
    memcpy(
        arreglo2,
        &arreglo1[500],
        100 * sizeof(int)
    );

    // Limpiar originales
    memset(
        &arreglo1[500],
        0,
        100 * sizeof(int)
    );


    // 4. Mostrar primeros y últimos 5
    cout << "Primeros 5: ";

    for (int i = 0; i < 5; i++)
        cout << arreglo1[i] << " ";

    cout << endl;

    cout << "Ultimos 5: ";

    for (int i = 995; i < 1000; i++)
        cout << arreglo1[i] << " ";

    cout << endl;


    // 5. Verificar que 500-599 sean 0
    bool correcto = true;

    for (int i = 500; i <= 599; i++)
    {
        if (arreglo1[i] != 0)
        {
            correcto = false;
            break;
        }
    }

    if (correcto)
        cout << "Indices 500-599 correctamente puestos en 0." << endl;
        
        
        cout<<"elemento 500 "<< arreglo1[500]<<"\n";

    return 0;
}
