#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <mpi.h>

#define MAX_LETTERS 50

typedef struct {
    char name[100];
} file;

typedef struct {
    char letters[MAX_LETTERS];
    int frequency;
    int nwords;
} word;

struct stat st;

const char arraydelim[] = " ,;.:-—_°#|*'’^<>«»!?\"=\n\r()[]{}\n";


word* insertWords(int nfiles, file files[]);

void splitWords(int nWords, int nproc, int elements_per_process[], int displs[]);

word* countFrequency(int nWord, word *words);



int main(int argc, char* argv[]) {

    int rank, nproc;
    MPI_Init(&argc, &argv);

    double starttime = MPI_Wtime();

    MPI_Request reqnumber, reqwords;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    
    word *words, *recvwords;
    MPI_Datatype MPI_WORD;
    int elements_per_process[nproc];
    int displs[nproc];
    int nwordsproc;
    int nfiles = -1;
    
    // creating type WORD
    MPI_Datatype oldtypes[2] = {MPI_CHAR, MPI_INT};
    int blockcounts[2] = {MAX_LETTERS, 2};
    MPI_Aint offsets[2] = {offsetof(word, letters), offsetof(word, frequency),};

    MPI_Type_create_struct(2, blockcounts, offsets, oldtypes, &MPI_WORD);
    MPI_Type_commit(&MPI_WORD);

    if(rank == 0) {
        DIR *directory;
        FILE *fd;
        struct dirent *dir;
        
        file *files = malloc(sizeof(file)*(nfiles+2));

        directory = opendir(argv[1]);

        // taking each file in the directory
        if(directory == NULL) {
            printf("No directory here\n");
        } else {
            while((dir = readdir(directory)) != NULL) {
                nfiles++;
                strcpy(files[nfiles].name, argv[1]);
                strcat(files[nfiles].name, "/");
                strcat(files[nfiles].name, dir->d_name);
                
                files = realloc(files, sizeof(file)*(nfiles+2));
            }
        }
        closedir(directory);

        // taking each word from each file and saving in words
        words = insertWords(nfiles, files);
        free(files);

        // counting how many words for each proc
        splitWords(words->nwords, nproc, elements_per_process, displs);

        // sending the number of words for each proc (to allocate memory)
        for(int i=1; i<nproc; i++)
            MPI_Send(&elements_per_process[i], 1, MPI_INT, i, 99, MPI_COMM_WORLD);

        nwordsproc = elements_per_process[0];
    }

    // receiving the number of words for each proc
    if(rank>0)
        MPI_Recv(&nwordsproc, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, NULL);

    recvwords = malloc(sizeof(word)*nwordsproc);
    
    // sending part of the words to each proc
    MPI_Scatterv(words, elements_per_process, displs, MPI_WORD, recvwords, nwordsproc, MPI_WORD, 0, MPI_COMM_WORLD);
    if(rank == 0) free(words);

    // counting the frequencies of the words for each proc
    recvwords = countFrequency(nwordsproc, recvwords);
    nwordsproc = recvwords->nwords;

    //sending the words with all frequencies to rank 0
    if(rank==0) {
        MPI_Isend(&nwordsproc, 1, MPI_INT, 0, 66, MPI_COMM_WORLD, &reqnumber);
        MPI_Isend(recvwords, nwordsproc, MPI_WORD, 0, 55, MPI_COMM_WORLD, &reqwords);
    } else {
        MPI_Send(&nwordsproc, 1, MPI_INT, 0, 88, MPI_COMM_WORLD);
        MPI_Send(recvwords, nwordsproc, MPI_WORD, 0, 77, MPI_COMM_WORLD); 
    }
    
    
    if(rank==0) {

        word *wordcountproc, *wordcountotal;
        int newords[nproc];
        int totwords;

        // initializing the word struct containing the total frequencies
        // here the frequencies of rank 0
        MPI_Recv(&newords[0], 1, MPI_INT, 0, 66, MPI_COMM_WORLD, NULL);
        wordcountproc = malloc(sizeof(word)* newords[0]);
        wordcountotal = malloc(sizeof(word)* newords[0]);
        MPI_Recv(wordcountotal, newords[0], MPI_WORD, 0, 55, MPI_COMM_WORLD, NULL);
        
        totwords = wordcountotal->nwords;

        // receiving the words from each proc and putting them together
        for(int i=1; i<nproc; i++) {
            MPI_Recv(&newords[i], 1, MPI_INT, i, 88, MPI_COMM_WORLD, NULL);
            totwords += newords[i];
            wordcountotal = realloc(wordcountotal, sizeof(word)*totwords);
            wordcountproc = realloc(wordcountproc, sizeof(word)*newords[i]);
            MPI_Recv(wordcountproc, newords[i], MPI_WORD, i, 77, MPI_COMM_WORLD, NULL);

            for(int j=0; j<newords[i]; j++)
                wordcountotal[totwords-newords[i]+j] = wordcountproc[j];
        }

        // counting the final frequencies of the words
        wordcountotal = countFrequency(totwords, wordcountotal);

        // creating file csv with the word and the frequency
        FILE *csvfile;
        csvfile = fopen(argv[2], "w");
        fprintf(csvfile, "Word,Count\n");
        for(int i=0; i<wordcountotal->nwords; i++){
            fprintf(csvfile, "%s,%d\n", wordcountotal[i].letters, wordcountotal[i].frequency);
        }
        fclose(csvfile);

        free(wordcountproc);
        free(wordcountotal);
    }
    free(recvwords);

    double endtime = MPI_Wtime();

    if(rank == 0) {
        printf("Total execution time in ms : %f\n", endtime-starttime);
    }

MPI_Type_free(&MPI_WORD);
MPI_Finalize();
return 0;
}


