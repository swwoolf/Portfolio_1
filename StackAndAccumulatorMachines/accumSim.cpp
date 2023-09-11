#include <iostream>
#include <string>
#include <stdint.h>
#include <cstddef>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <cstdint>
#define data_seg_size 1000
#define instr_seg_size 1000
#define kernel_data_size 200
#define Kernel_text_size 100

using namespace std;

//initializes memory locations, data types, memory pointers, and the program counter

typedef unsigned int uint32;
typedef uint32 mem_addr;
typedef int32_t mem_word;
typedef int32_t instr;
typedef uint8_t opcode;
typedef int accumulator;

//user text segment
instr text_seg[instr_seg_size];

//user data segment
mem_word data_seg[data_seg_size];

//kernel data and text segments
instr kernel_text_seg[kernel_data_size];
mem_word kernel_data_seg[Kernel_text_size];

//the accumulator
accumulator accum;

//addresses of bottom of user data and text segments
mem_addr data_min;
mem_addr text_min;


mem_addr string_begin1;
mem_addr string_end1;
mem_addr string_begin2;
mem_addr string_end2;

mem_addr text_min_hex;
mem_addr data_min_hex;

//program counter and pointer to first open address in user data segment
mem_addr ProgramCounter;
mem_addr data_current;

void load_text();

void load_kernel() {

    //the following statements place data that is relevant to the
    //internal workings of the simulator in the kernel data segment array
    //Address 100 is the beginning of the kernel data segment
    mem_addr kernel_pointer = 100;

    //user data segment begins at memory address 500 (0x000001f4 in hexadecimal)
    data_min = 500;
    kernel_data_seg[kernel_pointer] = data_min;
    kernel_pointer++;

    //user text segment begins at address 200 (0x000000c8 in hexadecimal)
    text_min = 200;
    kernel_data_seg[kernel_pointer] = text_min;
    kernel_pointer++;

    text_min_hex = 0x000000c8;
    kernel_data_seg[kernel_pointer] = text_min_hex;
    kernel_pointer++;

    data_min_hex = 0x000001f4;
    kernel_data_seg[kernel_pointer] = data_min_hex;
    kernel_pointer++;

    ProgramCounter = text_min;
    kernel_data_seg[kernel_pointer] = ProgramCounter;
    kernel_pointer++;

    data_current = data_min;
    kernel_data_seg[kernel_pointer] = data_current;
    kernel_pointer++;

    //The following statements put the accumulator machine opcodes
    //into the kernel text array. Address 0 is the beginning of the
    //kernel text segment.
    mem_addr kernel_text_pointer = 0;

    kernel_text_seg[kernel_text_pointer] = 0x81;
    kernel_text_pointer++;

    kernel_text_seg[kernel_text_pointer] = 0x82;
    kernel_text_pointer++;

    kernel_data_seg[kernel_text_pointer] = 0x83;
    kernel_text_pointer++;

    kernel_data_seg[kernel_text_pointer] = 0x84;
    kernel_text_pointer++;

    kernel_data_seg[kernel_text_pointer] = 0x85;
    kernel_text_pointer++;

    kernel_data_seg[kernel_text_pointer] = 0x86;
    kernel_text_pointer++;

    kernel_data_seg[kernel_text_pointer] = 0x90;
    kernel_text_pointer++;

    kernel_data_seg[kernel_text_pointer] = 0x87;
    kernel_text_pointer++;


}

void load(mem_addr x) {

accum = data_seg[x];

}

void load_immediate(mem_addr x) {
    accum = x;
}

void sto(mem_addr y) {

data_seg[y] = (mem_word) accum;

}

void add(mem_addr x) {

accum += data_seg[x];

}

void mult(mem_addr x) {

accum *= data_seg[x];

}

void print_string(mem_addr x) {

    x++;
    while (data_seg[x] != '\\') {

        load(x);

        cout << (char) accum;
        x++;
    }
}

void print_accum_value() {

    cout << accum;

}


void initial_load() {
fstream firststream("accumCode.txt");
fstream num_stream("accumCode.txt");
string data_type;
string token2;

string token;
mem_addr new_addr;
mem_word new_word;
int addr_from_stream;
int word_from_stream;
int n = 0;
char char_from_stream;

    while (firststream >> token) {  //firststream.eof()

        stringstream ss;
        stringstream s2;


        if (token.find(".text") != string::npos || token2.find(".text") != string::npos) {

            //call function for handling text segment
            load_text();
            return;

        }
        else if (token.find(".data") != string::npos || token2.find(".data") != string::npos) {

            continue;
        }
            ss << std::hex << token;
            ss >> addr_from_stream;
            new_addr = (mem_addr) addr_from_stream;


            firststream >> token2;
            char_from_stream = token2[0];

            s2 << token2;

            if (s2 >> word_from_stream) {
                new_word = (mem_word) word_from_stream;
            }
            else if (char_from_stream == '/' && n == 0){
                new_word = (mem_word) char_from_stream;
                string_begin1 = new_addr;
                n++;
            }
            else if (char_from_stream == '\\' && n==1) {
                new_word = (mem_word) char_from_stream;
                string_end1 = new_addr;
                n++;
            }
            else if (char_from_stream == '/' && n == 2){
                new_word = (mem_word) char_from_stream;
                string_begin2 = new_addr;
            }
            else if (char_from_stream == '\\' && n == 3) {
                new_word = (mem_word) char_from_stream;
                string_end2 = new_addr;
            }
            else {
                new_word = (mem_word) char_from_stream;
            }


            data_seg[new_addr] = new_word;

    }
}


void load_text() {

    ifstream sourceCode("accumCode.txt");
    string token;
    string token2;
    opcode new_opcode;
    mem_addr new_addr;

    while (sourceCode >> token) {

        if (token.find(".text") == string::npos) {


            continue;

        }
        else {
            break;
        }

    }

    while (sourceCode >> token) {
        int opcode_int;
        int addr_int;
        stringstream ss;
        stringstream s2;
        ss << std::hex << token;

        sourceCode >> token2;

        s2 << std::hex << token2;


        ss >> opcode_int;
        s2 >> addr_int;

        new_opcode = (opcode) opcode_int;
        new_addr = (mem_addr) addr_int;

        text_seg[ProgramCounter] = new_opcode;

        ProgramCounter++;
        text_seg[ProgramCounter] = new_addr;
        ProgramCounter++;

        switch(new_opcode) {

            case 0x81:
                load(new_addr);
                break;
            case 0x82:
                sto(new_addr);
                break;
            case 0x83:
                add(new_addr);
                break;
            case 0x84:
                mult(new_addr);
                break;
            case 0x85:
                print_string(new_addr);
                break;
            case 0x86:
                print_accum_value();
                break;
            case 0x90:
                load_immediate(new_addr);
                break;
            case 0x87:
                return;
            default:
                cout << "Invalid Opcode\n";
                break;
        }
    }
    return;
}


int main()
{

   //convertArrayTest();
    load_kernel();
    initial_load();
    //load_text();
    return 0;

}
