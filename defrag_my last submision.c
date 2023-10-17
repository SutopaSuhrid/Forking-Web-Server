#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


//every file found is stored as this
typedef struct {
	char* binary; //bytes read in
	size_t len; // length of bytes read in
} dataS;

dataS** data;//array of pointers to struc dataS
size_t length = 0; //max size of data (not exactly how many elements in data)


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//what happens when a thread is created, and will continue to happen for every found folder
void *thr_fn(void *arg){

	char* dir_name = (char*)arg;	
	DIR* current  = opendir(dir_name);
	dir_name = realloc(dir_name, sizeof(char)*(strlen(dir_name)+1+1));
	char* slash = "/";
	dir_name = strcat(dir_name, slash);
	struct dirent *entry;
	
	while( (entry = readdir(current)) ) { //exits when NULL, nothing found
		if (entry->d_name[0] != '.' ){
			if (entry->d_type == DT_LNK) { //do nothing

			} else if (entry->d_type == DT_DIR) { //if directory
				char* now = strdup(dir_name);
				
				now = realloc(now, sizeof(char)*(strlen(now)+strlen(entry->d_name)+1));
				now = strcat(now, entry->d_name);

				void* err = thr_fn((void*) strdup(now));  //recursion
				if (err != (void*)0) perror("can′t traverse directory");

				free(now);

			} else { //if file
				
				int num = atoi(entry->d_name);
				char* now = strdup(dir_name);
				now = realloc(now, sizeof(char)*(strlen(now)+strlen(entry->d_name)+1));
				now = strcat(now, entry->d_name);

				FILE *fd = fopen(now, "r");
				

				pthread_mutex_lock(&lock);

				if (length < (num + 1)){ //length is length of data. if [0] --> length = 1
					length = num +1;
					
				}  
				data = realloc(data, sizeof(dataS*) * length);
				
				data[num] = malloc(sizeof(dataS)); 

				//Fill Struc
				struct stat st;
				stat(now, &st);

				data[num]->binary = malloc(st.st_size); //st_size = file size
				data[num]->len =  fread(data[num]->binary , sizeof(char), st.st_size, fd); //put on two lines and put fread outside of mutex

				pthread_mutex_unlock(&lock);


				fclose(fd); //binary file descripter
				free(now);
				
			} //end of file type stuff
		}
	}//end while
	
	closedir(current);
	free(dir_name); //from strdup() and realloc

    	return((void *)0);
}


int main(int argc, char **argv){
	if (argc != 3) return -1;

	pthread_t* ntid = NULL;
	size_t num_threads = 0;

	char *newFile = argv[2]; //music.mp3

	chdir(argv[1]); //switch to current working directory
	DIR* current  = opendir("."); //open directory
	
	struct dirent *entry;

	int err;
	while( (entry = readdir(current)) ) { //exits when NULL, no more dirs
		if (entry->d_name[0] != '.' ){
			//add new space to array of threads
			//create and run new thread at one of the starting directorys
			ntid = realloc(ntid, sizeof(pthread_t)*(num_threads+1));			
		    	err = pthread_create(&ntid[num_threads], NULL, thr_fn, (void*) strdup(entry->d_name));
		    	if (err != 0) perror("can′t create thread");
			num_threads++;
		}
	}

	for (int i=0; i<num_threads;i++) pthread_join(ntid[i], NULL); //end threads, all stuff catch up
	


	//open file
	FILE *music = fopen(newFile, "w");

	for (int i=0; i<length;++i){
		
		
		//read data[i]->binary into file
		fwrite(data[i]->binary, sizeof(char), data[i]->len, music);

		free(data[i]->binary);
		free(data[i]);
	}

	//close file
	fclose(music);	




	free(data);
	closedir(current);
	free(ntid);
	
	
	
	chdir(".."); //up on directory

    	return 0;
}

