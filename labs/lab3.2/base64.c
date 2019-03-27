
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <signal.h>

#define SIGINFO 29
//CODE FROM---------------------------------------------------------------------
//https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};
float percentageDone = 0;

void build_decoding_table() {

    decoding_table = malloc(256);
    int i;
    for (i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(*output_length);
    if (encoded_data == NULL) return NULL;
    int i, j;
    for (i = 0, j = 0; i < input_length;) {
        percentageDone = (float) i / (float) input_length * 100;
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {
    if (decoding_table == NULL) build_decoding_table();
    //if (input_length % 4 != 0) return NULL;
    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;
    int i, j;
    for (i = 0, j = 0; i < input_length;) {
        percentageDone = (float) i / (float) input_length * 100;
        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
    return decoded_data;
}

void base64_cleanup() {
    free(decoding_table);
}

void printUsage() {
    panicf("Improper use of the code, Hint: ./base64 --encode file or ./base64 --encode file\n");
}

void signalManager(int sig) {
    infof("%d%% of file processed\n", (int) percentageDone);
}

int main(int argc, char *argv[]) {
    
    signal(SIGINT, signalManager); 
    signal(SIGINFO, signalManager);

    int option = 0; // 1: encode 2: decode

    if(strcmp(argv[1], "--encode") == 0){
        option = 1;
    }
    else if(strcmp(argv[1], "--decode") == 0){
        option = 2;
    }else{
        printUsage();
        return -1;
    }

    char *inputFile = calloc(1024, sizeof(char));
    char *outputFile = calloc(1024, sizeof(char));

    switch (option) {
        case 1: 
            strcpy(inputFile, argv[2]);
            strcpy(outputFile, "encoded.txt");
            break;
        case 2: 
            strcpy(inputFile, argv[2]);
            strcpy(outputFile, "decoded.txt");
            break;
        default:
            printUsage();
            return -1;
    }
      
    long fileSize;
    char *buffer;
    FILE *fp = fopen(inputFile,"r");
    
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(fileSize);
    if ((fread(buffer, 1, fileSize, fp)) == 0) {
        panicf("Cannot read from file [Empty?]\n");
        return -1;
    }
    fclose(fp);

    size_t output_size = 0;
    char *result;
    
    infof("%s %s\n", (option == 1) ? "encoding" : "decoding", inputFile);
    
    if (option == 1) {
        result = base64_encode(buffer, strlen(buffer), &output_size);
    } else if (option == 2) {
        result = (char *) base64_decode(buffer, strlen(buffer), &output_size);
    } else {
        panicf("Error while %s %s base64", (option == 1) ? "encoding" : "decoding", inputFile);
        return -1;
    }    
    if (result == NULL) {
        panicf("Error while %s %s base64", (option == 1) ? "encoding" : "decoding", inputFile);
        return -1;
    }

    base64_cleanup();

    FILE *ofp;
    ofp = fopen(outputFile,"w");
    if (ofp == NULL) {
        panicf("Error while creating %s\n", outputFile);
        return -1;
    } else {
        infof("File %s was successfully created.\n", outputFile);
    }

    fwrite(result, output_size, 1, ofp);
    fclose(ofp);
    return 0;
}