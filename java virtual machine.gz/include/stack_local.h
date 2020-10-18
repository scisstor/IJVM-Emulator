#include <parse_data.h>

typedef struct
{
    word_t *variables;
    int size;
}local;
//Local variable array;

local Local;
//Define an array for the local variables.

typedef struct
{
    int* data;
    int top;
    int size;
}Stack;

//Define a Stack.

typedef struct
{
    int current_pc;
    Stack *current_stack;
    local current_local;

}frame;

//Define a frame for method invocations
//Bir frame olustur, bu frame in icinde bir stack olustur, current pc yi bu frame e gore ayarla,
//ayni zamanda bu frame icerisinde onceki stackin pointer ini ve pc sini tut.

/*-----Eger bir fonksiyon return edilirse, current stack in son elemani alinir ve bir yere konulur.
Sonra previous stack, current stack olarak assign edilir ve ayni zamanda previous pc, asil pc olarak assign edilir.
Son asama olarak return edilen frame in stackinden alinan element current stackin basina push edilir.
*/


typedef struct
{
    int current_frame;
    frame* all_frames;
}frame_vector;

//define an array to keep all the frames and point to the current frame with current_frame

frame_vector myFrames;



Stack* initialize(int size)
{
    //allocate memory for the stack
    Stack *sp = (Stack*)malloc(sizeof(Stack));
    if(sp == '\0')
    {
        printf("Memory could not have been allocated for the Stack, the program is being terminated..\n");
        exit(1);
    }

    //allocate memory for the data array which will be held in the Stack
    sp->data = (int*)malloc(sizeof(int) * size);
    
    if(sp->data == '\0')
    {
        printf("Memory could not have been allocated for the Stack, the program is being terminated..\n");
        exit(1);
    }

    sp->top = -1;
    //initilazi the top variable with an arbitrary value "-1"
    sp->size = size;
    //equalize the size of the stack with the given parameter 'size' into the function "initialize"
    return sp;
}



void push(Stack *sp, int value)
{
    if(sp->top == sp->size - 1)
    {
        //double the size of the array
        int *temp = (int*)malloc(sizeof(int) * sp->size * 2);
        //new temporary stack pointer
        if(temp == '\0')
        {
            printf("Memory could not have been allocated...\n");
            return;
        }
        //memory has been succesfully allocated for the temporary array...
        for(int i = 0; i < sp->top; i++)
        {
            temp[i] = sp->data[i];
        }

        free(sp->data);
        //free the previous stack memory
        sp->data = temp;
        //replace the new stack with the old one
        sp->size *= 2;
        //double the size of the stack
    }

    sp->data[++sp->top] = value;
    //push the data on top of the new stack
}


int pop(Stack *sp)
{
    if(sp->top == - 1)
    {
        printf("POP error : The stack is empty!!!\n");
        return -1001;
    }
    else
    {
        return sp->data[sp->top--];
        //return the top element of stack and decrement the top with 1
    }
}


