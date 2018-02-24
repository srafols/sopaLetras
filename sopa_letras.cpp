#include "sopa_letras.hpp"



sopa_letras::sopa_letras() throw(error)

  :

  sopa(new char*[MAXFILAS]), num_files(0), num_columnes(0), mida_fixa(false)
  

{}




sopa_letras::sopa_letras(nat num_fil, nat num_col)throw(error){

  if( num_fil>MAXFILAS) throw  error(nom_mod, NumeroFilasIncorrecto, MsgNumFilasIncorr);
  if( num_col>MAXCOLUMNAS) throw  error(nom_mod, LongitudFilaIncorrecta, MsgLongFilaIncorr);

  num_files=num_fil;
  num_columnes=num_col;
  num_files_fix=0;
  mida_fixa=true;
  
  //reservem  memoria
  sopa=new char*[num_files];
 
  

  for(nat i=0; i<num_files; i++){
   
    
    sopa[i]=new char[num_columnes];
 
    for(nat j=0; j<num_columnes; j++){
      
      sopa[i][j]='*';
     
    }

  }
  
  
} 




sopa_letras::sopa_letras(const sopa_letras& S) throw(error){


  num_files=S.num_files;
  num_columnes=S.num_columnes;
  mida_fixa=S.mida_fixa;
  num_files_fix=S.num_files_fix;

  
  
    sopa=new char*[num_files];
 
    for(nat i=0; i<num_files; i++){
   
      sopa[i]=new char[num_columnes];
      
      
      for(nat j=0; j<num_columnes; j++){
	sopa[i][j]=S.sopa[i][j];
	
      }
      
    }
    
}




sopa_letras& sopa_letras::operator=(const sopa_letras& S) throw(error){

  if(this!=&S){
   
  

    //cas en que la mida de les dues matrius son diferents 
    if(num_files!=S.num_files || num_columnes!=S.num_columnes){
      
      for(nat i=0; i<num_files; i++){
	delete[] sopa[i];
      }
      delete [] sopa;
      num_files=S.num_files;;
      num_columnes=S.num_columnes;
      mida_fixa=S.mida_fixa;
      num_files_fix=S.num_files_fix;
      
      sopa=new char*[num_files];

      for(nat i=0; i< num_files; i++){
	sopa[i]=new char[num_columnes];
	
	for(nat j=0; j<num_columnes; j++){
	  sopa[i][j]=S.sopa[i][j];
         
	}
      }
      
    }  
      
    else if(num_files==S.num_files && num_columnes==S.num_columnes){
      
      
      mida_fixa=S.mida_fixa;
      num_files_fix=S.num_files_fix;
      
      for(nat i=0; i< num_files; i++){
       
	for(nat j=0; j<num_columnes; j++){
	  sopa[i][j]=S.sopa[i][j];
	 

	}
      }
      
    }

  }  

    return *this;
  

}


sopa_letras::~sopa_letras() throw(){


  for(nat i=0; i<num_files; i++){
    delete[] sopa[i];
  }
  delete [] sopa;
      

}




void sopa_letras::inserta_fila(string s) throw(error){

  
  
  //sopa construida sense especificar files ni columnes
  if(mida_fixa==false){
    
    //la sopa de lletras no conte cap fila
    if(num_files==0 && num_columnes==0){
    
      if(s.length()>MAXCOLUMNAS) throw error(nom_mod, LongitudFilaIncorrecta, MsgLongFilaIncorr);
      sopa[num_files]=new char[s.length()];
      
      for(nat i=0; i<s.length(); i++){
	sopa[num_files][i]=s[i];
      }
      

      num_files++;
      num_columnes=s.length();
    
   

    }
  
    //almenys ja s'ha col.locat una fila a la sopa de lletres
    else{
      
      if( s.length()!=num_columnes) throw error(nom_mod, LongitudFilaIncorrecta, MsgLongFilaIncorr);
      if( num_files>=MAXFILAS) throw  error(nom_mod, NumeroFilasIncorrecto, MsgNumFilasIncorr);
      sopa[num_files]=new char[s.length()];
      
      for(nat i=0; i<s.length(); i++){
	sopa[num_files][i]=s[i];
      }
      
      num_files++;
       
    
    }
      
      
  }


  
  //actuem sobre una sopa de lletres amb les files i les columnes especificades
    if(mida_fixa==true){

    
      if(num_files_fix==num_files) throw error(nom_mod,NumeroFilasIncorrecto, MsgNumFilasIncorr);
      
      if(s.length()>MAXCOLUMNAS || s.length()!=num_columnes) throw error(nom_mod, LongitudFilaIncorrecta, MsgLongFilaIncorr);
      
     

      for(nat j=0; j<num_columnes; j++){

	sopa[num_files_fix][j]=s[j];
      
      }
      num_files_fix++;
      
    
    }

  

}

 

 string sopa_letras::cadena(nat fil, nat col, nat lon, orientacion orient) const throw(error){

  
   string consulta;


   if(fil>=num_files || col>=num_columnes){ throw error(nom_mod, StringNoExiste, MsgStringNoExiste);}
   
   
   switch(orient){

   case orientacion::H: 
     
 
    
     
     if(lon<=num_columnes-col){
     
       for(nat i=0; i<lon; i++){ 
	 	 
	 consulta=consulta+ sopa[fil][col+i];
	 
     
       }
    }

    
    else { throw error(nom_mod, StringNoExiste, MsgStringNoExiste);}
    break;



   case orientacion::HR: 
     
     if(lon<=col+1){

       for(nat i=0; i<lon; i++){ 
	 consulta=consulta+ sopa[fil][col-i];

	
       }
       
     }
     else { throw error(nom_mod, StringNoExiste, MsgStringNoExiste);}
     break;
   
   

   case orientacion::V: 
     
     if(lon<=num_files-fil){

       for(nat i=0; i<lon; i++){ 
	 consulta=consulta + sopa[fil+i][col];
	
       
       }
       
     }
     else{ throw error(nom_mod, StringNoExiste, MsgStringNoExiste);}
     break;

   

   case orientacion::VR: 
     
     if(lon<= fil+1){

       for (nat i=0; i<lon; i++) { 
	 consulta= consulta +sopa[fil-i][col];
	

       }

     }
     else{ throw error(nom_mod, StringNoExiste, MsgStringNoExiste);}
     break;
   


   case orientacion::D:
     
     if(int(fil+lon-1)<int(num_files)  && int(col+lon-1)<int(num_columnes)){
     
      
       for(nat i=0; i<lon; i++){
	 consulta=consulta+sopa[fil+i][col+i];
	
       
       }
	
     }

     
     else{ throw error(nom_mod, StringNoExiste, MsgStringNoExiste);}
     break;

   

   case orientacion::DR: 
     
     if(int(fil-lon+1) >=0 && int(col-lon+1)>=0){

       for(nat i=0; i<lon; i++){ 

	 consulta=consulta+sopa[fil-i][col-i];
	
       
       }
       
            
     }
     
     else{ throw error(nom_mod, StringNoExiste, MsgStringNoExiste);}
     break;
     


   case orientacion::B: 

     if(int(fil-lon+1)>=0 && int(col+lon-1)<int(num_columnes)){

       for(nat i=0; i<lon; i++){
	 consulta=consulta + sopa[fil-i][col+i];
	
       }

     }


     else{ throw error(nom_mod, StringNoExiste, MsgStringNoExiste);}
     break;
  


   case orientacion::BR:
     
     if(int(fil+lon-1)<int(num_files) && int(col-lon+1)>=0){

       for(nat i=0; i<lon;i++){

	 consulta=consulta+ sopa[fil+i][col-i];

       }

     }

     else{ throw error(nom_mod, StringNoExiste, MsgStringNoExiste);}
     break;
      
     
   }

   return consulta;
   
 }


 

