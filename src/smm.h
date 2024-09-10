#ifndef __SMM_H
#define __SMM_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define SMM_BLOCK_ALIGN_BYTES    4
#define SMM_ELEMENT_HEAD_SIZE    (sizeof(struct smm_head))
#define SMM_NEXT_NODE_OFF_BITS   21
#define SMM_PRE_NODE_OFF_BITS    (SMM_NEXT_NODE_OFF_BITS)
#define SMM_NODE_LENGTH_OFF_BITS (64 - (SMM_NEXT_NODE_OFF_BITS) - (SMM_PRE_NODE_OFF_BITS))
#define SMM_INVALID_OFFSET       ((1 << SMM_NEXT_NODE_OFF_BITS) - 1)
#define SMM_VALID                1
#define SMM_INVALID              0
#define SMM_TRUE                 1
#define SMM_FALSE                0

#ifndef NULL
#define NULL (0)
#endif

typedef int (*smm_mem_opt)(u8 *dst_addr, u8 *src_addr, u32 size);

typedef enum {
    SMM_OK = 0,
    SMM_PARAMETER_INVALID,
    SMM_PTR_ERR,
    SMM_NO_FREE_SPACE,
    SMM_FREE_FAILED,
} SMM_RET_VAL;

#define SMM_ASSERT_PTR_VALID(ptr, ret_val) \
    do {                                   \
        if (!ptr)                          \
            return ret_val;                \
    } while (0);

#define SMM_ASSERT_RET_VALID(ret, ret_val) \
    do {                                   \
        if (ret)                           \
            return ret_val;                \
    } while (0);

struct smm_mem_info {
    u8 *base_addr;
    u32 size;
    smm_mem_opt read;
    smm_mem_opt write;
};

struct smm_head {
    u64 next_off : SMM_NEXT_NODE_OFF_BITS;
    u64 pre_off : SMM_PRE_NODE_OFF_BITS;
    u64 length : SMM_NODE_LENGTH_OFF_BITS;
};

/**
 * @brief smm alloc memory
 *
 * @param mem_info:         memory manager info
 * @param size:             size of memory alloc
 * @return u8*              success: addr of smm alloc addr, failed: NULL
 */
u8 *smm_malloc(struct smm_mem_info *mem_info, u32 size);

/**
 * @brief free alloced memory
 *
 * @param mem_info:         memory manager info
 * @param addr:             addr of alloced addr
 * @return int              failed: non-zero, success: 0
 */
int smm_free(struct smm_mem_info *mem_info, u8 *addr);

/**
 * @brief init a smm memory
 *
 * @param mem_info:         memory manager info
 * @return int              failed: non-zero, success: 0
 */
int smm_memory_create(struct smm_mem_info *mem_info);

#endif

