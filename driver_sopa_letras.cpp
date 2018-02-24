#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <strstream>
#include <eda/input_line>
#include <eda/error>
#include <eda/gen_driver>
#include <eda/util>

#include "orientacion.hpp"
#include "match.hpp"
#include "hueco.hpp"
#include "sopa_letras.hpp"
#include "diccionario.hpp"
#include "solver.hpp"
#include "builder.hpp"
#include "list_sort.hpp"

using util::nat;

// Imprimir bool

ostream& operator<=(ostream& os, const bool b)
{
  os << (b ? "true" : "false");
  return os;
}

// Imprimir orientacion

ostream& operator<<(ostream&os, const orientacion o)
{
  os << o.tostring();
  return os;
}

// Leer orientacion de un string

void str2ori(string& s, orientacion& o)
{
  s = "orientacion::" + s;
  if (s == "orientacion::H")
    o = orientacion::H;
  else if (s == "orientacion::HR")
    o = orientacion::HR;
  else if (s == "orientacion::V")
    o = orientacion::V;
  else if (s == "orientacion::VR")
    o = orientacion::VR;
  else if (s == "orientacion::D")
    o = orientacion::D;
  else if (s == "orientacion::DR")
    o = orientacion::DR;
  else if (s == "orientacion::B")
    o = orientacion::B;
  else if (s == "orientacion::BR")
    o = orientacion::BR;
  else
    o = orientacion::NO_OR;
}

// Imprimir match

ostream& operator<<(ostream& os, const match& m)
{
  os << m.palabra() << " " << m.fila() << " " << m.columna();
  os << " " << m.orient().tostring();
  return os;
}

// Imprimir hueco

ostream& operator<<(ostream& os, const hueco& h)
{
  os << h.fila() << " " << h.columna() << " ";
  os << h.orient().tostring() << " "    << h.longitud();
  return os;
}

// Imprimir sopa

ostream& operator<<(ostream& os, const sopa_letras& S)
{ 
  for (nat i = 0; i < S.num_filas(); i++)
  {
    for (nat j = 0; j < S.num_columnas(); j++)
      os << S.cadena(i, j, 1, orientacion::H);
    os << endl;
  }
  return os; 
} 

// imprimir list<T>

template<typename T>
void imprime_list(const list<T>& L,
		  const char sep = ' ',
		  ostream& os = std::cout)
{
  list<T>::const_iterator it = L.begin();
  while (it != L.end()) {
    os << *it << sep;
    ++it;
  }
}

// Comprueba la funcion build de builder
// devuelve:
// 0 lista vacia, todo correcto
// 1 todo correcto
// 2 error de algun tipo

int check_build(diccionario* D, list<hueco>* LH, list<match>* LM, sopa_letras* S) {
  int res;

  if (LM->empty()) {
    nat i = 0;
    res  = 0;
    string s(S->num_columnas(),'*'); // asumo que pasamos sopas recien  
                                     // creadas, con '*' en todas las 
                                     // posiciones. Para no tener que 
                                     // crear y pasar una copia. 
    while (i < S->num_filas() && res != 2)
      if (s != S->cadena(i, 0, S->num_columnas(), orientacion::H)) 
        res = 2;
      else ++i;            
  }	 
  else {
    if (LM->size() != LH->size()) res = 2;
    else res = 1;
    nat i = 0, j = 0;
    while (i < S->num_filas() && res != 2) {
      while (j < S->num_columnas() && res != 2) {
	string s = S->cadena(i, j, 1, orientacion::H);
	if (s > "Z" || s < "A") 
          res = 2; // la sopa contiene caracteres no deseados
	else ++j;
      }
      ++i;
    }
    list<match>::const_iterator itm = LM->begin();
    list<hueco>::const_iterator ith = LH->begin();
    string prev;
    while (itm != LM->end() && res != 2) {
      bool found = false;
      while (ith != LH->end() && !found)
	if (itm->palabra().size() == ith->longitud() && 
            itm->fila() == ith->fila() && 
            itm->columna() == ith->columna() &&
	    itm->orient() == ith->orient()) 
          found = true;
	else ++ith;

      if (!found) res = 2; // no se ha encontrado el hueco correspondiente
      else{
	list<string> ls;
	D->satisfacen_patron(itm->palabra(), ls);
	if (ls.size() != 1) res = 2; // la palabra no esta en diccionario
	else if (ls.back() != itm->palabra()) 
               res = 2; // la palabra retornada no coincidem
	else if (itm->palabra() != S->cadena(itm->fila(), itm->columna(), 
                 itm->palabra().size(), itm->orient()))
	       res = 2; // la palabra no aparece en la sopa donde debiera
	else if (prev > itm->palabra()) 
               res = 2; // la lista de match no esta ordenada
	else {prev = itm->palabra(); ++itm; ith = LH->begin();}
      }
    }
  }

  return res;
}

