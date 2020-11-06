
#ifndef __DEFINICIONES
#define __DEFINICIONES
/**
 * Archivo: Definiciones.h
 * Versión: 1.0
 * Descripción: Define un valor entero para cada uno de los componentes léxicos.
 * Objetivos:
 *       - Asociar cada componente léxico con un valor distintivo.
 *       - Exportar esta asociación para el resto de archivos.
 */


/**
 * EL IDENTIFICADOR DE CARACTERES ASCII (ej: .,;{}[]) es su valor ASCII. 
 * POR LO TANTO NO SE INCLUYEN EN ESTE ARCHIVO.
 */

//Palabras reservadas. No están todas las del leguaje, están las que se necesitan para hacer la práctica.
#define _IMPORT 300
#define _DOUBLE 301
#define _INT 302
#define _WHILE 303
#define _FOREACH 304
#define _RETURN 305
#define _VOID 306
#define _CAST 308

//Otros componentes léxicos (no están todos los del lenguaje solo los necesarios para analizar el código de referencia)
#define _ID 400
#define _STRING 401     //Ejemplo: "asd" 
#define _CACARTER 402   //Ejemplo: 'a'
#define _ENTERO 403     //Ejemplos: 123, 23_2, 0b123, 0x3EA, 123e+23
#define _FLOTANTE 404   //Ejemplos: 123.34, 0x23.A34

#define _DOBLE_IGUAL 405    // ==
#define _MAS_IGUAL 406      // +=
#define _DOBLE_MAS 407      // ++
#define _EOF 408            // EOF
#define _BARRA_IGUAL 409    // /=
#define _IGUAL_MAYOR 410    //  =>
#define _DOBLE_MENOS 411    // --
#define _MENOS_IGUAL 412    // -=
#define _MENOR_O_IGUAL 413  //<=
#define _POR_IGUAL 414      //*=


#endif	/* __DEFINICIONES */