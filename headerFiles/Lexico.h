#ifndef __LEXICO
#define __LEXICO

/**
 * Archivo: Lexico.h
 * Versión: 1.0
 * Descripción: Exporta la parte pública de Lexico.c.
 * Objetivos:
 *       - Devolver el siguiente componente léxico (+ lexema) al analizador sintáctico.
 *       - Exportar el tipo de datos que encapsula componente léxico + lexema.
 */

/**
 * Dato que encapsula el lexema junto con su componenteLexico.
 */
typedef struct {
    char* lexema;
    int componenteLexico;
}tipoLexico;

/**
 * @name siguienteComponente()
 * @objective: Obtiene el siguiente componente léxico del codigo a analizar y lo
 *             devuelve junto con su lexema. 
 */
tipoLexico siguienteComponente();


/**
 * Función para iniciar el sistema de entrada. 
 * Comprueba que existe el archivo.
 * Introduce en flex el archivo a analizar. 
 * @param path, ruta al archivo a compilar.
 * @return int, código de error (-1 fallo en el inicio, 1 inicio correcto)
 */ 
int inicia(char *path);

#endif	/* __LEXICO */