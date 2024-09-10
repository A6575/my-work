/*
Estudiante: Astrid Alvarado
User CodeForce: A_6575
Ejercicio: Sushi for two (https://codeforces.com/problemset/problem/1138/A)
Veredicto: Accepted

Solucion:
La idea consiste en ir recorriendo el vector que contiene
la orden de sushi (sushi_pieces) e ir almacenando en un vector las veces en las
que hay 1 o 2 consecutivos (helper), consiguiendo así tener el vector
original segmentado. Al final, iremos comparando elemento por 
elemento de helper, guardaremos el doble del menor valor e iremos buscando
el máximo valor entre estos. Finalmente, retornaremos este resultado.
*/

#include<iostream>
#include<assert.h>
#include<vector>

using namespace std;

int main(){
	
	//Variables del programa
	
	int n, sushi_roll;
	
	//Precondición:
	//n debe estar en el intervalo [2,10^5] y los tipos de sushi ser 1 o 2
	//assert( (2<=n<= 100000) && ( (sushi_roll == 1) || (sushi_roll == 2) ))
	
	int m = 0, len = 0, answer = 0;
    vector<int>sushi_pieces;
    vector<int>helper;
    
    //Inicializamos el vector helper con todos sus valores en 1.
    //Esto debido a que se garantiza que hay al menos una pieza
    //de sushi de cada tipo:
    for(int i=0; i<100002; i++){
    	helper.push_back(1);
    }
    
    cin>>n;
    for(int i=0; i<n; i++){
    	cin>>sushi_roll;
    	sushi_pieces.push_back(sushi_roll);
    }
    
    //Recorremos el vector, y si hay números consecutivos,
    //se aumentará en 1 los helper[i], consiguiendo de este modo
    //obtener un vector donde cada valor será la longitud de un 
	//subgrupo de números consecutivos en la orden de sushi.
    for(int i=1; i<n; i++){
    	if(sushi_pieces[i-1]==sushi_pieces[i]){
    		helper[m]++;
    	}else{
    		m++;
    	}
    }
    
    //Comparamos dichas longitudes y vemos cuál es el menor,
    //se duplica la cantidad e iremos guardando en una variable el
    //máximo entre estos valores, obteniendo así el resultado final
    for(int i=1; i<=m; i++){
    	if(helper[i-1] >= helper[i]){
    		len = 2*helper[i];
    		answer = max(answer,len);
    	}else if(helper[i]>helper[i-1]) {
    		len = 2*helper[i-1];
    		answer = max(answer,len);
        }
    }
    
    cout<<answer<<endl;
    
	return 0;
}
