#include "solver.hpp"
#include "list_sort.hpp"


namespace solver{

  void matchs_H(const  sopa_letras& S, const  diccionario& D, list<match>& Li) throw(error);
  void matchs_HR(const  sopa_letras& S, const  diccionario& D, list<match>& Li) throw(error);
  void matchs_V(const  sopa_letras& S, const  diccionario& D, list<match>& Li) throw(error);
  void matchs_VR(const sopa_letras& S,const  diccionario& D, list<match>& Li) throw(error);
  void matchs_D( const sopa_letras& S, const diccionario& D, list<match>& Li) throw(error);
  void matchs_DR( const sopa_letras& S, const diccionario& D, list<match>& Li) throw(error);
  void matchs_B( const sopa_letras& S, const diccionario& D, list<match>& Li) throw(error);
  void matchs_BR( const sopa_letras& S, const diccionario& D, list<match>& Li) throw(error);

void solve(const sopa_letras& S, const diccionario& D, list<match>& L) throw(error){

  if(S.num_filas()!=0){   
    
    matchs_H(S, D, L);
    matchs_HR(S, D, L);
    matchs_V(S, D, L);
    matchs_VR(S, D, L);
    matchs_D(S, D, L);
    matchs_DR(S, D, L);
    matchs_B(S, D, L);
    matchs_BR(S, D, L);

    list_sort::sort(L);

  }
 
}





  void matchs_H (const sopa_letras& S ,const  diccionario& D, list<match>&  Li) throw(error){
    

    for(nat i=0; i<S.num_filas(); ++i){
      
      
      for(nat j=0; j<S.num_columnas()-3; ++j){
       
	string pr;
	pr=S.cadena(i, j, S.num_columnas()-j, orientacion::H);
        
	pr=D.prefijo(pr);
	
	if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, i, j, orientacion::H));}
			       
      }


    }

  }




  void matchs_HR( const sopa_letras& S, const diccionario& D, list<match>& Li) throw(error){
   

    for(nat i=0; i<S.num_filas(); ++i){
      
      
      for(nat j=S.num_columnas()-1; j>2; --j){
       
	string pr;
	pr=S.cadena(i, j, S.num_columnas()-(S.num_columnas()-j-1), orientacion::HR);
	//cout << " Mirem la paraula: " << pr << endl; 
	pr=D.prefijo(pr);
	
	if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, i, j, orientacion::HR));}
			       
      }


    }

 }




  
  void matchs_V( const sopa_letras& S, const diccionario& D, list<match>& Li) throw(error){


    for(nat i=0; i<S.num_columnas(); ++i){
      
      
      for(nat j=0; j<S.num_filas()-3; ++j){
       
	string pr;
	pr=S.cadena(j, i, S.num_filas()-j, orientacion::V);
	//cout << " Mirem la paraula: " << pr << endl; 
	pr=D.prefijo(pr);
	
	if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, j, i, orientacion::V));}
			       
      }


    }

  }




  void matchs_VR( const sopa_letras& S, const diccionario& D, list<match>& Li) throw(error){


    for(nat i=0; i<S.num_columnas(); ++i){
      
      
      for(nat j=S.num_filas()-1; j>2; --j){
       
	string pr;
	pr=S.cadena(j, i, S.num_filas()-(S.num_filas()-j-1), orientacion::VR);
	//cout << " Mirem la paraula: " << pr << endl; 
	pr=D.prefijo(pr);
	
	if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, j, i, orientacion::VR));}
			       
      }


    }

}


  
  void matchs_D( const sopa_letras& S, const diccionario& D, list<match>& Li) throw(error){
    
    //+ files k columnes
    
    //cout << " + files k columnes o iguals" << endl;
    if(S.num_filas()>= S.num_columnas()){


      nat diff=S.num_filas()-S.num_columnas();


      //Diaogonals superiors
      for(nat i=0; i<S.num_columnas()-3; ++i){
      
	for(nat j=0; j<S.num_columnas()-3; ++j){ 
       
	  nat aux=i+j;
	  if(aux>=S.num_columnas()-3) break;
	
	  string pr;
	  pr=S.cadena(j, aux, S.num_columnas()-j-i, orientacion::D);
	  //  cout << "Mirem la paraula " << pr << endl; 
	  pr=D.prefijo(pr);
	
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, j, aux, orientacion::D));}
			       
	}
	
	
    }
      
      //diagonals inferors
    
      nat cont=0;
      for(nat i=1; i<S.num_filas()-3; ++i){
       
	
     
	for(nat j=0; j<S.num_columnas()-3; ++j){ 
	  
	  string pr;
	  nat aux=i+j;
	   if(aux>=S.num_filas()-3){ break;}
	
	  if(cont<diff){
	    pr=S.cadena(aux, j, S.num_columnas()-j, orientacion::D);	
	    //cout << " Mirem la paraula  " << pr << endl;
	  }
	 
	  else{
	  
	    pr=S.cadena(aux, j, S.num_columnas()-j-(cont-diff+1), orientacion::D);	
	    //cout << " Mirem la paraula " << pr << endl;
	  }

	  pr=D.prefijo(pr);
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, aux, j, orientacion::D));}
	  
	}
	cont++;

      }
    }
    
   
    //+ columnes k files 
    //cout << "+ columnas k files" << endl;
    if(S.num_filas()< S.num_columnas()){


      nat diff=S.num_columnas()-S.num_filas();


      //Diaogonals superiors
      nat cont=0;
      for(nat i=0; i<S.num_columnas()-3; ++i){
      
	
	for(nat j=0; j<S.num_filas()-3; ++j){ 
	  
	  string pr;
	  nat aux=i+j;
      
	  if(aux>=S.num_columnas()-3){ break;}
	 
	 
	  if(cont<=diff){
	    pr=S.cadena(j, aux, S.num_filas()-j, orientacion::D);	
	    //    cout << " Mirem la paraula cont < diff " << pr << endl;
	  }
	 
	  else{
	  
	    pr=S.cadena(j, aux, S.num_filas()-j-(cont-diff), orientacion::D);	
	    // cout << " Mirem la paraula " << pr << endl;
	  }
 
	  pr=D.prefijo(pr);
	
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, j, aux, orientacion::D));}
			       
	}
	
	cont ++;
    }
      
      //diagonals inferors
    
     
      for(nat i=1; i<S.num_filas()-3; ++i){
       
      
     
	for(nat j=0; j<S.num_columnas()-3; ++j){ 
	  
	  string pr;
	  nat aux=i+j;
	  if(aux>=S.num_filas()-3){ break;}

	  pr=S.cadena(aux, j, S.num_filas()-j-i, orientacion::D);	
	  //cout <<  " Mirem la paraula  " << pr << endl;
	  pr=D.prefijo(pr);
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, aux, j, orientacion::D));}
	  
	}


      }
    }



}





