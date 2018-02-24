#include "hueco.hpp"



hueco::hueco(nat fil, nat col, orientacion o, nat lon)throw(error)

  :
  _fila(fil), _columna(col), ori(o), _longitud(lon)
  
{}



hueco::hueco(const hueco& h) throw(error){

  _fila=h._fila;
  _columna=h._columna;
  ori=h.ori;
  _longitud=h._longitud;


}



hueco& hueco::operator=(const hueco& h) throw(error){

  if( this!= &h){
    
    _fila=h._fila;
    _columna=h._columna;
    ori=h.ori;
    _longitud=h._longitud;
     
  }
  return *this;
}
     

hueco::~hueco() throw(){}




nat hueco::fila() const throw(){

  return _fila;

}




nat hueco::columna() const throw(){

  return _columna;

}

orientacion hueco::orient() const throw(){
  
  return ori;

}


nat hueco::longitud() const throw(){

  return _longitud;

}



bool hueco::operator<(const hueco& t) const throw(){  


  if( _fila<t._fila)return true;
  else if( _fila==t._fila && _columna<t._columna)return true;
  else if(_fila==t._fila && _columna==t._columna && ori<t.ori) return true; 
  else if(_fila==t._fila && _columna==t._columna && ori==t.ori) return _longitud<t._longitud;
  else return false;
  
  
}




