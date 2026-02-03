#include <stdio.h>
#include <sodium.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <pwd.h>
#include <sys/types.h>

#define MAXDIM 100

unsigned char key[crypto_secretbox_KEYBYTES];
void init_key(const char *master) {
    crypto_generichash(key, sizeof key,(const unsigned char*)master, strlen(master),NULL, 0);
}


typedef struct Credenziali{
    char nome[100];
    char password[300];
}credenziali;

void inizializzazionePasswordManager();
void menu();
void visualizzaPassword();
void aggiungiPassword();
void modificaPassword();
void reset();
void eliminaPassword();

void stampa_banner() {
    printf("\n");
    printf(" /$$$$$$$                                                                      /$$       /$$      /$$                                                            \n");
    printf("| $$__  $$                                                                    | $$      | $$$    /$$$                                                            \n");
    printf("| $$  \\ $$ /$$$$$$   /$$$$$$$ /$$$$$$$ /$$  /$$  /$$  /$$$$$$   /$$$$$$   /$$$$$$$      | $$$$  /$$$$  /$$$$$$  /$$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$ \n");
    printf("| $$$$$$$/|____  $$ /$$_____//$$_____/| $$ | $$ | $$ /$$__  $$ /$$__  $$ /$$__  $$      | $$ $$/$$ $$ |____  $$| $$__  $$ |____  $$ /$$__  $$ /$$__  $$ /$$__  $$\n");
    printf("| $$____/  /$$$$$$$|  $$$$$$|  $$$$$$ | $$ | $$ | $$| $$  \\ $$| $$  \\__/| $$  | $$      | $$  $$$| $$  /$$$$$$$| $$  \\ $$  /$$$$$$$| $$  \\ $$| $$$$$$$$| $$  \\__/\n");
    printf("| $$      /$$__  $$ \\____  $$\\____  $$| $$ | $$ | $$| $$  | $$| $$      | $$  | $$      | $$\\  $ | $$ /$$__  $$| $$  | $$ /$$__  $$| $$  | $$| $$_____/| $$      \n");
    printf("| $$     |  $$$$$$$ /$$$$$$$//$$$$$$$/|  $$$$$/$$$$/|  $$$$$$/| $$      |  $$$$$$$      | $$ \\/  | $$|  $$$$$$$| $$  | $$|  $$$$$$$|  $$$$$$$|  $$$$$$$| $$      \n");
    printf("|__/      \\_______/|_______/|_______/  \\_____/\\___/  \\______/ |__/       \\_______/      |__/     |__/ \\_______/|__/  |__/ \\_______/ \\____  $$ \\_______/|__/      \n");
    printf("                                                                                                                                    /$$  \\ $$                    \n");
    printf("                                                                                                                                   |  $$$$$$/                    \n");
    printf("         Developed by Palumbs                                                                                                       \\______/                     \n");
    printf("\n");
}

void banner() {
    printw("\n");
    printw(" /$$$$$$$                                                                      /$$       /$$      /$$                                                            \n");
    printw("| $$__  $$                                                                    | $$      | $$$    /$$$                                                            \n");
    printw("| $$  \\ $$ /$$$$$$   /$$$$$$$ /$$$$$$$ /$$  /$$  /$$  /$$$$$$   /$$$$$$   /$$$$$$$      | $$$$  /$$$$  /$$$$$$  /$$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$ \n");
    printw("| $$$$$$$/|____  $$ /$$_____//$$_____/| $$ | $$ | $$ /$$__  $$ /$$__  $$ /$$__  $$      | $$ $$/$$ $$ |____  $$| $$__  $$ |____  $$ /$$__  $$ /$$__  $$ /$$__  $$\n");
    printw("| $$____/  /$$$$$$$|  $$$$$$|  $$$$$$ | $$ | $$ | $$| $$  \\ $$| $$  \\__/| $$  | $$      | $$  $$$| $$  /$$$$$$$| $$  \\ $$  /$$$$$$$| $$  \\ $$| $$$$$$$$| $$  \\__/\n");
    printw("| $$      /$$__  $$ \\____  $$\\____  $$| $$ | $$ | $$| $$  | $$| $$      | $$  | $$      | $$\\  $ | $$ /$$__  $$| $$  | $$ /$$__  $$| $$  | $$| $$_____/| $$      \n");
    printw("| $$     |  $$$$$$$ /$$$$$$$//$$$$$$$/|  $$$$$/$$$$/|  $$$$$$/| $$      |  $$$$$$$      | $$ \\/  | $$|  $$$$$$$| $$  | $$|  $$$$$$$|  $$$$$$$|  $$$$$$$| $$      \n");
    printw("|__/      \\_______/|_______/|_______/  \\_____/\\___/  \\______/ |__/       \\_______/      |__/     |__/ \\_______/|__/  |__/ \\_______/ \\____  $$ \\_______/|__/      \n");
    printw("                                                                                                                                    /$$  \\ $$                    \n");
    printw("                                                                                                                                   |  $$$$$$/                    \n");
    printw("         Developed by Palumbs                                                                                                       \\______/                     \n");
    printw("\n");
}

