#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <assert.h>


void serialize_data(ser_buff_t *b, const char *data, int nbytes){
    
// Check if buffer is valid
    if (b == NULL) assert(0);

    ser_buff_t *buff = (ser_buff_t *)(b);
    int Remaining_size = buff->size - buff->next;
    char issize = 0;

 // Check if buffer has enough space
    while(Remaining_size < nbytes){
        buff->size = buff->size * 2;
        Remaining_size = buff->size - buff->next;
        issize = 1;
    }

    if(issize == 0){
        memcpy((char *)buff->b + buff->next, data, nbytes);
        buff->next += nbytes;
        return;
    }

    // resize of the buffer
buff->b = realloc(buff->b, buff->size); 

// Copy data into buffer
    memcpy((char *)buff->b + buff->next, data, nbytes);
    buff->next += nbytes;
    return;
}

//De-serialization
void de_serialize_data(char *dest, ser_buff_t *buffer, int data_size){
	assert(buffer && buffer->data && data_size);
	if(data_size == 0) return;
	assert((buffer->total_size - buffer->next) >= data_size);

	memcpy(dest, buffer->data + buffer->next, data_size);
	buffer->next += data_size;
}

// Declare a function to initialize a serialized buffer of a defined size
void init_serialized_buffer_of_size(ser_buff_t **b, int size){
    // Allocate memory for the buffer struct
    (*b) = (ser_buff_t *)calloc(1, sizeof(ser_buff_t));
    
    // Allocate memory for the buffer data using the specified size
    (*b)->b = calloc(1, size);
    
    // Set the buffer size to the specified size
    (*b)->size = size;
    
    (*b)->next = 0;
}

void serialize_buffer_skip(ser_buff_t *buffer, int size){
    int available_space = buffer->size - buffer->next;

    if(available_space >= size){
        buffer->next += size;
        return;
    }

    while(available_space < size){
        buffer->size = buffer->size * 2;
        available_space = buffer->size - buffer->next;
    }   

    buffer->b = realloc(buffer->b, buffer->size);    
    buffer->next += size;
}
