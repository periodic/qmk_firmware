#include <string.h>

#ifdef SPLIT_USER_BUFFER_ENABLE

// TODO: Get a better generic byte type.
uint8_t user_buffer[SPLIT_USER_BUFFER_SIZE];

void* get_user_buffer(void) {
    return &user_buffer;
}

void set_user_buffer(const void* new_buffer) {
    memcpy(&user_buffer, new_buffer, SPLIT_USER_BUFFER_SIZE);
}

#endif
