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
	Queue *queue_2 = queue_init(input_file->len, 2, 5);
	Queue *queue_1 = queue_init(0, 1, 5);
	Queue *queue_0 = queue_init(0, 0, 5);
	int actual_tick = 0;

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
		process_insert(queue_2, process);
	}

	while (actual_tick >= 0) {
		Process *actual_process = process_pop(queue_2);

		printf("%i\n", actual_process->init_time);

		if (!actual_process) {
			actual_tick = -1;
		} else {
			actual_tick ++;
		}
	}

	queue_destroy(queue_2);
	queue_destroy(queue_1);
	queue_destroy(queue_0);
	input_file_destroy(input_file);
}