/* ------< BASICAS: Constructoras, asignacion y destruccion >---------- */

void* user_init(const input_line& L, gen_driver& dr)
{
  string tid = L[3];

  if (tid == "orientacion")
    {
      if (L.nargs() != 3 && L.nargs() != 4)
	throw error(gen_driver::nom_mod, gen_driver::WrongNumArgs,
		    gen_driver::WrongNumArgsMsg);
      orientacion* p; 
      if (L.nargs() == 3)
        p = new orientacion;
      else
	p = new orientacion(L[4]);
      return static_cast<void*> (p);
    }

  if (tid == "match")
    { 
      if (L.nargs() != 3 && L.nargs() != 7 ) 
	throw error(gen_driver::nom_mod, gen_driver::WrongNumArgs,
		    gen_driver::WrongNumArgsMsg);
      if (L.nargs() == 3)
	return static_cast<void*> (new match());
      else
	{ // chequeamos que filas y columnas son numeros enteros positivos
	  if (util::toint(L[5]) < 0 || util::toint(L[6]) < 0)
	    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
			gen_driver::WrongTypeArgsMsg);
	  string pal = L[4];          
	  nat fil = static_cast<nat> (util::toint(L[5]));
	  nat col = static_cast<nat> (util::toint (L[6]));
	  return static_cast<void*> (new match(pal, fil, col, orientacion(L[7])));
        }
    }

  if (tid == "hueco")
    {
      if (L.nargs() != 3 && L.nargs() != 7)
	throw error(gen_driver::nom_mod, gen_driver::WrongNumArgs,
		    gen_driver::WrongNumArgsMsg);
      if (L.nargs() == 3)
	return static_cast<void*> (new hueco());
      else 
	{ // chequeamos que filas, columnas y longitud son numeros 
	  // enteros positivos
	  if (util::toint(L[4]) < 0 || util::toint(L[5]) < 0 || 
	      util::toint(L[7]) < 0)
	    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
			gen_driver::WrongTypeArgsMsg);
	  
	  nat fil = static_cast<nat> (util::toint(L[4]));
	  nat col = static_cast<nat> (util::toint(L[5]));
	  nat lon = static_cast<nat> (util::toint(L[7]));
	  
	  return static_cast<void*> (new hueco(fil, col, orientacion(L[6]), lon));
	}
    }

  else if (tid == "sopa_letras")
    {
      if (L.nargs() != 3 && L.nargs() != 5)
	throw error(gen_driver::nom_mod, gen_driver::WrongNumArgs,
		    gen_driver::WrongNumArgsMsg);

      sopa_letras* p;

      if (L.nargs() == 5)
	{ // chequeamos que filas y columnas son numeros enteros positivos
	  if (util::toint(L[4]) < 0 || util::toint(L[5]) < 0)
	    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
			gen_driver::WrongTypeArgsMsg);
	  nat fil = static_cast<nat> (util::toint(L[4]));
	  nat col = static_cast<nat> (util::toint(L[5]));
	  p = new sopa_letras(fil, col);
	}
      else
	p = new sopa_letras;
      
      return static_cast<void*> (p);
    }

  else if (tid == "diccionario")
    {
      if (L.nargs() != 3)
	throw error(gen_driver::nom_mod, gen_driver::WrongNumArgs,
		    gen_driver::WrongNumArgsMsg);
      return static_cast<void*> (new diccionario);
    }

  else if (tid == "list<string>")
    {
      if (L.nargs() != 3)
	throw error(gen_driver::nom_mod, gen_driver::WrongNumArgs,
		    gen_driver::WrongNumArgsMsg);
      return static_cast<void*> (new list<string>);
    }

  else if (tid == "list<match>")
    {
      if (L.nargs() != 3)
	throw error(gen_driver::nom_mod, gen_driver::WrongNumArgs,
		    gen_driver::WrongNumArgsMsg);
      return static_cast<void*> (new list<match>);
    }

  else if (tid == "list<hueco>")
    {
      if (L.nargs() != 3)
	throw error(gen_driver::nom_mod, gen_driver::WrongNumArgs,
		    gen_driver::WrongNumArgsMsg);
      return static_cast<void*> (new list<hueco>);
    }

  throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
	      gen_driver::WrongTypeArgsMsg);
}

