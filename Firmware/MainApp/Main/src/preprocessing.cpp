// #include "preprocessing.h"

// void *operator new(std::size_t size)
// {
//     void *ptr = pvPortMalloc(size);
//     if (!ptr)
//         throw std::bad_alloc();
//     return ptr;
// }

// void operator delete(void *ptr) noexcept
// {
//     vPortFree(ptr);
// }

// void *operator new[](std::size_t size)
// {
//     void *ptr = pvPortMalloc(size);
//     if (!ptr)
//         throw std::bad_alloc();
//     return ptr;
// }

// void operator delete[](void *ptr) noexcept
// {
//     vPortFree(ptr);
// }
