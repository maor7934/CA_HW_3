/* 046267 Computer Architecture - Winter 2019/20 - HW #3 */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"


class Command{

};

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
                Command * real_first_command = this->all_commands[first_relevant_command];
                real_first_command->AddDependence();
            }
            //find second depended command if there is none, return NULL
            second_relevant_command = this->last_relevant_command[temp_ints->src2Idx];
            Command * real_second_command = NULL;
            if (second_relevant_command!= -1){
                Command * real_second_command = this->all_commands[second_relevant_command];
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
        this->max_exit_time = GetExitDepth(this->all_commands, numOfInsts);
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


