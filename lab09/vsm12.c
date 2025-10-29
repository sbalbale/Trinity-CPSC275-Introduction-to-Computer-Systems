/**
 * Program: vsm12.c
 * Purpose: A simple assembler that reads 16-bit binary instructions from standard input,
 *         decodes them into opcodes and operands, and prints the results in a human-readable format.
 * Author: Sean Balbale
 * Course: CPSC 275
 * Date: 10/29/2025
 **/




#include <stdio.h>



// instruction cycle functions
void decode(unsigned short instr, unsigned short *opcode, unsigned short *opn);
unsigned short fetch(unsigned short memory[], unsigned short pc);
void execute(unsigned short cMem[], unsigned short dMem[], unsigned short inputs[], short *accumulator, unsigned short *pc, unsigned short *instructionRegister, unsigned short *opCode, unsigned short *operand, unsigned short *dataPointer);
void store(unsigned short memory[], unsigned short address, unsigned short value);
unsigned short load(unsigned short memory[], unsigned short address);

// helper functions
unsigned short binstr2num(char *binStr);
void loadInstructions(unsigned short memory[], unsigned short inputs[]);
void dump(unsigned short cMem[], unsigned short dMem[], short accumulator, unsigned short pc, unsigned short instructionRegister, unsigned short opCode, unsigned short operand);

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
    unsigned short cMem[512] = {0};      // Initialize code memory to 0
    unsigned short dMem[64] = {0};      // Initialize data memory to 0
    unsigned short dataPointer = 0;      // Pointer to current data location in dMem
	unsigned short inputs[64];

    loadInstructions(cMem, inputs);

dump(cMem, dMem, accumulator, pc, instructionRegister, opCode, opn);

    while (1)
    {
        execute(cMem, dMem, inputs, &accumulator, &pc, &instructionRegister, &opCode, &opn, &dataPointer);
        if (opCode == 0) // HALT
            break;

    }

    printf("\n\nRegisters and Memory Dump:\n");
    dump(cMem, dMem, accumulator, pc, instructionRegister, opCode, opn);

    return 0;
}

/**
 * Function: binstr2num
 * Purpose: To convert a binary string to an unsigned short integer.
 * Parameters: char *binStr - A string representing a binary number (12 bits).
 * Returns: unsigned short result - The converted unsigned short integer.
 */
unsigned short binstr2num(char *binStr)
{
    unsigned short result = 0;

    // Process each character of the string
    for (int i = 0; i < 12; i++)
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
 * Purpose: To decode a 12-bit instruction into its opcode and operand.
 * Parameters:
 *          unsigned short inStr - The 12-bit instruction to decode.
 *          unsigned short *opCode - Pointer to store the extracted opcode (bits 0-2).
 *          unsigned short *opn - Pointer to store the extracted operand (bits 3-11).
 */
void decode(unsigned short inStr, unsigned short *opCode, unsigned short *opn)
{
    // Extract the opcode (bits 0-2)
    *opCode = (inStr >> 9) & 0xF;

    // extract the operand (bits 7-15)
    *opn = inStr & 0x1FF;

}

/**
 * Function: fetch
 * Purpose: Fetch the instruction at the given program counter (pc) from memory.
 * Parameters:
 *          unsigned short pc - The program counter indicating which instruction to fetch.
 * Returns: unsigned short - The fetched instruction.
 */
unsigned short fetch(unsigned short memory[], unsigned short pc)
{
    // Reconstruct 12-bit instruction from two bytes
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
void execute(unsigned short cMem[], unsigned short dMem[], unsigned short inputs[], short *accumulator, unsigned short *pc, unsigned short *instructionRegister, unsigned short *opCode, unsigned short *operand, unsigned short *dataPointer)
{
    // Fetch the instruction at the current pc
    *instructionRegister = fetch(cMem, *pc);

    decode(*instructionRegister, opCode, operand);
    // Execute based on opcode
    switch (*opCode)
    {
    case 0: // HALT
        break;
    case 1: // LOAD
        *accumulator = *operand; // Use the operand value directly
	*pc += 2;
        break;
    case 2: // ADD
	*accumulator += *operand; // Use the operand value directly
	*pc += 2;
	break;
    case 3: // STORE
        store(dMem, (*operand-1)*2, *accumulator);
        *pc += 2;
        break;
    case 4: // LOADM
        *accumulator = load(dMem, (*operand-1)*2);
	*pc += 2;
        break;
    case 5: // ADDM
	*accumulator += load(dMem, (*operand-1)*2);
	*pc += 2;
	break;
    case 6: // READ
    {
        // Read a value from the dMem
        unsigned short input = inputs[*operand-1];
     	
     *dataPointer += 2; // Move to next data value 
     store(dMem, (*operand-1)*2, input);
        *pc += 2;
        break;
    }
    case 7: // WRITE
    {
        printf("Output: %hu\n", *accumulator);
        *pc += 2;
        break;
    }
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
 * Purpose: Load a 12-bit value from memory at the specified address (reads two bytes).
 *          unsigned short memory[] - The memory array.
 *          unsigned short address - The address to load from.
 * Returns: unsigned short - The 12-bit value reconstructed from two bytes.
 */
unsigned short load(unsigned short memory[], unsigned short address)
{
    unsigned short highByte = memory[address];
    unsigned short lowByte = memory[address + 1];
    return (highByte << 8) | lowByte;
}

/**
 * Function: loadInstructions
 * Purpose: Load the binary instructions from standard input, decode them, and save them to memory
 *          After instructions, load any data values into dMem.
 *          Instructions stop at EOC (opcode 0) or blank line.
 *          Data values (e.g., "10 20") are read and stored in cMem section.
 * Parameters: unsigned short memory[] - The memory array to store instructions.
 * Returns: void
 */
void loadInstructions(unsigned short memory[], unsigned short inputs[])
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

    // Load data values
    if (c != EOF)
    {
        unsigned short dataAddress = 0;

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
            if (hasDigit && dataAddress < 64)
            {
                inputs[dataAddress]=value;
		    dataAddress += 1; // Each value takes  bytes
            }
        }
    }
}

/**
 * Function: dump
 * Purpose: Print a memory dump showing registers and memory contents in hexadecimal format.
 *          CODE section shows memory , DATA section shows dMem.
 * Parameters:
 *   unsigned short memory[] - The memory array
 *   short accumulator - Current accumulator value
 *   unsigned short pc - Program counter value
 *   unsigned short instructionRegister - Instruction register value
 *   unsigned short opCode - Current opcode
 *   unsigned short operand - Current operand
 * Returns: void
 */
void dump(unsigned short memory[], unsigned short dMem[], short accumulator, unsigned short pc,
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
    for (unsigned short address = 0; address < 64; address += 10)
    {
        printf("%04d ", address);
        for (int i = 0; i < 10; i++)
        {
            if (address + i < 64) // Don't exceed address 1123
                printf("%02X ", dMem[address + i] & 0xFF);
        }
        printf("\n");
    }
    printf("...\n");
}
