// SPDX-License-Identifier: GPL-3.0-or-later
// This file is part of VirtualINtel8080
// Copyright (C) 2025 OriginTravel
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <www.gnu.org>.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define PSW(cpu) ((uint16_t)cpu.A<<8 | cpu.flags)
struct cpu_t{
    uint8_t* mem;
    uint8_t A;
    union {
        struct {
            uint8_t C;
            uint8_t B;
        };
        uint16_t BC;
    };
    union {
        struct {
            uint8_t E;
            uint8_t D;
        };
        uint16_t DE;
    };
    union {
        struct {
            uint8_t L;
            uint8_t H;
        };
        uint16_t HL;
    };
    uint16_t SP;
    uint16_t PC;
    // 8-bit Flag register
    union {
        struct {
            uint8_t C:1;  // Carry, set if the last addition operation resulted in a carry or last sub required borrow
            uint8_t :1;
            uint8_t P:1;  // Parity bit, set if the number of true bits in the result is even
            uint8_t :1;
            uint8_t AC:1; // Auxiliary carry bit for binary coded decimal arithmetic 
            uint8_t :1;   
            uint8_t Z:1;  // Zero bit, set if the result is zero 
            uint8_t S:1;  // Sign bit, set if the result is negative
        } FLAGS;
        uint8_t flags;
    };
};




int main(int argc, char** argv){
    if (argc == 1){
        fputs("Intel8080: Arguments empty! Use -h for help\n", stderr);
        return 1;
    }
    if (*argv[1] == '-'){
        if (argv[1][1] == 'h'){

        }
        else{
            fputs("Intel8080: Arguments uncorrect! Use -h for help\n", stderr);
            return 1;
        }
        return 0;
    }
    FILE* file = fopen(argv[1], "r");
    if (file == NULL){
        fputs("Intel8080: open file error\n", stderr);
        return 1;
    }
    if(fseek(file, 0, SEEK_END) == -1){
        fputs("Intel8080: fseek error!\n", stderr);
        fclose(file);
        return 1;
    }
    size_t size = ftell(file);
    if (size > UINT16_MAX+1U){
        fputs("Intel8080: The file size exceeds 65KB!\n", stderr);
        fclose(file);
        return 1;
    }
    fseek(file, 0, SEEK_SET);
    // Init programm
    struct cpu_t cpu;
    cpu.mem = (uint8_t*) malloc(UINT16_MAX+1U);
    if (cpu.mem == NULL){
        return 1;
    }
    fread(cpu.mem, UINT16_MAX+1, 1, file);
    fclose(file);
    uint8_t error_code = 0;
    // EXECUTE 

    for (cpu.PC = 0; cpu.PC < size; ++cpu.PC){
        switch (cpu.mem[cpu.PC]){
            // nop
            case 0:
                break;
            // mvi A, Num
            case 0x3E:
                ++cpu.PC;
                cpu.A = cpu.mem[cpu.PC];
                break;
            // mvi B, Num
            case 0x06:
                ++cpu.PC;
                cpu.B = cpu.mem[cpu.PC];
                break;
            // mvi C, Num
            case 0x0E:
                ++cpu.PC;
                cpu.C = cpu.mem[cpu.PC];
                break;
            // mvi D, Num
            case 0x16:
                ++cpu.PC;
                cpu.D = cpu.mem[cpu.PC];
                break;
            // mvi E, Num
            case 0x1E:
                ++cpu.PC;
                cpu.E = cpu.mem[cpu.PC];
                break;
            // mvi H, Num
            case 0x26:
                ++cpu.PC;
                cpu.H = cpu.mem[cpu.PC];
                break;
            // mvi L, Num
            case 0x2E:
                ++cpu.PC;
                cpu.B = cpu.mem[cpu.PC];
                break;
            // lxi BC, Num(16-bit)
            case 0x01:
                ++cpu.PC;
                cpu.BC = (uint16_t)cpu.mem[cpu.PC+1]<<8 | cpu.mem[cpu.PC];
                ++cpu.PC;
                break;
            // lxi DE, Num(16-bit)
            case 0x11:
                ++cpu.PC;
                cpu.DE = (uint16_t)cpu.mem[cpu.PC+1]<<8 | cpu.mem[cpu.PC];
                ++cpu.PC;
                break;
            // lxi HL, Num(16-bit)
            case 0x21:
                ++cpu.PC;
                cpu.HL = (uint16_t)cpu.mem[cpu.PC+1]<<8 | cpu.mem[cpu.PC];
                ++cpu.PC;
                break;
            // lxi SP, Num(16-bit)
            case 0x31:
                ++cpu.PC;
                cpu.SP = (uint16_t)cpu.mem[cpu.PC+1]<<8 | cpu.mem[cpu.PC];
                ++cpu.PC;
                break;
            // sphl (SP = HL)
            case 0xF9:
                cpu.SP = cpu.HL;
                break;
            // mov B, A
            case 0x47:
                cpu.B = cpu.A;
                break; 
            // mov C, B
            case 0x48:
                cpu.B = cpu.A;
                break; 
            // mov D, C
            case 0x51:
                cpu.B = cpu.A;
                break; 
            // mov E, D
            case 0x5A:
                cpu.B = cpu.A;
                break; 
            // mov H, E
            case 0x63:
                cpu.B = cpu.A;
                break; 
            // mov L, H
            case 0x6C:
                cpu.B = cpu.A;
                break; 
            // pchl (PC = HL)
            case 0xE9:
                cpu.PC = cpu.HL;
                break;
            // STA Num(16-bit)
            case 0x32:
                ++cpu.PC;
                cpu.A = cpu.mem[(uint16_t)cpu.mem[cpu.PC+1]<<8 | cpu.mem[cpu.PC]];
                ++cpu.PC;
                break;
            // LDA Num(16-bit)
            case 0x3A:
                ++cpu.PC;
                cpu.A = cpu.mem[(uint16_t)cpu.mem[cpu.PC+1]<<8 | cpu.mem[cpu.PC]];
                ++cpu.PC;
                break;
            // LHLD Num(16-bit)
            case 0x2A:
                ++cpu.PC;
                cpu.L = cpu.mem[(uint16_t)cpu.mem[cpu.PC+1]<<8 | cpu.mem[cpu.PC]];
                cpu.H = cpu.mem[((uint16_t)cpu.mem[cpu.PC+1]<<8 | cpu.mem[cpu.PC])+1];
                ++cpu.PC;
                break;
            // SHLD Num(16-bit)
            case 0x22:
                ++cpu.PC;
                cpu.mem[(uint16_t)cpu.mem[cpu.PC+1]<<8 | cpu.mem[cpu.PC]] = cpu.L;
                cpu.mem[((uint16_t)cpu.mem[cpu.PC+1]<<8 | cpu.mem[cpu.PC])+1] = cpu.H;
                ++cpu.PC;
                break;
            

            // halt
            case 0x76U:
                goto Exit;
                break;
            // Unknown opcode
            default:
                goto Exit;
        }
    }
    Exit:
    fprintf(stderr, "\nA: %u\nB: %u\nC: %u\nD: %u\nH: %u\nL: %u\nE: %u\n", cpu.A, cpu.B, cpu.C, cpu.D, cpu.H, cpu.L, cpu.E );
    fprintf(stderr, "Exit code: %u\n", error_code);
    free(cpu.mem);
    return 0;
}