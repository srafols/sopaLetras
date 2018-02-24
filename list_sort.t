#include "list_sort.hpp"



namespace list_sort{



template<typename T>
void fusiona(list<T>& L, list<T>& M) throw(); 

template <typename T>
list<T>::iterator parteix(list<T>& L) throw();



template <typename T>

void sort(list<T>& L) throw(){

if (L.size()>1){


	list<T> M;
	list<T>::iterator meitat=parteix(L);
	L.splice(M.begin(), M, meitat, L.end());	
	sort(L);
	sort(M);
	fusiona(L,M);

}


}



template <typename T>

list<T>::iterator parteix(list<T>& L) throw(){

	
	list<T>::iterator meitat=L.begin();
	nat cont=0;
	while(cont<(L.size())/2){
	++meitat;
	++cont;
	}
return meitat;

}



template <typename T>

void fusiona(list<T>& L, list<T>& M) throw() {


	  list<T>::iterator it1=L.begin(); 
	  list<T>::iterator it2=M.begin();
	list<T> N;	



while (it1!=L.end() && it2!=M.end()){
	
	if (*it1 >= *it2){
		N.push_back(*it2);
		it2++;
		M.pop_front();
		
		
	}
	else{
		N.push_back(*it1);
		it1++;	
		L.pop_front();
		
		
		}
	}


if (it1==L.end() && it2!=M.end()) N.splice(N.end(),M);
if (it2==M.end() && it1!=L.end()) N.splice(N.end(),L);
L=N;

N.clear();
M.clear();

}



};








