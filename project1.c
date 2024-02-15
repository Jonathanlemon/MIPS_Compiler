#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//This returns a number associated with the instruction, or a -1 if it is not a valid instruction
//1-5 is R type: add, sub, and, or, slt
//6-8 is I type: lw, sw, beq
//9 is J type: j
int checkInstruction(char* buf){
    //R Types
    if(*(buf) == 'a' && *(buf+1) == 'd' && *(buf+2) == 'd' && *(buf+3) == '\0'){
        return 1;
    }
    if(*(buf) == 's' && *(buf+1) == 'u' && *(buf+2) == 'b' && *(buf+3) == '\0'){
        return 2;
    }
    if(*(buf) == 'a' && *(buf+1) == 'n' && *(buf+2) == 'd' && *(buf+3) == '\0'){
        return 3;
    }
    if(*(buf) == 'o' && *(buf+1) == 'r' && *(buf+2) == '\0'){
        return 4;
    }
    if(*(buf) == 's' && *(buf+1) == 'l' && *(buf+2) == 't' && *(buf+3) == '\0'){
        return 5;
    }

    //I Types
    if(*(buf) == 'l' && *(buf+1) == 'w' && *(buf+2) == '\0'){
        return 6;
    }
    if(*(buf) == 's' && *(buf+1) == 'w' && *(buf+2) == '\0'){
        return 7;
    }
    if(*(buf) == 'b' && *(buf+1) == 'e' && *(buf+2) == 'q' && *(buf+3) == '\0'){
        return 8;
    }

    //J Types
    if(*(buf) == 'j'&& *(buf+1) == '\0'){
        return 9;
    }

    //Invalid
    return -1;
};

//Sets the operation code
void setOpCode(char* code, int instructionType){
    memset(code, '0', 6);
    if(1 <= instructionType && instructionType <= 5){
        //R Type
        return;
    }
    switch(instructionType){
        case 6: //lw 100011
            *(code) = '1';
            *(code+4) = '1';
            *(code+5) = '1';
            break;
        case 7: //sw 101011
            *(code) = '1';
            *(code+2) = '1';
            *(code+4) = '1';
            *(code+5) = '1';
            break;
        case 8: //beq 000100
            *(code+3) = '1';
            break;
        case 9: //j 000010
            *(code+4) = '1';
            break;
        default:
            break;
    }
    return;
}

//Only for RType to set the function code
void setFunction(char* code, int instruction){
    switch(instruction){
        case 1:
            *(code+26) = '1';
            break;
        case 2:
            *(code+26) = '1';
            *(code+30) = '1';
            break;
        case 3:
            *(code+26) = '1';
            *(code+29) = '1';
            break;
        case 4:
            *(code+26) = '1';
            *(code+29) = '1';
            *(code+31) = '1';
            break;
        case 5:
            *(code+26) = '1';
            *(code+28) = '1';
            *(code+30) = '1';
            break;
        default:
            break;
    }
    return;
}

//Check if register token is valid and return the numeric value
int checkRegister(char* token){
    if(*token != '$'){
        return -1;
    }
    if(*(token+1)!= '1' && *(token+1)!= '2' && *(token+1)!= '3' && *(token+1)!= '4' && *(token+1)!= '5' && *(token+1)!= '6' && *(token+1)!= '7' && *(token+1)!= '8' && *(token+1)!= '9'  && *(token+1)!= '0'){
        return -1;
    }
    if(*(token+2)!= '1' && *(token+2)!= '2' && *(token+2)!= '3' && *(token+2)!= '4' && *(token+2)!= '5' && *(token+2)!= '6' && *(token+2)!= '7' && *(token+2)!= '8' && *(token+2)!= '9' && *(token+2)  != '0' && *(token+2)  != ',' && *(token+2)!= '\0' && *(token+2)!= '\n'){
        return -1;
    }
    if(*(token+2) != ',' && *(token+2) != '\n' && *(token+2) != '\0'){
        //2 Digits
        if(*(token+1) != '1' && *(token+1) != '2' && *(token+1) != '3'){
            return -1;
        }
        if(*(token+1) == '3'){
            if(*(token+2) != '1'){
                return -1;
            }
        }
    }
    //Valid
    char buf[2];
    buf[0] = *(token+1);
    buf[1] = *(token+2);
    return atoi(buf);
}


//Set $RS and $RT registers
int setRegisters(char* code, char* rs, char* rt){
    int r1 = checkRegister(rs);
    //Get binary representation of each token (numeric)
        char bitsRs[6];  // 5 bits + null terminator
        if (r1 < 0 || r1 > 31) {
            return -1;
        }
        for (int i = 4; i >= 0; i--) {
            bitsRs[i] = (r1 % 2) + '0';
            r1 /= 2;
        }
        bitsRs[5] = '\0';  // Null terminator

        r1 = checkRegister(rt);

        char bitsRt[6];  // 5 bits + null terminator
        if (r1 < 0 || r1 > 31) {
            return -1;
        }
        for (int i = 4; i >= 0; i--) {
            bitsRt[i] = (r1 % 2) + '0';
            r1 /= 2;
        }
        bitsRt[5] = '\0';  // Null terminator

    for(int i=0; i<5; i++){
        if(*(bitsRs+i) == '1'){
            *(code + 6 + i) = '1';
        }
    }
    for(int i=0; i<5; i++){
        if(*(bitsRt+i) == '1'){
            *(code + 11 + i) = '1';
        }
    }
    
    return 0;
}

