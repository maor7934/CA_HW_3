/* 046267 Computer Architecture - Winter 2019/20 - HW #3 */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"

ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts) {
    return PROG_CTX_NULL;
}

void freeProgCtx(ProgCtx ctx) {
}

int getInstDepth(ProgCtx ctx, unsigned int theInst) {
    return -1;
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst) {
    return -1;
}

int getProgDepth(ProgCtx ctx) {
    return 0;
}

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
    return one->GetDepth() > two->GetDepth() ? one->GetDepth() : two->GetDepth();
}
int GetExitDepth (Command** command_array, int size){
    int max_depth = 0;
    for (int i = 0; i < size; i++){
        if (command_array[i]->CheckLast()){
            if (command_array[i]->GetDepth() > max_depth){
                max_depth = command_array[i]->GetDepth();
            }
        }
        return max_depth;
    }
}
