#include "diccionario.hpp"



diccionario::diccionario() throw(error){
  
 
  
  arrel=new node(); 
  arrel->info='\0'; 
  arrel->esq=NULL;  
  arrel_real=arrel->centre=NULL;  
  arrel->dret=NULL;
  arrel-> final=true;
  
  
}



diccionario::node*  diccionario::inserta_tst( node* primer, const string& par, unsigned int pos) throw(error){
  
  if(primer==NULL){
     if( pos<par.length()){
       
       
       node* aux=new node();
       aux->info= par[pos];
       aux->esq=NULL;
       aux->dret=NULL;
      
     




       if(pos==par.length()-1){ aux->final=true;}
       else{ aux->final=false;}
      
       
       try{
	 aux->centre=inserta_tst(NULL, par, pos+ 1);
       }
       catch(error){
	 delete aux;
	 throw;
       }


       return aux;

     }
     else{ return NULL;
      
     }
  


     }

  else{
    
    
    if(primer->info==par[pos]){ 
      if(pos==par.length()-1){ primer->final=true;}
      
      primer->centre=inserta_tst(primer->centre, par, pos+1);  
      
    }
    
    
    if(primer->info > par[pos]){ 
      
      primer->esq=inserta_tst(primer->esq, par, pos);
      
    }
    
    

    if(primer->info < par[pos]){ 
      
      primer->dret=inserta_tst(primer->dret, par, pos);
     
    
    }

    return primer;
  }

}




void diccionario::borra_tst(node* first) throw(){

  if (first==NULL) return;
 
  borra_tst(first->esq);
  borra_tst(first->centre);
  borra_tst(first->dret);

  delete first;


}




diccionario::node* diccionario::copia_tst(const node* origen) throw(error){

  if (origen==NULL){ return NULL;}
  
  node* aux=new node();
  aux->info= origen->info;
  aux->final=origen->final;

  try{
    aux->esq=copia_tst(origen->esq);
  }
  catch(error){
    delete aux;
    throw;
  }
  
  
  try{
    aux->centre=copia_tst(origen->centre);
  }
  catch(error){
    borra_tst(origen->esq);
    delete aux;
    throw;
  }
  
   try{
    aux->dret=copia_tst(origen->dret);
  }
  catch(error){
    borra_tst(origen->esq);
    borra_tst(origen->centre);
    delete aux;
    throw;
  

  }
  
   return aux;

}




diccionario::diccionario(const diccionario& D) throw(error){

   arrel=copia_tst(D.arrel);
   arrel_real=arrel->centre;
}




diccionario& diccionario::operator=(const diccionario& D) throw(error){

  if(this!=&D){
    
    node* aux=copia_tst(D.arrel);
    borra_tst(arrel);
    arrel=aux;
    arrel_real=arrel->centre;

  }

  return *this;

}




diccionario:: ~diccionario() throw(){

  borra_tst(arrel_real);
  delete arrel;
}







void diccionario::inserta(const string& p) throw(error){

  
   arrel_real=  inserta_tst(arrel_real, p, 0);
  arrel->centre=arrel_real;
   
 

}

 



void diccionario::busca_paraula(const node* ori, const string& word, unsigned int pos, bool fi, string& cons, string& aux) throw(error){

 
 
  // cout << "final de paraula trobat " << fi << endl;
 
  if(ori==NULL && fi==true){ 
    //cout<< "estem al final" << endl;
    return;
  }
  
  
  if(ori!=NULL && fi==true){
   
    aux=cons;
    //cout << cons << endl;
  }  
  
  
  if(ori==NULL && fi==false){ 
  
    //cout << " no final de paraula "<< endl;
    cons=aux;	
    return;
  }

  
  
  if(pos>=word.length()&& fi==false){ 
    //cout << "prefix + curt" << endl;
    cons=aux; return;
  }
  
  
  
  if(pos<word.length()){
	    
    
    
    if(word[pos]==ori->info){
       
      // cout << "en el centre " <<  ori->info << endl;
       cons=cons+  ori->info;
       fi=ori->final;
       busca_paraula(ori->centre, word, pos+1, fi, cons, aux);
       
      		
     
    }
    
 

    if(word[pos]>ori->info){
      //cout << " a la dreta " << ori->info << endl;
      busca_paraula(ori->dret, word, pos, fi, cons, aux);
      


    }
    
    
    if(word[pos]< ori->info){
      //cout << " a l'esquerra " << ori-> info << endl;
      busca_paraula(ori->esq, word, pos, fi, cons, aux);
      

    }
 
  
  }


}




string diccionario::prefijo(const string& p) const throw(error){


  string aux; //sutilitza en el cas de paraula intermitja
  string consulta;
  busca_paraula(arrel_real, p, 0, false,  consulta, aux);


  return consulta;
  


}


void diccionario::buscar_patron(const string& pa, list<string>& Li, const node* first,unsigned int pos, string spatron, bool fi) throw (error){

  //cout << " string ara val: "<<  spatron << endl;

 

  if(first==NULL &&   fi==false){
    //cout << " no inserem res a la llista" << endl;
    return;
  }
  
    
  if(first==NULL && pos>=pa.length() &&  fi==true){
    //cout << "insertem string a la llista" << endl;  
    Li.push_back(spatron);
  }  

  
  
  if(first!=NULL && pos>=pa.length() && fi==true){

    //cout << "insertem string a la llista, no de keden chars al patro" << endl;  
    Li.push_back(spatron);
  
  }

 
 

  if(pos<pa.length() && first!=NULL){
    
    if(pa[pos]=='*'){
      
      // cout << " caracter del patro es un *  " << endl; 
      buscar_patron( pa, Li, first->esq, pos, spatron, first->final); 
      buscar_patron( pa, Li, first->centre, pos+1, spatron + first->info , first->final); 
      buscar_patron( pa, Li, first->dret, pos, spatron, first->final); 
     
    
     
    }
   

    else if(pa[pos]!='*' && pa[pos]<first->info){
  
      //cout << " caracter + gran k el del patro  " << endl; 
      buscar_patron( pa, Li, first->esq, pos, spatron, first->final);

	}
      
    
    else if(pa[pos]!='*' && pa[pos]==first->info){
      
      //cout << " caracter coincideix amb el del patro  " << endl;
      buscar_patron( pa, Li, first->centre, pos+1, spatron+first->info , first->final);

    }
    

    else if(pa[pos]!='*' && pa[pos]>first->info){
     
      //cout << " caracter + gran k el del patro  " << endl; 
      buscar_patron( pa, Li, first->dret, pos, spatron, first->final);
      
    }


 }



}




void diccionario::satisfacen_patron(const string& p, list<string>& L) const throw(error){


  L.clear();
  if(p=="") L.push_back("");
  
  //la llista L s'obte odenada
  else buscar_patron(p, L, arrel_real, 0, "", false);




}
  










