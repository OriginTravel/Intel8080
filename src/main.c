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
    uint8_t* memory;
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
    if (argc != 2){
        fputs("Intel8080: Arguments empty! Use -h for help\n", stderr);
        return 1;
    }
    if (*argv[1] == '-'){
        if (argv[1][1] == 'h'){
            puts("Intel8080 Emulator version 0.2!\nAuthor: OriginTravel\nLicense: GNU GPL v3.0\nUsage:\nintel8080 file.bin\nintel8080 file.com\n");
        }
        else{
            fputs("Intel8080: Arguments uncorrect! Use -h for help\n", stderr);
            return 1;
        }
        return 0;
    }
    FILE* file = fopen(argv[1], "r");
    if (file == NULL){
        fputs("Intel8080: Open file error\n", stderr);
        return 1;
    }
    if(fseek(file, 0, SEEK_END) == -1){
        fputs("Intel8080: Fseek error!\n", stderr);
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
    cpu.memory = (uint8_t*) malloc(UINT16_MAX+1U);
    if (cpu.memory == NULL){
        return 1;
    }
    fread(cpu.memory, UINT16_MAX+1, 1, file);
    fclose(file);
    uint8_t error_code = 0;

    // execute machine code
    for (cpu.PC = 0; cpu.PC < size; ++cpu.PC){
        switch (cpu.memory[cpu.PC]){

            // mvi A, Num
            case 0x3E:
                ++cpu.PC;
                cpu.A = cpu.memory[cpu.PC];
                break;
            // mvi B, Num
            case 0x06:
                ++cpu.PC;
                cpu.B = cpu.memory[cpu.PC];
                break;
            // mvi C, Num
            case 0x0E:
                ++cpu.PC;
                cpu.C = cpu.memory[cpu.PC];
                break;
            // mvi D, Num
            case 0x16:
                ++cpu.PC;
                cpu.D = cpu.memory[cpu.PC];
                break;
            // mvi E, Num
            case 0x1E:
                ++cpu.PC;
                cpu.E = cpu.memory[cpu.PC];
                break;
            // mvi H, Num
            case 0x26:
                ++cpu.PC;
                cpu.H = cpu.memory[cpu.PC];
                break;
            // mvi L, Num
            case 0x2E:
                ++cpu.PC;
                cpu.B = cpu.memory[cpu.PC];
                break;

            // mov B, C
            case 0x41:
                cpu.B = cpu.C;
                break;
            // mov B, D
            case 0x42:
                cpu.B = cpu.D;
                break;
            // mov B, E
            case 0x43:
                cpu.B = cpu.E;
                break;
            // mov B, H
            case 0x44:
                cpu.B = cpu.H;
                break;
            // mov B, L
            case 0x45:
                cpu.B = cpu.L;
                break;
            // mov B, [memory value from address(HL)]
            case 0x46:
                cpu.B = cpu.memory[cpu.HL];
                break;
            // mov B, A
            case 0x47:
                cpu.B = cpu.A;
                break; 

            // mov C, B
            case 0x48:
                cpu.B = cpu.A;
                break; 
            // mov C, D
            case 0x4A:
                cpu.C = cpu.D;
                break;
            // mov C, E
            case 0x4B:
                cpu.C = cpu.E;
                break;
            // mov C, H
            case 0x4C:
                cpu.C = cpu.H;
                break;
            // mov C, L
            case 0x4D:
                cpu.C = cpu.L;
                break;
            // mov C, [memory value from address(HL)]
            case 0x4E:
                cpu.C = cpu.memory[cpu.HL];
                break;
            // mov C, A
            case 0x4F:
                cpu.C = cpu.A;
                break;
 
            // mov D, B
            case 0x50:
                cpu.D = cpu.B;
                break; 
            // mov D, C
            case 0x51:
                cpu.D = cpu.C;
                break; 
            // mov D, E
            case 0x53:
                cpu.D = cpu.E;
                break;
            // mov D, H
            case 0x54:
                cpu.D = cpu.H;
                break;
            // mov D, L
            case 0x55:
                cpu.D = cpu.L;
                break;
            // mov D, [memory value from address(HL)]
            case 0x56:
                cpu.D = cpu.memory[cpu.HL];
                break;
            // mov D, A
            case 0x57:
                cpu.D = cpu.A;
                break;

            // mov E, B
            case 0x58:
                cpu.E = cpu.B;
                break; 
            // mov E, C
            case 0x59:
                cpu.E = cpu.C;
                break; 
            // mov E, D
            case 0x5A:
                cpu.E = cpu.E;
                break;
            // mov E, H
            case 0x5C:
                cpu.E = cpu.H;
                break;
            // mov E, L
            case 0x5D:
                cpu.E = cpu.L;
                break;
            // mov E, [memory value from address(HL)]
            case 0x5E:
                cpu.E = cpu.memory[cpu.HL];
                break;
            // mov E, A
            case 0x5F:
                cpu.E = cpu.A;
                break;

            // mov H, B
            case 0x60:
                cpu.H = cpu.B;
                break; 
            // mov H, C
            case 0x61:
                cpu.H = cpu.C;
                break; 
            // mov H, D
            case 0x62:
                cpu.H = cpu.D;
                break;
            // mov H, E
            case 0x63:
                cpu.H = cpu.E;
                break;
            // mov H, L
            case 0x65:
                cpu.H = cpu.L;
                break;
            // mov H, [memory value from address(HL)]
            case 0x66:
                cpu.H = cpu.memory[cpu.HL];
                break;
            // mov H, A
            case 0x67:
                cpu.H = cpu.A;
                break;

            // mov L, B
            case 0x68:
                cpu.L = cpu.B;
                break; 
            // mov L, C
            case 0x69:
                cpu.L = cpu.C;
                break; 
            // mov L, D
            case 0x6A:
                cpu.L = cpu.D;
                break;
            // mov L, E
            case 0x6B:
                cpu.L = cpu.E;
                break;
            // mov L, H
            case 0x6C:
                cpu.L = cpu.H;
                break;
            // mov L, [memory value from address(HL)]
            case 0x6E:
                cpu.L = cpu.memory[cpu.HL];
                break;
            // mov L, A
            case 0x6F:
                cpu.L = cpu.A;
                break;

            // mov A, B
            case 0x78:
                cpu.A = cpu.B;
                break; 
            // mov A, C
            case 0x79:
                cpu.A = cpu.C;
                break; 
            // mov A, D
            case 0x7A:
                cpu.A = cpu.D;
                break;
            // mov A, E
            case 0x7B:
                cpu.A = cpu.E;
                break;
            // mov A, H
            case 0x7C:
                cpu.A = cpu.H;
                break;
            // mov A, L
            case 0x7D:
                cpu.A = cpu.L;
                break;   
            // mov A, [memory value from address(HL)]
            case 0x7E:
                cpu.A = cpu.memory[cpu.HL];
                break;

            // mov [memory value from address(HL)], B
            case 0x70:
                cpu.memory[cpu.HL] = cpu.B;
                break;
            // mov [memory value from address(HL)], C
            case 0x71:
                cpu.memory[cpu.HL] = cpu.C;
                break;
            // mov [memory value from address(HL)], D
            case 0x72:
                cpu.memory[cpu.HL] = cpu.D;
                break;
            // mov [memory value from address(HL)], E
            case 0x73:
                cpu.memory[cpu.HL] = cpu.E;
                break;
            // mov [memory value from address(HL)], H
            case 0x74:
                cpu.memory[cpu.HL] = cpu.H;
                break;
            // mov [memory value from address(HL)], L
            case 0x75:
                cpu.memory[cpu.HL] = cpu.L;
                break;
            // mov [memory value from address(HL)], A
            case 0x77:
                cpu.memory[cpu.HL] = cpu.A;
                break;
         






                
      

            // lxi BC, Num(16-bit)
            case 0x01:
                ++cpu.PC;
                cpu.BC = (uint16_t)cpu.memory[cpu.PC+1]<<8 | cpu.memory[cpu.PC];
                ++cpu.PC;
                break;
            // lxi DE, Num(16-bit)
            case 0x11:
                ++cpu.PC;
                cpu.DE = (uint16_t)cpu.memory[cpu.PC+1]<<8 | cpu.memory[cpu.PC];
                ++cpu.PC;
                break;
            // lxi HL, Num(16-bit)
            case 0x21:
                ++cpu.PC;
                cpu.HL = (uint16_t)cpu.memory[cpu.PC+1]<<8 | cpu.memory[cpu.PC];
                ++cpu.PC;
                break;
            // lxi SP, Num(16-bit)
            case 0x31:
                ++cpu.PC;
                cpu.SP = (uint16_t)cpu.memory[cpu.PC+1]<<8 | cpu.memory[cpu.PC];
                ++cpu.PC;
                break;






            // sphl (SP = HL)
            case 0xF9:
                cpu.SP = cpu.HL;
                break;
            // pchl (PC = HL)
            case 0xE9:
                cpu.PC = cpu.HL;
                break;
            // STA Num(16-bit)
            case 0x32:
                ++cpu.PC;
                cpu.A = cpu.memory[(uint16_t)cpu.memory[cpu.PC+1]<<8 | cpu.memory[cpu.PC]];
                ++cpu.PC;
                break;
            // LDA Num(16-bit)
            case 0x3A:
                ++cpu.PC;
                cpu.A = cpu.memory[(uint16_t)cpu.memory[cpu.PC+1]<<8 | cpu.memory[cpu.PC]];
                ++cpu.PC;
                break;
            // LHLD Num(16-bit)
            case 0x2A:
                ++cpu.PC;
                cpu.L = cpu.memory[(uint16_t)cpu.memory[cpu.PC+1]<<8 | cpu.memory[cpu.PC]];
                cpu.H = cpu.memory[((uint16_t)cpu.memory[cpu.PC+1]<<8 | cpu.memory[cpu.PC])+1];
                ++cpu.PC;
                break;
            // SHLD Num(16-bit)
            case 0x22:
                ++cpu.PC;
                cpu.memory[(uint16_t)cpu.memory[cpu.PC+1]<<8 | cpu.memory[cpu.PC]] = cpu.L;
                cpu.memory[((uint16_t)cpu.memory[cpu.PC+1]<<8 | cpu.memory[cpu.PC])+1] = cpu.H;
                ++cpu.PC;
                break;
            



            // HLT: stop proccessor
            case 0x76U:
                goto Exit;
                break;
            /* nop(And instructions that don't do any work)
            0x49; 0x40; 0x52; 0x5B; 0x64; 0x6D; 0x7F; 0x00
            */
            default:
                break;
        }
    }
    Exit:
    free(cpu.memory);
    return 0;
}