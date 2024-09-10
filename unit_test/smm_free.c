#include <gtest/gtest.h>
#include <cstdint>
#include <stdint.h>
#include "../src/smm.h"
#include "smm_test_class.h"

TEST_F(SMMTest, TestSMMFreeInvalidSMMMem)
{
    u32 test = 0;
    EXPECT_EQ(SMM_OK, smm_memory_create(&mem_info));

    EXPECT_EQ(SMM_PTR_ERR, smm_free(NULL, (u8 *)&test));
}

TEST_F(SMMTest, TestSMMFreeInvalidAddr)
{
    EXPECT_EQ(SMM_OK, smm_memory_create(&mem_info));

    EXPECT_EQ(SMM_PARAMETER_INVALID, smm_free(&mem_info, NULL));
}

TEST_F(SMMTest, TestSMMFreeNormal)
{
    EXPECT_EQ(SMM_OK, smm_memory_create(&mem_info));

    smm_malloc(&mem_info, 0x8);
    smm_malloc(&mem_info, 0x20);
    smm_malloc(&mem_info, 0x30);
    smm_malloc(&mem_info, 0x100);

    EXPECT_EQ(SMM_OK, smm_free(&mem_info, mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*2));
    EXPECT_EQ(SMM_OK, smm_free(&mem_info, mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*3 + 0x8));
    EXPECT_EQ(SMM_OK, smm_free(&mem_info, mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*4 + 0x8 + 0x20));
    EXPECT_EQ(SMM_OK, smm_free(&mem_info, mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*5 + 0x8 + 0x20 + 0x30));
}

TEST_F(SMMTest, TestSMMFreeNonExitAddr)
{
    EXPECT_EQ(SMM_OK, smm_memory_create(&mem_info));

    smm_malloc(&mem_info, 0x8);
    smm_malloc(&mem_info, 0x20);
    smm_malloc(&mem_info, 0x30);
    smm_malloc(&mem_info, 0x100);

    EXPECT_EQ(SMM_FREE_FAILED, smm_free(&mem_info, mem_info.base_addr + SMM_ELEMENT_HEAD_SIZE*80));
}