int main(){
    if (sodium_init() < 0) {
        printf("Errore inizializzazione libsodium\n");
        return 1;
    }
    inizializzazionePasswordManager();
    menu();
    return 0;
}

void inizializzazionePasswordManager(){
    if (sodium_init() < 0) {
        printf("Errore inizializzazione libsodium\n");
        return;
    }

    FILE *file=fopen("passwordUtente.bin","rb");
    if(!file){
        file = fopen("passwordUtente.bin","wb");
        if(!file){
            printf("Impossibile creare il file\n");
            return;
        }
        fclose(file);
        file = fopen("passwordUtente.bin","rb");
    }

    int firstByte=fgetc(file);
    fclose(file);

    if(firstByte==EOF){
        stampa_banner();
        char passwordUtente[MAXDIM];
        struct termios oldt,newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        printf("Inizializzazione del Password Manager:\n");
        printf("--------------------------------------\n");
        printf("Inserisci la nuova password: ");
        fgets(passwordUtente, sizeof(passwordUtente), stdin);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        printf("\n");
        passwordUtente[strcspn(passwordUtente, "\n")] = '\0';
        init_key(passwordUtente);
        unsigned char nonce[crypto_secretbox_NONCEBYTES];
        randombytes_buf(nonce, sizeof(nonce));
        size_t password_len = strlen(passwordUtente) + 1;
        unsigned char cipher[crypto_secretbox_MACBYTES + password_len];
        crypto_secretbox_easy(cipher, (unsigned char*)passwordUtente, password_len, nonce, key);
        FILE *file = fopen("passwordUtente.bin","wb");
        if(!file){
            printf("Errore apertura file\n");
            sodium_memzero(passwordUtente, sizeof(passwordUtente));
            return;
        }
        fwrite(nonce, 1, sizeof(nonce), file);
        fwrite(cipher, 1, crypto_secretbox_MACBYTES + password_len, file);
        fclose(file);

        sodium_memzero(passwordUtente, sizeof(passwordUtente));
        printf("Inizializzazione conclusa con successo!\n");
    }

    sleep(2);
    system("clear");
}

