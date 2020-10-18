#include <execution.h>


int init_ijvm(char *file_name)
{
    source_file = file_name;
    FILE *fp;
    fp = fopen(file_name,"rb");
    if (fp == '\0') {
        printf( "The file %s could not be opened\n", file_name);
        return -1;
    }
    else{
        //keep the name of the file as a constant outside of the operation in case you wanna use it later on.
        parse_data(fp);
        //printf("\n\n\n\n");
        stack = initialize(128);
        Local.variables = malloc(sizeof(word_t) * 1024);
        Local.size = 0;
        pc = 0;
        out = stdout;
        in = stdin;
        myFrames.current_frame = -1;
        myFrames.all_frames = (frame*)malloc(sizeof(frame) * 128);
        inst_size = 1;
        return 0;
    }
}

void destroy_ijvm()
{
    free(stack->data);
    free(myContainer.commands);
    free(myContainer.constant_pool);
    myContainer.constant_b_size = 0;
    myContainer.text_origin = 0;
    myContainer.text_size = 0;
}

void run()
{
    while(finished()){
        step();
    }
    //Run the vm with the current state until the machine halts.
}



void set_input(FILE *fp)
{
    in = fp;
}

void set_output(FILE *fp)
{
    out = fp;
}

bool step()
{
    int increment = execute(myContainer.commands);
    pc += increment;
}

int text_size()
{
    return myContainer.text_size;
}


byte_t *get_text()
{
    return myContainer.commands;
}

int get_program_counter()
{
    return pc;
}


word_t tos()
{
    if(stack->top == - 1)
    {
        printf("tos() error : The stack is empty!!!\n");
        return 9999;
    }
    else
    {
        return stack->data[stack->top];
    }
}

word_t *get_stack()
{
    return stack->data;
}

int stack_size()
{
    return stack->top + 1;
}

word_t get_local_variable(int i)
{
    return Local.variables[i];
}

word_t get_constant(int i)
{
    return myContainer.constant_pool[i];
}

bool finished(void)
{
    if(pc < myContainer.text_size)
    {
        if(get_instruction() == OP_HALT || get_instruction() == OP_ERR)
        {
            //printf("Program halted\n");
            return false;
        }
        return true;
    }
    return false;
}

byte_t get_instruction(void)
{
    return myContainer.commands[pc];
}





