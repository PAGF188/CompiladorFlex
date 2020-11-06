/**
 * Archivo: main.c
 * Versión: 1.0
 * Descripción: Archivo principal del proyecto compilador. Actúa a modo de disparador.
 * Objetivos:
 *       - Lectura del archivo a compilar
 *       - Inicialización de la tabla de símbolos.
 *       - Invocación del analizador sintático.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "./headerFiles/Errores.h"
#include "./headerFiles/Sintactico.h"
#include "./headerFiles/TablaSimbolos.h"
#include "./headerFiles/Definiciones.h"
#include "./headerFiles/Lexico.h"


int main(int argc, char *argv[])
{
    //Comprobaciones correctos argumentos pasados por línea de comandos. 
    //En principio un único archivo vamos a compilar.

    if(argc!=2){
        imprimeError(1,-1);
        return(-1);
    }
    
    //Iniciamos la tabla de símbolos
    crearTablaSimbolos();

    //Insertamos palabras reservadas
    //Para facilitar la insercción de palabras reservadas al inicio:
    char * palabrasReservadas[8] = {"import", "double","int",
                                     "while", "foreach", "return", "void", "cast"};
    int id[8] = {_IMPORT,_DOUBLE,_INT, _WHILE, _FOREACH, _RETURN, _VOID,  _CAST};
    
    for(int i=0; i<sizeof(id)/sizeof(id[0]); i++){
        tipoelem aux;
        aux.lexema = palabrasReservadas[i];
        aux.componenteLexico = id[i];
        insertarPalabraReservada(aux);
    }

    printf("\n\nImpresion de la Tabla de Símbolos\n");
    imprimirArbol();
    printf("\n\n");

    //Pasamos el archivo hasta flex
    if(inicia(argv[1])==-1){
        exit(-1);
    }

    //Invocamos al analizador sintáctico
    sintactico();

    printf("\n\n\nImpresion de la Tabla de Símbolos\n");
    imprimirArbol();
    
    destruirTablaSimbolos();

    return(0);
}
