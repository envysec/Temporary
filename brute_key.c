#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BUFLEN 50
#define COMMAND_BUFLEN 256
#define KEY_BUFLEN 256


typedef struct keys {
    char key_guess[DEFAULT_BUFLEN];
    char challenge1[DEFAULT_BUFLEN];
    char challenge2[DEFAULT_BUFLEN];
} key; 


key key_array[KEY_BUFLEN];


char* hex_bytes[] = {
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F", 
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", 
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", 
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", 
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F", 
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F", 
    "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F", 
    "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F", 
    "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F", 
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F", 
    "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF", 
    "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF", 
    "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF", 
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF", 
    "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF", 
    "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
};


void print_collisions(int key_index){
    int i;
    printf("%d\n", key_index);
    for (i = 0; i < key_index; i++){
        printf("=============\n");
        printf("Collision %d:\n", i);
        printf("Key Guess: %s\n", key_array[i].key_guess);
        printf("Challenge 1: 0x%s\n", key_array[i].challenge1);
        printf("Challenge 2: 0x%s\n", key_array[i].challenge2);
    }
}


int main(int argc, char* argv[]){
    if (!(argc == 4)){
        printf("Usage: %s [bitpos] [rand1] [rand2]\n", argv[0]);
        exit(0);
    }
    
    int bit_position = atoi(argv[1]);
    char cmd_prefix[DEFAULT_BUFLEN] = "";
    char cmd_suffix[DEFAULT_BUFLEN] = "";
    char padding[] = "00";
    
    int i, j;
    int key_index = 0;
    
    for (i = 0; i < bit_position; i++)
        strncat(cmd_prefix, padding, sizeof(cmd_prefix) - 1);

    for (i = 0; i < (7 - bit_position); i++)
        strncat(cmd_suffix, padding, sizeof(cmd_suffix) - 1);

    for (i = 0; i < sizeof(hex_bytes)/sizeof(hex_bytes[0]); i++){
        system("date\n");
        printf("%s\n", hex_bytes[i]);
        for (j = 0; j < sizeof(hex_bytes)/sizeof(hex_bytes[0]); j++){
            //./main 0x<rand> 0x<key> 
            char key_guess[DEFAULT_BUFLEN] = "0x";
            strncat(key_guess, cmd_prefix, sizeof(key_guess) - 1);
            strncat(key_guess, hex_bytes[i], sizeof(key_guess) - 1);
            strncat(key_guess, cmd_suffix, sizeof(key_guess) - 1);
            strncat(key_guess, cmd_prefix, sizeof(key_guess) - 1);
            strncat(key_guess, hex_bytes[j], sizeof(key_guess) - 1);
            strncat(key_guess, cmd_suffix, sizeof(key_guess) - 1);
            
            char command1[COMMAND_BUFLEN] = "./main 0x";
            strncat(command1, argv[2], sizeof(command1) - 1);
            strncat(command1, " 0x", 1);
            strncat(command1, key_guess, sizeof(command1) - 1);
            
            //printf("Getting Hash 1\n");
            FILE *cmd1= popen(command1, "r");
            char hash1[DEFAULT_BUFLEN] = {0x0};
            while (fgets(hash1, sizeof(hash1), cmd1) != NULL)
            pclose(cmd1);

            char command2[COMMAND_BUFLEN] = "./main 0x";
            strncat(command2, argv[3], sizeof(command1) - 1);
            strncat(command2, " 0x", 1);
            strncat(command2, key_guess, sizeof(command1) - 1);

            //printf("Getting Hash 2\n");
            FILE *cmd2 = popen(command2, "r");
            char hash2[DEFAULT_BUFLEN] = {0x0};
            while (fgets(hash2, sizeof(hash2), cmd2) != NULL)
            pclose(cmd2);

            if (!(strcmp(hash1, hash2))){
                printf("Collision %d:\n", i);
                printf("Key Guess: %s\n", key_guess);
                printf("Challenge 1: 0x%s\n", argv[2]);
                printf("Challenge 2: 0x%s\n", argv[3]);
            }
            if (!(strcmp(hash1, hash2))){
                key *key_struct = &key_array[key_index];
                strncpy(key_struct->key_guess, key_guess, sizeof(key_struct->key_guess));
                strncpy(key_struct->challenge1, argv[2], sizeof(key_struct->challenge1));
                strncpy(key_struct->challenge2, argv[3], sizeof(key_struct->challenge2));
                key_index++;
            }
        }
    }
    print_collisions(key_index);
}

/*
08000000000000004700000000000000
865D0C1E930121B722A5F800
E2000000000000002D00000000000000
865D0C1E930121B722A5F800
./brute 0 08000000000000004700000000000000 E2000000000000002D00000000000000
*/