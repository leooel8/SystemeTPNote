/*
    Léo RESSAYRE 
    Groupe 5            TP 10 Noté

    Mon TP est composé des deux fonctions suivantes : 'copie' et 'copieRepertoire'.
    La fonction 'copie' répond aux deux premiers points du TP, et 'copieRepertoire' répond aux deux dernières. 
    Pour les faires fonctionner sur les répertoires et fichiers donnés, vérifier qu'aucun fichier 'Gros_FichierCopie.txt' 
    n'existe dans le répertoire courant. De même,vérifier que le répertoire 'DossierTestCopie' est vide. Enfin, décommenter 
    le code dans la fonction 'main'.

*/

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

/*
    Fonction 'copie':
    Prend en paramètre un fichier source à copier, et le copie dans un second fichier dont le nom est à spécifier
    en second argument.
    La copie conserve les permissions.
*/

void copie (char *file1,char *file2) {
    int fichier1 = 0;
    int fichier2 = 0;
    
    fichier1 = open(file1, O_RDONLY); //Ouverture du fichier source en 'read only'
    fichier2 = open(file2, O_WRONLY | O_CREAT | O_EXCL); //Création (O_CREAT) et ouverture du fichier destination


    struct stat state;               //Création d'une structure
    fstat(fichier1,&state);          //qui permettra de récupérer
    int permission1 = state.st_mode; //les droits du fichier source

    if (fichier1 < 0) {
        printf("Ouverture du Fichier 1 impossible\n");
    }


    int reader = 0;
    int writer = 0;
    char content[4096]; //On choisi ici des blocs de 4096 pour la copie

    do {
        reader = read(fichier1,(char*)content,4096); //Lecture du fichier source
        if(reader > 0) {
            writer = write(fichier2,(char*)content,reader); //Ecriture de ce qui a été lu dans le fichier destination
            if (writer == -1) {
                printf("Erreur écriture : errno = %d\n",errno); //Permet d'afficher le code erreur s'il y en a un
            }
        }
    } while ((reader == 4096) && (writer > 0));

    fchmod(fichier2,permission1); //Copie des permissions
    
    close(fichier1);
    close(fichier2); //Fermeture des fichiers
}

/*
    Fonction 'copieRepertoire':
    Prend en paramètre un dossier à copier, et le copie dans un répertoire déjà existant dont le nom est à spécifier en second
    argument.
    Conserve les permissions.
*/

void copieRepertoire(char *directory1,char *directory2) {

    DIR *direct1 = opendir(directory1);
    if(direct1 == NULL) {
        printf("Ouverture du dossier source impossible");
    }

    struct dirent *currentDir;     //Création d'une structure permettant 
    currentDir = readdir(direct1); //de naviguer dans le repertoire

    while (currentDir != NULL) {

        char *currentName = currentDir->d_name;
        
        if (!((strcmp(currentName,".") == 0) || (strcmp(currentName,"..") == 0))) {

            char *target = (char *)malloc(256); 
            strcpy(target,directory2);  //Allocation d'un char qui servira
            strcat(target,"/");         //à accueillir l'adresse destination
            strcat(target,currentName); //('repertoireDestination/fileName')

            char *source = (char *)malloc(256); 
            strcpy(source,directory1);  //Allocation d'un char qui servira       
            strcat(source,"/");         //à accueillir l'adresse source
            strcat(source,currentName); //('repertoireCourant/fileName')

            struct stat state;               //Création d'une structure
            stat(source,&state);             //qui permettra de récupérer
            int permission1 = state.st_mode; //les droits du répertoire source
            
            if (!S_ISDIR(permission1)) { //Vérifie s'il ne s'agit pas d'un répertoire
                copie(source,target); //Copie
            }
            else {
                mkdir(target,permission1);      //Ici, il s'agit d'un répertoire,on crée donc un nouveau répertoire 
                copieRepertoire(source,target); //dans le dossier destination, puis on appelle récursivement la fonction
            }

            free(source);
            free(target);
        }
       
        currentDir = readdir(direct1);
    }
    closedir(direct1);
}
 

int main() {
    // printf("Copie fichier : ..\n");
    // copie("Gros_Fichier.txt","Gros_FichierCopie.txt");
    // printf("Copie fichier : DONE\n");
    // printf("Copie dossier : ..\n");
    // copieRepertoire("DossierTest","DossierTestCopie");
    // printf("Copie dossier : DONE\n");
    return 0;
}