void visualizzaPassword(){

    int scelta=-1;
    FILE *fileNomi=fopen("nomi.txt","r");
    if(!fileNomi){
        printf("Impossibile aprire il file\n");
        return;
    }
    FILE *filePass=fopen("pass.bin","rb");
    if(!filePass){
        printf("Impossibile aprire il file\n");
        return;
    }
    credenziali *arr=NULL;
    char nome[MAXDIM];
    char password[MAXDIM];
    unsigned char nonce[crypto_secretbox_NONCEBYTES];

    int n=0;
    while(fgets(nome,sizeof(nome),fileNomi)){
        nome[strcspn(nome, "\n")] = '\0';
        int pw_len;
        if(fread(&pw_len, sizeof(int), 1, filePass) != 1) break;
        if(fread(nonce, 1, sizeof(nonce), filePass) != sizeof(nonce)) break;

        unsigned char cipher[crypto_secretbox_MACBYTES + pw_len];
        if(fread(cipher, 1, crypto_secretbox_MACBYTES + pw_len, filePass) != crypto_secretbox_MACBYTES + pw_len) break;

        if(crypto_secretbox_open_easy((unsigned char*)password, cipher, crypto_secretbox_MACBYTES + pw_len, nonce, key) != 0){
            printf("Errore decifratura password %s\n", nome);
            continue;
        }
        arr = realloc(arr,(n+1)*sizeof(credenziali));
        strcpy(arr[n].nome,nome);
        strcpy(arr[n].password,password);
        n++;
    }
    fclose(fileNomi);
    fclose(filePass);
    initscr();
    cbreak();
    scrollok(stdscr, TRUE);
    while(scelta!=n+1){
        clear();
        banner();
        printw("Lista Password:\n");
        for(int i=0;i<n;i++){
            printw("%d %s\n", i+1, arr[i].nome);
        }
        printw("%d Exit\n",n+1);
        printw("Quale desideri visualizzare?\n> ");
        scanw("%d",&scelta);
        if(scelta==n+1){
            endwin();
            return;
        }

        if(scelta<1 || scelta>n){
            printw("Inserimento non valido riprova..\n");
            refresh();
            sleep(1);
            continue;
        }else{
            clear();
            printw("Nome: %s",arr[scelta-1].nome);
            printw(" - Password: %s\n", arr[scelta-1].password);

            printw("Premi Invio per tornare indietro\n");
            refresh();
            while(getch() != '\n');
        }
    }
    endwin();
    free(arr);
}

void aggiungiPassword(){
    system("clear");
    stampa_banner();
    char nuovaPassword[MAXDIM];
    char nome[MAXDIM];
    FILE *fileNomi=fopen("nomi.txt","a+");
    if(!fileNomi){
        printf("Impossibile aprire il file\n");
        return;
    }
    FILE *filePass=fopen("pass.bin","ab");
    if(!filePass){
        printf("Impossibile aprire il file\n");
        return;
    }
    printf("Nome: ");
    fgets(nome,sizeof(nome),stdin);
    nome[strcspn(nome, "\n")] = '\0';
    printf("Password: ");
    fgets(nuovaPassword,sizeof(nuovaPassword),stdin);
    nuovaPassword[strcspn(nuovaPassword, "\n")] = '\0';

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof nonce);
    int pw_len = strlen(nuovaPassword) + 1;
    unsigned char cipher[crypto_secretbox_MACBYTES + pw_len];
    crypto_secretbox_easy(cipher, (unsigned char*)nuovaPassword, strlen(nuovaPassword)+1, nonce, key);

    fseek(fileNomi, 0, SEEK_END);
    fseek(filePass, 0, SEEK_END);
    int size = ftell(fileNomi);
    int size2 = ftell(filePass);
    if (size > 0 && size2 > 0) {

        fprintf(fileNomi, "\n%s", nome);
        fwrite(&pw_len, sizeof(int), 1, filePass);
        fwrite(nonce,1,sizeof(nonce),filePass);
        fwrite(cipher, 1, crypto_secretbox_MACBYTES + strlen(nuovaPassword)+1, filePass);
        sodium_memzero(nuovaPassword, sizeof nuovaPassword);
        printf("Password Inserita con successo!!\n");

    } else {

        fprintf(fileNomi, "%s", nome);
        fwrite(&pw_len, sizeof(int), 1, filePass);
        fwrite(nonce,1,sizeof(nonce),filePass);
        fwrite(cipher, 1, crypto_secretbox_MACBYTES + strlen(nuovaPassword)+1, filePass);
        sodium_memzero(nuovaPassword, sizeof nuovaPassword);
        printf("Password Inserita con successo!!\n");

    }

    fclose(fileNomi);
    fclose(filePass);

}

