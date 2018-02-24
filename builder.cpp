#include "builder.hpp"
#include "list_sort.hpp"
#include <iostream>
using namespace std;

namespace builder{

  
  bool  build_rec (list<hueco>::const_iterator it, const diccionario& dic,  sopa_letras& aux, const list<hueco>& forats) throw(error); 
  void omplir_aleatori(sopa_letras& soap) throw(error); 


 

  void build(const diccionario& D, const list<hueco>& Huecos, list<match>& L, sopa_letras& S) throw(error){
    
    sopa_letras aux=S;
    bool construible;

    if(Huecos.size()>0){
    
      try{
       construible=build_rec(Huecos.begin(), D, aux, Huecos);     
      }
      catch(error){
	throw error(nom_mod, HuecosNoColocables, MsgHuecosNoColocables);
      }
    
    


      if(construible){       

	//posem els match a la llista
	S=aux;
	list<hueco>::const_iterator ite=Huecos.begin();
	
	while(ite!=Huecos.end()){

	  string wd;
	  wd=S.cadena((*ite).fila(), (*ite).columna(), (*ite).longitud(), (*ite).orient());
	  L.push_back(match(wd, (*ite).fila(), (*ite).columna(), (*ite).orient()));
	  ++ite;
	}   
  
	list_sort::sort(L);
	omplir_aleatori(S);

      }
    
    
    }
      
       
  }  







  bool  build_rec (list<hueco>::const_iterator it, const diccionario& dic,  sopa_letras& aux, const list<hueco>& forats) throw(error){
    
    
   
    bool b=false;
    
    if(it== forats.end()) b=true;
    else {
        
      string par=aux.cadena((*it).fila(), (*it).columna(), (*it).longitud(), (*it).orient());
    
      list<string> p_patrons;

      dic.satisfacen_patron(par, p_patrons);
   
      if (p_patrons.size()==0) b=false;
      else{

	list<string>::iterator ip=p_patrons.begin();
      
      
	while(!b && ip!=p_patrons.end()){
     
	  aux.inserta_cadena( *ip ,(*it).fila(), (*it).columna(), (*it).orient());
	  b=build_rec(++it, dic, aux, forats);
    
	  --it;
	  if (!b) aux.inserta_cadena(par,(*it).fila(), (*it).columna(), (*it).orient());  
	  ++ip;
     
   
       
	}
  
      }
    }
    return b;

}



  void omplir_aleatori(sopa_letras& soap) throw(error){
    string ale;
    srand((unsigned int) time(NULL));
    for(nat i=0; i<soap.num_filas(); ++i){

      for(nat j=0; j<soap.num_columnas(); ++j){
	
	
	if(soap.cadena(i, j, 1, orientacion::H)=="*"){
	  ale='A' + (rand()%('Z' - 'A'));
	  soap.inserta_cadena(ale, i, j, orientacion::H);
	
	}
      }

    }
  }


};










































