/**
 * Assem.c
 * Sean Balbale
 * CPSC 275
 */

#include <stdio.h>

unsigned short binstr2num(char *binStr);
unsigned short decode(unsigned short instr, unsigned short *opcode, unsigned short *opn);

// Turn 15 bit binary string into an unsigned short
unsigned short binstr2num(char *binStr)
{
    unsigned short result = 0;

    // Process each character of the string
    for (int i = 0; i < 16; i++)
    {
        // shift left by 1 to make space for the next bit
        result = result << 1;
        if (binStr[i] == '1')
        {
            // set the least significant bit to 1 using bitwise OR
            result = result | 1;
        }
    }
    return result;
}

// Decode the instruction into opcode and operand
// Return m (0 for memory address, 1 for constant)
unsigned short decode(unsigned short inStr, unsigned short *opCode, unsigned short *opn)
{
    // Extract the opcode (bits 0-3)
    *opCode = (inStr >> 12) & 0xF;

    // extract m (bit 4)
    unsigned short m = (inStr >> 11) & 0x1;

    // if m is 0, the operand is a memory address
    if (m == 0)
    {
        // extract the operand
        *opn = inStr & 0x7FF; // bits 5-15
    }
    // if m is 1, the operand is an constant
    else
    {
        // extract the operand
        *opn = inStr & 0x7FF; // bits 5-15
    }

    return m;
}

int main()
{
    // array of mnemonics for opcodes 0-15
    char mnemonic[16][10] = {
        "EOC  ",
        "LOAD ",
        "STORE",
        "READ ",
        "WRITE",
        "ADD  ",
        "SUB  ",
        "MUL  ",
        "DIV  ",
        "MOD  ",
        "NEG  ",
        "NOP  ",
        "JUMP ",
        "JNEG ",
        "JZERO",
        "HALT "};
    char c = getchar();
    unsigned short opCode, opn;

    while (c != EOF)
    {
        // 16 bits and null terminator
        char line[17];
        int count = 0;

        // read each line
        while (c != '\n' && c != EOF)
        {
            line[count++] = c;
            c = getchar();
        }

        // add null terminator
        line[count] = '\0';

        // convert binary string to number and decode it
        unsigned short num = binstr2num(line);         // binary string to unsigned short
        unsigned short m = decode(num, &opCode, &opn); // decode the instruction

        // print the number in hex, the mnemonic, and the operand
        printf("%04X %s %04u\n", num, mnemonic[opCode], opn);
        c = getchar();
    }

    return 0;
}