void matchs_DR( const sopa_letras& S, const diccionario& D, list<match>& Li) throw(error){

  //+ files k columnes
    
   
    if(S.num_filas()>= S.num_columnas()){
      //cout << " + files k columnes o iguals" << endl;

      nat diff=S.num_filas()-S.num_columnas();

      nat cont=0;
      //Diagonals superiors
      for(nat i=S.num_filas()-1; i>2; --i){
	
	nat count=0;
	for(nat j=S.num_columnas()-1; j>2; --j){ 
	  
	 
	  string pr;
	  nat aux=i-count;
	  if(aux<=2) break;
	 
	  if(cont<=diff){
	    
	    pr=S.cadena(aux, j, j+1, orientacion::DR);	
	    //    cout << " Mirem la paraula " << pr << endl;
	  }
	 
	  else{
	  
	    pr=S.cadena(aux, j, j+1-(cont-diff), orientacion::DR);	
	    //cout << " Mirem la paraula " << pr << endl;
	  }

	  pr=D.prefijo(pr);
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, aux, j, orientacion::DR));}
	  count++;
	}
	
	cont++;
      }
      
      //diagonals inferiors
    
      nat con=0;
      for(nat i=S.num_columnas()-2; i>2; --i){
       
       
	nat count=0;
     
	for(nat j=S.num_filas()-1; j>2; --j){ 
	  
	  string pr;
	  nat aux=i-count; 
	  if(aux<=2){ break;}
	  
	   pr=S.cadena(j, aux, S.num_columnas()-1-con-count, orientacion::DR);	
	   //cout << " Mirem la paraula  " << pr << endl;
	   pr=D.prefijo(pr);
	   if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, j, aux, orientacion::DR));}
	   
	   count++;
	}
      
	con++;
      }
    
    }
    
   

    //+ columnes k files 
   
    if(S.num_filas()< S.num_columnas()){
     

      //cout << " + columnes k files" << endl;
    
      nat count=0;
      //Diagonals superiors
      for(nat i=S.num_filas()-1; i>2; --i){
	
	nat cont=0;
	for(nat j=S.num_columnas()-1; j>2; --j){ 
	  
	 
	  string pr;
	  nat aux=i-cont;
	  if(aux<=2){ break;} 
	  pr=S.cadena(aux, j, S.num_filas()-cont-count, orientacion::DR);	
	  //  cout << " Mirem la paraula " << pr << endl;
	        
	  pr=D.prefijo(pr);
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, aux, j, orientacion::DR));}
	  cont++;
      }
	
      count++;
    }
      
     


      //diagonals inferiors
    
    

      nat diff=S.num_columnas()-S.num_filas();
      nat con=0;
      
      for(nat i=S.num_columnas()-2; i>2; --i){
      
       
	nat count=0;
     
	for(nat j=S.num_filas()-1; j>2; --j){ 
	  
	  string pr;
	  nat aux=i-count;
	  if(aux<=2){ break;}
	  
	  if(con<diff){ 
	    pr=S.cadena(j, aux, S.num_filas()-count , orientacion::DR);	
	    //cout << " Mirem la paraula  " << pr << endl;
	  }
	  
	  else{
	    pr=S.cadena(j, aux, S.num_filas()-1-count-(con-diff), orientacion::DR);	
	    //cout << " Mirem la paraula  " << pr << endl;
	    
	  }  

	  pr=D.prefijo(pr);
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, j, aux, orientacion::DR));}
	   
	  count++;
	}
      
	con++;
      }
    
    }
   

}