void* user_initcopy(void* orig, const string& _class)
{
  if (_class == "orientacion")
    return static_cast<void*>
      (new orientacion(*(static_cast<orientacion*>(orig))));
  else if (_class == "match")
    return static_cast<void*>(new match(*(static_cast<match*>(orig))));
  else if (_class == "hueco")
    return static_cast<void*>(new hueco(*(static_cast<hueco*>(orig))));
  else if (_class == "sopa_letras")
    return static_cast<void*>
      (new sopa_letras(*(static_cast<sopa_letras*>(orig))));
  else if (_class ==  "diccionario")
    return static_cast<void*>
      (new diccionario(*(static_cast<diccionario*>(orig))));
  else if (_class ==  "list<string>")
    return static_cast<void*>
      (new list<string>(*(static_cast<list<string>*>(orig))));
  else if (_class == "list<match>")
    return static_cast<void*>
      (new list<match>(*(static_cast<list<match>*>(orig))));
  else if (_class == "list<hueco>")
    return static_cast<void*>
      (new list<hueco>(*(static_cast<list<hueco>*>(orig))));
  else
    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
		gen_driver::WrongTypeArgsMsg);
}


void user_copy(void* dest, void* orig, const string& _class)
{
  if(_class == "orientacion")
      *((orientacion*) dest) = *((orientacion*) orig);
  else if (_class == "match")
    *((match*) dest) = *((match*) orig);
  else if (_class == "hueco")
    *((hueco*) dest) = *((hueco*) orig);
  else if (_class == "sopa_letras")
    *((sopa_letras*) dest) = *((sopa_letras*) orig);
  else if (_class == "diccionario")
    *((diccionario*) dest) = *((diccionario*) orig);
  else if (_class == "list<string>")
    *((list<string>*) dest) = *((list<string>*) orig);
  else if (_class == "list<match>")
    *((list<match>*) dest) = *((list<match>*) orig);
  else if (_class == "list<hueco>")
    *((list<hueco>*) dest) = *((list<hueco>*) orig);
  else
    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
		gen_driver::WrongTypeArgsMsg);
}

void user_destroy(void* D, const string& _class)
{
  if (_class == "orientacion")
    delete ((orientacion*) D);
  else if (_class == "match")
    delete ((match*) D);
  else if (_class == "hueco")
    delete ((hueco*) D);
  else if (_class == "sopa_letras")
    delete ((sopa_letras*) D);
  else if (_class == "diccionario")
    delete ((diccionario*) D);
  else if (_class == "list<string>")
    delete ((list<string>*) D);
  else if (_class == "list<match>")
    delete ((list<match>*) D);
  else if (_class == "list<hueco>")
    delete ((list<hueco>*) D);
  else
    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
		gen_driver::WrongTypeArgsMsg);
}

/* -----------------------< ORIENTACION >----------------------------- */

void trata_inc(gen_driver& dr, const input_line& L, const string& obj_name)
{
  orientacion* p = (orientacion*) dr.get_obj(obj_name);
  ++(*p);
}

void trata_menig(gen_driver& dr, const input_line& L, const string& obj_name)
{
  orientacion* p = (orientacion*) dr.get_obj(obj_name);
  orientacion* q = (orientacion*) dr.get_obj (L[2]);
  dr.get_ostream() <= (*p <= *q);
  dr.get_ostream() << endl;
}