void modificaPassword(){
    int scelta=-1;
    int scelta2=-1;

    FILE *fileNomi=fopen("nomi.txt","r+");
    if(!fileNomi){
        printf("Impossibile aprire il file\n");
        return;
    }
    FILE *filePass=fopen("pass.bin","rb");
    if(!filePass){
        printf("Impossibile aprire il file\n");
        return;
    }
    credenziali *arr=NULL;
    char nome[MAXDIM];
    char password[MAXDIM];
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    int pw_len;
    int n=0;
    while(fgets(nome,sizeof(nome),fileNomi)){
        nome[strcspn(nome, "\n")] = '\0';
        if(fread(&pw_len,sizeof(int),1,filePass)!=1) break;
        if(fread(nonce,1,sizeof(nonce),filePass)!=sizeof(nonce)) break;
        unsigned char cipher[crypto_secretbox_MACBYTES + pw_len];
        if(fread(cipher,1,crypto_secretbox_MACBYTES + pw_len,filePass)!=(size_t)(crypto_secretbox_MACBYTES+pw_len)) break;
        if(crypto_secretbox_open_easy((unsigned char*)password,cipher,crypto_secretbox_MACBYTES+pw_len,nonce,key)!=0){
            printf("Errore decifratura password %s\n",nome);
            continue;
        }
        arr = realloc(arr,(n+1)*sizeof(credenziali));
        strcpy(arr[n].nome,nome);
        strcpy(arr[n].password,password);
        n++;
    }
    fclose(fileNomi);
    fclose(filePass);
    while(scelta2!=3){
        system("clear");
        stampa_banner();
        printf("Digita:\n1 - Per modificare il nome\n2 - Per modificare la password\n3 - Exit\n> ");
        scanf("%d",&scelta2);
        while(getchar() != '\n');
        switch(scelta2){
            case 1:
                while(scelta!=n+1){
                    system("clear");
                    stampa_banner();
                    printf("Lista Password:\n");
                    for(int i=0;i<n;i++){
                        printf("%d %s\n", i+1, arr[i].nome);
                    }
                    printf("%d Exit\n",n+1);
                    printf("Quale nome vuoi modificare?\n> ");
                    scanf("%d",&scelta);
                    while(getchar() != '\n');
                    if(scelta==n+1){
                        break;
                    }
                    if(scelta<1 || scelta>n+1){
                        printf("Inserimento non valido riprova..\n");
                        sleep(1);
                        system("clear");
                    }else{
                        char nomeNuovo[MAXDIM];

                        system("clear");
                        stampa_banner();
                        printf("Inserisci il nuovo nome: ");
                        fgets(nomeNuovo,sizeof(nomeNuovo),stdin);
                        nomeNuovo[strcspn(nomeNuovo, "\n")] = '\0';
                        strcpy(arr[scelta-1].nome,nomeNuovo);
                        FILE *fileNomi=fopen("nomi.txt","w");
                        if(!fileNomi){
                            printf("Impossibile aprire il file\n");
                            break;
                        }
                        for(int i=0;i<n;i++){
                            fprintf(fileNomi,"%s",arr[i].nome);
                            if(i!=n-1){
                                fprintf(fileNomi,"\n");
                            }
                        }
                        printf("Modifica avvenuta con successo!!\n");
                        sleep(1);
                        system("clear");
                        fclose(fileNomi);
                    }
                }
                break;
            case 2:
                while(scelta!=n+1){
                    system("clear");
                    stampa_banner();
                    printf("Lista Password:\n");
                    for(int i=0;i<n;i++){
                        printf("%d %s\n", i+1, arr[i].nome);
                    }
                    printf("%d Exit\n",n+1);
                    printf("Quale password vuoi modificare?\n> ");
                    scanf("%d",&scelta);
                    while(getchar() != '\n');
                    if(scelta==n+1){
                        break;
                    }
                    if(scelta<1 || scelta>n+1){
                        printf("Inserimento non valido riprova..\n");
                        sleep(1);
                        system("clear");
                    }else{
                        char passwordNuova[MAXDIM];
                        system("clear");
                        stampa_banner();
                        printf("Inserisci la nuova password: ");
                        fgets(passwordNuova,sizeof(passwordNuova),stdin);
                        passwordNuova[strcspn(passwordNuova, "\n")] = '\0';
                        strcpy(arr[scelta-1].password,passwordNuova);
                        sodium_memzero(passwordNuova,sizeof(passwordNuova));
                        FILE *filePass=fopen("pass.bin","wb");
                        if(!filePass){
                            printf("Impossibile aprire il file\n");
                            break;
                        }
                        for(int i=0;i<n;i++){
                            unsigned char nonce_new[crypto_secretbox_NONCEBYTES];
                            randombytes_buf(nonce_new,sizeof(nonce_new));
                            int len = strlen(arr[i].password)+1;
                            unsigned char cipher_new[crypto_secretbox_MACBYTES+len];
                            crypto_secretbox_easy(cipher_new,(unsigned char*)arr[i].password,len,nonce_new,key);
                            fwrite(&len,sizeof(int),1,filePass);
                            fwrite(nonce_new,1,sizeof(nonce_new),filePass);
                            fwrite(cipher_new,1,crypto_secretbox_MACBYTES+len,filePass);
                        }
                        printf("Modifica avvenuta con successo!!\n");
                        sleep(1);
                        system("clear");
                        fclose(filePass);
                    }
                }
                break;
            case 3:
                break;
            default:
                printf("Inserimento non valido riprova..\n");
                sleep(1);
                system("clear");
                break;
        }
    }
}