void matchs_B( const sopa_letras& S, const diccionario& D, list<match>& Li) throw(error){

  //+ files k columnes
    
   
    if(S.num_filas()>= S.num_columnas()){
      //cout << " + files k columnes o iguals" << endl;

      nat diff=S.num_filas()-S.num_columnas();
      nat cont=0;
      //Diagonals superiors
      for(nat i=S.num_filas()-1; i>2; --i){
	
      
	for(nat j=0; j<S.num_columnas()-3; ++j){ 
	  
	 
	  string pr;
	  nat aux=i-j;
	  if(aux<=2){ break;}
	 
	  if(cont<=diff){
	    //cout << "Posicio " <<  aux << j << endl;
	    pr=S.cadena(aux, j, S.num_columnas()-j, orientacion::B);	
	    //    cout << " Mirem la paraula " << pr << endl;
	  }
	 
	  else{
	  
	    pr=S.cadena(aux, j, S.num_columnas()-j-(cont-diff), orientacion::B);	
	    //cout << " Mirem la paraula " << pr << endl;
	  }

	  pr=D.prefijo(pr);
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, aux, j, orientacion::B));}
	 
	}
	
	cont++;
      }
      
     
      //diagonals inferiors
    
      
      for(nat i=1; i<S.num_columnas()-3; ++i){
       
       
	nat count=0;
     
	for(nat j=S.num_filas()-1; j>2; --j){ 
	  
	  string pr;
	  nat aux=i+count; 
	  if(aux>=S.num_columnas()-3){ break;}
	  
	   pr=S.cadena(j, aux, S.num_columnas()-i-count, orientacion::B);	
	   //cout << " Mirem la paraula  " << pr << endl;
	   pr=D.prefijo(pr);
	   if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, j, aux, orientacion::B));}
	   
	   count++;
	   
      
      
	}
    
      }
    }
    

    //+ columnes k files 
   
  

    if(S.num_filas()< S.num_columnas()){
     	 
      //cout << " + columnes k files" << endl;
    
      nat count=0;
      //Diagonals superiors
      for(nat i=S.num_filas()-2; i>2; --i){
	
	count++;
       
	for(nat j=0; j<S.num_columnas()-3; ++j){ 
	  
	 
	  string pr;
	  nat aux=i-j;
	  if(aux<=2){ break;} 
	  pr=S.cadena(aux, j, S.num_filas()-count-j, orientacion::B);	
	  //  cout << " Mirem la paraula " << pr << endl;
	        
	  pr=D.prefijo(pr);
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, aux, j, orientacion::B));}
	  
	}
	
   
      }
      
     


      //diagonals inferiors
    
    

      nat diff=S.num_columnas()-S.num_filas();
      nat con=0;    
      for(nat i=0; i<S.num_columnas()-3; ++i){
      
       
	nat count=0;
     
	for(nat j=S.num_filas()-1; j>2; --j){ 
	  
	  string pr;
	 
	  nat aux=i+count;
	  if(aux>=S.num_columnas()-3){ break;}
	  
	  if(con<=diff){
	     
	    pr=S.cadena(j, aux, S.num_filas()-count , orientacion::B);	
	    //cout << " Mirem la paraula " << pr << endl;
	  }
	  
	  else{
	    pr=S.cadena(j, aux, S.num_filas()-count-(con-diff), orientacion::B);	
	    //cout << " Mirem la paraula  " << pr << endl;
	    
	  }

	  pr=D.prefijo(pr);
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, j, aux, orientacion::B));}
	   
	  count++;
	}
	con++;
	
      }
    
    }
    

}

   




