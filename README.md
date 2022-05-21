# SQueue (Static Queue)

A C++ static memory allocated Queue component to store any kind of data type elements without using any kind of dynamic memory allocation.

This function could be used to replace C++ STL queue component that uses dinamyc memory.

The implementation of this Queue component is based on the use of a circular buffer with two incremental indexes (head and tail) that returns to zero when overflows on the queue maximum size. Appending a new element increments the head index while removing an element increments the tail index. The number of elements that are currently stored in the queue is given by the difference between head and tail indexes. When the Queue is full, new elements will overwrite older elements.
