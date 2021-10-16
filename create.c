#include <stdio.h>
#include <stdlib.h>


int main (int argc, char** argv){
    FILE* fp=fopen("utils/strong_results.csv","w");
    fprintf(fp,"NP,Time,Nfile,Nwords\n");
    FILE* fp_weak=fopen("utils/weak_results.csv","w");
    fprintf(fp_weak,"NP,Time,Nfile,Nwords\n");
    fclose(fp);
    fclose(fp_weak);
}