void matchs_BR( const sopa_letras& S, const diccionario& D, list<match>& Li) throw(error){

  
    if(S.num_filas()>= S.num_columnas()){
     	 
      //cout << " + files k columnes" << endl;
    
      //Diagonals superiors
      nat cont=0;
      for(nat i=S.num_columnas()-2; i>2; --i){
	
       
	nat co=0;
	for(nat j=0; j<S.num_filas()-3; ++j){ 
	  
	  
	  string pr;
	  nat aux=i-co;
	  if(aux<=2){ break;} 
	  pr=S.cadena(j, aux, S.num_columnas()-1-co-cont, orientacion::BR);	
	  //  cout << " Mirem la paraula " << pr << endl;
	        
	  pr=D.prefijo(pr);
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, j, aux, orientacion::BR));}
	  
	  co++;
	}
	
	cont++;
      }
      

      //diagonals inferiors
    
    

      nat diff=S.num_filas()-S.num_columnas();
     
      nat con=0;    
      for(nat i=0; i<S.num_filas()-3; ++i){
      
       
	nat count=0;
     
	for(nat j=S.num_columnas()-1; j>2; --j){ 
	  
	  string pr;
	 
	  nat aux=i+count;
	  if(aux>=S.num_filas()-3){ break;}
	  
	  if(con<=diff){
	     
	    pr=S.cadena(aux, j, S.num_columnas()-count , orientacion::BR);	
	    //cout << " Mirem la paraula " << pr << endl;
	  }
	  
	  else{
	    pr=S.cadena(aux, j, S.num_columnas()-count-(con-diff), orientacion::BR);	
	    //cout << " Mirem la paraula  " << pr << endl;
	    
	  }

	  pr=D.prefijo(pr);
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, aux, j, orientacion::BR));}
	   
	  count++;
	}
	con++;
	
      }
    
    }
    

    //+columnes k files
     
    if(S.num_filas()  < S.num_columnas()){
      //cout << " + columnas k files" << endl;

      nat diff=S.num_columnas()-S.num_filas();
      nat cont=0;
      //Diagonals superiors
      for(nat i=S.num_columnas()-1; i>2; --i){
	
      
	for(nat j=0; j<S.num_filas()-3; ++j){ 
	  
	 
	  string pr;
	  nat aux=i-j;
	  if(aux<=2){ break;}
	 
	  if(cont<=diff){
	    pr=S.cadena(j, aux, S.num_filas()-j, orientacion::BR);	
	    //    cout << " Mirem la paraula " << pr << endl;
	  }
	 
	  else{
	  
	    pr=S.cadena(j, aux, S.num_filas()-j-(cont-diff), orientacion::BR);	
	    //cout << " Mirem la paraula " << pr << endl;
	  }

	  pr=D.prefijo(pr);
	  if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, j, aux, orientacion::BR));}
	 
	}
	
	cont++;
      }
      
     
      //diagonals inferiors
    
      for(nat i=1; i<S.num_filas()-3; ++i){
       
       
	nat count=0;
     
	for(nat j=S.num_columnas()-1; j>2; --j){ 
	  
	  string pr;
	  nat aux=i+count; 
	  if(aux>=S.num_filas()-3){ break;}
	  
	   pr=S.cadena(aux, j, S.num_filas()-i-count, orientacion::BR);	
	   //cout << " Mirem la paraula  " << pr << endl;
	   pr=D.prefijo(pr);
	   if(pr!="" && pr.length()>=4){ Li.push_back(match (pr, aux, j, orientacion::BR));}
	   
	   count++;  
      
	}
    
      }
    }
    


}



};














