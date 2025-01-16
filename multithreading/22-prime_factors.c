#include "multithreading.h"
#include <stdlib.h>

/**
 * create_task - Creates a new task
 * @entry: Pointer to the entry function
 * @param: Parameter to be passed to the entry function
 * Return: Pointer to the created task
 */
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task = malloc(sizeof(task_t));

	if (!task)
		return (NULL);

	task->entry = entry;
	task->param = param;
	task->status = PENDING;
	task->result = NULL;
	pthread_mutex_init(&task->lock, NULL);

	return (task);
}

/**
 * destroy_task - Destroys a task
 * @task: Pointer to the task to destroy
 */
void destroy_task(task_t *task)
{
	if (!task)
		return;

	pthread_mutex_destroy(&task->lock);

	if (task->result)
		list_destroy(task->result, free);

	free(task->result);
	free(task);
}

/**
 * exec_tasks - Thread entry point for task execution
 * @tasks: Pointer to the list of tasks
 * Return: NULL
 */
void *exec_tasks(list_t const *tasks)
{
	node_t *node;
	task_t *task;
	int task_id = 0;

	for (node = tasks->head; node; node = node->next)
	{
		task = node->content;
		pthread_mutex_lock(&task->lock);

		if (task->status == PENDING)
		{
			task->status = STARTED;
			tprintf("[%02d] Started\n", task_id);
			pthread_mutex_unlock(&task->lock);

			task->result = task->entry(task->param);

			pthread_mutex_lock(&task->lock);
			task->status = task->result ? SUCCESS : FAILURE;
			tprintf("[%02d] Success\n", task_id);
		}

		pthread_mutex_unlock(&task->lock);
		task_id++;
	}
	return (NULL);
}
