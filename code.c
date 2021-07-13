#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** STRUCTURES **/
struct Client
{
    int ID;
    char Nom[30];
    char Prenom[30];
    char RS[30];
};
struct Produit
{
    int Code;
    char Designation[50];
    double PrixUnitaireHT;
    int Quantite;
    float TVA;
};
struct Facture
{
    int Num_Facture;
    int ID_Client;
    double Total_HT, Total_TVA, Total_TTC;
    int Mois, Annee;
    struct Produit produits[20];
};

/** variables Globales **/
int NBR_FACTURE = 0;
int NBR_CLIENT = 0;
int NBR_PRODUIT = 0;
struct Client clients[20];
struct Produit produits[20];
struct Facture factures[20];

/** FONCTIONS **/
void append_to_file(char file_name[], char line[]);
/*
        1 pour client
        2 pour produit
        3 pour facture
*/
void write_to_file(char file_name[], int tab_id);
void tri_clients();
// client
char* Client_line(struct Client c);
void Create_Client();
void get_clients();
void list_des_clients();
int client_existe(int id);
void supprimer_client();
void afficher_client_entet();
// produit
char* Produit_line(struct Produit p);
void Create_Produit();
void get_produits();
void list_des_produits();
void tri_produits();
int produit_existe(int code);
bool produit_dispo(int code);
void disponibiliter_du_produit();
bool qnt_existante(int code, int qnt);
void supprimer_produit();
void destocker();
void afficher_produit(struct Produit p);
void afficher_produit_entet();
// facture
char* Facture_line(struct Facture f);
void Create_facture();
void get_factures();
void tri_factures();
void list_des_factures();
void list_des_factures_par_client();
void list_des_factures_par_date();
void etat_de_caisse();
void detail_facture();
void afficher_facture(struct Facture f);
void afficher_facture_entet();