word* insertWords(int nfiles, file files[]) {

    word *words=malloc(sizeof(word)*1);
    FILE *fd;
    int puntWord=0;

    for(int i=0; i<nfiles+1; i++){
        fd = fopen(files[i].name, "r");
        if(fd==NULL){
            perror("\n Error opening the file");
        } else {
            char *file_contents = malloc(st.st_size);
            while (fscanf(fd, "%[^\n ] ", file_contents) != EOF) {
                char *singlewords = strtok(file_contents, arraydelim);
                while(singlewords != NULL) {
                    strcpy(words[puntWord].letters, singlewords);
                    words[puntWord].frequency = 1;
                    puntWord++;
                    words = realloc(words, sizeof(word)*(puntWord+1));
                    singlewords = strtok(NULL, arraydelim);
                }
            }
            free(file_contents);
        }
        fclose(fd);
    }
    words->nwords = puntWord;
    return words;
}

void splitWords(int nWords, int nproc, int elements_per_process[], int displs[]) {
    
        int div = nWords/nproc;
        int resto = nWords % nproc;
        
        displs[0] = 0;
        for(int i=0; i<nproc; i++){
            if(i < resto)
                div = div+1;
            elements_per_process[i] = div;
            displs[i+1] = displs[i] + elements_per_process[i];
            div = nWords/nproc;
        }
}

word* countFrequency(int nWord, word *words) {

    int nSingleWord = nWord;

    for(int i=0; i<nWord; i++) {
        for(int j=i+1; j<nWord; j++) {
            if(strcasecmp(words[i].letters, words[j].letters) == 0 && (words[i].frequency != 0)) {
                words[i].frequency += words[j].frequency;
                words[j].frequency = 0;
                nSingleWord--;
            }
            if((words[i].frequency == 0) && (words[j].frequency != 0)) {
                char wordtemp[MAX_LETTERS];
                int frequencytemp = words[i].frequency;
                strcpy(wordtemp, words[i].letters);
                strcpy(words[i].letters, words[j].letters);
                strcpy(words[j].letters, wordtemp);
                words[i].frequency = words[j].frequency;
                words[j].frequency = frequencytemp;
            }
        }
    }
    words = realloc(words, sizeof(word)*nSingleWord);
    words->nwords = nSingleWord;
    return words;
}

