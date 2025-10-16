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
void execute(unsigned short memory[], short *accumulator, unsigned short *pc, unsigned short *instructionRegister, unsigned short *opCode, unsigned short *operand);
void store(unsigned short memory[], unsigned short address, unsigned short value);
unsigned short load(unsigned short memory[], unsigned short address);

// helper functions
unsigned short binstr2num(char *binStr);
void loadInstructions(unsigned short memory[]);
void printInstructions(unsigned short memory[]);
void printData(unsigned short memory[]);
void dump(unsigned short memory[], short accumulator, unsigned short pc, unsigned short instructionRegister, unsigned short opCode, unsigned short operand);

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
 * Returns: void
 */
void execute(unsigned short memory[], short *accumulator, unsigned short *pc, unsigned short *instructionRegister, unsigned short *opCode, unsigned short *operand)
{
    // Fetch the instruction at the current pc
    *instructionRegister = fetch(memory, *pc);

    // Decode the instruction to get opcode and operand
    unsigned short m = decode(*instructionRegister, opCode, operand);

    // Increment pc to point to the next instruction (2 bytes per instruction)
    *pc += 2;

    // Execute based on opcode
    switch (*opCode)
    {
    case 0: // EOC
        break;
    case 1: // LOAD
        if (m == 0) // direct addressing
            *accumulator = load(memory, *operand);
        else // indirect addressing
            *accumulator = load(memory, load(memory, *operand));
        break;
    case 2: // STORE
        if (m == 0) // direct addressing
            store(memory, *operand, *accumulator);
        else // indirect addressing
            store(memory, load(memory, *operand), *accumulator);
        break;
    case 3: // READ
    {
        // Read a value into memory
        unsigned short input;
        printf("Input a value: ");
        fflush(stdout); // Ensure prompt is printed before input
        
        if (scanf("%hu", &input) == 1) {
            if (m == 0) // direct addressing
                store(memory, *operand, input);
            else // indirect addressing
                store(memory, load(memory, *operand), input);
        } else {
            printf("Error: Invalid input\n");
            // Clear input buffer on error
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        break;
    }
    case 4: // WRITE
    {
        // Write (output) the value from memory
        if (m == 0) // direct addressing
            printf("Output: %hu\n", load(memory, *operand));
        else // indirect addressing
            printf("Output: %hu\n", load(memory, load(memory, *operand)));
        break;
    }
    case 5: // ADD
        if (m == 0) // direct addressing
            *accumulator += load(memory, *operand);
        else // indirect addressing
            *accumulator += load(memory, load(memory, *operand));
        break;
    case 6: // SUB
        if (m == 0) // direct addressing
            *accumulator -= load(memory, *operand);
        else // indirect addressing
            *accumulator -= load(memory, load(memory, *operand));
        break;
    case 7: // MUL
        if (m == 0) // direct addressing
            *accumulator *= load(memory, *operand);
        else // indirect addressing
            *accumulator *= load(memory, load(memory, *operand));
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
        else // indirect addressing
        {
            unsigned short divisor = load(memory, load(memory, *operand));
            if (divisor != 0)
                *accumulator /= divisor;
            else
                printf("Error: Division by zero\n");
        }
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
        else // indirect addressing
        {
            unsigned short divisor = load(memory, load(memory, *operand));
            if (divisor != 0)
                *accumulator %= divisor;
            else
                printf("Error: Division by zero\n");
        }
        break;
    case 10: // NEG
        *accumulator = -(*accumulator);
        break;
    case 11: // NOP
        break;
    case 12: // JUMP
        *pc = *operand;
        break;
    case 13: // JNEG
        if (*accumulator < 0)
            *pc = *operand;
        break;
    case 14: // JZERO
        if (*accumulator == 0)
            *pc = *operand;
        break;
    case 15: // HALT
        break;
    default:
        printf("Error: Unknown opcode %hu\n", *opCode);
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
 *          Should stop loading when EOF is reached or when memory is full.
 *          Each instruction takes 1 memory location (2 bytes).
 * Parameters: unsigned short memory[] - The memory array to store instructions.
 * Returns: void
 */
void loadInstructions(unsigned short memory[])
{
    unsigned short opCode, opn;
    unsigned short m;
    unsigned short address = 0; // Initialize memory address to 0
    char c;

    // Read the first character
    c = getchar();

    while (c != EOF && address < 1024)
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

        // Skip empty lines
        if (count > 0)
        {
            // convert binary string to number and decode it
            unsigned short num = binstr2num(line); // binary string to unsigned short
            m = decode(num, &opCode, &opn);        // decode the instruction

            // Store the instruction as two bytes (high byte, then low byte)
            memory[address] = (num >> 8) & 0xFF;     // High byte
            memory[address + 1] = num & 0xFF;        // Low byte
            address += 2; // Increment address by 2 (each instruction uses 2 bytes)
        }

        // Read the first character of the next line
        if (c == '\n')
        {
            c = getchar();
        }
    }
}

/**
 * Function: printInstructions
 * Purpose: Print the contents of memory from address 0000 to 1023 in binary format.
 *          Should also print the address in hex, the mnemonic, and the operand.
 * Parameters: unsigned short memory[] - The memory array containing instructions.
 * Returns: void
 */
void printInstructions(unsigned short memory[])
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
    unsigned short opCode, opn;
    unsigned short m;

    for (unsigned short address = 0; address < 1024; address++)
    {
        unsigned short instruction = memory[address];
        if (instruction == 0)
            continue; // Skip empty memory locations

        m = decode(instruction, &opCode, &opn); // decode the instruction

        // print the address in hex, the instruction in binary, the mnemonic, and the operand
        printf("%04X ", address);

        // Print the instruction in binary format
        for (int i = 15; i >= 0; i--)
        {
            putchar((instruction & (1 << i)) ? '1' : '0');
        }

        printf(" %s %04u\n", mnemonic[opCode], opn);
    }
}

/**
 * Function: printData
 * Purpose: Print a dump of the data section (memory addresses 1024-2047).
 *          Should print the address in hex and data values in decimal format.
 * Parameters: unsigned short memory[] - The memory array containing data.
 * Returns: void
 */
void printData(unsigned short memory[])
{
    printf("Address (Dec)  Value (Hex)  Value (Dec)\n");
    for (unsigned short address = 1024; address < 2048; address++)
    {
        if (memory[address] != 0) // Only print non-zero values
        {
            printf("%04d           0x%04X       %05u\n", address, memory[address], memory[address]);
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
    printf("accumulator                 0x%04X\n", accumulator);
    printf("instructionCounter          0x%04X\n", pc);
    printf("instructionRegister         0x%04X\n", instructionRegister);
    printf("opCode                      0x%X\n", opCode);
    printf("operand                     0x%04X\n", operand);

    // Print CODE section (addresses 0-1023)
    printf("\nCODE:\n");
    printf("     0  1  2  3  4  5  6  7  8  9\n");
    for (unsigned short address = 0; address < 1024; address += 10)
    {
        printf("%04d ", address);
        for (int i = 0; i < 10; i++)
        {
            if (address + i < 1024) // Don't exceed address 1023
                printf("%02X ", memory[address + i] & 0xFF);
        }
        printf("\n");
    }

    // Print DATA section (addresses 1024-2047)
    printf("\nDATA:\n");
    printf("     0  1  2  3  4  5  6  7  8  9\n");
    for (unsigned short address = 1024; address < 2048; address += 10)
    {
        printf("%04d ", address);
        for (int i = 0; i < 10; i++)
        {
            if (address + i < 2048) // Don't exceed address 2047
                printf("%02X ", memory[address + i] & 0xFF);
        }
        printf("\n");
    }
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
    unsigned short opCode, opn;
    unsigned short m;
    short accumulator = 0;                  // Initialize accumulator to 0
    unsigned short pc = 0;                  // Initialize program counter to 0
    unsigned short instructionRegister = 0; // Initialize instruction register to 0
    unsigned short memory[2048] = {0};      // Initialize memory to 0

    loadInstructions(memory);

    // Reopen stdin to read user input from the terminal
    #ifdef _WIN32
        freopen("CON", "r", stdin);  // Windows
    #else
        freopen("/dev/tty", "r", stdin);  // Linux, Mac, WSL
    #endif

    while (1)
    {
        execute(memory, &accumulator, &pc, &instructionRegister, &opCode, &opn);
        if (opCode == 0 || opCode == 15) // EOC or HALT
            break;
       
    }
    
    printf("\n\nRegisters and Memory Dump:\n");
    dump(memory, accumulator, pc, instructionRegister, opCode, opn);

    return 0;
}
