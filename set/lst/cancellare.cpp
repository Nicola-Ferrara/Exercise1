#include "setlst.hpp"
#include <iostream>
using namespace std;
using namespace lasd;

int main() {
    lasd::SetLst<int> lista;

    lista.Insert(5);
    lista.Insert(2);
    lista.Insert(8);
    lista.Insert(3);
    lista.Insert(5); // Non verrà inserito perché già presente

    // Stampa la lista ordinata
    lista.Traverse([](const int& val) {
        cout << val << " ";
    });
    cout << endl;

    return 0;
}