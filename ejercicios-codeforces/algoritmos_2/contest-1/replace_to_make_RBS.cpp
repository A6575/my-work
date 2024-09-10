/*
Estudiante: Astrid Alvarado.
User CodeForce: A_6575
Ejercicio: Replace to Make Regular Bracket Sequence (https://codeforces.com/problemset/problem/612/C)
Veredicto: Accepted

Solucion:
La idea consiste en generar un stack en el cual se irán agregando
los brackets que abren, y se irán sacando del stack en caso de 
que este tenga su pareja o que ya se haya contado para ser remplazado.
Una vez que éste esté vacío y que la cantidad de brackets que abren y 
cierran sean la misma, se retorna la cantidad de remplazos en caso
de haber o "Impossible" en caso de no ser posible convertir el string S
en un RBS.
*/

#include<iostream>
#include<assert.h>
#include<stack>
#include<string>
#include<cstdlib>
#include<cmath>

using namespace std;

//Variables del programa

int replacement = 0;
string RBS;
stack<char> saver;

bool has_pair_RBS(int i){
	/*
	Función que verifica si un bracket tiene su pareja.
	
	Arg:
	   i: número entero mayor o igual a 0 y menos que la longitud del string
	Return:
	   Retorna True si el i-ésimo bracket del string tiene su pareja una vez que 
	   se haya guardado en el stack. En caso contrario, retorna False.
	*/
	
	//Precondición:
	//El número debe ser menor que la longitud del string ingresado y mayor o igual a 0.
	//assert (0<=i<RBS.length());
	
    if(saver.top() == '(' && RBS[i] == ')'){
		return true;
	} else if(saver.top() == '[' && RBS[i] == ']'){
		return true;
	} else if(saver.top() == '{' && RBS[i] == '}'){
		return true;
	} else if(saver.top() == '<' && RBS[i] == '>'){
		return true;
    }
    return false;
}

//Programa Principal 

int main() {
	//Variables que cuentan la cantidad de brackets que abren y cierran.
	int opc = 0, closc = 0;
	
	cin >> RBS;
	
	//Precondición:
	//La longitud del string ingresado debe ser menor or igual a 10^6. 
	//assert (0<RBS.length()<=pow(10,6));
	
	if(RBS[0]=='}' || RBS[0]==')' || RBS[0]=='>' || RBS[0]==']'){
		cout<<"Impossible"<<endl;
	} else{
	    for(int i = 0; i<RBS.length(); i++){
	    	if(RBS[i] == '(' || RBS[i] == '{' || RBS[i] == '<' || RBS[i] == '['){
	    		saver.push(RBS[i]);
	    		opc++;
	    	} else if(!saver.empty() && has_pair_RBS(i)){
	    		saver.pop();
	    		closc++;
	    	} else if(!saver.empty() && (RBS[i] == ')' || RBS[i] == '}' || RBS[i] == '>' || RBS[i] == ']') ){
	    		replacement++;
	    		saver.pop();
	    		closc++;
	    	} else{
	    		closc++;
	    	}
	    }
	    if (saver.empty() && opc == closc){
	    	cout<<replacement;
        }else{
        	cout<<"Impossible"<<endl;
        }
		
	}
    
	return 0;
}
