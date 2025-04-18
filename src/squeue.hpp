/**
 * @file    squeue.hpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    18-04-2025
 * @version 1.1.0
 *
 * @section DESCRIPTION
 *
 * A static memory allocated Queue to store any kind of data type elements
 * without using any kind of dynamic memory allocation.
 *
 * This function could be used to replace C++ STL queue component that uses
 * dynamic memory.
 *
 * The implementation of this Queue component is based on the use of a
 * circular buffer with two incremental indexes (head and tail) that returns
 * to zero when overflows on the queue maximum size. Appending a new element
 * increments the head index while removing an element increments the tail
 * index. The number of elements that are currently stored in the queue is
 * given by the difference between head and tail indexes. When the Queue is
 * full, the new elements will overwrite the older elements.
 */

/*****************************************************************************/

/* Include Guard */

#ifndef STATIC_QUEUE_H_
#define STATIC_QUEUE_H_

/*****************************************************************************/

/* Libraries */

// Standard C++ libraries
#include <cstddef>
#include <cstdint>
#include <cstring>
#if defined(SQUEUE_ENABLE_CONTAINS)
    #include <type_traits>
#endif // SQUEUE_ENABLE_CONTAINS

/*****************************************************************************/

/* Data Types */

/**
 * @brief SQueue functions result code values.
 */
enum class t_squeue_rc : uint8_t
{
    OK       = 0U,
    OVERFLOW = 1U,
    ERROR   = 255U,
};

/*****************************************************************************/

/* Class Interface */

template <typename T_QUEUE_ELEMENTS, uint32_t QUEUE_SIZE>
class SQueue
{
#if defined(SQUEUE_ENABLE_CONTAINS)
    static_assert(
        std::is_convertible<
            decltype(std::declval<T_QUEUE_ELEMENTS>()
            == std::declval<T_QUEUE_ELEMENTS>()), bool>::value,
        "T_QUEUE_ELEMENTS must support operator=="
    );
#endif // SQUEUE_ENABLE_CONTAINS

    public:

        /* Public Methods */

        /**
         * @brief Construct a SQueue object.
         */
        SQueue()
        :
            queue_head(0U),
            queue_tail(0U),
            num_elements_stored(0U),
            queue_overflow(false)
        {}

        /**
         * @brief Clear the Queue.
         */
        void clear()
        {
            queue_head = 0U;
            queue_tail = 0U;
            num_elements_stored = 0U;
            queue_overflow = false;
        }

        /**
         * @brief Returns the number of elements currently stored in the Queue.
         * @return uint32_t The number of elements in the Queue.
         */
        uint32_t size() const
        {
            return num_elements_stored;
        }

        /**
         * @brief Check if the Queue is empty (no elements in the buffer).
         * @return true if the Queue is empty.
         * @return false otherwise.
         */
        bool empty() const
        {
            return ( num_elements_stored == 0U );
        }

        /**
         * @brief Checks if the Queue is full.
         * @return true if the Queue is full.
         * @return false otherwise.
         * @details
         * This function checks if the Queue is full by checking if current
         * size is equal maximum static queue size (QUEUE_SIZE).
         */
        bool full() const
        {
            return ( num_elements_stored >= QUEUE_SIZE );
        }

        /**
         * @brief Returns reference to the first element in the Queue. This
         * element will be the first element to be removed on a call to pop().
         * @return T_QUEUE_ELEMENTS* Reference to the first element.
         * @details
         * This function get the address of the first stored element of the
         * queue. If there is no elements on the Queue, a nullptr is returned.
         */
        T_QUEUE_ELEMENTS* front()
        {
            if ( empty() )
            {   return nullptr;   }
            else
            {   return &(buffer[queue_tail]);   }
        }

        /**
         * @brief Returns reference to the last element in the Queue. This is
         * the most recently pushed element.
         * @return T_QUEUE_ELEMENTS* Reference to the last element.
         * @details
         * This function get the address of the last stored element of the
         * queue. If there is no elements on the Queue, a nullptr is returned.
         */
        T_QUEUE_ELEMENTS* back()
        {
            if ( empty() )
            {   return nullptr;   }

            uint32_t last_index = (queue_head == 0U) ?
                (QUEUE_SIZE - 1U) : (queue_head - 1U);
            return &(buffer[last_index]);
        }

