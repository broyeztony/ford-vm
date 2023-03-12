//
//  ForVM.h
//  Ford-VM
//
//  Created by Tony on 07/03/2023.
//

#ifndef ForVM_h
#define ForVM_h

#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <iomanip>
#include "OpCode.h"
#include "Logger.h"
#include "FordValue.h"

#define READ_BYTE() *ip++
#define GET_CONST() constants[READ_BYTE()]

class FordVM {
    
public:
    FordVM() {}

    void push(const FordValue& value) {
        if((size_t)(sp - stack.begin()) == 512) {
            DIE << "push(): stack overflow.\n";
        }
        *sp = value;
        sp++;
    }
    
    FordValue pop() {
        if(sp == stack.begin()) {
            DIE << "pop(): empty stack.\n";
        }
        --sp;
        return *sp;
    }
    
    FordValue exec(const std::string &program) {
        
        // 1. parse AST
        // 2. compile AST to bytecode
        
        constants.push_back(NUMBER(2));
        constants.push_back(NUMBER(3));
        
        code = {OP_CONST, 0, OP_CONST, 1, OP_ADD, OP_HALT};
        
        ip = &code[0];
        sp = &stack[0];
        
        return eval();
    }
        
    FordValue eval() {
        for(;;) {
            
            auto opcode = READ_BYTE();
            
            switch (opcode) {
                case OP_HALT:
                    return pop();
                case OP_CONST:
                    push(GET_CONST());
                    break;
                case OP_ADD: {
                    auto op2 = AS_NUMBER(pop());
                    auto op1 = AS_NUMBER(pop());
                    auto result = op1 + op2;
                    push(NUMBER(result));
                    break;
                }
                default:
                    DIE << "unknown opcode: " << std::hex << std::setfill('0') << std::setw(2) << unsigned(opcode);
            }
        }
    }
    
    uint8_t* ip;
    FordValue* sp;
    
    std::array<FordValue, 512> stack;
    std::vector<uint8_t> code;
    std::vector<FordValue> constants;
};

#endif /* ForVM_h */
