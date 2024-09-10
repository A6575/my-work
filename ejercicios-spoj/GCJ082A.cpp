/*
Estudiante: Astrid Alvarado
User Spoj: a_7565
Ejercicio: Cheating a Boolean Tree (https://www.spoj.com/problems/GCJ082A/)
Veredicto: Accepted

Soluci�n:
La idea consiste en utilizar el principio de Programaci�n Din�mica para as� obtener la cantidad m�nima de cambios necesarios para 
que el nodo ra�z del �rbol sea del valor deseado. Para esto, se va buscando en distintas combinaciones en entre los nodos y comparando
los resultados en distintos casos para conseguir el m�nimo valor de cambios.
*/
#include <iostream>
#include<vector>
#include<queue>
#include<stack>
#include <string>
#include<algorithm>
#include<set>
#include <cmath>

using namespace std;

#define INF 1e9+11
#define _INF (1<<16)-1

//Variables Globales
const int MaxM = 1e4+5;		//n�mero m�ximo de nodos
/* Matriz con el n�mero m�ximo de nodos como filas y 3 columnas para recrear el boolean tree.
 * Se tiene que para cada fila, las columnas representan: 
 * 		- columna 0 >> tipo de puerta (1 para AND gate, 0 para OR gate, -1 si es nodo hoja)
 * 		- columna 1 >> si es posible cambiar la puerta (1 para indicar que s�, 0 para indicar que no , -1 si es nodo hoja)
 * 		- columna 2 >> valor booleano de la puerta (1 para TRUE, 0 para FALSE, -1 para indicar que no ha sido calculado)
*/
int boolean_tree[MaxM][3];

//funci�n auxiliar que permite obtener el valor m�nimo entre tres n�meros enteros
int min_of_three(int a, int b, int c){
	return min(min(a, b), c);
}

