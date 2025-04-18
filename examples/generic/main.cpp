/**
 * @file main.cpp
 * @brief Squeue library basic usage example.
 * @author Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date 14-04-2025
 * @version 1.0.0
 */

/*****************************************************************************/

/* Libraries */

// Standard C++ Libraries
#include <cmath>
#include <cstdint>
#include <cstdio>

// SQueue Library
#include "squeue.hpp"

/*****************************************************************************/

/* Constants */

static constexpr uint32_t QUEUE_SIZE = 5U;

/*****************************************************************************/

/* Data Types */

/**
 * @brief Example data structure.
 */
struct s_data
{
    uint16_t id;
    float value;
    bool completed;

// If we enable Squeue library contains() method, it requires that the
// elements that are going to be stored has a == operator implemented
#if defined(SQUEUE_ENABLE_CONTAINS)
    bool operator==(const s_data& data) const
    {
        if (data.id != id)
        {   return false;   }
        if (std::fabs(data.value - value) >= 0.00001F)
        {   return false;   }
        if (data.completed != completed)
        {   return false;   }
        return true;
    }
#endif // SQUEUE_ENABLE_CONTAINS
};

/*****************************************************************************/

/* Function Prototypes */

/**
 * @brief Show Queue Content through STDOUT stream.
 * @param queue Queue to show it content.
 */
void show_queue_content(SQueue<s_data, QUEUE_SIZE>& queue);

/*****************************************************************************/

/* Main Function */

/**
 * @brief Main Function.
 * @return int Program process exit result success/fail (0/1).
 */
int main()
{
    using std::printf;
    printf("\nExample start\n\n");

    // Create the Queue
    SQueue<s_data, QUEUE_SIZE> queue_main;

    // Check Queue is empty
    if (queue_main.empty())
    {   printf("At the beginning, the queue is empty\n");   }
    printf("\n");

    // Add an element to the queue
    printf("Adding one element to the queue...\n");
    s_data first_data = { 0U, 0.0F, false };
    if (queue_main.push(first_data) == t_squeue_rc::OVERFLOW)
    {   printf("It's always good to check if the queue overflows\n");   }
    printf("Element 0 added to the queue\n");
    printf("\n");

    // Check Queue is not empty
    if (queue_main.empty() == false)
    {   printf("Of course, now the queue is not empty\n");   }
    printf("\n");

    // Add 4 elements to the queue
    printf("Adding 4 elements to the queue...\n");
    static constexpr uint16_t NUM_ELEMENTS_TO_ADD = 4U;
    for (uint16_t i = 1U; i < NUM_ELEMENTS_TO_ADD+1U; i++)
    {
        s_data data = { i, static_cast<float>(i), true };
        if (queue_main.push(data) == t_squeue_rc::OVERFLOW)
        {   printf("Queue overflows\n");   }
        printf("Element %d added to the Queue\n", static_cast<int>(i));
    }
    printf("\n");

    // Show Queue Content
    show_queue_content(queue_main);
    printf("\n");

    // Check Queue overflow when it is full
    printf("Checking adding another element makes the queue to overflow...\n");
    s_data data = { 5U, 5.0F, true };
    if (queue_main.push(data) == t_squeue_rc::OVERFLOW)
    {
        printf("Queue push overflows (expected, due it is full)\n");
        printf("  Element 5 added to the Queue, element 0 must be lost\n");
    }
    printf("\n");

    // Show Queue Content
    show_queue_content(queue_main);
    printf("\n");

    // Check if element is already in the Queue
    #if defined(SQUEUE_ENABLE_CONTAINS)
        printf("Checking if the queue contains the element 5...\n");
        if (queue_main.contains(data))
        {   printf("Element 5 is in in the queue\n");   }
        else
        {   printf("Element 5 is not in the queue\n");   }
        printf("\n");
    #endif // SQUEUE_ENABLE_CONTAINS

    printf("Example end\n\n");
    return 0;
}

/*****************************************************************************/

/* Auxiliary Functions */

/**
 * @details
 * Due Queue doesn't offers a way to get or iterate through it elements,
 * a full queue copy is done to get each of it elements to show them.
 * This function is just useful for this example were the queue is
 * small, it is not correct to copy the full queue in other scenarios.
 * This function does nothing if the Queue is empty.
 */
void show_queue_content(SQueue<s_data, QUEUE_SIZE>& queue)
{
    using std::printf;

    if (queue.empty())
    {   return;   }

    int i = 0;
    SQueue<s_data, QUEUE_SIZE> queue_copy = queue;
    printf("Queue Content:\n");
    while (queue_copy.empty() == false)
    {
        s_data* element = queue_copy.front();
        if (element == nullptr)
        {
            printf("No more elements in the queue\n");
            break;
        }
        queue_copy.pop();

        const char* str_completed = element->completed ? "true" : "false";
        printf("  Element %d: { %d, %f, %s}\n",
            i, static_cast<int>(element->id),
            static_cast<double>(element->value), str_completed);
        i = i + 1;
    }
}

/*****************************************************************************/
