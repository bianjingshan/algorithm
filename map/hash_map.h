#ifndef HASH_MAP_H_
#define HASH_MAP_H_

#include <stdint.h>

typedef struct {
    uint8_t valid;
    uint8_t reserved[7];
    uint64_t key;
    uint64_t value;
} HashMapItem_t;

class HashMap {
 public:
    HashMap();
    ~HashMap();
    void init(uint32_t max_item_num);
    bool set(uint64_t key, uint64_t value);
    bool get(uint64_t key, uint64_t *value);

 private:
    uint32_t max_item_num_;
    uint32_t remainder_value_;
    HashMapItem_t *items_ = nullptr;
};

#endif  // HASH_MAP_H_


