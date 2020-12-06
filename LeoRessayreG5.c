#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

void copie (char *file1,char *file2) {
    int fichier1 = 0;
    int fichier2 = 0;

    printf("Ouverture du fichier1..\n");
    fichier1 = open(file1, O_RDONLY);
    printf("Fichier1 ouvert\n");

    printf("Ouverture fichier2..\n");
    fichier2 = open(file2, O_WRONLY | O_CREAT | O_EXCL);
    printf("Fichier2 ouvert\n");

    struct stat state;
    fstat(fichier1,&state);
    int permission1 = state.st_mode;

    if (fichier1 >= 0) {
        printf("Fichier bien ouvert, adresse : %d\n",fichier1);
    }
    else {
        printf("Ouverture du Fichier 1 impossible\n");
    }

    int reader = 0;
    int writer = 0;
    char content[4096];

    do {
        reader = read(fichier1,(char*)content,4096);
        if(reader > 0) {
            writer = write(fichier2,(char*)content,reader);
            if (writer == -1) {
                printf("Erreur écriture : errno = %d\n",errno);
            }
        }
    } while ((reader == 4096) && (writer > 0));

    fchmod(fichier2,permission1);

    struct stat state2;
    fstat(fichier2,&state2);
    int permission2 = state2.st_mode;

    printf("PERMISSIONS : Fichier1 : %o\n              Fichier2 : %o \n",permission1,permission2);
    
    close(fichier1);
    close(fichier2);
    printf("Fin copie!!\n");
}

void copieRepertoire(char *directory1,char *directory2) {

    DIR *direct1 = opendir(directory1);
    if(direct1 == NULL) {
        printf("Ouverture du dossier source impossible");
    }


    printf("SourceFileName : %s\n",directory1);
    printf("SourceFileName : %s\n",directory2);

    struct dirent *currentDir;
    currentDir = readdir(direct1);
    while (currentDir != NULL) {
        char *currentName = currentDir->d_name;
        if (!((strcmp(currentName,".") == 0) || (strcmp(currentName,"..") == 0))) {
            char *target = (char *)malloc(256);
            strcpy(target,directory2);
            strcat(target,"/");
            strcat(target,currentName);
            char *source = (char *)malloc(256);
            strcpy(source,directory1);
            strcat(source,"/");
            strcat(source,currentName);
            printf("source : %s\n",source);
            printf("Target : %s\n",target);
            copie(source,target);
        }
        currentDir = readdir(direct1);
    }
}
 

int main() {
    printf("Copie fichier : ..\n");
    copie("Gros_Fichier.txt","Gros_FichierCopie.txt");
    printf("Copie fichier : DONE\n");
    printf("Copie dossier : ..\n");
    copieRepertoire("/mnt/e/LEO/Paris-Sud/L3/S1/SYSTEME/TPs/TPNote/DossierTest/","/mnt/e/LEO/Paris-Sud/L3/S1/SYSTEME/TPs/TPNote/DossierTestCopie/");
    printf("Copie dossier : DONE\n");
    return 0;
}