#include "smm.h"

static int is_smm_mem_info_valid(struct smm_mem_info *mem_info)
{
    SMM_ASSERT_PTR_VALID(mem_info, SMM_PTR_ERR);
    SMM_ASSERT_PTR_VALID(mem_info->base_addr, SMM_PTR_ERR);
    SMM_ASSERT_PTR_VALID(mem_info->read, SMM_PTR_ERR);
    SMM_ASSERT_PTR_VALID(mem_info->write, SMM_PTR_ERR);

    return SMM_OK;
}

static int get_suitable_min_free_space(struct smm_mem_info *mem_info, u32 alloc_size, u32 *suitable_pre_off)
{
    struct smm_head cur, next;
    u32 min_free_space_size = 0;
    u32 element_free_interval;
    u32 found = SMM_FALSE;

    mem_info->read((u8 *)&cur, mem_info->base_addr, SMM_ELEMENT_HEAD_SIZE);

    while (cur.next_off != SMM_INVALID_OFFSET) {
        mem_info->read((u8 *)&next, mem_info->base_addr + cur.next_off, SMM_ELEMENT_HEAD_SIZE);
        element_free_interval = cur.next_off - next.pre_off - cur.length;

        if (alloc_size <= element_free_interval && (!found || min_free_space_size > element_free_interval)) {
            min_free_space_size = element_free_interval;
            *suitable_pre_off = next.pre_off;
            found = SMM_TRUE;
        }

        cur = next;
    }

    return found ? SMM_OK : SMM_NO_FREE_SPACE;
}

u8 *smm_malloc(struct smm_mem_info *mem_info, u32 size)
{
    int ret;
    struct smm_head pre = {0}, next = {0}, cur = {0};
    u32 suitable_pre_off = 0;

    ret = is_smm_mem_info_valid(mem_info);
    SMM_ASSERT_RET_VALID(ret, NULL);

    if (size & (SMM_BLOCK_ALIGN_BYTES - 1)) {
        return NULL;
    }

    ret = get_suitable_min_free_space(mem_info, size, &suitable_pre_off);
    SMM_ASSERT_RET_VALID(ret, NULL);

    u8 *pre_addr = mem_info->base_addr + suitable_pre_off;
    mem_info->read((u8 *)&pre, pre_addr, SMM_ELEMENT_HEAD_SIZE);
    u8 *next_addr = mem_info->base_addr + pre.next_off;
    mem_info->read((u8 *)&next, next_addr, SMM_ELEMENT_HEAD_SIZE);

    cur.pre_off = suitable_pre_off;
    cur.next_off = pre.next_off;
    cur.length = size + SMM_ELEMENT_HEAD_SIZE;
    pre.next_off = suitable_pre_off + pre.length;
    next.pre_off = pre.next_off;

    mem_info->write(pre_addr, (u8 *)&pre, SMM_ELEMENT_HEAD_SIZE);
    mem_info->write(next_addr, (u8 *)&next, SMM_ELEMENT_HEAD_SIZE);
    mem_info->write(mem_info->base_addr + pre.next_off, (u8 *)&cur, SMM_ELEMENT_HEAD_SIZE);

    return mem_info->base_addr + suitable_pre_off + pre.length + SMM_ELEMENT_HEAD_SIZE;
}

static int is_addr_valid(struct smm_mem_info *mem_info, u32 off)
{
    u8 *cur_addr = mem_info->base_addr;
    struct smm_head cur;

    while (1) {
        mem_info->read((u8 *)&cur, cur_addr, SMM_ELEMENT_HEAD_SIZE);

        if (cur.next_off == off) {
            return SMM_VALID;
        }

        if (cur.next_off == SMM_INVALID_OFFSET) {
            break;
        }

        cur_addr = mem_info->base_addr + cur.next_off;
    }

    return SMM_INVALID;
}

int smm_free(struct smm_mem_info *mem_info, u8 *addr)
{
    int ret = SMM_OK;
    u32 actuall_off = 0;
    struct smm_head pre = {0};
    struct smm_head next = {0};
    struct smm_head cur = {0};
    u8 *cur_addr = NULL, *pre_addr = NULL, *next_addr = NULL;

    ret = is_smm_mem_info_valid(mem_info);
    SMM_ASSERT_RET_VALID(ret, ret);
    SMM_ASSERT_PTR_VALID(addr, SMM_PARAMETER_INVALID);

    if ((addr < mem_info->base_addr + SMM_ELEMENT_HEAD_SIZE) ||
        (addr > (mem_info->base_addr + mem_info->size - SMM_ELEMENT_HEAD_SIZE))) {
        return SMM_FREE_FAILED;
    }

    actuall_off = addr - mem_info->base_addr - SMM_ELEMENT_HEAD_SIZE;
    if (is_addr_valid(mem_info, actuall_off) == SMM_INVALID) {
        return SMM_FREE_FAILED;
    }

    cur_addr = mem_info->base_addr + actuall_off;
    mem_info->read((u8 *)&cur, cur_addr, SMM_ELEMENT_HEAD_SIZE);

    pre_addr = mem_info->base_addr + cur.pre_off;
    next_addr = mem_info->base_addr + cur.next_off;
    mem_info->read((u8 *)&pre, pre_addr, SMM_ELEMENT_HEAD_SIZE);
    mem_info->read((u8 *)&next, next_addr, SMM_ELEMENT_HEAD_SIZE);

    pre.next_off = cur.next_off;
    next.pre_off = cur.pre_off;
    mem_info->write(pre_addr, (u8 *)&pre, SMM_ELEMENT_HEAD_SIZE);
    mem_info->write(next_addr, (u8 *)&next, SMM_ELEMENT_HEAD_SIZE);

    return SMM_OK;
}

int smm_memory_create(struct smm_mem_info *mem_info)
{
    int ret = SMM_OK;
    struct smm_head space_head = {0};

    ret = is_smm_mem_info_valid(mem_info);
    SMM_ASSERT_RET_VALID(ret, ret);

    if ((mem_info->size % SMM_BLOCK_ALIGN_BYTES) ||
        (mem_info->size < SMM_ELEMENT_HEAD_SIZE * 2) ||
        mem_info->size > SMM_MAX_MANAGER_MEM_SIZE) {
        return SMM_PARAMETER_INVALID;
    }

    space_head.pre_off = SMM_INVALID_OFFSET;
    space_head.next_off = mem_info->size - SMM_ELEMENT_HEAD_SIZE;
    space_head.length = SMM_ELEMENT_HEAD_SIZE;
    mem_info->write(mem_info->base_addr, (u8 *)&space_head, SMM_ELEMENT_HEAD_SIZE);

    space_head.pre_off = 0;
    space_head.next_off = SMM_INVALID_OFFSET;
    space_head.length = SMM_ELEMENT_HEAD_SIZE;
    mem_info->write(mem_info->base_addr + mem_info->size - SMM_ELEMENT_HEAD_SIZE, (u8 *)&space_head, SMM_ELEMENT_HEAD_SIZE);

    return SMM_OK;
}

