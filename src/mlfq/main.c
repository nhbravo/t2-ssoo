#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <stdbool.h>
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
	Queue **queues;
	for (int i = 2; i >= 0; i --) {
		queues[i] = queue_init(0, i, atoi(argv[3]), input_file->len);
	}
	int actual_tick = 0;
	bool cpu_free = true;

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
		process_insert(queues[2], process);
	}

	while (actual_tick >= 0) {
		int index = 2;
		while (queues[index] -> process_quantity == 0) {
			index --;
			if (index < 0) {
				break;
			}
		}

		if (index < 0) {
			break;
		}

		for (int i; i < queues[index] -> process_quantity; i ++) {
			Process *actual_process = queues[index] -> processes[i];

			if (actual_process) {
				if (actual_process -> init_time > actual_tick) {
					break;
				}
				if (actual_process -> state == READY || actual_process -> init_time <= actual_tick) {
					actual_process -> state = READY;
					if (cpu_free) {
						cpu_free = false;
						actual_process -> state = RUNNING;
					}
				} else if (actual_process -> state == RUNNING) {

				} else if (actual_process -> state == FINISHED) {
					Process *finished_process = process_pop(queues[index]);
					process_destroy(finished_process);
				}
			}
		}

		index --;
		for (index; index >= 0; index--) {
			for (int i; i < queues[index] -> process_quantity; i++) {
				Process *actual_process = queues[index] -> processes[i];

				if ((actual_tick - actual_process -> init_time) % actual_process -> s == 0) {
					continue;
				}
			}
		}
		
		actual_tick ++;
	}

	for (int i = 2; i >= 0; i --) {
		queue_destroy(queues[i]);
	}
	input_file_destroy(input_file);
}