#include <gtest/gtest.h>
#include <cstdint>
#include <stdint.h>
#include "../src/smm.h"
#include "smm_test_class.h"

int smm_mem_read(u8 *dst_addr, u8 *src_addr, u32 size)
{
    memcpy(dst_addr, src_addr, size);
    return 0;
}

int smm_mem_write(u8 *dst_addr, u8 *src_addr, u32 size)
{
    memcpy(dst_addr, src_addr, size);
    return 0;
}

void SMMTest::SetUp(void)
{
    SMMTest::mem_info.base_addr = (u8 *)malloc(0x1000);
    SMMTest::mem_info.size = 0x1000;
    SMMTest::mem_info.read = smm_mem_read;
    SMMTest::mem_info.write = smm_mem_write;
}

void SMMTest::TearDown(void)
{
    free(SMMTest::mem_info.base_addr);
}

int main(int argc, char **argv)
{
    printf("LY_DEBUG\n");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

