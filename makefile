CC= gcc -Wall

HEADER_FILES_DIR = ./headerFiles
INCLUDES = -I $(HEADER_FILES_DIR)
MAIN= compilador
SRCS = main.c Lexico.c Sintactico.c TablaSimbolos.c Errores.c
DEPS = $(HEADER_FILES_DIR)/Definiciones.h $(HEADER_FILES_DIR)/Lexico.h $(HEADER_FILES_DIR)/Sintactico.h $(HEADER_FILES_DIR)/TablaSimbolos.h $(HEADER_FILES_DIR)/Errores.h


OBJS = $(SRCS:.c=.o)

$(MAIN): $(OBJS)

	$(CC) -o $(MAIN) $(OBJS)

%.o: %.c $(DEPS)

	gcc -Wall -c -o $@ $< $(INCLUDES) -lfl

cleanall: clean

	rm -f $(MAIN)

clean:

	rm -f *.o *~
