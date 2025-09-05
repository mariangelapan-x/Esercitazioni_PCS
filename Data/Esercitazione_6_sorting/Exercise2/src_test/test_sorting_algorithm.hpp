#pragma once

#include <iostream>
#include <vector>

#include <gtest/gtest.h>
#include "SortingAlgorithm.hpp"

namespace SortLibrary {
     
TEST(TestSorting, TestBubbleSort)
{
    std::vector<int> v = {100 ,5, 25, 9, 18 ,4, 3, 1, 99, 7, 6 ,9 ,28, 15, 3};
    BubbleSort<int>(v);
    std::vector<int> sortedV = {1, 3, 3, 4, 5, 6, 7, 9, 9, 15, 18, 25, 28, 99, 100};
    EXPECT_EQ(v, sortedV);
}

TEST(TestSorting, TestHeapSort)
{
    std::vector<int> v = {100 ,5, 25, 9, 18 ,4, 3, 1, 99, 7, 6 ,9 ,28, 15, 3};
    HeapSort<int>(v);
    std::vector<int> sortedV = {1, 3, 3, 4, 5, 6, 7, 9, 9, 15, 18, 25, 28, 99, 100};
    EXPECT_EQ(v, sortedV);
}

TEST(TestSorting, TestHeapSortCustom)
{
    std::vector<int> v = {100 ,5, 25, 9, 18 ,4, 3, 1, 99, 7, 6 ,9 ,28, 15, 3};
    HeapSortCustom<int>(v);
    std::vector<int> sortedV = {1, 3, 3, 4, 5, 6, 7, 9, 9, 15, 18, 25, 28, 99, 100};
    EXPECT_EQ(v, sortedV);
}

}