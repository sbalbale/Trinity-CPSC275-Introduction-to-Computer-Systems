/**
 * Program: assem.c
 * Purpose: A simple assembler that reads 16-bit binary instructions from standard input,
 *         decodes them into opcodes and operands, and prints the results in a human-readable format.
 * Author: Sean Balbale
 * Course: CPSC 275
 * Date: 10/20/2025
 */

#include <stdio.h>

// instruction cycle functions
unsigned short decode(unsigned short instr, unsigned short *opcode, unsigned short *opn);
unsigned short fetch(unsigned short memory[], unsigned short pc);
void execute(unsigned short memory[], short *accumulator, unsigned short *pc, unsigned short *instructionRegister, unsigned short *opCode, unsigned short *operand, unsigned short *dataPointer);
void store(unsigned short memory[], unsigned short address, unsigned short value);
unsigned short load(unsigned short memory[], unsigned short address);

// helper functions
unsigned short binstr2num(char *binStr);
void loadInstructions(unsigned short memory[]);
void dump(unsigned short memory[], short accumulator, unsigned short pc, unsigned short instructionRegister, unsigned short opCode, unsigned short operand);

/**
 * Function: main
 * Purpose: To read binary instructions from standard input, decode them, and print the results.
 * Returns: int - Exit status of the program.
 */
int main()
{
    unsigned short opCode, opn;
    short accumulator = 0;                  // Initialize accumulator to 0
    unsigned short pc = 0;                  // Initialize program counter to 0
    unsigned short instructionRegister = 0; // Initialize instruction register to 0
    unsigned short memory[2048] = {0};      // Initialize memory to 0
    unsigned short dataPointer = 1024;      // Pointer to current data location in DATA section

    loadInstructions(memory);

    while (1)
    {
        execute(memory, &accumulator, &pc, &instructionRegister, &opCode, &opn, &dataPointer);
        if (opCode == 0 || opCode == 15) // EOC or HALT
            break;
       
    }
    
    printf("\n\nRegisters and Memory Dump:\n");
    dump(memory, accumulator, pc, instructionRegister, opCode, opn);

    return 0;
}

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
 *          unsigned short inStr - The 16-bit instruction to decode.
 *          unsigned short *opCode - Pointer to store the extracted opcode (bits 0-3).
 *          unsigned short *opn - Pointer to store the extracted operand (bits 5-15).
 * Returns: unsigned short m - The addressing mode (bit 4).
 */
unsigned short decode(unsigned short inStr, unsigned short *opCode, unsigned short *opn)
{
    // Extract the opcode (bits 0-3)
    *opCode = (inStr >> 12) & 0xF;

    // extract m (bit 4)
    unsigned short m = (inStr >> 11) & 0x1;

    // extract the operand (bits 5-15)
    *opn = inStr & 0x7FF;

    return m;
}

/**
 * Function: fetch
 * Purpose: Fetch the instruction at the given program counter (pc) from memory.
 * Parameters:
 *          unsigned short memory[] - The memory array containing instructions.
 *          unsigned short pc - The program counter indicating which instruction to fetch.
 * Returns: unsigned short - The fetched instruction.
 */
unsigned short fetch(unsigned short memory[], unsigned short pc)
{
    // Reconstruct 16-bit instruction from two bytes
    unsigned short highByte = memory[pc];
    unsigned short lowByte = memory[pc + 1];
    return (highByte << 8) | lowByte;
}

/**
 * Function: execute
 * Purpose: Execute the instruction pointed to by the program counter (pc). Stores data in memory if needed.
 * Parameters:
 *          unsigned short memory[] - The memory array containing instructions and data.
 *          short *accumulator - Pointer to the accumulator register.
 *          unsigned short *pc - Pointer to the program counter.
 *          unsigned short *instructionRegister - Pointer to the instruction register.
 *          unsigned short *opCode - Pointer to the opcode.
 *          unsigned short *operand - Pointer to the operand.
 *          unsigned short *dataPointer - Pointer to the current data location for READ operations.
 * Returns: void
 */