//Set destination register
int setDestination(char* code, char* rd){
    int r1 = checkRegister(rd);
    //Get binary representation of each token (numeric)
        char bitsRd[6];  // 5 bits + null terminator
        if (r1 < 0 || r1 > 31) {
            return -1;
        }
        for (int i = 4; i >= 0; i--) {
            bitsRd[i] = (r1 % 2) + '0';
            r1 /= 2;
        }
        bitsRd[5] = '\0';  // Null terminator

    for(int i=0; i<5; i++){
        if(*(bitsRd+i) == '1'){
            *(code + 16 + i) = '1';
        }
    }

    return 0;
}

//Writes bits to code from an immediate stored in token for length amount of bits
void setImmediate(char* code, char* tok, int length){
    int value = atoi(tok);

    char* bits = malloc(sizeof(char)*(length+1));
    bits[length] = '\0';

    if(value < 0){
        value = pow(2, length) + value;
    }
    for (int i = length-1; i >= 0; i--) {
        bits[i] = (value % 2) + '0';
        value /= 2;
    }
    for(int i=0; i<length; i++){
        if(*(bits+i) == '1'){
            *(code + 32 - length + i) = '1';
        }
    }
}

char getCharacter(char* code){
    int value = 0;
    for(int i = 7; i >= 0; i--){
        if(*(code + i) == '1'){
            value += pow(2, (7-i));
        }
    }
    char x = value;
    return x;
}

int main(int argc, char *argv[]) {
    // Check if correct number of command line arguments are provided
    if (argc != 3) {
        printf("Usage: %s <input_file.txt> <output_file>\n", argv[0]);
        return 1; // Exit with an error code
    }

    // Open the input file for reading
    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1; // Exit with an error code
    }

    // Open the output file for writing
    FILE *binaryFile = fopen(argv[2], "w");
    if (binaryFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        return 1; // Exit with an error code
    }




    // Sample: Read from input file and write to output file
    char buffer[1024];

    char* token1;
    char* token2;
    char* token3;
    char* token4;
    char* token5;

    char code[33];

    int valid = 1;
    int lineCount = 0;

    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
        lineCount++;
        memset(code, '0', 32);

        token1 = strtok(buffer, " ");
        token2 = strtok(NULL, " ");
        token3 = strtok(NULL, " ");
        token4 = strtok(NULL, " ");
        token5 = strtok(NULL, " ");

        if(token5 != NULL){
            printf("Error, invalid formatting. Line %d", lineCount);
            valid = 0;
            continue;
        }

        //Check for valid instruction
        int instructionType = checkInstruction(token1);
        if(instructionType < 1){
            printf("Error, invalid instruction. Line %d", lineCount);
            valid = 0;
            continue;
        }

        //Set the operation code
        setOpCode(code, instructionType);

        //R Type - Set the $RS and $RT registers along with function code
        if(1 <= instructionType && instructionType <= 5){
            setFunction(code, instructionType);
            if(setRegisters(code, token3, token4) == -1){
                printf("Error, invalid register. Line %d", lineCount);
                valid = 0;
                continue;
            }
            if(setDestination(code, token2) == -1){
                printf("Error, invalid register. Line %d", lineCount);
                valid = 0;
                continue;
            }
        }

        //I Type (No example shown on project pdf so I'm not sure how to handle)
        else if(6 <= instructionType && instructionType <= 8){
            //If the instruction is beq, handle it separately. Set the rs and rt registers and set immediate
            if(instructionType == 8){
                if(setRegisters(code, token2, token3) == -1){
                    printf("Error, invalid register. Line %d", lineCount);
                    valid = 0;
                    continue;
                }
                setImmediate(code, token4, 16);
            }
            //LW and SW
            else{
                char* immediateTok = strtok(token3, "(");
                char* rsTok = strtok(NULL, ")");
                if(setRegisters(code, rsTok, token2) == -1){
                    printf("Error, invalid register. Line %d", lineCount);
                    valid = 0;
                    continue;
                }
                setImmediate(code, immediateTok, 16);
            }
        }

        //J Type
        else{
            setImmediate(code, token2, 26);
        }

        //Only write to file if the input file was valid
        if(valid == 1){
            //Convert the char[32] array of '1' and '0' to 4 chars (1 byte each*4 = 32 bits) and binary write
            char a = getCharacter(code);
            char b = getCharacter(code+8);
            char c = getCharacter(code+16);
            char d = getCharacter(code+24);
            fwrite(&a, 1, 1, binaryFile);
            fwrite(&b, 1, 1, binaryFile);
            fwrite(&c, 1, 1, binaryFile);
            fwrite(&d, 1, 1, binaryFile);
        }
    }

    // Close the opened files
    fclose(inputFile);

    //NOTE*** FClose is intentionally commented out. After testing with XXD to hexdump my binary, FCLOSE was adding extra data onto the end of the file (presumably data regarding the file being closed.)
    //I commented it out so that the only data in the hexdump is the code from my compiler
    //fclose(binaryFile);
    return 0; // Exit successfully
}