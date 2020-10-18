#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ijvm.h>
#include <stdlib.h>
#include <sys/stat.h>

FILE *out;
FILE *in;

char *source_file;

static uint32_t swap_uint32(uint32_t num)
{
    return ((num>>24)&0xff)|((num<<8)&0xff0000)|((num>>8)&0xff00)|((num<<24)&0xff000000);
}

typedef struct
{
    word_t magic_number, constant_origin, constant_b_size, *constant_pool, text_origin, text_size, *operation;
    byte_t *commands;
}container;

container myContainer;

void parse_data(FILE *fp)
{
	//keep the name of the file as a constant outside of the operation in case you wanna use it later on.
    fread(&myContainer.magic_number, sizeof(word_t), 1, fp);
    myContainer.magic_number = swap_uint32(myContainer.magic_number);
    //printf("-------------------------------------------------\n");
    //printf("magic_number = %0X\n", myContainer.magic_number);
    
    fread(&myContainer.constant_origin, sizeof(word_t), 1, fp);
    myContainer.constant_origin = swap_uint32(myContainer.constant_origin);
    //printf("constant_origin = %0X\n", myContainer.constant_origin);

    fread(&myContainer.constant_b_size, sizeof(word_t), 1, fp);
    myContainer.constant_b_size = swap_uint32(myContainer.constant_b_size);
    //printf("constant_b_size = %0X\n", myContainer.constant_b_size);
    
    myContainer.constant_pool = malloc(sizeof(word_t) * myContainer.constant_b_size/4);
    fread(myContainer.constant_pool, sizeof(word_t), myContainer.constant_b_size/4, fp);
    
    for(int i= 0; i< myContainer.constant_b_size/4; i++)
    {
        myContainer.constant_pool[i] = swap_uint32(myContainer.constant_pool[i]);
        //printf("constant_pool = %0X\n", myContainer.constant_pool[i]);
    }
    

    fread(&myContainer.text_origin, sizeof(word_t), 1, fp);
    myContainer.text_origin = swap_uint32(myContainer.text_origin);
    //printf("text_origin = %0X\n", myContainer.text_origin);

    fread(&myContainer.text_size, sizeof(word_t), 1, fp);
    myContainer.text_size = swap_uint32(myContainer.text_size);
    //printf("text_size = %0X\n", myContainer.text_size);

    myContainer.commands = malloc(sizeof(byte_t) * myContainer.text_size);
    fread(myContainer.commands, sizeof(byte_t), myContainer.text_size, fp);
    // for(int i = 0; i < myContainer.text_size; i++)
    // {
    //     printf("operation[%i]= %0X\n",i, myContainer.commands[i]);
    // }
}
