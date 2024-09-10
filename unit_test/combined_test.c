#include <gtest/gtest.h>
#include <cstdint>
#include <stdint.h>
#include "../src/smm.h"
#include "smm_test_class.h"

TEST_F(SMMTest, TestSMMCombined)
{
    u8 *ptr1 = NULL;
    u8 *ptr2 = NULL;
    EXPECT_EQ(SMM_OK, smm_memory_create(&mem_info));

    ptr1 = smm_malloc(&mem_info, 0x8);
    EXPECT_EQ(mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*2, ptr1);
    EXPECT_EQ(SMM_OK, smm_free(&mem_info, ptr1));

    ptr1 = smm_malloc(&mem_info, 0x20);
    EXPECT_EQ(mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*2, ptr1);
    EXPECT_EQ(SMM_OK, smm_free(&mem_info, ptr1));

    ptr1 = smm_malloc(&mem_info, 0x30);
    EXPECT_EQ(mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*2, ptr1);

    ptr2 = smm_malloc(&mem_info, 0x100);
    EXPECT_EQ(mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*3 + 0x30, ptr2);

    EXPECT_EQ(SMM_OK, smm_free(&mem_info, ptr1));
    ptr1 = smm_malloc(&mem_info, 0x10);
    EXPECT_EQ(mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*2, ptr1);

    EXPECT_EQ(SMM_FREE_FAILED, smm_free(&mem_info, ptr1+0x10));
    EXPECT_EQ(SMM_OK, smm_free(&mem_info, ptr2));
    EXPECT_EQ(SMM_OK, smm_free(&mem_info, ptr1));

    ptr1 = smm_malloc(&mem_info, 0x1000);
    EXPECT_EQ(nullptr, ptr1);
}

