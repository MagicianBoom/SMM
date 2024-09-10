#include <gtest/gtest.h>
#include <cstdint>
#include <stdint.h>
#include "../src/smm.h"
#include "smm_test_class.h"

extern int smm_mem_read(u8 *dst_addr, u8 *src_addr, u32 size);
extern int smm_mem_write(u8 *dst_addr, u8 *src_addr, u32 size);

TEST_F(SMMTest, TestSMMMemoryCreateInvalidMeminfo)
{
    EXPECT_EQ(SMM_PTR_ERR, smm_memory_create(NULL));
}

TEST_F(SMMTest, TestSMMMemoryCreateInvalidBaseaddr)
{
    struct smm_mem_info mem_info;

    mem_info.base_addr = NULL;
    mem_info.size = 0x1000;
    mem_info.read = smm_mem_read;
    mem_info.write = smm_mem_write;
    EXPECT_EQ(SMM_PTR_ERR, smm_memory_create(&mem_info));
}

TEST_F(SMMTest, TestSMMMemoryCreateSmallSize)
{
    struct smm_mem_info mem_info;

    mem_info.base_addr = (u8 *)malloc(0x1000);
    mem_info.size = 4;
    mem_info.read = smm_mem_read;
    mem_info.write = smm_mem_write;
    EXPECT_EQ(SMM_PARAMETER_INVALID, smm_memory_create(&mem_info));
}

TEST_F(SMMTest, TestSMMMemoryCreateInvalidSize)
{
    struct smm_mem_info mem_info;

    mem_info.base_addr = (u8 *)malloc(0x1000);
    mem_info.size = 0x1000 - 1;
    mem_info.read = smm_mem_read;
    mem_info.write = smm_mem_write;
    EXPECT_EQ(SMM_PARAMETER_INVALID, smm_memory_create(&mem_info));
}

TEST_F(SMMTest, TestSMMMemoryCreateInvalidRead)
{
    struct smm_mem_info mem_info;

    mem_info.base_addr = (u8 *)malloc(0x1000);
    mem_info.size = 0x1000;
    mem_info.read = NULL;
    mem_info.write = smm_mem_write;
    EXPECT_EQ(SMM_PTR_ERR, smm_memory_create(&mem_info));
}

TEST_F(SMMTest, TestSMMMemoryCreateInvalidWrite)
{
    struct smm_mem_info mem_info;

    mem_info.base_addr = (u8 *)malloc(0x1000);
    mem_info.size = 0x1000;
    mem_info.read = smm_mem_read;
    mem_info.write = NULL;
    EXPECT_EQ(SMM_PTR_ERR, smm_memory_create(&mem_info));
}

