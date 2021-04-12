#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>

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
			char song_name[1000];
			sprintf(song_name, c);
			printf("song_name is: %s\n", song_name);
			char kil[100];

			pid_t audio_pid;

			if(0 == (audio_pid = fork())){
				//child process
				execlp("mpg123", "mpg123", "-q", song_name, 0);
			}else{
				sprintf(kil,"%s%d",kil,audio_pid);
				getchar();  // wait for user input
				system(kil);
			}

                }
        }
}

