/*
Estudiante: Astrid Alvarado
User CodeForces: A_6575
Ejercicio: Min Max Sort (https://codeforces.com/problemset/problem/1792/C)
Veredicto: Accepted

Soluci�n:
La idea consiste en organizar el arreglo siguiendo el algoritmo empezando por los valores minimo y m�ximo m�s peque�o, es decir,
aquellos que formen un "subarreglo" interno. Por lo que, se divide en dos mitades el arreglo y se busca aplicar el algoritmo
analizando diversos casos.
*/
#include<iostream>
#include<queue>
#include<stack>
#include<string>
#include<algorithm>
#include<set>

using namespace std;

#define INF 2e9+11

//Variables globales
const int MaxN = 2*1e5+5;
int array_p[MaxN],			//arreglo que se desea ordenar aplicando el algoritmo
	index[MaxN];			//arreglo de �ndices correspondiente al n�mero

int min_max_sort(int n){
	int min_op = n/2;		//el n�mero de operaciones totales que se pueden realizar dado que se requieren ordenar por pares
	int middle[2] = {(n+1)/2, (n+2)/2};		//se debe ordenar el algoritmo revisando el arreglo en dos mitades, para as� poder
											//calcular desde el rango m�s peque�o al m�s grande
	int index_aux[2] = {index[middle[0]], index[middle[1]]};	//se busca los �ndices de estas mitades
	//arreglo auxiliar para almacenar los �ndices anteriores
	int prev_indx[2];
	
	//en caso de que la cantidad de n�meros a ordenar sea impar, es necesario verificar que
	if(n%2 == 1){
		//si el �ndice del �ltimo n�mero de la primera mitad es menor al calculado y adem�s
		//el �ndice del primer n�mero de la segunda mitad es mayor al �ndice calculado, es
		//decir que, por transitividad, el �ndice del �ltimo n�mero de la primera mitad es menor
		//al �ndice del primer n�mero de la segunda mitad
		if(index[middle[0]-1]<index_aux[0] && (index_aux[0] < index[middle[0]+1])){
			//se actualiza las mitades para poder tener acceso a ambas partes
			middle[0]--;
			middle[1]++;
			//se toman los �ndices de las nuevas mitades
			index_aux[0] = index[middle[0]];
			index_aux[1] = index[middle[1]];
		}else{
			//en caso de que esta transitividad no ocurra, la �nica forma de ordenarlo es realizando el 
			//n�mero m�ximo de operaciones
			return min_op;
		}
	}
	//se guardan los valores previos de los �ndices obtenidos
	prev_indx[0] = index_aux[0]; prev_indx[1] = index_aux[1];
	//mientras que el �ndice correspondiente a la primera mitad sea menor o igual al �ndice previo de este
	//y que adem�s el �ndice previo de la segunda mitad sea menor o igual al �ndice correspondiente de este
	//y adem�s que nos sigamos mantengamos en el rango deseado
	while((index_aux[0] < prev_indx[0] + 1) && (prev_indx[1] < index_aux[1] + 1) && prev_indx[0] < prev_indx[1]){
		//guardamos los �ndices anteriores
		prev_indx[0] = index_aux[0];
		prev_indx[1] = index_aux[1];
		//reducimos la cantidad m�nima de operaciones pues hemos hayado una forma con menos pasos a tomar
		min_op--;
		//actualizamos las mitades correspondientes junto a su �ndice
		middle[0]--;
		middle[1]++;
		index_aux[0] = index[middle[0]];
		index_aux[1] = index[middle[1]];
		//al momento en que hayamos revisado totalmente la primera mitad, terminamos el ciclo
		if(middle[0] == 0){
			break;
		}
	}
	//finalmente se retorna las operaciones.
	return min_op;
}

int main(){
	// Modificaci�n para la optimizaci�n de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//variables del programa
	int T, n;
	//se ingresa el n�mero de casos
	cin>>T;
	for(int t=0; t<T; t++){
		//se ingresa la cantidad de n�meros a ordenar
		cin>>n;
		
		//se almacena el arreglo y su �ndice correspondiente
		for(int i=0; i<n; i++){
			cin>>array_p[i];
			index[array_p[i]] = i;
		}
		//si el arreglo ya se encuentra ordenado, simplemente se imprime 0
		if(is_sorted(begin(array_p), end(array_p))){
			cout<<0<<endl;
			continue;
		}else{
			//en otro caso llamamos a la funci�n min_max_sort
			cout<< min_max_sort(n) << endl;
		}
		
	}
	
	return 0;
}
