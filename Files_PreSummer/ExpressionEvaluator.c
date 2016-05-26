#include <stdio.h>
#include <stdlib.h>

struct stack_node{
    //nodes for stack containing the operands
    struct stack_node* next;
    float data;
};

/*
struct operators_stack_node{
    //node for the stak containing the operators
    struct operators_stack_node *next;
    char operator;
};
*/

struct HEAD{
    //node containing information about the stack
    //int length;                //Use for debugging
    struct stack_node *top;
    struct stack_node *bot;
};

FILE *fp = NULL;
char file_name[] =  "SampleFile_ExpressionEvaluator.txt";
struct HEAD stack_results;

//Stack Operations
void push(float data,struct HEAD *my_stack){
    struct stack_node *new = (struct stack_node *)malloc(sizeof(struct stack_node));
    new-> data = data;
    
   //printf("  pushing %f into stack", data);
    if(my_stack-> top == NULL){
       //printf("  new stack created  ");
        my_stack-> top = my_stack-> bot = new;
        my_stack-> bot-> next = NULL;
        
    }
    else{
        new-> next = my_stack-> top;
        my_stack-> top = new;
    }
    
}

float pop(struct HEAD *stack){
    if(stack-> top == NULL){
        fprintf(stderr, "Trying to pop an empty stack");
        return 0;
    }
    else{
        float data = stack-> top-> data;
        struct stack_node *temp = stack-> top;
        
        stack-> top = stack-> top-> next;
        free(temp);
        if (stack-> top == NULL){
            stack-> bot == NULL;
           //printf("   stack is empty now");
        }
       //printf("  Removed %f", data);
        return data;
    }
}

void print_stack(struct HEAD *my_stack){
    struct stack_node *temp = my_stack-> top;
   //printf("\nStack : ");
    while(temp){
       //printf("%f, ", temp-> data);
        temp = temp-> next;
    }
   //printf("\n");
}

void initialize_stack(struct HEAD *my_stack){
    my_stack-> top = NULL;
    my_stack-> bot = NULL;
}
//End Stack Operations

int is_digit(char c){
    if (c <= '9' & c >= '0'){
        return 1;
    }
    return 0;
}

float power(int x, int y){
    int i = 0;
    int result = 1;
    while(i < y){
        result *= x;
        i++;
    }
    return result;
}

float evaluate_expression(struct HEAD *stack_operands, struct HEAD *stack_operators){
    float result = 0;
    float op1 = pop(stack_operands);
    float op2 = pop(stack_operands);
    int operator = pop(stack_operators);
    
   //printf("  Evaluating expression : %f op(%d) %f", op2, (int)operator, op1);
    print_stack(stack_operands);
    switch(operator){
        /*
            7 = **
            5 = *
            4 = /
            2 = +
            1 = -
        */
        case 7:
            result = power(op2, op1);
            break;
        case 5:
            result = op2 * op1;
            break;
        case 4:
            result = op2 / op1;
            break;
        case 2:
            result = op2 + op1;
            break;
        case 1:
            result = op2 - op1;
            break;
        default:
            break;
    }
   //printf("\n\tEvaluated %f op %f  = %f", op2, op1, result);
    
    push(result, stack_operands);
    return result;
}

int file_read(){
    struct HEAD stack_operands;
    struct HEAD stack_operators;
    char c = 0;
    FILE *temp_file = fopen("temp_file.txt", "w");
    
   //printf("\nInitializing stacks");
    initialize_stack(&stack_operands);
    initialize_stack(&stack_operators);
    print_stack(&stack_operands);
    print_stack(&stack_operators);
   //printf("  Reading File");
    while(!feof(fp)){
        //read line
        
        fscanf(fp, "%c", &c);
       //printf("\n  %c read", c);
        if(c == '\r'){
            float result = 0;
            
           //printf("\n  End of line read");
            
            //fprintf(fp, "=");
            while((stack_operands.top)-> next){
                evaluate_expression(&stack_operands, &stack_operators);
                //push(result, &stack_operands);
            }
            //fprintf(fp, "%d\n", (int)result);
           //printf("\nresult of line =  %d", (int)(stack_operands.bot-> data));
            fprintf(temp_file, "=%f\r", stack_operands.bot-> data);
           //printf("Removin the result node, stack must be empty now");
            pop(&stack_operands);
            print_stack(&stack_operands);
            print_stack(&stack_operators);
            
        }
        else{
            if(is_digit(c)){
                float num;
               //printf("\nRead a digit");
                fseek(fp, -1, SEEK_CUR);
                fscanf(fp, "%f", &num);
               //printf(",read %f ", num);
                fprintf(temp_file, "%d", (int)num);
                //printf(" pushing %f to stack", num);
                push(num, &stack_operands);
            }
            else{
                float operator = 0;
                
                fprintf(temp_file, "%c", c);
                switch(c){
                    char next_char = 0;
                    
                    case '*':
                        fscanf(fp, "%c", &next_char);
                        if(next_char == '*'){
                            operator = 7;
                            fprintf(temp_file, "*");
                        }
                        else{
                            fseek(fp, -1, SEEK_CUR);
                            operator = 5;
                        }
                        break;
                    case '/':
                        operator = 4;
                        break;
                    case '+':
                        operator = 2;
                        break;
                    case '-':
                        operator = 1;
                        break;
                    default:
                        //fprintf(stderr, "\n\t\t\tUnknown operator %c", c);
                        break;
                }
                if(operator){
                    //Pushing the operator into the stack
                   //printf("  operator = %f\n",operator);
                    while((stack_operators.top) && ( (int)( (stack_operators.top-> data)-operator ) ) > -2){
                       //printf("\n(int)( (stack_operators.top-> data)-operator ) = %d", (int)( (stack_operators.top-> data)-operator ));
                        evaluate_expression(&stack_operands, &stack_operators);
                        
                    }
                    push(operator, &stack_operators);
                }
            }
        }
    }
   //printf("\n\t\t\tFinished evaluating\n");
    fclose(fp);
    fclose(temp_file);
    if(!(fp = fopen(file_name, "w"))){
       //printf("\nError opening source file while copying");
    }
    else{
        if(!(temp_file = fopen("temp_file.txt", "r"))){
           //printf("\nError while opening temp_file while copying");
        }
        else{
            char c;
            while(!feof(temp_file)){
                fscanf(temp_file, "%c", &c);
                fprintf(fp, "%c", c);
            }
            fclose(temp_file);
        }
        fclose(fp);
    }
   //printf("\n\t\t\t\tFinished Copying");
    //system("rm temp_file.txt");
}

int main(){
    
    fp = fopen(file_name, "r");
    if(!fp){
       //printf("Error opening file");
    }
    else{
        file_read();
    }
    
    return 0;
}