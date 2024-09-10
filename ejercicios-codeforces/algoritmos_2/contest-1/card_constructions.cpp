/*
Estudiante: Astrid Alvarado
User CodeForce: A_6575
Ejercicio: Card Constructions (https://codeforces.com/problemset/problem/1345/B)
Veredicto: Accepted

Solucion:
La idea consiste en ir contando la cantidad de pirámides
que se pueden realizar con n cartas. Para ello se emplea
binary search para calcular la cantidad niveles que pueden
realizarse con las cartas ingresadas. Luego, iremos contando
la cantidad de pirámides se pueden hacer con las cartas restantes
*/

#include<iostream>
#include<assert.h>
#include<cmath>

using namespace std;

bool piramides(long long N, long long m){
	
	/*
	Note que 2 + 7 + 15 + 26 + ... + N = (N*(3*N+1))/2
	es la cantidad de cartas necesarias para hacer una pirámide
	de N pisos.
	*/
	return ( (m*(3*m + 1))/2 <= N );
}

int binary_search_p(long long val){
	/*
	Buscamos un punto medio mid, luego, si con esa cantidad
	de cartas es posible realizar una pirámide, entonces es posible 
	realizar más pirámides con los números inferiores a ese. Sino, se
	reduce el rango por la derecha. Finalmente, retornamos cualquiera
	de los lados.
	*/
	int l = 0;
	int r = pow(10,6);
	
	while(l<r){
		int mid = (l+r+1)/2;
		
		if(piramides(val, mid)){
			l = mid;
		}else{
			r = mid-1;
		}
	}
	return l;
}

//Programa principal

int main(){
	int t;
	long long n;
	int counter;
	
	cin >> t;
	for (int i=0; i<t; i++){
		cin >> n;

		counter = 0;
		while( (n > 1) && (n >= (binary_search_p(n)*(3*binary_search_p(n) +1))/2) ){
			counter++;
			n = n - (binary_search_p(n)*(3*binary_search_p(n) +1))/2;
		}
		cout<<counter<<endl;
	}
    return 0;
}