void eliminaPassword(){
    FILE *fileNomi=fopen("nomi.txt","r");
    FILE *filePass=fopen("pass.bin","rb");
    if(!fileNomi){
        printf("Impossibile aprire il file\n");
        return;
    }
    if(!filePass){
        printf("Impossibile aprire il file\n");
        return;
    }
    int scelta=-1;
    credenziali *arr=NULL;
    char nome[MAXDIM];
    char password[MAXDIM];
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    int pw_len;
    int n=0;
    while(fgets(nome,sizeof(nome),fileNomi)){
        nome[strcspn(nome, "\n")] = '\0';
        if(fread(&pw_len,sizeof(int),1,filePass)!=1) break;
        if(fread(nonce,1,sizeof(nonce),filePass)!=sizeof(nonce)) break;
        unsigned char cipher[crypto_secretbox_MACBYTES + pw_len];
        if(fread(cipher,1,crypto_secretbox_MACBYTES + pw_len,filePass)!=(size_t)(crypto_secretbox_MACBYTES+pw_len)) break;
        if(crypto_secretbox_open_easy((unsigned char*)password,cipher,crypto_secretbox_MACBYTES+pw_len,nonce,key)!=0){
            printf("Errore decifratura password %s\n",nome);
            continue;
        }
        arr=realloc(arr,(n+1)*sizeof(credenziali));
        strcpy(arr[n].nome,nome);
        strcpy(arr[n].password,password);
        n++;
    }
    fclose(fileNomi);
    fclose(filePass);
    while(scelta!=n+1){
        system("clear");
        stampa_banner();
        printf("Lista Password:\n");
        for(int i=0;i<n;i++){
            printf("%d %s\n", i+1, arr[i].nome);
        }
        printf("%d Exit\n",n+1);
        printf("Quale password vuoi eliminare?\n> ");
        scanf("%d",&scelta);
        while(getchar() != '\n');
        if(scelta==n+1){
            break;
        }
        if(scelta<1 || scelta>n+1){
            printf("Inserimento non valido riprova..\n");
            sleep(1);
            system("clear");
        }else{
            arr[scelta-1]=arr[n-1];
            n--;
            arr=realloc(arr,n*sizeof(credenziali));
            FILE *fileNomi=fopen("nomi.txt","w");
            FILE *filePass=fopen("pass.bin","wb");
            for(int i=0;i<n;i++){
                fprintf(fileNomi,"%s",arr[i].nome);
                if(i!=n-1){
                    fprintf(fileNomi,"\n");
                }
            }
            if(!filePass){ printf("Errore apertura pass.bin\n"); break; }
            for(int i=0;i<n;i++){
                unsigned char nonce_new[crypto_secretbox_NONCEBYTES];
                randombytes_buf(nonce_new,sizeof(nonce_new));
                int len=strlen(arr[i].password)+1;
                unsigned char cipher_new[crypto_secretbox_MACBYTES+len];
                crypto_secretbox_easy(cipher_new,(unsigned char*)arr[i].password,len,nonce_new,key);
                fwrite(&len,sizeof(int),1,filePass);
                fwrite(nonce_new,1,sizeof(nonce_new),filePass);
                fwrite(cipher_new,1,crypto_secretbox_MACBYTES+len,filePass);
            }
            printf("Eliminazione avvenuta con successo!!\n");
            sleep(1);
            system("clear");
            fclose(fileNomi);
            fclose(filePass);
        }
    }

}

