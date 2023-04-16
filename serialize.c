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
void 
deserialize(char *dest, buffer_t *b, int size){
	if(!b || !b->b) assert(0);
	if(!size) return;	
	if((b->size - b->next)< size) assert(0);	 

	memcpy(dest, b->b + b->next, size);
	b->next += size;
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
/ Declare a function to initialize a serialized buffer
void init_serialized_buffer(ser_buff_t **b){
    // Allocate memory for the buffer struct
    (*b) = (ser_buff_t *)calloc(1, sizeof(ser_buff_t));
    
    // Allocate memory for the buffer data
    (*b)->b = calloc(1, BUFFER_DEFAULT_SIZE);
    
    // Set the buffer size
    (*b)->size = BUFFER_DEFAULT_SIZE;
    
    // Set the initial next index to 0
    (*b)->next = 0;
   
    return;
}
// This function skips a certain number of bytes in a serialized buffer.
void serialize_buffer_skip(ser_buff_t *buffer, int size){

    // Calculate the amount of available space in the buffer.
    int available_space = buffer->size - buffer->next;

    if(available_space >= size){
        buffer->next += size;
        return;
    }

    while(available_space < size){
        buffer->size = buffer->size * 2;
        available_space = buffer->size - buffer->next;
    }   

    // After creating enough space, use `realloc()` function to reallocate
    // memory for the buffer and assign it to the pointer `b` inside the ser_buff_t struct.
    buffer->b = realloc(buffer->b, buffer->size);    

    // Finally, increment the `next` variable by the size of bytes to be skipped,
    // indicating that the skipped bytes are no longer part of the buffer.
    buffer->next += size;
}
// Declare a function to check if a serialized buffer is empty
int is_serialized_buffer_empty(const ser_buff_t *b){
    if (b->next == 0) {
        return 1; // Return true if next index is 0
}
else
    return 0; // Otherwise, return false
}

// Declare a function to free the memory allocated for a serialized buffer
void free_serialized_buffer(ser_buff_t *b){
    if (b != NULL) { // Check if buffer exists
        free(b->b); // Free the memory allocated for the buffer data
        free(b);    // Free the memory allocated for the buffer struct
    }
}