//funci�n resursiva para obtener el n�mero m�nimo de cambios necesarios para que el nodo raiz tenga el valor deseado
int min_change(int root, int value, int nodes){
	/*Arreglo de pares de tama�o 2. Aqu� se almacenan la cantidad de formas en las que los hijos del nodo ra�z pueden
	 *tener distintos valores. Se tiene que:
	 *		- posici�n 0 >> cantidad de formas en la que los nodos hijos pueden tener el valor de 0  
	 *		- posici�n 1 >> cantidad de formas en la que los nodos hijos pueden tener el valor de 1 
	 *Y siempre teniendo en cuenta que para cada par:
	 *		- La primera posici�n representa al nodo que se encuentra a la izquierda 
	 *		- La segunda posici�n representa el nodo que se encuentra a la derecha
	*/
	pair<int, int> childs[2];
	
	//variables auxiliares para determinar la cantidad de formas en las que un nodo puede obtener distintos valores
	int found = INF,
		changed_found = INF;
	
	//Caso base: verificar los valores para los nodos hoja de �rbol
	if(root > (nodes-1)/2){
		//Si el valor que buscamos cambiar es igual al establecido, se dice que no hay cambios necesarios para
		//obtener dicho valor, por lo que se retorna 0. En caso contrario, se establece el valor de INF para 
		//indicar que es imposible obtener este valor
		if(value == boolean_tree[root][2]){
			return 0;
		}else{
			return INF;
		}
	}
	
	//Caso recursivo: buscar la cantidad de cambios necesarios para que un nodo tenga distintos valores
	//con esto, buscamos ver las diversos casos posibles para cada nodo en obtener TRUE o FALSE
	childs[0].first = min_change(2*root, 0, nodes);
	childs[1].first = min_change(2*root, 1, nodes);
	childs[0].second = min_change(2*root+1, 0, nodes);
	childs[1].second = min_change(2*root+1, 1, nodes);
	
	//Si el nodo ra�z es una AND gate
	if(boolean_tree[root][0] == 1){
		//y adem�s queremos que el valor del nodo ra�z sea igual a TRUE, basta con ver la suma de la cantidad de formas  
		//para que sus nodos hijos sean 1. En caso de que el valor buscado para el nodo ra�z sea FALSE, buscamos la  
		//cantidad m�nima de formas en la que sus nodos hijos permitan que su nodo padre sea FALSE.
		if(value==1){
			found = childs[1].first + childs[1].second;
		}else{
			found = min_of_three(childs[1].first + childs[0].second, childs[1].second + childs[0].first, childs[0].first + childs[0].second);
		}
	//Si el nodo ra�z es una OR gate
	}else{
		//y adem�s queremos que el valor del nodo ra�z sea igual a FALSE, basta con ver la suma de la cantidad de formas  
		//para que sus nodos hijos sean 0. En caso de que el valor buscado para el nodo ra�z sea TRUE, buscamos la  
		//cantidad m�nima de formas en la que sus nodos hijos permitan que su nodo padre sea TRUE.
		if(value==0){
			found = childs[0].first + childs[0].second;
		}else{
			found = min_of_three(childs[1].first + childs[0].second, childs[1].second + childs[0].first, childs[1].first + childs[1].second);
		}
	}
	//Luego, si es posible cambiar el tipo de puerta del nodo ra�z
	if(boolean_tree[root][1] == 1){
		//Y esa puerta es una OR gate, se hace el mismo procedimiento para una AND gate
		if(boolean_tree[root][0] == 0){
			if(value==1){
				changed_found = childs[1].first + childs[1].second;
			}else{
				changed_found = min_of_three(childs[1].first + childs[0].second, childs[1].second + childs[0].first, childs[0].first + childs[0].second);
			}
		//En caso de que sea una AND gate, se hace el mismo procedimiento para una OR gate
		}else{
			if(value==0){
				changed_found = childs[0].first + childs[0].second;
			}else{
				changed_found = min_of_three(childs[1].first + childs[0].second, childs[1].second + childs[0].first, childs[1].first + childs[1].second);
			}
		}
	}
	//Finalmente, se compara entre ambos valores encontrados. Es necesario tener en cuenta que en caso de haber conseguido cierto
	//valor cambiando una puerta, a la cantidad encontrada se aumenta en uno para indicar el paso extra realizado
	found = min(found, changed_found + 1);

	//En caso de que el valor encontrado sea mayor o igual a INF, se resetea como INF 
	if(found >= INF){
		found = INF;
	}
	//Finalmente se retorna la cantidad encontrada
	return found;
}

//PROGRAMA PRINCIPAL
int main(){
	// Modificaci�n para la optimizaci�n de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);

	//Variables del programa
	int N, M, V, sol;
	//Se ingresa la cantidad de casos
	cin>>N;
	for(int n=0; n<N; n++){
		//se ingresa el n�mero de nodos y el valor deseado de la ra�z
		cin>>M>>V;
		
		//Iteramos por el n�mero de nodos
		for(int i=1; i<M+1; i++){
			//en caso de que i sea menor a (M-1)/2+1, se lee los datos correspondientes a los
			//nodos que son puerta. En caso contrario, se lee los datos de los nodos hoja
			if(i<(M-1)/2 + 1){
				cin>>boolean_tree[i][0]>>boolean_tree[i][1];
				boolean_tree[i][2] = -1;
			}else{
				boolean_tree[i][0] = -1; boolean_tree[i][1] = -1;
				cin>>boolean_tree[i][2];
			}
		}
		
		//Obtenemos la cantidad m�nima de cambios
		sol = min_change(1, V, M);
		
		//Y mostramos la soluci�n. En caso de que sea distina a INF, entonces se retorna el n�mero de cambios m�nimos.
		//De otro modo, retornamos que es imposible
		cout<<"Case #"<<n+1<<": ";
		if(sol != INF){
			cout<<sol<<endl;
		}else{
			cout<<"IMPOSSIBLE"<<endl;
		}
		
	}
	
	return 0;
}
