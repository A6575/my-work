/*
Estudiante: Astrid Alvarado
User CodeForce: A_6575
Ejercicio: Christmas Spruce (https://codeforces.com/problemset/problem/913/B)
Veredicto: Accepted

Solucion:
La idea consiste en generar un array p donde se almacena el árbol ingresado, en
donde el valor del elemento representa el valor del padre, y las veces que
se repite, la cantidad de hijos que tiene. La cantidad de hijos que posee cada nodo
se almacena en el array nodes, y luego se genera otro array leaf_nodes donde se 
almacena la cantidad de hijos que son hojas de cada nodo. Iterando en el array nodes
y leaf_nodes, se verifica que si nodes[i] es mayor a 0 y que además leaf_nodes[i] es
mayor o igual a 3, cumple con la definición de "abeto" y por lo tanto retorna "Yes",
en caso contrario, si nodes[i] es mayor a 0 y leaf_nodes[i] es menor a 3, retorna "No".
*/

#include<iostream>
#include<assert.h>
#include<vector>

using namespace std;

int main(){
	
	// Variables del programa
	int n, m;
	
	cin >> n;
	// Precondición:
	// n debe estar en el intervalo [n, 1000]
	// assert(3<=n<=1000);
	
	int p[n],  
	    nodes[n],
		leaf_nodes[n];
	
	// Inicialización de los array con el valor de 0
	for(int i=0; i<n; i++){
		p[i] = 0;
		nodes[i] = 0;
		leaf_nodes[i] = 0;
	}
	
	// Se almacenan los valores ingresados en el array p
	// y se cuentan las veces que los valores repetidos
	// aparece, almacenándolo en el array nodes
	for(int i = 1; i<n; i++){
		cin >> m;
		p[i] = m;
		nodes[p[i]-1]++;
	}
	
	// Se cuenta la cantidad de hijos hojas tiene el nodo padre.
	// Dado que nodes almacena la cantidad de nodos que tiene cada
	// nodo padre, si alguno de estos valores es 0, quiere decir que
	// dicho nodo no tiene hijos y por lo tanto es una hoja del árbol
	for(int i = 0; i<n; i++){
		if(nodes[i] == 0){
			leaf_nodes[p[i]-1]++;
	    }
	}
	
	// Se revisa que cumpla con la definición de abeto.
	// Iteramos sobre los arrays nodes y leaf_nodes,  
	// si la cantidad de hojas es menor a 3, entonces
	// se retorna "No" y termina el programa.
	for(int i = 0; i<n; i++){
		if(nodes[i] > 0 && leaf_nodes[i] < 3){
			cout<<"No";
			return 0;
		}
	}
	cout<<"Yes";
	return 0;
}