void execute(unsigned short memory[], short *accumulator, unsigned short *pc, unsigned short *instructionRegister, unsigned short *opCode, unsigned short *operand, unsigned short *dataPointer)
{
    // Fetch the instruction at the current pc
    *instructionRegister = fetch(memory, *pc);

    // Decode the instruction to get opcode and operand
    unsigned short m = decode(*instructionRegister, opCode, operand);

    // Execute based on opcode
    switch (*opCode)
    {
    case 0: // EOC
        *pc += 2;
        break;
    case 1: // LOAD
        if (m == 0) // direct addressing
            *accumulator = load(memory, *operand);
        else // m == 1, immediate addressing
            *accumulator = *operand; // Use the operand value directly
        *pc += 2;
        break;
    case 2: // STORE
        if (m == 0) // direct addressing
            store(memory, *operand, *accumulator);
        else // indirect addressing
            store(memory, load(memory, *operand), *accumulator);
        *pc += 2;
        break;
    case 3: // READ
    {
        // Read a value from the pre-loaded data section
        unsigned short input = load(memory, *dataPointer);
        *dataPointer += 2; // Move to next data value
        
        if (m == 0) // direct addressing
            store(memory, *operand, input);
        else // indirect addressing
            store(memory, load(memory, *operand), input);
        *pc += 2;
        break;
    }
    case 4: // WRITE
    {
        // Write (output) the value from memory
        if (m == 0) // direct addressing
            printf("%hu\n", load(memory, *operand));
        else // indirect addressing
            printf("%hu\n", load(memory, load(memory, *operand)));
        *pc += 2;
        break;
    }
    case 5: // ADD
        if (m == 0) // direct addressing
            *accumulator += load(memory, *operand);
        else // m == 1, immediate addressing
            *accumulator += *operand; // Use the operand value directly
        *pc += 2;
        break;
    case 6: // SUB
        if (m == 0) // direct addressing
            *accumulator -= load(memory, *operand);
        else // m == 1, immediate addressing
            *accumulator -= *operand; // Use the operand value directly
        *pc += 2;
        break;
    case 7: // MUL
        if (m == 0) // direct addressing
            *accumulator *= load(memory, *operand);
        else // m == 1, immediate addressing
            *accumulator *= *operand; // Use the operand value directly
        *pc += 2;
        break;
    case 8: // DIV
        if (m == 0) // direct addressing
        {
            unsigned short divisor = load(memory, *operand);
            if (divisor != 0)
                *accumulator /= divisor;
            else
                printf("Error: Division by zero\n");
        }
        else // m == 1, immediate addressing
        {
            unsigned short divisor = *operand; // Use the operand value directly
            if (divisor != 0)
                *accumulator /= divisor;
            else
                printf("Error: Division by zero\n");
        }
        *pc += 2;
        break;
    case 9: // MOD
        if (m == 0) // direct addressing
        {
            unsigned short divisor = load(memory, *operand);
            if (divisor != 0)
                *accumulator %= divisor;
            else
                printf("Error: Division by zero\n");
        }
        else // m == 1, immediate addressing
        {
            unsigned short divisor = *operand; // Use the operand value directly
            if (divisor != 0)
                *accumulator %= divisor;
            else
                printf("Error: Division by zero\n");
        }
        *pc += 2;
        break;
    case 10: // NEG
        *accumulator = -(*accumulator);
        *pc += 2;
        break;
    case 11: // NOP
        *pc += 2;
        break;
    case 12: // JUMP
        *pc = *operand;
        break;
    case 13: // JNEG
        if (*accumulator < 0)
            *pc = *operand; // Jump is taken
        else
            *pc += 2; // Jump is not taken, increment normally
        break;
    case 14: // JZERO
        if (*accumulator == 0)
            *pc = *operand; // Jump is taken
        else
            *pc += 2; // Jump is not taken, increment normally
        break;
    case 15: // HALT
        break;
    default:
        printf("Error: Unknown opcode %hu\n", *opCode);
        *pc += 2;
        break;
    }
}

/** Function: store
 * Purpose: Store a value in memory at the specified address as two bytes.
 *          unsigned short memory[] - The memory array.
 *          unsigned short address - The address to store the value.
 *          unsigned short value - The value to store.
 */
void store(unsigned short memory[], unsigned short address, unsigned short value)
{
    memory[address] = (value >> 8) & 0xFF;  // High byte
    memory[address + 1] = value & 0xFF;      // Low byte
}

/** Function: load
 * Purpose: Load a 16-bit value from memory at the specified address (reads two bytes).
 *          unsigned short memory[] - The memory array.
 *          unsigned short address - The address to load from.
 * Returns: unsigned short - The 16-bit value reconstructed from two bytes.
 */
unsigned short load(unsigned short memory[], unsigned short address)
{
    unsigned short highByte = memory[address];
    unsigned short lowByte = memory[address + 1];
    return (highByte << 8) | lowByte;
}

