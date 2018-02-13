/**
 * apix.h - PROS Extended API header
 *
 * Contains additional declarations for use by advaned users of PROS. These
 * functions do not typically have as much error handling or require deeper
 * knowledge of real time operating systems.
 *
 * Copyright (c) 2017-2018, Purdue University ACM SIGBots.
 * All rights reservered.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_API_EXTENDED_H_
#define _PROS_API_EXTENDED_H_

#include "api.h"
#include "tmei.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**                             RTOS FACILITIES                              **/
/**                                                                          **/
/**                                                                          **/
/** See https://pros.cs.purdue.edu/v5/extended/multitasking to learn more.   **/
/******************************************************************************/

typedef void* queue_t;
typedef void* sem_t;

/**
 * Unblocks a task in the Blocked state (e.g. waiting for a delay, on a semaphore, etc.)
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#abort_delay for details.
 */
bool task_abort_delay(task_t task);

/**
 * Creates a recursive mutex which can be locked recursively by the owner.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#recursive_mutexes for details.
 *
 * \return
 *          A newly created recursive mutex.
 */
mutex_t mutex_recursive_create(void);

/**
 * Takes a recursive mutex
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#recursive_mutexes for details.
 *
 * \param mutex
 *          A mutex handle created by mutex_recursive_create
 * \param wait_time
 *          Amount of time to wait before timing out
 * \return
 *          1 if the mutex was obtained, 0 otherwise
 */
bool mutex_recursive_take(mutex_t mutex, uint32_t wait_time);

/**
 * Gives a recursive mutex
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#recursive_mutexes for details.
 *
 * \param mutex
 *          A mutex handle created by mutex_recursive_create
 * \return
 *          1 if the mutex was obtained, 0 otherwise
 */
bool mutex_recursive_give(mutex_t mutex);

/**
 * Returns a handle to the current owner of a mutex.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#extra for details.
 *
 * \param mutex
 *          A mutex handle
 * \return
 *          A handle to the current task that owns the mutex, or NULL if the mutex isn't owned.
 */
task_t mutex_get_owner(mutex_t mutex);

/**
 * Creates a counting sempahore.
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/multitasking#semaphores for details.
 *
 * \param max_count
 *          The maximum count value that can be reached.
 * \param init_count
 *          The initial count value assigned to the new semaphore.
 * \return
 *          A newly created semaphore. If an error occurred, NULL will be
 *			returned and errno can be checked for hints as to why sem_create failed.
 */
sem_t sem_create(uint32_t max_count, uint32_t init_count);

/**
 * Creates a binary semaphore.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#binary_semaphores for details.
 *
 * \return
 *          A newly created semaphore.
 */
sem_t sem_binary_create(void);

/**
 * Waits for the semaphore's value to be greater than 0. If the value is already
 * greater than 0, this function immediately returns.
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/multitasking#semaphores for details.
 *
 * \param sem
 *          Semaphore to wait on
 * \param timeout
 *          Time to wait before the semaphore's becomes available. A timeout of 0
 *          can be used to poll the sempahore. TIMEOUT_MAX can be used to block
 *          indefinitely.
 * \return
 *          True if the semaphore was successfully take, false otherwise.
 *          If false is returned, then errno is set with a hint about why the
 *          sempahore couldn't be taken.
 */
bool sem_wait(sem_t sem, uint32_t timeout);

/**
 * Increments a semaphore's value.
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/multitasking#semaphores for details.
 *
 * \param sem
 *          Semaphore to post
 * \return
 *          True if the value was incremented, false otherwise. If false is
 *          returned, then errno is set with a hint about why the semaphore
 *          couldn't be taken.
 */
bool sem_post(sem_t sem);

/**
 * Returns the current value of the semaphore.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#extra for details.
 *
 * \param sem
 *          A semaphore handle
 * \return
 *          The current value of the semaphore (e.g. the number of resources available)
 */
uint32_t sem_get_count(sem_t sem);

/**
 * Creates a queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#queues for details.
 *
 * \param length
 *          The maximum number of items that the queue can contain.
 * \param item_size
 *          The number of bytes each item in the queue will require.
 * \return
 *          A handle to a newly created queue, or NULL if the queue cannot be created.
 */
queue_t queue_create(uint32_t length, uint32_t item_size);

/**
 * Posts an item to the front of a queue. The item is queued by copy, not by reference.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#queues for details.
 *
 * \param queue
 *          The queue handle
 * \param item
 *          A pointer to the item that will be placed on the queue.
 * \param timeout
 *          Time to wait for space to become available. A timeout of 0 can
 *          be used to attempt to post without blocking. TIMEOUT_MAX can be used to
 *          block indefinitely.
 * \return
 *          true if the item was preprended, false otherwise.
 */
