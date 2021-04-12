#include <stdio.h>
#include <cstring>
#include <stdlib.h>

int main(int argc, char** argv){
        if(argc > 1){ 
                char c[1000];
                FILE *fptr;
                printf("hello World\n");
                if ((fptr = fopen("audio.txt", "r")) == NULL) {
                    printf("Error! opening audio.txt");
                    // Program exits if file pointer returns NULL.
                }else{

                        fscanf(fptr,"%s", c); 
                        printf("Data from the file:\n%s", c); 
                        fclose(fptr);
			char command[1000];
			sprintf(command, "omxplayer ");
			sprintf(command + strlen(command), c);
			printf("Command is: %s\n", command);
			system(command);

                }
        }
}