void trata_tostring(gen_driver& dr, const input_line& L,
		    const string& obj_name)
{
  orientacion* p = (orientacion*) dr.get_obj(obj_name);
  dr.get_ostream() << p->tostring() << endl;
}

/* ----------------------< MATCH & HUECO >---------------------------- */

void trata_palabra(gen_driver& dr, const input_line& L, const string& obj_name)
{
  match* m = (match*) dr.get_obj(obj_name);
  dr.get_ostream() << m -> palabra() << endl;
}

void trata_fila(gen_driver& dr, const input_line& L, const string& obj_name)
{
  if (dr.get_obj_type(obj_name)=="match")
    {
      match* m = (match*) dr.get_obj(obj_name);
      dr.get_ostream() << m -> fila() << endl;
    }
  else if (dr.get_obj_type(obj_name)=="hueco")
    {
      hueco* h = (hueco*) dr.get_obj(obj_name);
      dr.get_ostream() << h -> fila() << endl;
    }
  else
    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
		gen_driver::WrongTypeArgsMsg);
}

void trata_columna(gen_driver& dr, const input_line& L, const string& obj_name)
{
  if (dr.get_obj_type(obj_name)=="match")
    {
      match* m = (match*) dr.get_obj(obj_name);
      dr.get_ostream() << m -> columna() << endl;
    }
  else if (dr.get_obj_type(obj_name)=="hueco")
    {
      hueco* h = (hueco*) dr.get_obj(obj_name);
      dr.get_ostream() << h -> columna() << endl;
    }
  else
    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
		gen_driver::WrongTypeArgsMsg);  
}

void trata_orient(gen_driver& dr, const input_line& L, const string& obj_name)
{
    if (dr.get_obj_type(obj_name)=="match")
    {
      match* m = (match*) dr.get_obj(obj_name);
      dr.get_ostream() << (m -> orient()).tostring() << endl;
    }
  else if (dr.get_obj_type(obj_name)=="hueco")
    {
      hueco* h = (hueco*) dr.get_obj(obj_name);
      dr.get_ostream() << (h -> orient()).tostring() << endl;
    }
  else
    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
		gen_driver::WrongTypeArgsMsg);
}

void trata_long(gen_driver& dr, const input_line& L, const string& obj_name)
{
  hueco* h = (hueco*) dr.get_obj(obj_name);
  dr.get_ostream() << h -> longitud() << endl;
}

void trata_menor(gen_driver& dr, const input_line& L, const string& obj_name)
{
  if (dr.get_obj_type(obj_name)=="match")
    {
      match* m1 = (match*) dr.get_obj(obj_name);
      match* m2 = (match*) dr.get_obj(L[2]);
      dr.get_ostream() <= m1 -> operator<(*m2);
      dr.get_ostream() << endl;
    }
  else if (dr.get_obj_type(obj_name)=="hueco")
    {
      hueco* h1 = (hueco*) dr.get_obj(obj_name);
      hueco* h2 = (hueco*) dr.get_obj(L[2]);
      dr.get_ostream() <= h1 -> operator<(*h2);
      dr.get_ostream() << endl;
    }
  else
    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
		gen_driver::WrongTypeArgsMsg);
}

/* -----------------------< SOPA_LETRAS >----------------------------- */

void trata_inserta_fila(gen_driver& dr, const input_line& L,
			const string& obj_name)
{
  sopa_letras* S = (sopa_letras*) dr.get_obj(obj_name);
  string s = static_cast<string> (L[2]);
  S -> inserta_fila(s);
}

void trata_cadena(gen_driver& dr, const input_line& L, const string& obj_name)
{
  sopa_letras* S = (sopa_letras*) dr.get_obj(obj_name);

  // chequeamos que los argumentos son numeros enteros positivos
  if (util::toint(L[2]) < 0 || util::toint(L[3]) < 0 ||
      util::toint(L[4]) < 0)
    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
		gen_driver::WrongTypeArgsMsg);
  nat fil = static_cast<nat>(util::toint(L[2]));
  nat col = static_cast<nat>(util::toint(L[3]));
  nat lon = static_cast<nat>(util::toint(L[4]));
  dr.get_ostream() << S -> cadena(fil, col, lon, orientacion(L[5])) << endl;
}

