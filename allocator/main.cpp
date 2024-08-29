#include <iostream>
#include <list>
#include <vector>

#include "allocator.h"
#include "myclass.h"
#include "poolalloc.h"
#include "linearallocator.h"
#include "forwardlist.h"

int main()
{
    // std::list<int, PoolAllocator<int, 7>> mylist;
    // mylist.emplace_back(52);
    // mylist.emplace_back(25);
    // mylist.emplace_back(25);
    // std::list<int, PoolAllocator<int, 7>> mylist1{mylist};
    // mylist1.emplace_back(255);
    // std::cout << mylist1.back() << ", at: " << &mylist1.back() << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // std::vector<int, PoolAllocator<int>> myvector;
    // myvector.push_back(52);
    // myvector.push_back(52);

    //////////////////////////////////////////////////////////////////////
    ForwardList<int, PoolAllocator<int, 30>> customList;
    customList.pushBack(25);
    customList.pushBack(52);
    customList.pushBack(522);
    auto it = customList.find(52);
    customList.insertAfter(it, 53);
    for(int elem : customList)
    {
        std::cout << elem << std::endl;
    }

    /////////////////////////////////////////////////////////////////
    // std::vector<int, PoolAllocator<int, 100>> vec;
    // vec.push_back(52);
    // vec.push_back(52);
    // vec.push_back(52);
    return 0;
}