void reset(){
    FILE *fileUtente=fopen("passwordUtente.bin","wb");
    if(!fileUtente){
        printf("Impossibile aprire il file\n");
        return;
    }
    fclose(fileUtente);
    FILE *fileNomi=fopen("nomi.txt","w");
    if(!fileNomi){
        printf("Impossibile aprire il file\n");
        return;
    }
    fclose(fileNomi);
    FILE *filePass=fopen("pass.bin","wb");
    if(!filePass){
        printf("Impossibile aprire il file\n");
        return;
    }
    fclose(filePass);
    inizializzazionePasswordManager();
}

void menu(){
    if (sodium_init() < 0) {
        printf("Errore inizializzazione libsodium\n");
        return;
    }

    char pass[MAXDIM] = "";
    FILE *file = fopen("passwordUtente.bin", "rb");
    if (!file) {
        printf("Impossibile aprire il file\n");
        return;
    }

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    if (fread(nonce, 1, sizeof(nonce), file) != sizeof(nonce)) {
        printf("Errore lettura nonce\n");
        fclose(file);
        return;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    long cipher_size = file_size - crypto_secretbox_NONCEBYTES;
    fseek(file, crypto_secretbox_NONCEBYTES, SEEK_SET);

    unsigned char *cipher = malloc(cipher_size);
    if (!cipher) {
        printf("Errore allocazione memoria\n");
        fclose(file);
        return;
    }

    if (fread(cipher, 1, cipher_size, file) != (size_t)cipher_size) {
        printf("Errore lettura cipher\n");
        free(cipher);
        fclose(file);
        return;
    }
    fclose(file);
    while (1) {
        stampa_banner();
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        printf("Inserisci la password: ");
        fgets(pass, sizeof(pass), stdin);
        pass[strcspn(pass, "\n")] = '\0';

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        init_key(pass);
        unsigned char passwordDecifrata[MAXDIM];
        memset(passwordDecifrata, 0, sizeof(passwordDecifrata));
        if (crypto_secretbox_open_easy(passwordDecifrata, cipher, cipher_size, nonce, key) == 0) {
            printf("\nPassword corretta!\n");
            sodium_memzero(passwordDecifrata, sizeof(passwordDecifrata));
            break;
        } else {
            printf("\nPassword errata, riprova!\n");
        }
        sodium_memzero(pass, sizeof(pass));
        sleep(1);
        system("clear");
    }

    sodium_memzero(pass, sizeof(pass));
    sodium_memzero(cipher, cipher_size);
    free(cipher);

    sleep(1);
    system("clear");
    int scelta=-1;
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    while(scelta!=6){
        stampa_banner();
        printf("USER: %s\n\n", pw->pw_name);
        printf("Digita:\n1 - Visualizza Password\n2 - Aggiungi Password\n3 - Modifica Password\n4 - Elimina Password\n5 - Reset Completo\n6 - Exit\n> ");
        char buf[10];
        fgets(buf, sizeof(buf), stdin);
        scelta = atoi(buf);
        switch(scelta){
            case 1:
                visualizzaPassword();
                system("clear");
                break;
            case 2:
                aggiungiPassword();
                sleep(1);
                system("clear");
                break;
            case 3:
                modificaPassword();
                sleep(1);
                system("clear");
                break;
            case 4:
                eliminaPassword();
                sleep(1);
                system("clear");
                break;
            case 5:
                reset();
                sleep(1);
                system("clear");
                break;
            case 6:
                printf("GOODBYE!\n");
                sleep(1);
                system("clear");
                exit(EXIT_SUCCESS);
            default:
                printf("Riprova");
                sleep(1);
                system("clear");
                break;
        }
    }
}