void trata_inserta_cadena(gen_driver& dr, const input_line& L,
			  const string& obj_name)
{
  sopa_letras* S = (sopa_letras*) dr.get_obj(obj_name);
  string s = static_cast<string>(L[2]);
  // chequeamos que filas y columnas son numeros enteros positivos
  if (util::toint(L[3]) < 0 || util::toint(L[4]) < 0)
    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
		gen_driver::WrongTypeArgsMsg);
  nat fil = static_cast<nat>(util::toint(L[3]));
  nat col = static_cast<nat>(util::toint(L[4]));
  S -> inserta_cadena(s, fil, col, orientacion(L[5]));
}

void trata_num_filas(gen_driver& dr, const input_line& L,
		     const string& obj_name)
{
  sopa_letras* S = (sopa_letras*) dr.get_obj(obj_name);
  dr.get_ostream() << S -> num_filas() << endl;
}

void trata_num_columnas(gen_driver& dr, const input_line& L,
			  const string& obj_name)
{
  sopa_letras* S = (sopa_letras*) dr.get_obj(obj_name);
  dr.get_ostream() << S -> num_columnas() << endl;
}

/* ------------------------< DICCIONARIO >----------------------------- */

void trata_inserta(gen_driver& dr, const input_line& L, const string& obj_name)
{
  diccionario* D = (diccionario*) dr.get_obj(obj_name);
  string s = static_cast<string> (L[2]);
  D -> inserta(s);
}

void trata_prefijo(gen_driver& dr, const input_line& L, const string& obj_name)
{
  diccionario* D = (diccionario*) dr.get_obj(obj_name);
  string s = static_cast<string> (L[2]);
  dr.get_ostream() << D -> prefijo(s) << endl;
}

void trata_satisfacen_patron(gen_driver& dr, const input_line& L,
			     const string& obj_name)
{
  diccionario* D = (diccionario*) dr.get_obj(obj_name);
  string s = static_cast<string> (L[2]);
  list<string>* l = (list<string>*) dr.get_obj(L[3]);
  D -> satisfacen_patron(s, *l);
}

void trata_satisfacen_patron_pantalla(gen_driver& dr, const input_line& L,
                             const string& obj_name)
{
  diccionario* D = (diccionario*) dr.get_obj(obj_name);
  string s = static_cast<string> (L[2]);
  list<string> l;
  D -> satisfacen_patron(s, l);
  imprime_list(l, ' ',dr.get_ostream());
  dr.get_ostream() << endl;
}

/* ------------------------< LIST_SORT >------------------------------ */

void trata_sort(gen_driver& dr, const input_line& L, const string& obj_name)
{
  string infilename = L[2] + ".vi";
  string outfilename = L[2] + ".vo";
  ifstream inf(infilename.c_str());
  ofstream outf(outfilename.c_str());
  string s;
  inf >> s;
  if (s == "int")
    {
      int n;
      list<int> L;
      while (inf >> n)
	L.push_back(n);
      list_sort::sort(L);
      imprime_list(L, '\n', outf);
    }
  else if (s == "string")
    {
      list<string> L;
      while (inf >> s) L.push_back(s);
      list_sort::sort(L);
      imprime_list(L, '\n', outf);
    } 
  else if (s == "match")
    {
      string _orien;
      nat _fila, _colu;
      orientacion _o;
      string _pala;
      list<match> L;
      getline(inf,s);
      while (getline(inf,s)) 
        { 
	  istrstream iss(s.c_str());
          iss >> _pala;
	  iss >> _fila;
	  iss >> _colu;
	  iss >> _orien;
	  str2ori(_orien, _o);
	  match m (_pala, _fila, _colu, _o);
          L.push_back(m);
	}
      list_sort::sort(L);
      imprime_list(L, '\n', outf);
    }
  else
    throw error(gen_driver::nom_mod, gen_driver::WrongTypeArgs,
		gen_driver::WrongTypeArgsMsg);
}

/* ------------------------< SOLVER >-------------------------------- */

