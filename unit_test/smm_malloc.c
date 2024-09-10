#include <gtest/gtest.h>
#include <cstdint>
#include <stdint.h>
#include "../src/smm.h"
#include "smm_test_class.h"

TEST_F(SMMTest, TestSMMMallocInvalidParameter)
{
    void *ptr = nullptr;
    EXPECT_EQ(SMM_OK, smm_memory_create(&mem_info));

    EXPECT_EQ(nullptr, smm_malloc(NULL, 0x8));
}

TEST_F(SMMTest, TestSMMMallocInvalidSize)
{
    void *ptr = nullptr;
    EXPECT_EQ(SMM_OK, smm_memory_create(&mem_info));

    EXPECT_EQ(nullptr, smm_malloc(&mem_info, 0x7));
}

TEST_F(SMMTest, TestSMMMallocSpaceFull)
{
    void *ptr = nullptr;
    EXPECT_EQ(SMM_OK, smm_memory_create(&mem_info));

    EXPECT_EQ(nullptr, smm_malloc(&mem_info, 0x1000));
}

TEST_F(SMMTest, TestSMMMallocNormal)
{
    void *ptr = nullptr;
    EXPECT_EQ(SMM_OK, smm_memory_create(&mem_info));

    EXPECT_EQ(mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*2, smm_malloc(&mem_info, 0x8));
    EXPECT_EQ(mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*3 + 0x8, smm_malloc(&mem_info, 0x20));
    EXPECT_EQ(mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*4 + 0x8 + 0x20, smm_malloc(&mem_info, 0x30));
    EXPECT_EQ(mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*5 + 0x8 + 0x20 + 0x30, smm_malloc(&mem_info, 0x100));
    EXPECT_EQ(nullptr, smm_malloc(&mem_info, 0xF00));
}

