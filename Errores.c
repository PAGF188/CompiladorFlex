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

void imprimeError(int codigo, int linea, char * lexema){

    switch (codigo)
    {
    case 1:
        printf("Argument error! : Solo puedes pasar un archivo para compilar.\n\n");
        break;
    case 2: 
        printf("Entry System error! : El archivo no es accesible.\n\n");
        break;
    case 3:
        printf("Entry System error! : El archivo no se pudo cerrar.\n\n");
        break;
    case 5:
        printf("(linea %d) Lexical Error! : Falta exponente.\n\n",linea);
        break;
    case 6:
        printf("(linea %d) Lexical Error! Caracter extraño: %s\n\n",linea,lexema);
        break;
    case 7:
        printf("(linea %d) Lexical Error! Entero mal formado: %s\n\n",linea, lexema);
        break;
    case 8:
        printf("(linea %d) Lexical Error! Flotante mal formado: %s\n\n",linea, lexema);
        break;
     case 9:
        printf("(linea %d) Lexical Error! Falta la \" de cierre del String: \" \n\n",linea);
        break;
    case 10:
        printf("(linea %d) Lexical Error! Falta cierre del comentario: +/\n\n",linea);
        break;
    default:
        break;
    }
}