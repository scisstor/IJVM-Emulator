#include <stack_local.h>

word_t first_number, second_number;
int jumping, previous_pc, pc;
Stack *stack, *previous_stack;
local previous_Local;
int inst_size;


void check_inst_size()
{
    if(inst_size > 1){
    	pc++;
    }
}
//Determining the size of the next instruction

signed short get_signed_short(byte_t num1, byte_t num2) {
    int16_t result = ((num1 << 8) & 0xFF00) | (num2 & 0xFF);
    return result;
}

uint16_t get_unsigned_short(byte_t num1, byte_t num2) {
    uint16_t result = ((num1 & 0xFF) << 8 | (num2 & 0xFF));
    return result;
}

int push_op(byte_t *commands){
	check_inst_size();
	if(inst_size > 1){
		int number = get_signed_short(commands[pc], commands[pc+1]);
		//printf("%0X pushed onto the stack\n", number);
		push(stack, number);
	}
	else{
	    push(stack, (signed char)commands[pc+1]);
	    //printf("%0X pushed onto the stack\n", (signed char)commands[pc+1]);
	}
    return 2;
}

//BIPUSH

int pop_op(){
	pop(stack);
	return 1;
}

int iadd_op(){
	first_number = (signed char)pop(stack);
    second_number = (signed char)pop(stack);
    first_number = first_number + second_number;
    push(stack, first_number);
    return 1;
}

int isub_op(){
	first_number = (signed char)pop(stack);
    second_number = (signed char)pop(stack);
    //printf("First number = %0d\n", first_number);
    //printf("Second number = %0d\n", second_number);
    first_number = second_number - first_number;
    push(stack, first_number);
    //printf("Result = %0x \n", first_number);
    return 1;
}

int iand_op(){
	first_number = pop(stack);
    second_number = pop(stack);
    push(stack, first_number & second_number);
    return 1;
}

int ior_op(){
	first_number = pop(stack);
    second_number = pop(stack);
    push(stack, first_number | second_number);
    return 1;
}

int swap_op(){
	first_number = pop(stack);
    second_number = pop(stack);
    push(stack,first_number);
    push(stack,second_number);
    return 1;
}

int out_op(){
	first_number = pop(stack);
    fprintf(out,"%c", first_number);
    return 1;
}

int in_op(){
	first_number = getc(in);
    if(first_number != -1){
        push(stack, (char)first_number);
    }
    else{
        push(stack, 0);
    }
    return 1;
}

int dup_op(){
	first_number = tos();
    push(stack, first_number);
    //printf("TOP = %0x\n", first_number);
    return 1;
}

int ldc_w_op(byte_t *commands){
	first_number = get_unsigned_short(commands[pc+1], commands[pc+2]);
    second_number = get_constant(first_number);
    push(stack, second_number);
    return 3;
}

int istore_op(byte_t *commands){
	//printf("Top of the stack rn = %0X\n", tos());
	check_inst_size();
	first_number = (int)pop(stack);
	if(inst_size > 1){
		int index = get_unsigned_short(commands[pc], commands[pc+1]);
		Local.variables[index] = first_number;
	}
	else{
	    Local.variables[commands[pc + 1]] = first_number;
	}
    // printf("%0xnth element is equal to = %0X\n", commands[pc+1],first_number);
    return 2;
}

int iload_op(byte_t *commands){
	check_inst_size();
	if(inst_size > 1){
		int index = get_unsigned_short(commands[pc], commands[pc+1]);
		push(stack, Local.variables[index]);
	}
	else{
		push(stack, Local.variables[commands[pc + 1]]);
	}
	return 2;
}

int iinc_op(byte_t *commands){
	check_inst_size();
	if(inst_size > 1){
		int index = get_unsigned_short(commands[pc], commands[pc+1]);
		Local.variables[index] += (signed char)commands[pc + 2];
	}
	else{
		Local.variables[commands[pc + 1]] += (signed char)commands[pc + 2];
	} 
	return 3;
}

int invoke_op(byte_t *commands){
	check_inst_size();
	if(myFrames.current_frame == -1){
        previous_pc = pc;
        previous_stack = stack;
        previous_Local = Local;
    }
    //if there is no frame, assign the main stream's variables to previous_pc, previous_stack and previous_Local;

    frame new_frame;
    new_frame.current_stack = initialize(128);
    new_frame.current_pc = get_constant(commands[pc + 1] + commands[pc + 2]);
    new_frame.current_local.variables = malloc(sizeof(word_t) * 1024);
    new_frame.current_local.size = 0;
    
    /*
   	  create a new frame for the invocation and set up its stack and pc by using the 
      element in the constant pool
    */

    myFrames.all_frames[myFrames.current_frame].current_pc = pc;
    myFrames.all_frames[myFrames.current_frame].current_stack = stack;
    myFrames.all_frames[myFrames.current_frame].current_local = Local;
    /*
      if you're already in a frame, before leaving this frame, save the frame's variables in
      frame array.
    */ 
    myFrames.current_frame ++;
    myFrames.all_frames[myFrames.current_frame] = new_frame;
    /*
      increment the frame array's counter and assign the newly created array to the current empty
      frame in frame array
	*/
    Local = new_frame.current_local;
    //assign the
    pc = new_frame.current_pc;
    //assign the global pc as the new frame's pc
    
    
   
    int num_of_arguments = get_unsigned_short(commands[pc], commands[pc+1]) - 1;
    /*
      if there exist parameters pass into the method, find the number of them
	  then, hold the first n element from the top of previous stack to use as parameters in the method
    */
    int local_variable_index = 1;
    //printf("Number of arguments needed to be passed to the function = %d\n", num_of_arguments);
    for (int i = num_of_arguments; i >0 ; --i)
    {   
        Local.variables[local_variable_index] = stack->data[stack->top - i + 1];
        local_variable_index++;
    }
    //Save the arguments to pass into the method in the local variable array
    for(int i = 0; i <= num_of_arguments; i++){
        pop(stack);
    }
    //Delete the arguments from the stack
    stack = new_frame.current_stack;
    //Assign the global stack as the new frame's empty stack
    return 4;
}


int return_op(){
	check_inst_size();
	if(myFrames.current_frame == 0){
        first_number = pop(stack);
        stack = previous_stack;
        pc = previous_pc;
        Local = previous_Local;
        push(stack, first_number);
        myFrames.current_frame = -1;
        return 3;
    }
    /*
    	If the current frame is 0 - meaning that all the invocations are returned, 
    	then assign the previous pc and stack as the global ones
    */
    first_number = pop(stack);
    //Pop the first element of the current stack and save it to push onto the previous stack later on
    free(stack);
    free(Local.variables);
    myFrames.current_frame --;
    stack = myFrames.all_frames[myFrames.current_frame].current_stack;
    pc = myFrames.all_frames[myFrames.current_frame].current_pc;
    Local = myFrames.all_frames[myFrames.current_frame].current_local;
    push(stack, first_number);
    /*
		Decrease the current frame counter and point to the previous one.
		Then, assignt the previous stack and pc as the global ones.
		Finally, push the popped element from the returned frame's stack onto the current stack.
    */
    return 3;
}






