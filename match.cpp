#include "match.hpp"


match::match(const string& pal, nat fil, nat col, orientacion o)throw(error)

  :
  paraula(pal), _fila(fil), _columna(col), ori(o)

{}   


match::match( const match& s) throw(error){

  paraula=s.paraula;
  _fila=s._fila;
  _columna=s._columna;
  ori= s.ori;


}


match& match::operator=(const match& s) throw(error){

  
  if(this!= &s){

    paraula=s.paraula;
    _fila=s._fila;
    _columna=s._columna;
    ori=s.ori;


  }

  return *this;

}


match::~match() throw(){}



string match::palabra() const throw(){

  
  return paraula;

}


nat match::fila() const throw(){

  return _fila;

}



nat match::columna() const throw(){

  return _columna;

}


orientacion match::orient() const throw(){

  return ori;

}


bool match::operator<(const match& t) const throw(){

  if(paraula<t.paraula) return true;
  else if(paraula==t.paraula && _fila<t._fila)return true;
  else if(paraula==t.paraula && _fila==t._fila && _columna<t._columna)return true;
  else if(paraula==t.paraula &&_fila==t._fila && _columna==t._columna) return (ori<t.ori);
  else return false;


}