bool queue_prepend(queue_t queue, const void* item, uint32_t timeout);

/**
 * Posts an item to the end of a queue. The item is queued by copy, not by reference.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#queues for details.
 *
 * \param queue
 *          The queue handle
 * \param item
 *          A pointer to the item that will be placed on the queue.
 * \param timeout
 *          Time to wait for space to become available. A timeout of 0 can
 *          be used to attempt to post without blocking. TIMEOUT_MAX can be used to
 *          block indefinitely.
 * \return
 *          true if the item was preprended, false otherwise.
 */
bool queue_append(queue_t queue, const void* item, uint32_t timeout);

/**
 * Receive an item from a queue without removing the item from the queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#queues for details.
 *
 * \param queue
 *          The queue handle
 * \param buffer
 *          Pointer to a buffer to which the received item will be copied
 * \param timeout
 *          Time to wait for an item to be placed on the queue if the queue is empty.
 *          A timeout 0 will return immediately if the queue is empty. TIMEOUT_MAX can
 *          be used to block indefinitely.
 * \return
 *          True if an item was copied into the buffer, false otherwise.
 */
bool queue_peek(queue_t queue, void* buffer, uint32_t timeout);

/**
 * Receive an item from the queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#queues for details.
 *
 * \param queue
 *          The queue handle
 * \param buffer
 *          Pointer to a buffer to which the received item will be copied
 * \param timeout
 *          Time to wait for an item to be placed on the queue if the queue is empty.
 *          A timeout 0 will return immediately if the queue is empty. TIMEOUT_MAX can
 *          be used to block indefinitely.
 * \return
 *          True if an item was copied into the buffer, false otherwise.
 */
bool queue_recv(queue_t queue, void* buffer, uint32_t timeout);

/**
 * Return the number of messages stored in a queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#queues for details.
 *
 * \param queue
 *          The queue handle.
 * \return
 *          The number of messages available in the queue.
 */
uint32_t queue_get_waiting(const queue_t queue);

/**
 * Return the number of spaces left in a queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#queues for details.
 *
 * \param queue
 *          The queue handle.
 * \return
 *          The number of spaces available in the queue.
 */
uint32_t queue_get_available(const queue_t queue);

/**
 * Delete a queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking#queues for details.
 *
 * \param queue
 *          Queue handle to delete
 */
void queue_delete(queue_t queue);

/**
 * Resets a queue to an empty state
 *
 * \param queue
 *          Queue handle to reset
 */
void queue_reset(queue_t queue);

/******************************************************************************/
/**                           Device Registration                            **/
/******************************************************************************/

/*
 * \brief List of possible v5 devices
 *
 * This list contains all current V5 Devices, and mirrors V5_DeviceType from the
 * api.
 */
typedef enum v5_device_e {
	E_DEVICE_NONE = 0,
	E_DEVICE_MOTOR = 2,
	E_DEVICE_LED = 3,
	E_DEVICE_RGB = 4,
	E_DEVICE_BUMPER = 5,
	E_DEVICE_IMU = 6,
	E_DEVICE_RANGE = 7,
	E_DEVICE_RADIO = 8,
	E_DEVICE_TETHER = 9,
	E_DEVICE_BRAIN = 10,
	E_DEVICE_VISION = 11,
	E_DEVICE_ADI = 12,
	E_DEVICE_GYRO = 0x46,
	E_DEVICE_SONAR = 0x47,
	E_DEVICE_GENERIC = 128,
	E_DEVICE_UNDEFINED = 255
} v5_device_e;

/*
 * \brief Registers a device in the given port
 *
 * Registers a device of the given type in the given port into the registry, if
 * that type of device is detected to be plugged in to that port.
 *
 * \param[in]  port             the port number to register the device
 * \param[in]  device			the type of device to register
 *
 * \return 1 upon success, PROS_ERR upon failure
 *
 * \exception EINVAL Port number is out of range.
 * \exception EINVAL A different device than specified is plugged in.
 * \exception EADDRINUSE the port is already registered to another device
 */
int registry_bind_port(unsigned int port, v5_device_e device_type);

/*
 * \brief Deregisters a devices from the given port
 *
 * Removes the device registed in the given port, if there is one.
 *
 * \param[in]  port      		the port number to deregister
 *
 * \return 1 upon success, PROS_ERR upon failure
 *
 * \exception EINVAL the port number is out of range
 */
int registry_unbind_port(unsigned int port);

#ifdef __cplusplus
}
#endif

#endif /* _PROS_API_EXTENDED_H_ */
