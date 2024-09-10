/*
Estudiante: Astrid Alvarado
User Spoj: a_7565
Ejercicio: Subset with all Digits (https://www.spoj.com/problems/THECODE/)
Veredicto: Accepted

Soluci�n
La idea consiste en usar una m�scara de bits para poder llevar un registro de aquellas cifras que contengan un n�mero entre el 0 y el 9. De esta forma,
se musca la m�nima cantidad de cifras que consigan cambiar todas las posiciones de las m�scara de bits a 1. Para esto, se utiliza programaci�n din�mica
usando como gu�a la m�scara de bits establecida.
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

#define INF 2e9+11
#define _INF (1<<16)-1

//PROGRAMA PRINCIPAL
int main() {
	// Modificaci�n para la optimizaci�n de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);

	//Variables del programa
    int T, n, d, bitmask;
    string number;
    
    //Ingresamos el n�mero de casos
    cin >> T;
    for(int t=0; t<T; t++){
    	//ingresamos el n�mero de cifras y el n�mero de d�gitos que tendr� cada uno
        cin >> n >> d;
        
        //Se crea un vector de tama�o 2^10, inicialmente con todos sus valores como _INF = (2^16) - 1
        //Se selecciona este valor pues en sistema binario es representado por 1111 1111 1111 1111, es decir
        //una cadena de bits donde todos sus valores se encuentran marcados como 1
        vector<int> dp(1 << 10, _INF);
        //se coloca el valor de 0 en la primera posici�n para iniciar el conteo
        dp[0] = 0;
        
        for(int i = 0; i < n; i++){
        	//ingresamos la cifra
            cin >> number;
            //iniciamos el valor de la m�scara de bits en 0. Cada d�gito en la m�scara de bits representar� un n�mero del 0 al 9, por lo que
            //se tiene que inicialmente la m�scara ser�
			//                    00 0000 0000
			//para los n�meros:   98 7654 3210
			//Con esto, cada posici�n de la m�scara de bits indicar� si se ha encontrado una coincidencia de un n�mero del 0 al 9 en alguna cifra
            bitmask = 0;
            for(int i = 0; i < d; i++){
            	//Iteramos por todos los d�gitos del n�mero dado y se realiza la operaci�n bitmask|2^x, donde x es el d�gito de cada n�mero.
            	//Con esto, se consigue marcar en la m�scara de bits las cifras que contienen los d�gitos que representan el conjunto de [0,9]
                bitmask |= (1 << (number[i] - '0'));
            }
            //Iteramos por el vector de dp buscando la menor cantidad de cifras tales que permitan crear el conjunto de [0,9]
            //Esto se hace al comparar la posici�n j|bitmask con el valor de la j-�sima posici�n aumentado en 1, dado que se va
            //contando la cantidad de cifras encontradas, esto sucesivamente hasta tener el valor deseado en la �ltima posici�n.
            for(int j = 0; j < (1 << 10); j++){
                dp[j | bitmask] = min(dp[j | bitmask], dp[j] + 1);
            }
        }
        //Una vez realizado todos los c�lculos, se verifica si la �ltima posici�n del arreglo dp es distinto de _INF, en ese caso se retorna
        //la cantidad m�nima encontrada de cifras que pertenecer�n al conjunto. En caso contrario, se retorna -1 
        if(dp[(1<<10) - 1] != _INF){
        	cout<<dp[(1<<10)-1]<<endl;
        }else{
        	cout<<-1<<endl;
        }
		
    }   
    return 0;
}