        /**
         * @brief Pushes the given element value to the end of the Queue.
         * @param element The value of the element to push.
         * @return true If the push have
         * @details
         * This function append a new element to the last position of Queue
         * buffer. It increments the Queues head index (new element at the
         * next Queue back position) by using module to keep the value inside
         * the range 0 to QUEUE_SIZE-1) and add a copy of the provided element
         * to this new head position of the queue buffer. In case of Queue is
         * full, the buffer overflow flag attribute is set and the tail index
         * is increased (set the oldest front element to the next one). At the
         * end, the function return if an overflow of the buffer has occurred
         * (notifying that the oldest element has been overwritten).
         */
        t_squeue_rc push(const T_QUEUE_ELEMENTS& element)
        {
            // Handle if Queue is full or not
            if ( full() )
            {
                // Set overflow flag and remove oldest Queue element
                queue_overflow = true;
                queue_tail = (queue_tail + 1U) % QUEUE_SIZE;
            }
            else
            {
                queue_overflow = false;
                num_elements_stored = num_elements_stored + 1U;
            }

            // Add the new element and iIncrease Queue back element position
            buffer[queue_head] = element;
            queue_head = (queue_head + 1U) % QUEUE_SIZE;

            // Return push result on buffer overflow
            if ( queue_overflow )
            {   return t_squeue_rc::OVERFLOW;   }
            else
            {   return t_squeue_rc::OK;   }
        }

        /**
         * @brief Removes an element from the front of the Queue.
         * @details
         * This function remove the first element of the Queue buffer. It just
         * increase the queue tail index (using module to keep the value
         * inside the range 0 to QUEUE_SIZE-1). If the Queue is empty, do
         * nothing.
         * Note: Any element pop will clear the queue overflow flag.
         */
        void pop()
        {
            if ( empty() )
            {   return;   }

            queue_tail = (queue_tail + 1U) % QUEUE_SIZE;
            num_elements_stored = num_elements_stored - 1U;
            queue_overflow = false;
        }

#if defined(SQUEUE_ENABLE_CONTAINS)
        /**
         * @brief Checks if a given element is already in the queue.
         * @param element Element to check if is in the queue.
         * @return true Element is already in the queue.
         * @return false Element is not in the queue.
         * @warning Take care that T_QUEUE_ELEMENTS data type must be
         * comparable (check for padding, complex data types with
         * pointers, functions, etc).
         */
        bool contains(T_QUEUE_ELEMENTS& element) const
        {
            for (size_t i = 0U; i < num_elements_stored; i++)
            {
                size_t index = (queue_tail + i) % QUEUE_SIZE;
                if (element == buffer[index])
                {   return true;   }
            }
            return false;
        }
#endif // SQUEUE_ENABLE_CONTAINS

#if 0 /* The next methods are not currently supported */
        /**
         * @brief Pushes a new element to the end of the Queue. The element is
         * constructed in-place, i.e. no copy or move operations are
         * performed. The constructor of the element is called with exactly
         * the same arguments as supplied to the function.
         * @param args Arguments to forward to the constructor of the element.
         * @details
         * This function functionality is incompatible with just static memory
         * allocation usage implementation.
         */
        void emplace(Args&&... args);

        /**
         * @brief Exchanges the contents of the Queue with another Queue.
         * @param other Queue to exchange the contents with.
         * @details
         * This function should check if the Queues are of the same elements
         * type and has the same size before any swap operation is done.
         */
        void swap(squeue& other);
#endif

    /*********************************/

    private:

        /* Private Attributes */

        /**
         * @brief Internal buffer to store Queue elements.
         */
        T_QUEUE_ELEMENTS buffer[QUEUE_SIZE];

        /**
         * @brief Queue circular buffer head location index.
         */
        uint32_t queue_head;

        /**
         * @brief Queue circular buffer tail location index.
         */
        uint32_t queue_tail;

        /**
         * @brief Current number of elements stored in the buffer.
         */
        uint32_t num_elements_stored;

        /**
         * @brief Queue is full and has been overflowed.
         */
        bool queue_overflow;

};

/*****************************************************************************/

#endif /* STATIC_QUEUE_H_ */
