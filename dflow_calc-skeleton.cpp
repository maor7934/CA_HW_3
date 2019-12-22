/* 046267 Computer Architecture - Winter 2019/20 - HW #3 */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"



class Command {
private:
    bool dependet_on_me;
    int dep_one;
    int dep_two;
    int depth;
    int my_time;
public:
    Command (int dep_one_t, int dep_two_t, int depth_t, int my_time_t) : dependet_on_me(false), dep_one(dep_one_t),
                                                                         dep_two(dep_two_t), depth(depth_t),my_time(my_time_t){}
    ~Command() = default;
    void AddDependence (){
        this->dependet_on_me = true;
    }
    int GetDepOne (){
        return this->dep_one;
    }
    int GetDepTwo (){
        return this->dep_two;
    }
    int GetDepth (){
        return this->depth;
    }
    int GetMyTime (){
        return this->my_time;
    }
    bool CheckLast (){
        return this->dependet_on_me;
    }
};
int GetMax (Command* one, Command* two){
    if (!one and !two){
        return 0;
    }
    if (one == nullptr){
        return (two->GetDepth() + two->GetMyTime());
    }
    if (two == nullptr){
        return (one->GetDepth() + one->GetMyTime());
    }
    return (one->GetDepth() + one->GetMyTime()) > (two->GetDepth() + two->GetMyTime())
    ? (one->GetDepth() + one->GetMyTime()) : (two->GetDepth() + two->GetMyTime());
}
int CalcExitDepth (Command** command_array, unsigned size){
    int max_depth = 0;
    for (int i = 0; i < size; i++){
        if (!command_array[i]->CheckLast()){
            if (command_array[i]->GetDepth() + command_array[i]->GetMyTime() > max_depth){
                max_depth = command_array[i]->GetDepth() + command_array[i]->GetMyTime();
            }
        }
    }
    return max_depth;
}

class Dependecies_Calc{
private:
    const unsigned* opcodes_times;
    int last_relevant_command [MAX_OPS];
    Command** all_commands;
    unsigned max_exit_time;
    unsigned num_of_insts;

    void _calc_my_data(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts){
        const InstInfo*  temp_ints;
        int first_relevant_command;
        int second_relevant_command;
        for (int i=0; i< numOfInsts; i++){
            temp_ints = &progTrace[i];

            //find first depended command if there is none, return NULL
            first_relevant_command = this->last_relevant_command[temp_ints->src1Idx];
            Command * real_first_command = NULL;
            if (first_relevant_command!= -1){
                real_first_command = this->all_commands[first_relevant_command];
                real_first_command->AddDependence();
            }
            //find second depended command if there is none, return NULL
            second_relevant_command = this->last_relevant_command[temp_ints->src2Idx];
            Command * real_second_command = NULL;
            if (second_relevant_command!= -1){
                real_second_command = this->all_commands[second_relevant_command];
                real_second_command->AddDependence();
            }
            //create new command
            this->all_commands[i] = new Command(first_relevant_command, second_relevant_command,
                                                GetMax(real_first_command, real_second_command),
                                                this->opcodes_times[temp_ints->opcode]);
            // update last_relevant_command table
            this->last_relevant_command[temp_ints->dstIdx] = i;
        }
    }
public:
    Dependecies_Calc(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts):
    num_of_insts(numOfInsts){
        this->opcodes_times = opsLatency;
        this->all_commands = new Command*[numOfInsts]; // TODO: Maybe need to restart each one?
        for (int i=0;i<MAX_OPS;i++){
            this->last_relevant_command[i] = -1;
        }
        this->_calc_my_data(opsLatency,  progTrace,  numOfInsts);
        this->max_exit_time = CalcExitDepth(this->all_commands, numOfInsts);
    }

    ~Dependecies_Calc(){
        for (int i=0; i< this->num_of_insts; i++){
            delete this->all_commands[i];
        }
        delete this->all_commands;
    }

    int GetExitDepth(){
        return this->max_exit_time;
    }

    int GetInstDepth(unsigned inst_idx){
        return this->all_commands[inst_idx]->GetDepth();
    }

    int GetInstDependencies(unsigned inst_idx, int *src1DepInst, int *src2DepInst){
        *src1DepInst = this->all_commands[inst_idx]->GetDepOne();
        *src2DepInst = this->all_commands[inst_idx]->GetDepTwo();
        if (*src1DepInst == -1 and *src2DepInst == -1){
            return -1;
        }
        return 0;
    }

};

ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts) {
    Dependecies_Calc* new_calc = new Dependecies_Calc(opsLatency, progTrace, numOfInsts);
    return new_calc;
}

void freeProgCtx(ProgCtx ctx) {
    delete (Dependecies_Calc*)ctx;
}

int getInstDepth(ProgCtx ctx, unsigned int theInst) {
    return ((Dependecies_Calc*)ctx)->GetInstDepth(theInst);
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst) {
    return ((Dependecies_Calc*)ctx)->GetInstDependencies(theInst, src1DepInst, src2DepInst);
}

int getProgDepth(ProgCtx ctx) {
    return ((Dependecies_Calc*)ctx)->GetExitDepth();
}

