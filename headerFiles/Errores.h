#ifndef __ERRORES
#define __ERRORES

/**
 * Archivo: Errores.h
 * Versión: 1.0
 * Descripción: Exporta la parte pública de Errores.c.
 * Objetivos:
 *       - Exportar las funciones del gestor de errores que necesitarán invocar
 *         el resto de partes del compilador.
 */

/**
 * @name imprimeError
 * @objective: imprimir el error correspondiente en función de su código.
 * @param codigo, valor identificativo del error. Puede ser:
 *                (*) 1 -> se ha pasado más de 1 argumento.
 *                (*) 2 -> no se pudo acceder al archivo a compilar. 
 *                (*) 3 -> no se pudo cerrar el archivo de compilación al acabar.
 *                (*) 6-> caracter no conocido.
 *                (*) 7-> entero mal formado.
 *                (*) 8-> flotante mal formado.
 *                (*) 9-> falta la comilla de cierre del String
 *                (*) 10-> falta cierre de comentario +/
 * @param linea del error.
 * @param lexema
 */
void imprimeError(int codigo, int linea, char * lexema);


#endif	/* __ERRORES */