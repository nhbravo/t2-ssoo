#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	printf("Procesos:\n");
	Process *processes[input_file->len];
	for (int i = 0; i < input_file->len; ++i) {
		Process *process = process_init(
			atoi(input_file->lines[i][1]),
			input_file->lines[i][0],
			atoi(input_file->lines[i][2]),
			atoi(input_file->lines[i][3]),
			atoi(input_file->lines[i][4]),
			atoi(input_file->lines[i][5]),
			atoi(input_file->lines[i][6])
		);
		processes[i] = process;
	}

	for (int i = 0; i < input_file->len; ++i) {
		process_destroy(processes[i]);
	}
	input_file_destroy(input_file);
}