#ifndef __SMM_TEST_CLASS_H
#define __SMM_TEST_CLASS_H

#include "../src/smm.h"

class SMMTest : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    struct smm_mem_info mem_info;
};

#endif