void trata_solve(gen_driver& dr, const input_line& L, const string& obj_name)
{
  sopa_letras* S = (sopa_letras*) dr.get_obj(obj_name);
  diccionario* D = (diccionario*) dr.get_obj(L[2]);
  list<match> l; 
  solver::solve(*S, *D, l);
  imprime_list(l, '\n',dr.get_ostream());
}

/* ------------------------< BUILDER >-------------------------------- */

void trata_build(gen_driver& dr, const input_line& L, const string& obj_name)
{
  string s;
  list<hueco>* l1 = (list<hueco>*) dr.get_obj(obj_name);
  diccionario* D = (diccionario*) dr.get_obj(L[2]);
  sopa_letras* S = (sopa_letras*) dr.get_obj(L[3]);
  list<match> l2; 
  builder::build(*D, *l1, l2, *S);
  imprime_list(l2, '\n',dr.get_ostream());
  dr.get_ostream() << *S;
}

void trata_check_build(gen_driver& dr, const input_line& L, const string& obj_name)
{
  string s;
  list<hueco>* l1 = (list<hueco>*) dr.get_obj(obj_name);
  diccionario* D = (diccionario*) dr.get_obj(L[2]);
  sopa_letras* S = (sopa_letras*) dr.get_obj(L[3]);
  list<match> l2;

  builder::build(*D, *l1, l2, *S);
  switch (check_build(D, l1, &l2, S))
  {
    case 0: s = "lista vacia"; break;
    case 1: s = "OK"; break;
    default: s = "NOT OK"; break;
  }
  dr.get_ostream() << s << endl;
}

/* ---------------------<afegir a LIST>------------------------------ */

void trata_inserta_lista_hueco(gen_driver& dr, const input_line& L,
			const string& obj_name)
{  
  list<hueco>* Lh = (list<hueco>*) dr.get_obj(obj_name);
  hueco* h = (hueco*) dr.get_obj(L[2]);
  Lh -> push_back(*h);
}

/* -------------------------< MAIN >--------------------------------- */

