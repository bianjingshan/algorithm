#include "hash_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t ALIGN_UP(uint32_t x)
{
    uint32_t y = x - 1;
    y |= y >> 1;
    y |= y >> 2;
    y |= y >> 4;
    y |= y >> 8;
    y |= y >> 16;
    return (y < 0) ? 1 : (y + 1);
}

HashMap::HashMap()
{

}

HashMap::~HashMap()
{

}

void HashMap::init(uint32_t max_item_num)
{
    max_item_num_ = ALIGN_UP(max_item_num * 2);
    remainder_value_ = max_item_num_ - 1;
    printf("HashMap::init(%u):%u\n", max_item_num, max_item_num_);
    uint32_t mem_size = sizeof(HashMapItem_t) * max_item_num_;
    items_ = reinterpret_cast<HashMapItem_t *>(malloc(mem_size));
    if (nullptr == items_)
    {
        printf("HashMap::init error\n");
    }
    memset(items_, 0, mem_size);
}

bool HashMap::get(uint64_t key, uint64_t *value)
{
    uint32_t temp_index = key % remainder_value_;
    HashMapItem_t *p_item = &items_[temp_index];
    if (p_item->valid && (p_item->key == key))
    {
        *value = p_item->value;
        return true;
    }
    else
    {
        for (uint32_t i = 0; i < remainder_value_; i++)
        {
            temp_index++;
            if (temp_index >= max_item_num_)
            {
                temp_index = 0;
            }
            p_item = &items_[temp_index];
            if (p_item->valid && (p_item->key == key))
            {
                *value = p_item->value;
                return true;
            }
        }
    }
    return false;
}

bool HashMap::set(uint64_t key, uint64_t value)
{
    uint32_t temp_index = key % remainder_value_;
    HashMapItem_t *p_item = &items_[temp_index];
    if (0 == p_item->valid)
    {
        p_item->valid = 1;
        p_item->key = key;
        p_item->value = value;
        return true;
    }
    else if (p_item->key == key)
    {
        p_item->value = value;
        return true;
    }
    else
    {
        for (uint32_t i = 0; i < remainder_value_; i++)
        {
            temp_index++;
            if (temp_index >= max_item_num_)
            {
                temp_index = 0;
            }
            p_item = &items_[temp_index];
            if (0 == p_item->valid)
            {
                p_item->valid = 1;
                p_item->key = key;
                p_item->value = value;
                return true;
            }
            else if (p_item->key == key)
            {
                p_item->value = value;
                return true;
            }
        }
    }
    return false;
}

