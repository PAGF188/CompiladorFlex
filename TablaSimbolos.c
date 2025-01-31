/**
 * Archivo: TablaSimbolos.c
 * Versión: 1.0
 * Descripción: Implementa la tabla de símbolos del compilador.
 * Funciones:
 *       - Almacenar las palabras reservadas del lenguaje.
 *       - Almacenar los identificadores insertados por el analizador léxico.
 *       - Permitir distinguir al analizador léxico si una cadena alfanumérica es un identificador
 *         o una palabra reservada. 
 *       - Devolver el componente léxico de cada lexema almacenado.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "TablaSimbolos.h"
#include "Definiciones.h"

//Nodo del árbol. Información y punteros a hijo derecho/izquierdo.
struct celda {
	tipoelem info;
	struct celda *izq, *der;
};
typedef struct celda *abb;

//Clave de búsqueda. ES EL LEXEMA. 
typedef char* tipoclave;

/////// VARIABLE GLOBAL CON LA TABLA DE SÍMBOLOS (nodo raíz)
abb tablaSimbolos;



/////////////////// DEFINICIÓN FUNCIONES PRIVADAS

//Compara las claves de dos nodos (dos lexemas). (0 iguales. -1 cl1<cl2. 1 cl1>cl2)
int _comparar_claves(tipoclave cl1, tipoclave cl2);
// Extraer la clave de una celda (el lexema de un nodo)
tipoclave _clave_elem(tipoelem * E);
// compara una clave (cl) con la clave del nodo E. [0 iguales. -1 cl1<cl2. 1 cl1>cl2]
int _comparar_clave_elem(tipoclave cl, tipoelem E);
// Devuelve 0 si A es nulo, 1 en caso contrario.
unsigned es_vacio(abb A);
// Devuelve la referencia al hijo derecho de A.
abb der(abb A);
// Devuelve la referencia al hijo izquierdo de A.
abb izq(abb A);
// Inserta en A el elemento E.
void insertar(abb *A, tipoelem E);
// Almacena en E la referencia a la información del nodo A. 
void info(abb A, tipoelem *E);
// 1 si el nodo con lexema cl está ya en el árbol. 0 caso contrario
unsigned es_miembro_clave(abb A, tipoclave cl);
// devuele en *nodo, el nodo cuya clave coincida con cl.
void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo);
// eliminar el arbol y limpia la memoria.
void destruir_arbol(abb *A);
//Las dos siguientes permiten eliminar un nodo concreto del árbol.
//Sin embargo, en esta práctica no se usan. Quedan hechas por si se necesitan en la siguiente.
void suprimir(abb *A, tipoelem E);
tipoelem _suprimir_min(abb *A);

// BORRAR!!!!!! Función de comprobación. Imprime toda la tabla de símbolos. Recorrido inorden del árbol.
void imprime_tabla(abb A){
    tipoelem E;
    if (!es_vacio(A)){
        imprime_tabla(izq(A));
        info(A, &E);
        printf("%s, %d\n", E.lexema, E.componenteLexico);
        imprime_tabla(der(A));
    }
}

/////////////////// FUNCIONES PÚBLICAS (definición en .h)

void crearTablaSimbolos(){
	tablaSimbolos = NULL;
}

void destruirTablaSimbolos() {
    destruir_arbol(&tablaSimbolos);
}

int insertarPalabraReservada(tipoelem E){
    //Si no está activado el campo de palabra reservada
    // o lexema es nulo o palabra reservada ya insertada -> error.
    if(E.lexema==NULL || es_miembro_clave(tablaSimbolos,E.lexema)){
        return(-1);
    }
    insertar(&tablaSimbolos,E);
    return(1);
}

int insertarComponenteLexico(char* lexema){
    //Si ya está el nodo en la tabla de símbolos puede ser palabra reservada o identificador,
    //independientemente de ello devolvemos su componente léxico.
    if(es_miembro_clave(tablaSimbolos,lexema)){
        tipoelem nodo;
        buscar_nodo(tablaSimbolos,lexema,&nodo);
        return(nodo.componenteLexico);
    }
    //si no está es un identificador -> lo insertamos y devolvemos id asignado.
    else{
        tipoelem aux;
        aux.lexema = lexema; 
        aux.componenteLexico = _ID;
        insertar(&tablaSimbolos,aux);
        return(_ID);
    }
}

void imprimirArbol(){
    imprime_tabla(tablaSimbolos);
}


/////////////////// IMPLEMENTACIÓN FUNCIONES PRIVADAS
int _comparar_claves(tipoclave cl1, tipoclave cl2){
    return strcmp(cl1,cl2)==0 ? 0 : strcmp(cl1,cl2)>0 ? 1 : -1; 
}

int _comparar_clave_elem(tipoclave cl, tipoelem E){
	return _comparar_claves(cl, _clave_elem(&E)); 
}

tipoclave _clave_elem(tipoelem * E){
	return (E->lexema);
}

unsigned es_vacio(abb A) {
	return A == NULL;
}

abb izq(abb A) {
    return A->izq;
}

abb der(abb A) {
    return A->der;
}

void insertar(abb *A, tipoelem E) {
    if (es_vacio(*A)) {
        *A = (abb) malloc(sizeof (struct celda));
        (*A)->info = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return;
    } 
    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info );
    if (comp > 0 ) {
        insertar(&(*A)->der, E);
    } else {
        insertar(&(*A)->izq, E);
    }
}

void info(abb A, tipoelem *E){
    *E = A->info;
}

unsigned es_miembro_clave(abb A, tipoclave cl) {
    if (es_vacio(A)) {
        return 0;
    } 
    int comp = _comparar_clave_elem(cl, A->info);
    if (comp == 0) {
        return 1;
    } 
    if (comp > 0) {
        return es_miembro_clave(A->der, cl);
    } 
    return es_miembro_clave(A->izq, cl);
}

void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo) {
    if (es_vacio(A)) {
        return;
    } 
    int comp = _comparar_clave_elem(cl, A->info);
    
	if (comp == 0) {
    	*nodo = A->info;    //aqui almacenamos referencia
	} else if (comp < 0) {
	    buscar_nodo(A->izq, cl, nodo);
	} else {
	    buscar_nodo(A->der, cl, nodo);
	}
}

void destruir_arbol(abb *A) {
	if (!es_vacio(*A)) {
		destruir_arbol(&((*A)->izq));
		destruir_arbol(&((*A)->der));
		free(*A);
		*A = NULL;
	}
}


//Las dos siguientes permiten eliminar un nodo concreto del árbol.
//Sin embargo, en esta práctica no se usan. Quedan hechas por si se necesitan en la siguiente.

void suprimir(abb *A, tipoelem E) {
    abb aux;
    if(es_vacio(*A)){
       return;
    }
    
    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if(comp < 0){ 
        suprimir(&(*A)->izq, E);
    } else if (comp > 0){ 
        suprimir(&(*A)->der, E);
    } else if (es_vacio((*A)->izq) && es_vacio((*A)->der)) {
        free(*A);
        *A = NULL;
    } else if (es_vacio((*A)->izq)) { 	// pero no es vacio derecha
        aux = *A;
        *A = (*A)->der;
        free(aux);
    } else if (es_vacio((*A)->der)) { 	//pero no es vacio izquierda
        aux = *A;
        *A = (*A)->izq;
        free(aux);
    } else { //ni derecha ni izquierda esta vacio
        (*A)->info = _suprimir_min(&(*A)->der);
    }
}

tipoelem _suprimir_min(abb *A) {
    abb aux;
    tipoelem ele;
    if (es_vacio((*A)->izq)) {
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->der;
        free(aux);
        return ele;
    } else {
        return _suprimir_min(&(*A)->izq);
	}
}


