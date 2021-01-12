//Vector v/s deque
/* 
    1) operations: vector provided insertion & deletion at end & middle.
        push_back, pop_back, insert
        deque provided insertion & deleteion from both end & front and middle
        push_back, pop_back, push_front, pop_front, insert
    2) arch:
            > vector stores element in contiguous memory
            >deque contains a list of memory blocks which stores element contiguously and each of the memory chunks is linked with both its prev & next memory block node

    3) op perf: 
        > vector provides better performance for push & pop operations at the end, due to its arch difference, than deque
        > In deque : insertion at front is fast as compared to vector because it doesn’t need the elements to shift by 1 in current memory block to make space at front. It just checks if any space is left at left of first element in current memory block, if not then allocates a new memory block.

    4) iteration invalidation doesnt happen in  deque, when elements are inserted in the end or front because deque doesnt have to shift elements from one to another memory in case current allocated memory is not sufficient to store newly added element.
        in vector, iteration invalidation may happen whenever element is inserted in end or middle.
    5) in both deque & vector, iteration invalidation happpens when element is inserted/ deleted in middle
    6) both vector & deque supports random access using [] & at() but performance of random access in vector is better than deque 


when to choose deque ?
    when insertion & deleteion is required from both front & end
    eg. implementing a queue
when to choose vector?
    when insertion & deletion is required from mostly end
    eg : implementing a stack

 */