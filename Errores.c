/**
 * Archivo: Errores.c
 * Versión: 1.0
 * Descripción: Centraliza la gestión de errores por parte del compilador
 * Objetivos:
 *       - Mostrar el error correspondiente en función del valor de entrada.
 */


#include <stdio.h>
#include "./headerFiles/Errores.h"

//Las definiciones de las funciones públicas se encuentran en el .h

void imprimeError(int codigo, int linea){

    switch (codigo)
    {
    case 1:
        printf("\n\nArgument error! : Solo puedes pasar un archivo para compilar.\n\n");
        break;
    case 2: 
        printf("\n\nEntry System error! : El archivo no es accesible.\n\n");
        break;
    case 3:
        printf("\n\nEntry System error! : El archivo no se pudo cerrar.\n\n");
        break;
    case 4:
        printf("\n\n(linea %d) Lexical Error! : El tamaño del lexema excede al máximo permitido.\n\n", linea);
        break;
    case 5:
        printf("\n\n(linea %d) Lexical Error! : Falta exponente.\n\n",linea);
        break;
    case 6:
        printf("\n\n(linea %d) Lexical Error! : Caracter extraño.\n\n",linea);
        break;
    default:
        break;
    }
}