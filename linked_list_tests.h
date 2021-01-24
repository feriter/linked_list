#include "linked_list.h"
#include "gtest/gtest.h"

namespace googleTests {
    class linked_list_tests : public ::testing::Test {
    protected:
        linked_list<double>* list;
        void SetUp() override {
            list = new linked_list<double>();
            fill_list(*list);
        }
        void TearDown() override {
            delete list;
        }
    public:
        static void fill_list(linked_list<double> & list) {
            list.push_back(1.1);
            list.push_back(2.2);
            list.push_back(3.3);
        }
    };

    TEST_F(linked_list_tests, empty) {
        EXPECT_EQ(false, list->empty());
        list->clear();
        EXPECT_EQ(true, list->empty());
    }
    TEST_F(linked_list_tests, clear_front_back) {
        list->clear();
        EXPECT_THROW(list->front(), linked_list_exception);
        EXPECT_THROW(list->back(), linked_list_exception);
    }
    TEST_F(linked_list_tests, remove) {
        EXPECT_EQ(list->remove(1.1), 1);
        list->push_back(2.2);
        EXPECT_EQ(list->remove(2.2), 2);
    }
    TEST_F(linked_list_tests, size) {
        EXPECT_EQ(list->size(), 3);
        list->pop_front();
        EXPECT_EQ(list->size(), 2);
        list->clear();
        EXPECT_EQ(list->size(), 0);
    }
    TEST_F(linked_list_tests, eqoperator) {
        auto list2 = *list;
        EXPECT_EQ(true, operator==(*list, list2));
        list->push_front(0.0);
        list2.push_front(0.0);
        EXPECT_EQ(true, operator==(*list, list2));
        list2.pop_front();
        EXPECT_EQ(false, operator==(*list, list2));
    }
}
