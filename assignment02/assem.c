/**
 * Program: assem.c
 * Purpose: A simple assembler that reads 16-bit binary instructions from standard input,
 *         decodes them into opcodes and operands, and prints the results in a human-readable format.
 * Author: Sean Balbale
 * Course: CPSC 275
 * Date: 10/10/2025
 */

#include <stdio.h>

unsigned short binstr2num(char *binStr);
unsigned short decode(unsigned short instr, unsigned short *opcode, unsigned short *opn);

/**
 * Function: binstr2num
 * Purpose: To convert a binary string to an unsigned short integer.
 * Parameters: char *binStr - A string representing a binary number (16 bits).
 * Returns: unsigned short result - The converted unsigned short integer.
 */
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

/**
 * Function: decode
 * Purpose: To decode a 16-bit instruction into its opcode and operand.
 * Parameters:
 *   unsigned short inStr - The 16-bit instruction to decode.
 *   unsigned short *opCode - Pointer to store the extracted opcode (bits 0-3).
 *   unsigned short *opn - Pointer to store the extracted operand (bits 5-15).
 * Returns: unsigned short m - The addressing mode (bit 4).
 */
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

/**
 * Function: main
 * Purpose: To read binary instructions from standard input, decode them, and print the results.
 * Returns: int - Exit status of the program.
 */
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