/**
 * Function: loadInstructions
 * Purpose: Load the binary instructions from standard input, decode them, and save them to memory from 0000 to 1023.
 *          After instructions, load any data values into memory starting at address 1024.
 *          Instructions stop at EOC (opcode 0) or blank line.
 *          Data values (e.g., "10 20") are read and stored in DATA section.
 * Parameters: unsigned short memory[] - The memory array to store instructions.
 * Returns: void
 */
void loadInstructions(unsigned short memory[])
{
    unsigned short opCode, opn;
    unsigned short address = 0; // Initialize memory address to 0
    char c;
    int foundEOC = 0;

    // Read the first character
    c = getchar();

    // Load instructions (CODE section)
    while (c != EOF && address < 1024 && !foundEOC)
    {
        // 16 bits and null terminator
        char line[17];
        int count = 0;

        // read each line - first character is already in c
        while (c != '\n' && c != EOF)
        {
            line[count++] = c;
            c = getchar();
        }

        // add null terminator
        line[count] = '\0';

        // Check for empty line (signals end of instructions)
        if (count == 0)
        {
            foundEOC = 1;
        }
        else
        {
            // convert binary string to number and decode it
            unsigned short num = binstr2num(line); // binary string to unsigned short
            decode(num, &opCode, &opn);        // decode the instruction

            // Store the instruction as two bytes (high byte, then low byte)
            memory[address] = (num >> 8) & 0xFF;     // High byte
            memory[address + 1] = num & 0xFF;        // Low byte
            address += 2; // Increment address by 2 (each instruction uses 2 bytes)

            // Check for EOC instruction (opcode 0)
            if (opCode == 0)
            {
                foundEOC = 1;
            }
        }

        // Read the first character of the next line
        if (c == '\n')
        {
            c = getchar();
        }
    }

    // Load data values (DATA section starting at address 1024)
    if (c != EOF)
    {
        unsigned short dataAddress = 1024;
        
        // Read remaining input as data values
        while (c != EOF)
        {
            // Skip whitespace
            while (c == ' ' || c == '\t' || c == '\n')
            {
                c = getchar();
                if (c == EOF) break;
            }
            
            if (c == EOF) break;
            
            // Read a number
            unsigned short value = 0;
            int hasDigit = 0;
            
            while (c >= '0' && c <= '9')
            {
                value = value * 10 + (c - '0');
                hasDigit = 1;
                c = getchar();
            }
            
            // Store the value if we read digits
            if (hasDigit && dataAddress < 2048)
            {
                store(memory, dataAddress, value);
                dataAddress += 2; // Each value takes 2 bytes
            }
        }
    }
}

/**
 * Function: dump
 * Purpose: Print a memory dump showing registers and memory contents in hexadecimal format.
 *          CODE section shows memory from 0-1023, DATA section shows memory from 1024-2047.
 * Parameters:
 *   unsigned short memory[] - The memory array
 *   short accumulator - Current accumulator value
 *   unsigned short pc - Program counter value
 *   unsigned short instructionRegister - Instruction register value
 *   unsigned short opCode - Current opcode
 *   unsigned short operand - Current operand
 * Returns: void
 */
void dump(unsigned short memory[], short accumulator, unsigned short pc,
          unsigned short instructionRegister, unsigned short opCode, unsigned short operand)
{
    // Print registers
    printf("\nREGISTERS:\n");
    printf("accumulator                 0x%04X\n", (unsigned short) accumulator);
    printf("instructionCounter          0x%04X\n", pc);
    printf("instructionRegister         0x%04X\n", instructionRegister);
    printf("opCode                      0x%X\n", opCode);
    printf("operand                     0x%04X\n", operand);

    // Print CODE section (first 100 bytes, addresses 0-99)
    printf("\nCODE:\n");
    printf("     0  1  2  3  4  5  6  7  8  9\n");
    for (unsigned short address = 0; address < 100; address += 10)
    {
        printf("%04d ", address);
        for (int i = 0; i < 10; i++)
        {
            if (address + i < 100) // Don't exceed byte 99
                printf("%02X ", memory[address + i] & 0xFF);
        }
        printf("\n");
    }

    // Print DATA section (first 100 bytes, addresses 1024-1123)
    printf("...\n\nDATA:\n");
    printf("     0  1  2  3  4  5  6  7  8  9\n");
    for (unsigned short address = 1024; address < 1124; address += 10)
    {
        printf("%04d ", address);
        for (int i = 0; i < 10; i++)
        {
            if (address + i < 1124) // Don't exceed address 1123
                printf("%02X ", memory[address + i] & 0xFF);
        }
        printf("\n");
    }
    printf("...\n");
}
