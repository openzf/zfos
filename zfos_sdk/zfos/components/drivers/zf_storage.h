#ifndef __ZF_STORE_H__
#define __ZF_STORE_H__

struct zf_storage_info_t
{
    unsigned long long block_num;
    uint16_t block_size;
    uint8_t card_type;
    uint32_t size;
};

struct zf_storage_t
{
    struct zf_storage_info_t info;
    int (*init)(void);
    int (*get_info)(struct zf_storage_info_t *info);
    int (*get_status)(void);
    int (*erase_all)(void);
    int (*erase_range)(uint32_t addr, uint32_t size);
    int (*read)(uint32_t addr, uint8_t *buff, uint32_t size);
    int (*write)(uint32_t addr, uint8_t *buff, uint32_t size);
};

#endif