int main()
{
  gen_driver d;
  string args2[4];

  /* d.add_call(string nombre_funcion,
                funcion a la que llama en este cpp,
		int num_params,
		string atratayuda,
		string[] de params,
		string nombre_modulo,
		obj_name);                                        
  */


  // ORIENTACION 

  // operator++

  args2[0] = "string";
  d.add_call("incremento", trata_inc, 0, "(inc)remento", args2, "orientacion");
  d.add_call("inc", trata_inc, 0, "(inc)remento", args2, "orientacion");

  // operator <=

  args2[1] = "string";
  d.add_call("menor_o_igual", trata_menig, 1, "(men)or_o_(ig)ual", args2,
	     "orientacion");

  d.add_call("menig", trata_menig, 1, "(men)or_o_(ig)ual", args2,
	     "orientacion");

  // tostring

  d.add_call("tostring", trata_tostring, 0, "(t)o(s)tring", args2,
	     "orientacion");

  d.add_call("ts", trata_tostring, 0, "(t)o(s)tring", args2, "orientacion");

  // MATCH & HUECO

  // palabra

  d.add_call("palabra", trata_palabra, 0, "(pal)abra", NULL, "match");
  d.add_call("pal", trata_palabra, 0, "(pal)abra", NULL, "match");

  // fila

  d.add_call("fila", trata_fila, 0, "(fil)a", NULL, "*");
  d.add_call("fil", trata_fila, 0, "(fil)a", NULL, "*");

  // columna

  d.add_call("columna", trata_columna, 0, "(col)umna", NULL, "*");
  d.add_call("col", trata_columna, 0, "(col)umna", NULL, "*");

  // orientacion

  d.add_call("orientacion", trata_orient, 0, "(ori)entacion", NULL, "*");
  d.add_call("ori", trata_orient, 0, "(ori)entacion", NULL, "*");

  // operator <

  d.add_call("menor", trata_menor, 1, "(men)or", args2, "*");
  d.add_call("men", trata_menor, 1, "(men)or", args2, "*");

  // longitud

  d.add_call("longitud", trata_long, 0, "(lon)gitud", NULL, "hueco");
  d.add_call("lon", trata_long, 0, "(lon)gitud", NULL, "hueco");

  // SOPA_LETRAS

  // inserta_fila

  d.add_call("inserta_fila", trata_inserta_fila, 1, "(i)nserta_(fil)a",
	     args2, "sopa_letras");

  d.add_call("ifil", trata_inserta_fila, 1, "(i)nserta_(fil)a",
	     args2, "sopa_letras");

  // cadena
  args2[0] = "int"; args2[1] = "int"; args2[2] = "int"; args2[3] = "string";
  d.add_call("cadena", trata_cadena, 4, "(cad)ena", args2, "sopa_letras");
  d.add_call("cad",  trata_cadena, 4, "(cad)ena", args2, "sopa_letras");

  // inserta_cadena
  args2[0] = "string"; args2[1] = "int"; args2[2] = "int"; args2[3] = "string";
  d.add_call("inserta_cadena", trata_inserta_cadena, 4, "(i)nserta_(cad)ena",
	     args2, "sopa_letras");

  d.add_call("icad",  trata_inserta_cadena, 4, "(i)nserta_(cad)ena",
	     args2, "sopa_letras");

  // num_filas

  d.add_call("num_filas", trata_num_filas, 0, "(n)um_(fil)as", NULL,
	     "sopa_letras");

  d.add_call("nfil", trata_num_filas, 0, "(n)um_(fil)as", NULL,
	     "sopa_letras");

  // num_columnas

  d.add_call("num_columnas", trata_num_columnas, 0, "(n)um_(col)umnas", NULL,
	     "sopa_letras");

  d.add_call("ncol", trata_num_columnas, 0, "(n)um_(col)umnas", NULL,
	     "sopa_letras");

  // DICCIONARIO

  // inserta

  d.add_call("inserta", trata_inserta, 1, "(ins)erta", args2, "diccionario");
  d.add_call("ins", trata_inserta, 1, "(ins)erta", args2, "diccionario");

  // prefijo

  d.add_call("prefijo", trata_prefijo, 1, "(pre)fijo", args2, "diccionario");
  d.add_call("pre", trata_prefijo, 1, "(pre)fijo", args2, "diccionario");

  // satisfacen_patron

  // si la lista resultado se imprime pero no se almacena en un objeto de sistema

  d.add_call("sastisfacen_patron_p", trata_satisfacen_patron_pantalla, 1,
	     "(s)atisfacen_(pat)ron_(p)", args2, "diccionario");

  d.add_call("spatp",  trata_satisfacen_patron_pantalla, 1,
	     "(s)atisfacen_(pat)ron_(p)", args2, "diccionario");

  // si la lista resultado se quiere almacenar en un objeto de sistema;
  // habra que hacerse el user_init de list<string> y alguna funcion para imprimir


  d.add_call("sastisfacen_patron", trata_satisfacen_patron, 2,
	     "(s)atisfacen_(pat)ron", args2, "diccionario");

  d.add_call("spat",  trata_satisfacen_patron, 2,
	     "(s)atisfacen_(pat)ron", args2, "diccionario");

  // LIST_SORT

  // sort
  
  d.add_call("sort", trata_sort, 1, "sort<file>", args2, "*", false);

  // SOLVER

  // solve

  d.add_call("solve", trata_solve, 1, "(sol)ve", args2, "*");
  d.add_call("sol",  trata_solve, 1, "(sol)ve", args2, "*");

  // BUILDER

  // build

  args2[0] = args2[1] = args2[2] = args2[3] = "string";
  d.add_call("build", trata_build, 2, "(b)ui(ld)", args2, "*");
  d.add_call("bld", trata_build, 2, "(b)ui(ld)", args2, "*");

  d.add_call("checkbuild", trata_check_build, 2, "(c)heck(b)ui(ld)", args2, "*");
  d.add_call("cbld", trata_check_build, 2, "(c)heck(b)ui(ld)", args2, "*");

  // add to list<hueco>

  d.add_call("inserta_lista<hueco>", trata_inserta_lista_hueco, 1, "(i)nserta_(l)ista<(h)ueco>", args2, "list<hueco>");
  d.add_call("ilh", trata_inserta_lista_hueco, 1, "(i)nserta_(l)ista<(h)ueco>", args2, "list<hueco>");


  d.go();
}










