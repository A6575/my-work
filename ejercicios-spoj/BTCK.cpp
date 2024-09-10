/*
Estudiante: Astrid Alvarado
User Spoj: a_7565
Ejercicio: A problem of Backtracking (https://www.spoj.com/problems/BTCK/)
Veredicto: Accepted

Soluci�n:
La idea consiste en utilizar una m�scara de bits para llevar registro de los valores permutados tales que cumpla la condici�n.
Por lo tanto, se contempla todas las posibles permutaciones tales que no se pase del valor de K dado y se va marcando en la 
m�scara de bits los n�meros ya permutados hasta conseguir que todas las posiciones se encuentren en 1 y que dicha suma siga 
siendo menor o igual a K
*/
#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<string>
#include<algorithm>
#include<set>

using namespace std;

#define INF 2e9+11

//Variables globales del programa
long long K, n_i[11], sol[11];
int T;

//funci�n que permite conseguir la permutaci�n m�s peque�a lexicogr�ficamente usando m�scara de bits
bool permutation(long long A, int _mask, int count = 0){
	//Caso base: en caso de que la m�scara de bits sea igual a 11 1111 1111 y adem�s el acumulado sea
	//menor o igual a K, se retorna que se pudo realizar la permutaci�n correctamente
	if (_mask == (1 << 10) - 1 && A < K+1){
		return true;
	}
	//Caso recursivo: conseguir para cada n�mero su permutaci�n
	for(int i=0; i<10; i++){
		//en caso de que _mask&2^i sea 0, quiere decir que dicho valor no ha sido marcando a�n
		if((_mask & (1 << i)) == 0){
			//En caso de que el acumulado sumado a la multiplicaci�n de alg�n valor entre 1 y 10 con los valores ingresados
			//sobrepase el valor de K, se retorna que no es posible realizar la permutaci�n
			if(A + n_i[count]*i > K){
				return false;
			}
			
			//se guarda en vector de soluci�n el valor de i, la permutaci�n actual hallada
			sol[count] = i;
			
			//En caso de que se pueda realizar la siguiente permutaci�n, se retorna TRUE.
			//La siguiente permutaci�n se calcular� actualizando el acumulado como indica el problema, actualizando
			//la m�scara de bits correspondiente y aumentando el contador en 1 para guardar correctamente la 
			//permutaci�n en el arreglo de soluci�n
			if(permutation(A + n_i[count]*i, _mask + (1<<i), count+1)){
				return true;
			}
		}
	}
	//si no se consigui� la permutaci�n se retorna FALSE
	return false;
}

//PROGRAMA PRINCIPAL
int main(){
	// Modificaci�n para la optimizaci�n de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//Se ingresa el n�mero de casos
	cin>>T;
	for(int t=0; t<T; t++){
		//Se ingresan los 10 n�meros positivos
		for(int i=0; i<10; i++){
			cin>>n_i[i];
		}
		//Se ingresa el valor de K
		cin>>K;
		//Si la permutaci�n es posible, se retorna dicha permutaci�n, en caso contrario se retorna -1
		if(permutation(0,0,0)){
			for(int j=0; j<10; j++){
				cout << sol[j] << " "; 
			}
			cout<<endl;
		}else{
			cout<< -1 << endl;
		}
	}
	
	return 0;
}