void sopa_letras::inserta_cadena(string s, nat fil, nat col, orientacion orient) throw(error){




  if(fil>=num_files || col>=num_columnes){ throw error(nom_mod, StringNoCabe, MsgStringNoCabe);}
   
  switch(orient){

  case orientacion::H: 
     
    if(s.length()<=num_columnes-col){
     
      for(nat i=0; i<s.length(); i++){  sopa[fil][col+i]=s[i];}
       
    }
    else{ throw error(nom_mod, StringNoCabe, MsgStringNoCabe);}
    break;

    

  case orientacion::HR: 
     
    if(s.length()<=col+1){

      for(nat i=0; i<s.length(); i++){ sopa[fil][col-i]=s[i];}
       
    }
    else{ throw error(nom_mod, StringNoCabe, MsgStringNoCabe);}
    break;
   
   

  case orientacion::V: 
     
    if(s.length()<=num_files-fil){

      for(nat i=0; i<s.length(); i++){ sopa[fil+i][col]=s[i];}
       
    }
    else{ throw error(nom_mod, StringNoCabe, MsgStringNoCabe);}
    break;

   

  case orientacion::VR: 
     
    if(s.length()<= fil+1){
      
      for (nat i=0; i<s.length(); i++) { sopa[fil-i][col]=s[i];}
      
    }
    else{ throw error(nom_mod, StringNoCabe, MsgStringNoCabe);}
    break;
   


  case orientacion::D:
    
    if(int(fil+s.length()-1)<int(num_files)  && int(col+s.length()-1)<int(num_columnes)){
     
      
      for(nat i=0; i<s.length(); i++){ sopa[fil+i][col+i]=s[i];}
      
    }

     
    else{ throw error(nom_mod, StringNoCabe, MsgStringNoCabe);}
    break;

   

  case orientacion::DR: 
     
    if(int(fil-s.length()+1) >=0 && int(col-s.length()+1)>=0){

      for(nat i=0; i<s.length(); i++){ sopa[fil-i][col-i]=s[i];}
       
            
    }
     
    else{ throw error(nom_mod, StringNoCabe, MsgStringNoCabe);}
    break; 



  case orientacion::B: 
    
    if(int(fil-s.length()+1)>=0 && int(col+s.length()-1)<int(num_columnes)){

      for(nat i=0; i<s.length(); i++){ sopa[fil-i][col+i]=s[i];}
      
    }


    else{ throw error(nom_mod, StringNoCabe, MsgStringNoCabe);}
    break;
    


  case orientacion::BR:
     
    if(int(fil+s.length()-1)<int(num_files) &&  int(col-s.length()+1)>=0){

      for(nat i=0; i<s.length();i++){ sopa[fil+i][col-i]=s[i];}
      
    }

    else{ throw error(nom_mod, StringNoCabe, MsgStringNoCabe);}
    break;
      
     
  }

}

 

nat sopa_letras::num_filas() const throw(){

 

  return num_files;


}

 

  
 

 nat sopa_letras::num_columnas() const throw(){

   return num_columnes;
    
   
 }


 