/** MAIN **/
void main(){
    int fonctionalite;
    get_clients();
    get_produits();
    get_factures();
    do{
        system("clear"); /// cls if in windows
        printf("\n************************************\n");
        printf("1) Creer un client\n");
        printf("2) List des clients\n");
        printf("3) Supprimer un client\n");
        printf("4) Creer un produit\n");
        printf("5) List des produits\n");
        printf("6) Supprimer un produit\n");
        printf("7) Disponibiliter d'un produit\n");
        printf("8) Destoker (vider le stock des produits)\n");
        printf("9) Creer une facture\n");
        printf("10) Details de facture\n");
        printf("11) List des factures\n");
        printf("12) List des factures par client\n");
        printf("13) List des factures par date(mois/annee)\n");
        printf("14) Etat de caisse\n");
        printf("0) Quiter\n");
        printf("\n************************************\n");
        printf("\nsaisir le numero de fonctionalité : \n");
        scanf("%d", &fonctionalite);
        system("clear"); /// cls if in windows
        switch(fonctionalite){
            case 1:
                Create_Client();
                break;
            case 2:
                list_des_clients();
                break;
            case 3:
                supprimer_client();
                break;
            case 4:
                Create_Produit();
                break;
            case 5:
                list_des_produits();
                break;
            case 6:
                supprimer_produit();
                break;
            case 7:
                disponibiliter_du_produit();
                break;
            case 8:
                destocker();
                break;
            case 9:
                Create_facture();
                break;
            case 10:
                detail_facture();
                break;
            case 11:
                list_des_factures();
                break;
            case 12:
                list_des_factures_par_client();
                break;
            case 13:
                list_des_factures_par_date();
                break;
            case 14:
                etat_de_caisse();
                break;
            case 0:
                // exit
                break;
            default:
                printf("\nveuiller sesir un numero de fonction valable \n");
        }
    }while(fonctionalite != 0);
}
/** Fontions **/
void append_to_file(char file_name[], char line[]){
    FILE *fPtr;
    fPtr = fopen(file_name, "a");
    if (fPtr == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open '%s' file.\n", file_name);
        exit(EXIT_FAILURE);
    }
    fputs(line, fPtr);
    fputs("\n", fPtr);
    fclose(fPtr);
}
void write_to_file(char file_name[], int tab_id){
    FILE *fPtr;
    fPtr = fopen(file_name, "w");
    if (fPtr == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open '%s' file.\n", file_name);
        exit(EXIT_FAILURE);
    }
    switch(tab_id){
        // Client
        case 1:
            for(int j=0; j<NBR_CLIENT; j++)
            {
                char client[200];
                strcpy(client,Client_line(clients[j]));
                fputs(client, fPtr);
            }
            break;
        // Produit
        case 2:
            for(int j=0; j<NBR_PRODUIT; j++)
            {
                char produit[200];
                strcpy(produit,Produit_line(produits[j]));
                fputs(produit, fPtr);
                fputs("\n", fPtr);
            }
            break;
        //facture
        case 3:
            for(int j=0; j<NBR_FACTURE; j++)
            {
                char facture[200];
                strcpy(facture,Facture_line(factures[j]));
                fputs(facture, fPtr);
                fputs("\n", fPtr);
            }
            break;
        default:
            printf("\n *********** error ***********\n");
            break;
    }
    fclose(fPtr);
}
void create_or_clear_file(char file_name[]){
    FILE *fPtr;
    fPtr = fopen(file_name, "w");
    if (fPtr == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open '%s' file.\n", file_name);
        exit(EXIT_FAILURE);
    }
    fclose(fPtr);
}
void tri_produits(){
    struct Produit temp_p;
    for(int i=0; i<NBR_PRODUIT-1; i++){
        for(int j=i+1; j<NBR_PRODUIT; j++){
            if(produits[i].Code > produits[j].Code){
                temp_p = produits[i];
                produits[i] = produits[j];
                produits[j] = temp_p;
            }
        }
    }
}
void tri_clients(){
    struct Client temp_c;
    for(int i=0; i<NBR_CLIENT-1; i++){
        for(int j=i+1; j<NBR_CLIENT; j++){
            if(clients[i].ID > clients[j].ID){
                temp_c = clients[i];
                clients[i] = clients[j];
                clients[j] = temp_c;
            }
        }
    }
}
void tri_factures(){
    struct Facture temp_f;
    for(int i=0; i<NBR_FACTURE-1; i++){
        for(int j=i+1; j<NBR_FACTURE; j++){
            if(factures[i].Num_Facture > factures[j].Num_Facture){
                temp_f = factures[i];
                factures[i] = factures[j];
                factures[j] = temp_f;
            }
        }
    }
}
char* Client_line(struct Client c){
    static char client[200];
    char temp_id[10];
    sprintf(&temp_id, "%d", c.ID);
    strcat(strcpy(client, temp_id), " ");
    strcat(client, c.Nom);
    strcat(client, " ");
    strcat(client, c.Prenom);
    strcat(client, " ");
    strcat(client, c.RS);
    return client;
}
void Create_Client(){
    struct Client c1;
    char client[200];
    printf("** Creer un client **\n");
    if(clients[NBR_CLIENT-1].ID != 0)
        c1.ID = clients[NBR_CLIENT-1].ID + 1;
    else
        c1.ID = 1;
    printf("Nom  : ");
    scanf("%s", c1.Nom);
    printf("Prenom  : ");
    scanf("%s", c1.Prenom);
    printf("Raison social  : ");
    scanf("%s", c1.RS);
    strcpy(client,Client_line(c1));
    append_to_file("FClient.txt",client);
    get_clients();
}
void get_clients(){
    FILE *fPtr;
    fPtr = fopen("FClient.txt", "r");
    char chunk[128];
    int i = 0;
    if(fPtr == NULL) {
        perror("Unable to open file!");
        exit(1);
    }
    while(fgets(chunk, sizeof(chunk), fPtr) != NULL) {
        char * token = strtok(chunk, " ");
        sscanf(token,"%d",&clients[i].ID);
        token = strtok(NULL, " ");
        strcpy(clients[i].Nom,token);
        token = strtok(NULL, " ");
        strcpy(clients[i].Prenom,token);
        token = strtok(NULL, " ");
        strcpy(clients[i].RS,token);
        i++;
    }
    NBR_CLIENT = i;
    fclose(fPtr);
    tri_clients();
}
void list_des_clients(){
    int input;
    do{
    system("clear");
    afficher_client_entet();
    for(int j=0;j<NBR_CLIENT;j++){
        printf("\n %d             %s           %s            %s\n",clients[j].ID, clients[j].Nom, clients[j].Prenom, clients[j].RS);
    }
        printf("\n 0 : retourner au menu\n");
        scanf("%d",&input);
    }while(input != 0);
}
int client_existe(int id){
    int existe = -1;
    for(int j=0;j<NBR_CLIENT;j++){
        if(clients[j].ID == id){
            existe = j;
        }
    }
    return existe;
}
void supprimer_client(){
    int id,exist,input;
    do{
        system("clear");
        printf("\nSaisir l'id du client : ");
        scanf("%d",&id);
        exist = client_existe(id);
        if( exist != -1){
            for(int j=exist;j<NBR_CLIENT-1;j++){
                clients[j] = clients[j+1];
            }
            NBR_CLIENT--;
            write_to_file("FClient.txt",1);
        }else{
            printf("\n Client n'existe pas");
        }
        printf("\n 0 : Retour au menu");
        printf("\n 1 : Suprimer un autre client");
        scanf("%d",&input);
    }while(input != 0);
}
char* Produit_line(struct Produit p){
    static char produit[200];
    char temp_id[30];
    sprintf(&temp_id, "%d", p.Code);
    strcat(strcpy(produit, temp_id), " ");
    strcat(produit, p.Designation);
    strcat(produit, " ");
    sprintf(&temp_id, "%lf", p.PrixUnitaireHT);
    strcat(produit, temp_id);
    strcat(produit, " ");
    sprintf(&temp_id, "%d", p.Quantite);
    strcat(produit, temp_id);
    strcat(produit, " ");
    sprintf(&temp_id, "%f", p.TVA);
    strcat(produit, temp_id);
    return produit;
}
void Create_Produit(){
    struct Produit p1;
    static char produit[200];
    printf("** Creer un produit **\n");
    if(produits[NBR_PRODUIT-1].Code != 0)
        p1.Code = produits[NBR_PRODUIT-1].Code + 1;
    else
        p1.Code = 1;
    printf("Designation  : ");
    scanf("%s", p1.Designation);
    printf("Prix Unitaire HT  : ");
    scanf("%lf", &p1.PrixUnitaireHT);
    printf("Quantite  : ");
    scanf("%d", &p1.Quantite);
    printf("TVA  : ");
    scanf("%f", &p1.TVA);
    strcpy(produit,Produit_line(p1));
    append_to_file("FProduit.txt",produit);
    get_produits();
}
void get_produits(){
    FILE *fPtr;
    fPtr = fopen("FProduit.txt", "r");
    char chunk[128];
    int i = 0;
    if(fPtr == NULL) {
        perror("Unable to open file!");
        exit(1);
    }
    while(fgets(chunk, sizeof(chunk), fPtr) != NULL) {
        char * token = strtok(chunk, " ");
        sscanf(token,"%d",&produits[i].Code);
        token = strtok(NULL, " ");
        strcpy(produits[i].Designation,token);
        token = strtok(NULL, " ");
        produits[i].PrixUnitaireHT = atof(token);
        token = strtok(NULL, " ");
        sscanf(token,"%d",&produits[i].Quantite);
        token = strtok(NULL, " ");
        produits[i].TVA = atof(token);
        i++;
    }
    NBR_PRODUIT = i;
    fclose(fPtr);
    tri_produits();
}
void list_des_produits(){
    int input;
    do{
        system("clear");
        afficher_produit_entet();
        for(int j=0;j<NBR_PRODUIT;j++){
            afficher_produit(produits[j]);
        }
        printf("\n 0 : retourner au menu\n");
        scanf("%d",&input);
    }while(input != 0);
}
int produit_existe(int code){
    int existe = -1;
    for(int j=0;j<NBR_PRODUIT;j++){
        if(produits[j].Code == code){
            existe = j;
        }
    }
    return existe;
}
bool produit_dispo(int code){
    int indice;
    bool existe = false;
    indice = produit_existe(code);
    if( indice != -1 ){
        if( produits[indice].Quantite > 0){
                existe = true;
            }
        }
    return existe;
}
void disponibiliter_du_produit(){
    int code,indice,input;
    bool existe = false;
    do{
        system("clear");
        printf("\nSaisir le code du produit : ");
        scanf("%d",&code);
        indice = produit_existe(code);
        if( indice != -1 ){
            if( produits[indice].Quantite > 0){
                existe = true;
                printf("\nQuantite restante : %d",produits[indice].Quantite);
            }else{
                printf("\n Produit expire");
            }
        }else{
            printf("\n Produit n'existe pas");
        }
        printf("\n 0 : Retour au menu");
        printf("\n 1 : Verifier un autre produit");
        scanf("%d",&input);
    }while(input != 0);
}
bool qnt_existante(int code, int qnt){
    bool existe = false;
    for(int j=0;j<NBR_PRODUIT;j++){
        if(produits[j].Code == code && produits[j].Quantite >= qnt){
            existe = true;
        }
    }
    return existe;
}
void supprimer_produit(){
    int code,exist,input;
    do{
        system("clear");
        printf("\nSaisir le code du produit : ");
        scanf("%d",&code);
        exist = produit_existe(code);
        if( exist != -1){
            for(int j=exist;j<NBR_PRODUIT-1;j++){
                produits[j] = produits[j+1];
            }
            NBR_PRODUIT--;
            write_to_file("FProduit.txt",2);
        }else{
            printf("\n Produit n'existe pas");
        }
        printf("\n 0 : Retour au menu");
        printf("\n 1 : Suprimer un autre produit");
        scanf("%d",&input);
    }while(input != 0);
}
void destocker(){
    int confirm;
    printf("\n Voulais vous vraiment vider le stock ? \n 1 : pour confirmer \n 0 : pour retourner au menu");
    scanf("%d", &confirm);
    if(confirm == 1){
        NBR_PRODUIT = 0;
        create_or_clear_file("FProduit.txt");
    }
}
char* Facture_line(struct Facture f){
    static char facture[200];
    char temp[40];
    sprintf(&temp, "%d", f.Num_Facture);
    strcat(strcpy(facture, temp), " ");
    sprintf(&temp, "%d", f.ID_Client);
    strcat(facture, temp);
    strcat(facture, " ");
    sprintf(&temp, "%lf", f.Total_HT);
    strcat(facture, temp);
    strcat(facture, " ");
    sprintf(&temp, "%lf", f.Total_TVA);
    strcat(facture, temp);
    strcat(facture, " ");
    sprintf(&temp, "%lf", f.Total_TTC);
    strcat(facture, temp);
    strcat(facture, " ");
    sprintf(&temp, "%d", f.Mois);
    strcat(facture, temp);
    strcat(facture, " ");
    sprintf(&temp, "%d", f.Annee);
    strcat(facture, temp);
    return facture;
}
void Create_facture(){
    struct Facture F1;
    char facture[200];
    int ajouter_produit = 1, nombre_de_produit = 0, id_client;
    double prix_total_ht = 0, prix_total_tva = 0;
    printf("** Creer une facture **\n");
    if(factures[NBR_FACTURE-1].Num_Facture != 0)
        F1.Num_Facture = factures[NBR_FACTURE-1].Num_Facture + 1;
    else
        F1.Num_Facture = 1;

    do{
        printf("id du client : ");
        scanf("%d", &id_client);
        if(client_existe(id_client) != -1){
            F1.ID_Client = id_client;
        }else{
            printf("\nSe client n'existe pas \n");
        }
    }while(client_existe(id_client) == -1);
    printf("Les produits : \n");
    while(ajouter_produit != 0){
        int code_produit,i_produit,qnt_produit;
        printf("Code du produit : \n");
        scanf("%d", &code_produit);
        i_produit = produit_existe(code_produit);
        if(i_produit != -1){
            if(produit_dispo(code_produit)){
                bool cont_existante;
                do{
                    printf("Quantite : \n");
                    scanf("%d", &qnt_produit);
                    cont_existante = qnt_existante(code_produit, qnt_produit);
                    if(cont_existante){
                        F1.produits[nombre_de_produit] = produits[i_produit];
                        F1.produits[nombre_de_produit].Quantite = qnt_produit;
                        produits[i_produit].Quantite = produits[i_produit].Quantite - qnt_produit;
                        int prix_total_produit =  F1.produits[nombre_de_produit].PrixUnitaireHT * qnt_produit;
                        prix_total_ht = prix_total_ht + prix_total_produit;
                        prix_total_tva = prix_total_tva + ( prix_total_produit * (F1.produits[nombre_de_produit].TVA / 100) );
                        nombre_de_produit++;
                    }else{
                        printf("\n quantite indsponible !!!!");
                        printf("\n il ne reste que %d \n",produits[i_produit].Quantite);
                    }
                }while(!cont_existante);
            }else{
                printf("\n stock vide !!!!");
            }
        }else{
            printf("\n un produit inexistant !!!!");
        }
        printf("1/0 (1 pour ajouter, 0 pour terminer) \n");
        scanf("%d", &ajouter_produit);
    }
    // total ht
    F1.Total_HT = prix_total_ht;
    // total tva
    F1.Total_TVA = prix_total_tva;
    // total ttc
    F1.Total_TTC = prix_total_ht + prix_total_tva;
    printf("Mois  : (ex: jan :1, fev:2 .... dec:12)");
    scanf("%d", &F1.Mois);
    printf("Annee  : (ex: 2021)");
    scanf("%d", &F1.Annee);
    strcpy(facture,Facture_line(F1));
    append_to_file("FFacture.txt",facture);
    write_to_file("FProduit.txt",2);
    get_factures();
}
void get_factures(){
    FILE *fPtr;
    fPtr = fopen("FFacture.txt", "r");
    char chunk[128];
    int i = 0;
    if(fPtr == NULL) {
        perror("Unable to open file!");
        exit(1);
    }
    while(fgets(chunk, sizeof(chunk), fPtr) != NULL) {
        char * token = strtok(chunk, " ");
        sscanf(token,"%d",&factures[i].Num_Facture);
        token = strtok(NULL, " ");
        sscanf(token,"%d",&factures[i].ID_Client);
        token = strtok(NULL, " ");
        factures[i].Total_HT = atof(token);
        token = strtok(NULL, " ");
        factures[i].Total_TVA = atof(token);
        token = strtok(NULL, " ");
        factures[i].Total_TTC = atof(token);
        token = strtok(NULL, " ");
        sscanf(token,"%d",&factures[i].Mois);
        token = strtok(NULL, " ");
        sscanf(token,"%d",&factures[i].Annee);
        i++;
    }
    NBR_FACTURE = i;
    fclose(fPtr);
    tri_factures();
}
void list_des_factures(){
    int input;
    do{
        system("clear");
        afficher_facture_entet();
        for(int j=0;j<NBR_FACTURE;j++){
            afficher_facture(factures[j]);
        }
        printf("\n 0 : retourner au menu\n");
        scanf("%d",&input);
    }while(input != 0);
}
void list_des_factures_par_client(){
    int input, client_id, client_exist;
    do{
        system("clear");
        printf("\nSaisir l'id de du client : ");
        scanf("%d",&client_id);
        client_exist = client_existe(client_id);
        if(client_exist != -1){
            afficher_facture_entet();
            for(int j=0;j<NBR_FACTURE;j++){
                if(factures[j].ID_Client == client_id){
                    afficher_facture(factures[j]);
                }
            }
        }else{
            printf("\nClient n'existe pas");
        }
        printf("\n 0 : retourner au menu\n 1 : pour rechercher un autre client ");
        scanf("%d",&input);
    }while(input != 0);
}
void list_des_factures_par_date(){
    int input, mois, annee, nbr=0;
    do{
        system("clear");
        printf("\nSaisir le mois : ");
        scanf("%d",&mois);
        printf("\nSaisir l'annee : ");
        scanf("%d",&annee);
        afficher_facture_entet();
        for(int j=0;j<NBR_FACTURE;j++){
            if(factures[j].Mois == mois && factures[j].Annee == annee){
                afficher_facture(factures[j]);
                nbr++;
            }
        }
        if(nbr == 0){
            printf("\nAucune facture ce mois ci");
        }
        printf("\n 0 : retourner au menu\n 1 : pour rechercher une autre date ");
        scanf("%d",&input);
    }while(input != 0);
}
void etat_de_caisse(){
    int input, mois_debut, annee_debut, mois_fin, annee_fin;
    double benifice = 0;
    do{
        system("clear");
        printf("\nSaisir le mois de debut : ");
        scanf("%d",&mois_debut);
        printf("\nSaisir l'annee de debut : ");
        scanf("%d",&annee_debut);
        printf("\nSaisir le mois de fin : ");
        scanf("%d",&mois_fin);
        printf("\nSaisir l'annee de fin : ");
        scanf("%d",&annee_fin);
        for(int j=0;j<NBR_FACTURE;j++){
            if(annee_debut > annee_fin){
                printf("\n Date invalide \n");
                break;
            }else if(annee_debut == annee_fin){
                if(mois_debut > mois_fin){
                    printf("\n Date invalide \n");
                    break;
                }else if(factures[j].Mois >= mois_debut && factures[j].Mois <= mois_fin && factures[j].Annee == annee_fin){
                    benifice = benifice + factures[j].Total_HT;
                }
            }else{
                if(factures[j].Annee >= annee_debut && factures[j].Annee <= annee_fin){
                    if(factures[j].Annee == annee_debut && factures[j].Mois >= mois_debut){
                        benifice = benifice + factures[j].Total_HT;
                    }
                    if(factures[j].Annee == annee_fin && factures[j].Mois <= mois_fin){
                        benifice = benifice + factures[j].Total_HT;
                    }
                    if(factures[j].Annee > annee_debut && factures[j].Annee < annee_fin){
                        benifice = benifice + factures[j].Total_HT;
                    }
                }
            }
        }
        printf("\nBenifice = %lf \n",benifice);
        printf("\n 0 : retourner au menu\n 1 : pour rechercher une autre date ");
        scanf("%d",&input);
    }while(input != 0);
}
int facture_existe(int numero){
    int existe = -1;
    for(int j=0;j<NBR_FACTURE;j++){
        if(factures[j].Num_Facture == numero){
            existe = j;
        }
    }
    return existe;
}
void detail_facture(){
    int indice, numero, input;
    do{
        system("clear");
        printf("\nSaisir le numero de la facture (0 pour retourner au menu) : ");
        scanf("%d",&numero);
        if(numero == 0 ){ break; }
        indice = facture_existe(numero);
        if(indice != -1){
            struct Facture f = factures[indice];
            afficher_facture_entet();
            afficher_facture(f);
            printf("\n 0 : retourner au menu\n 1 : pour rechercher une autre facture \n");
            scanf("%d",&input);
        }
    }while(indice == -1 || input != 0);
}
void afficher_facture(struct Facture f){
    printf("\n%d    %d    %lf     %lf    %lf      %d      %d\n",f.Num_Facture,f.ID_Client,f.Total_HT,f.Total_TVA,f.Total_TTC,f.Mois,f.Annee);
}
void afficher_produit(struct Produit p){
    printf("\n%d     %s                         %lf     %d        %f\n",p.Code ,p.Designation ,p.PrixUnitaireHT ,p.Quantite, p.TVA);
}
void afficher_facture_entet(){
    printf("\n=====================================================================\n");
    printf("\n N.FCT  Cl     T_HT         T_TVA        T_TTC      Mois      Annee \n");
    printf("\n=====================================================================\n");
}
void afficher_produit_entet(){
    printf("\n====================================================================\n");
    printf("\n C.P  designation                 HT         Quantite          TVA\n");
    printf("\n====================================================================\n");
}
void afficher_client_entet(){
    printf("\n===================================================\n");
    printf("\n ID             Nom           Prenom            RS \n");
    printf("\n===================================================\n");
}
