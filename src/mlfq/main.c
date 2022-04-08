#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <stdbool.h>
#include <stdint.h>
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
	// FILE *fpt, *debug;
	FILE *fpt;
	fpt = fopen((char *)argv[2], "w+");
	// debug = fopen("debug.txt", "w+");
	Queue *queues[3];
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

	queue_merge_sort(queues[2], 0, queues[2] -> process_quantity - 1);

	while (actual_tick >= 0) {
		bool change_cpu_state = false;
		bool added_to_q0 = false;
		int index = 2;
		bool find_running_process = true;

		while (queues[index] -> process_quantity == 0) {
			index --;
			if (index < 0) {
				break;
			}
		}

		if (index < 0) {
			break;
		}

		for (int i = 0; i < queues[index] -> process_quantity; i ++) {
			Process *actual_process = queues[index] -> processes[i];

			if (actual_process) {
				if (actual_process -> init_time == actual_tick && actual_process -> state == CREATED) {
					actual_process -> state = READY;
				}

				if (actual_process -> init_time > actual_tick) {
					break;
				}

				if (actual_process -> state == READY) {
					// El proceso no puede entrar de una a CPU, tiene que estar READY, revisar
					if (cpu_free && actual_process -> in_ready_count > 0) {
						cpu_free = false;
						actual_process -> state = RUNNING;
						find_running_process = false;
						if (actual_process -> in_cpu_count == 0) {
							actual_process -> first_execution = actual_tick;
						}
						actual_process -> in_cpu_count ++;
					} else {
						actual_process -> in_ready_count ++;
					}
				} else if (actual_process -> state == RUNNING) {
					find_running_process = false;
					actual_process -> running_time ++;
					actual_process -> quantum_time ++;
					if (actual_process -> running_time == actual_process -> cycles) {
						actual_process -> state = FINISHED;
						process_pop(queues[index], i);
						i --;
						fprintf(
							fpt,"%s,%i,%i,%i,%i,%i\n",
							actual_process -> name,
							actual_process -> in_cpu_count,
							actual_process -> interrupt_count,
							actual_tick - actual_process -> init_time,
							actual_process -> first_execution - actual_process -> init_time,
							actual_process -> in_ready_count + actual_process -> in_waiting_count
						);
						process_destroy(actual_process);
						actual_process = NULL;
						change_cpu_state = true;
					} else if (actual_process -> running_time % actual_process -> wait == 0) {
						actual_process -> state = WAITING;
						actual_process -> quantum_time = 0;
						change_cpu_state = true;
						if (index != 2) {
							actual_process -> enter_queue_time = actual_tick;
							process_pop(queues[index], i);
							i --;
							process_insert(queues[2], actual_process);
							actual_process = NULL;
						}
					} else if (index != 0) {
						if (actual_process -> quantum_time == queues[index] -> quantum) {
							actual_process -> state = READY;
							actual_process -> quantum_time = 0;
							actual_process -> interrupt_count ++;
							change_cpu_state = true;
							actual_process -> enter_queue_time = actual_tick;
							process_pop(queues[index], i);
							i --;
							process_insert(queues[index - 1], actual_process);
							if (index - 1 == 0) {
								added_to_q0 = true;
							}
							actual_process = NULL;
						}
					}
				} else if (actual_process -> state == WAITING) {
					actual_process -> in_waiting_count ++;
					if (actual_process -> in_waiting_count % actual_process -> waiting_delay == 0) {
						actual_process -> state = READY;
					}
				}

				// Check S
				if (actual_process) {
					if (
						index != 2
						&& (actual_process -> state == WAITING || actual_process -> state == READY)
						&& (actual_tick - actual_process -> init_time % actual_process -> s == 0)
					) {
							process_pop(queues[index], i);
							i --;
							process_insert(queues[2], actual_process);
					}
				}
				// if (actual_process) {
				// 	fprintf(debug, "----------------------------------------\n");
				// 	fprintf(debug, "NAME: %s\n", actual_process -> name);
				// 	fprintf(debug, "ACTUAL_TICK: %i\n", actual_tick);
				// 	fprintf(debug, "STATE: %i\n", actual_process -> state);
				// 	fprintf(debug, "RUNNING_TIME: %i\n", actual_process -> running_time);
				// 	fprintf(debug, "QUANTUM_TIME: %i\n", actual_process -> quantum_time);
				// 	fprintf(debug, "S: %i\n", actual_process -> s);
				// 	fprintf(debug, "LAST_S: %i\n", actual_process -> last_s);
				// 	fprintf(debug, "ACTUAL_QUEUE: %i\n", index);
				// 	fprintf(debug, "INDEX %i\n", i);
				// 	fprintf(debug, "----------------------------------------\n");
				// }
			} else {
				break;
			}
		}

		index --;
		for (int j = index; j >= 0; j--) {
			for (int i = 0; i < queues[j] -> process_quantity; i++) {
				Process *actual_process = queues[j] -> processes[i];

				if (actual_process) {
					if (find_running_process) {
						if (actual_process -> state == READY) {
							// El proceso no puede entrar de una a CPU, tiene que estar READY, revisar
							if (cpu_free && actual_process -> in_ready_count > 0) {
								cpu_free = false;
								actual_process -> state = RUNNING;
								find_running_process = false;
								if (actual_process -> in_cpu_count == 0) {
									actual_process -> first_execution = actual_tick;
								}
								actual_process -> in_cpu_count ++;
							} else {
								actual_process -> in_ready_count ++;
							}
						} else if (actual_process -> state == RUNNING) {
							find_running_process = false;
							actual_process -> running_time ++;
							actual_process -> quantum_time ++;
							if (actual_process -> running_time == actual_process -> cycles) {
								actual_process -> state = FINISHED;
								process_pop(queues[j], i);
								i --;
								fprintf(
									fpt,"%s,%i,%i,%i,%i,%i\n",
									actual_process -> name,
									actual_process -> in_cpu_count,
									actual_process -> interrupt_count,
									actual_tick - actual_process -> init_time,
									actual_process -> first_execution - actual_process -> init_time,
									actual_process -> in_ready_count + actual_process -> in_waiting_count
								);
								process_destroy(actual_process);
								actual_process = NULL;
								change_cpu_state = true;
							} else if (actual_process -> running_time % actual_process -> wait == 0) {
								actual_process -> state = WAITING;
								actual_process -> quantum_time = 0;
								change_cpu_state = true;
								actual_process -> enter_queue_time = actual_tick;
								process_pop(queues[j], i);
								i --;
								process_insert(queues[2], actual_process);
								actual_process = NULL;
							} else if (j != 0) {
								if (actual_process -> quantum_time == queues[j] -> quantum) {
									actual_process -> state = READY;
									actual_process -> quantum_time = 0;
									actual_process -> interrupt_count ++;
									change_cpu_state = true;
									actual_process -> enter_queue_time = actual_tick;
									process_pop(queues[j], i);
									i --;
									if (actual_tick - actual_process -> init_time % actual_process -> s == 0 || actual_process -> s_debt) {
										actual_process -> s_debt = 0;
										process_insert(queues[2], actual_process);
									} else {
										process_insert(queues[j - 1], actual_process);
									}
									actual_process = NULL;
									if (j - 1 == 0) {
										added_to_q0 = true;
									}
								}
							}
						}
					}
				} else {
					break;
				}

				// if (actual_process) {
				// 	fprintf(debug, "----------------------------------------\n");
				// 	fprintf(debug, "NAME: %s\n", actual_process -> name);
				// 	fprintf(debug, "ACTUAL_TICK: %i\n", actual_tick);
				// 	fprintf(debug, "STATE: %i\n", actual_process -> state);
				// 	fprintf(debug, "RUNNING_TIME: %i\n", actual_process -> running_time);
				// 	fprintf(debug, "QUANTUM_TIME: %i\n", actual_process -> quantum_time);
				// 	fprintf(debug, "S: %i\n", actual_process -> s);
				// 	fprintf(debug, "LAST_S: %i\n", actual_process -> last_s);
				// 	fprintf(debug, "ACTUAL_QUEUE: %i\n", j);
				// 	fprintf(debug, "INDEX %i\n", i);
				// 	fprintf(debug, "----------------------------------------\n");
				// }

				// Check S
				if (actual_process) {
					if (
						(actual_process -> state == WAITING || actual_process -> state == READY)
						&& (actual_tick - actual_process -> init_time % actual_process -> s == 0)
					) {
							process_pop(queues[index], i);
							i --;
							process_insert(queues[2], actual_process);
					} else if (actual_process -> state == RUNNING
						&& actual_tick - actual_process -> init_time % actual_process -> s == 0) {
							actual_process -> s_debt = 1;
					}
				}
			}
		}

		if (change_cpu_state) {
			cpu_free = true;
		}
		
		if (added_to_q0 && queues[0] -> process_quantity > 1) {
			queue_merge_sort(queues[0], 0, queues[0] -> process_quantity - 1);
		}
		actual_tick ++;
	}

	fclose(fpt);
	for (int i = 2; i >= 0; i --) {
		queue_destroy(queues[i]);
	}
	input_file_destroy(input_file);
}
