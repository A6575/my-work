/*
Estudiante: Astrid Alvarado
User CodeForce: A_6575
Ejercicio: Misha and Changing Handle (https://codeforces.com/problemset/problem/501/B)
Veredicto: Accepted

Solucion:
la idea consiste en almacenar en un map los users ingresados
OLD y NEW, representando un par (OLD, NEW), donde OLD representa el key
y NEW el value correspondiente a esa key, al mismo tiempo que verificamos si 
existe algún value igual al OLD user actual, y en caso de haberlo,
cambiambos el value con el NEW user actual ingresado.
*/

#include<iostream>
#include<vector>
#include<string>
#include<map>

using namespace std;

int main(){
	//Inicializando varaibles
	string OLD, NEW;
	map<string, string> users;
	map<string, string>::iterator k;
	int q;
	cin>>q;
	for(int i=0; i<q; i++){
		bool has_changed = false; //Variable para saber si algún user volvió a cambiar el mismo
		cin>>OLD>>NEW;
		//Iteramos sobre el map
		for(k = users.begin(); k != users.end(); k++){
			//Si existe algún value igual a algún OLD user, marcamos que hubo un cambio
			if(k->second == OLD){
				has_changed = true;
			}
		}
		//Si no cambió, agregamos al map
		if(!has_changed){
			users[OLD] = NEW;
		}else{
			//En cambio, si hubo algún otro cambio, cambiamos el value de dicho OLD user
			for(k = users.begin(); k != users.end(); k++){
				if(k->second == OLD){
					k->second = NEW;
			    }
		    }
		}
	}
	//Retornamos el tamaño final del map luego de los cambios
	cout<<users.size()<<endl;
	for(k = users.begin(); k != users.end(); k++){
		//Y los valores del mismo
		cout<<k->first<<" "<<k->second<<endl;
	}
	return 0;
}
