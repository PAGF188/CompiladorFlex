/**
 * Archivo: Sintactico.c
 * Versión: 1.0
 * Descripción: Implementa el analizador sintáctico del compilador.
 * Funciones:
 *       - Pedir los componentes léxicos al analizador léxico.
 *       - Imprimirlos por pantalla
 */

#include <stdio.h>
#include <stdlib.h>
#include "./headerFiles/Sintactico.h"
#include "./headerFiles/Lexico.h"
#include "./headerFiles/Definiciones.h"


//Las definiciones de las funciones públicas se encuentran en el .h

void sintactico(){

    tipoLexico l;
    do{
        l = siguienteComponente();
        printf("[<%d>, '%s']\n",l.componenteLexico, l.lexema);
    }while(l.componenteLexico != _EOF);

}