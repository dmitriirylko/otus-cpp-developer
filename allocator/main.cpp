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
    std::list<int, PoolAllocator<int, 7>> mylist;
    mylist.emplace_back(52);
    // mylist.pop_back();
    mylist.emplace_back(25);
    mylist.emplace_back(25);
    std::list<int, PoolAllocator<int, 7>> mylist1{mylist};
    // mylist1 = mylist;
    mylist1.emplace_back(255);
    // mylist1.emplace_back(44);
    // mylist1.emplace_back(43);
    std::cout << mylist1.back() << ", at: " << &mylist1.back() << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    // std::vector<int, PoolAllocator<int>> myvector;
    // myvector.push_back(52);
    // myvector.push_back(52);
    //////////////////////////////////////////////////////////////////////
    // ForwardList<int, PoolAllocator<int, 3>> customList;
    // customList.push_back(25);
    // customList.push_back(52);
    // customList.push_back(522);
    // auto it = customList.find(52);
    // customList.insert_after(it, 53);
    // for(int elem : customList)
    // {
    //     std::cout << elem << std::endl;
    // }
    return 0;
}