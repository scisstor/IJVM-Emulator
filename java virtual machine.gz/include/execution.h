#include <opcode_functions.h>





int execute(byte_t *commands){
    //printf("Command[%d]:",pc);
    //printf("%0X\n",get_instruction());
    // for (int i = 0; i <= stack->top; ++i)
    // {
    //     printf("Stack[%d] = %0X\n",i, stack->data[i]);
    // }

    //jumping =  get_signed_short(commands[pc+1], commands[pc+2]);
    switch(get_instruction()){
        case OP_BIPUSH:
            //printf("BIPUSH called\n");
            return push_op(commands);
            //add a new element


        case OP_POP:
            //printf("POP called\n\n");
            return pop_op();
            //delete the element on the top


        case OP_IADD:
            //printf("OP_IADD called\n\n");
            return iadd_op();
            //addition


        case OP_ISUB:
            //printf("OP_ISUB called\n\n");
            return isub_op();
            //subtraction

        case OP_IAND:
            //printf("OP_IAND called\n\n");
            return iand_op();
            //bitwise AND operation

        case OP_IOR:
            //printf("OP_IOR called\n\n");
            return ior_op();
            //bitwise or operation

        case OP_SWAP:
            //printf("OP_SWAP called\n\n");
            return swap_op();
            //swap the top 2 words

        case OP_OUT:
            //printf("OP_OUT called\n");
            return out_op();

        case OP_DUP:
            //printf("OP_DUP called\n");
            return dup_op();

        case OP_GOTO:
            //printf("OP_GOTO called\n");
            return get_signed_short(commands[pc+1], commands[pc+2]);

        case OP_IFEQ:
            //printf("OP_IFEQ called\n");
            if(pop(stack) == 0){
                return get_signed_short(commands[pc+1], commands[pc+2]);
            }
            return 3;

        case OP_ICMPEQ:
            //rintf("OP_ICMPEQ called\n");
            first_number = pop(stack);
            second_number = pop(stack);
            if(first_number == second_number){
                return get_signed_short(commands[pc+1], commands[pc+2]);
            }
            return 3;

        case OP_IFLT:
            //printf("OP_IFLT called\n");
            first_number = pop(stack);
            if(first_number < 0){
                return get_signed_short(commands[pc+1], commands[pc+2]);
            }
            return 3;

        case OP_LDC_W:
            //printf("OP_LDC_W called\n");
            return ldc_w_op(commands);

        case OP_ISTORE:
            //printf("OP_ISTORE called\n");
            return istore_op(commands);

        case OP_ILOAD:
            //printf("OP_ILOAD called\n");
            return iload_op(commands);

        case OP_IINC:
            //printf("OP_IINC called\n");
            return iinc_op(commands);
        
        case OP_INVOKEVIRTUAL:
            //printf("OP_INVOKEVIRTUAL called\n");
            return invoke_op(commands);

        case OP_IRETURN:
            //printf("OP_IRETURN called\n");
            return return_op();

        case OP_IN:
            //printf("OP_IN called\n");
            return in_op();
        
        case OP_HALT:
            //printf("OP_HALT called\n");
            return 1;

        case OP_NOP:
            //printf("OP_NOP called\n");
            return 1;

        case OP_ERR:
            //printf("OP_ERR called\n");
            return 1;

        case OP_WIDE:
            //printf("OP_WIDE called\n");
            pc++;
            inst_size++;
            step();
            inst_size--;
            return 0;

        default:
            printf("There most be an error, switch case ended up in default state...\n");
            break;
    }
}
