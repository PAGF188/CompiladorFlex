#ifndef __TABLA_S
#define __TABLA_S
/**
 * Archivo: TablaSimbolos.h
 * Versión: 1.0
 * Descripción: Exporta la parte pública de TablaSimbolos.c.
 * Objetivos:
 *       - Exportar las funciones de la tabla de símbolos que necesitarán invocar
 *         el resto de partes del compilador.
 *       - Exportar los tipos de datos que necesitarán emplear el resto de partes
 *         para interactuar correctamente con la TS. 
 */

/**
 * Estructura elegida: Árbol binario de búsqueda.
 * Motivo: En principio un árbol binario de búsqueda puede ser ídoneo para garantizar 
 * un rendimiento "aceptable" y no desperdiciar el uso de memoria. También podríamos 
 * pensar en una tabla Hash; pero en este caso, al no contemplar ámbitos, tendría que 
 * almacenar todas las palabras reservadas/identificadores del código por lo que su 
 * tamaño puede incrementarse notoriamente. Al aumentar su tamaño también aumenta la cantidad
 * de memoria desperdiciada para evitar colisiones (que la función hash funcione mejor).
 * Es decir al empelar árboles binarios conseguimos una mayor eficiencia espacial, 
 * pero perdemos la eficiencia temporal que nos proporciona las 
 * búsquedas/insercciones/eliminaciones "directas" de las tablas Hash.
 * Aún así estas operaciones en los árboles binarios de búsqueda siguen siendo eficientes, 
 * concretamente tienen complejidad temporal logarítmica.
 */

/**
 * @name tipoelem
 * Encapsula un lexema con su componente lexico.
 * Campo adicional para saber si se trata de una palabra reservada (1 si, 0 no).
 */
typedef struct {
    char* lexema;
    int componenteLexico;
}tipoelem;


/**
 * @name crearTablaSimbolos.
 * @objective: Inicializar la tabla.
 */
void crearTablaSimbolos();

/**
 * @name insertarPalabraReservada.
 * @objective: introducir en la tabla de símbolos las palabras reservadas del lenguaje.
 * @param E, lexema + componente léxico espalabraReservada.
 * @return int. -1 fallo al insertar la palabra (ej, palabra ya insertada).
 */
int insertarPalabraReservada(tipoelem E);

/**
 * @name insertarComponenteLexico.
 * @objective: Comportamiento:
 *           1) Si el lexema a insertar se trata de una palabra reservada, devolver entero 
 *             asociado al componente léxico de "lexema".
 *           2) Si el lexema es un identificador ya insertado previamente, devolver entero
 *             asociado al componente léxico de "lexema".
 *           3) Si el lexema no está insertado en la tabla, insertarlo (como identificador) y devolver entero
 *             asociado al componente léxico de "lexema".
 * @param E, lexema (identificador o palabra reservada).
 * @return int. Devuelve el valor entero asociado al componente léxico de "lexema".
 */
int insertarComponenteLexico(char* lexema);

/**
 * @name destruirTablaSimbolos
 * @objective: eliminar la tabla de símbolos liberando toda la memoria.
 */
void destruirTablaSimbolos();

//borrar
//void imprimirArbol();

#endif	/* __TABLA_S */