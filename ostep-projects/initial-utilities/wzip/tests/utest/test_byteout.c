/* test_byteout.c: check that write functions are working as expected:
 * */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


size_t getwritesz (int num, u_int8_t ch)
{
    size_t nbytes = 0;
    nbytes += sizeof(num);
    nbytes += sizeof(ch);
    return nbytes;
}

void asciitobin(u_int8_t asciiInput)
{
    int res = 0, i = 1, rem;
    printf("ascii input: %d\n", asciiInput);
    while (asciiInput > 0) 
    {
        rem = asciiInput % 2;
        res = res + (i * rem);
        asciiInput = asciiInput / 2;
        i = i * 10;  
    }
    printf("converted char: %d\n", res);
}

/* write number of occurrences of char as 4 byte int,
 * followed by char as ASCII */
int writenumch (u_int32_t num, u_int8_t ch) 
{
    size_t bytecheck;
    
    //asciitobin(ch);

    bytecheck = getwritesz(num, ch);

    printf("num bytes being written: %zu\n", bytecheck);

    fwrite(&num, sizeof(u_int32_t), 1, stdout);
    printf("%d", ch);
    
    //} else {
    //    perror("Error: number of bytes output is not five\n"); 
    //    printf("Trying to write %zu bytes\n", bytecheck);
    //}
    num = 0; /* resets count to zero if write successful  */
    return num;
}

int main (int argc, char *argv[])
{
    int rec; /* number of recurrences */
    char c; /* char that recurs */
    u_int8_t ch; /* our char will be ascii, so we can store it in 8 bits */

    int count = 0;

    printf("nargs: %d\n", argc);
    if (argc != 3) {
        printf("Usage: test_byteout <u_int32_t> <char>\n");
    }

    if (argc == 3) {
        rec = atoi(argv[1]);
        c = *argv[2];
        if ((c < 0 ) || (c > 127)) { /* outside ascii range */
            perror("Input char must be an ascii value in range 0-127\n");
            return 1;
        }
        ch = c; 
        printf("size of u_int8_t: %lu\n", sizeof(u_int8_t));
        printf("argv[2]: %c as int: %d\n", c, ch);
        count = writenumch (rec, ch);
    }
    if (count != 0) printf("Write unsuccessful!\n");
    
    return 0;
}
