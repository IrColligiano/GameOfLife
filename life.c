// grandezza per allocazione stringhe
#define PATHLEN 256 
// grandezza massima stringa input
#define INPUTLEN 101
// numero di secondi da aspettare tra una visualizzazione e l' altra
#define SECOND 1

#include <math.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>


/////////////////////////////////////////// variabili globali /////////////////////////////////////

// variabili globali di riga e colonna
int rig=0;
int col=0;
// variabile globale del numero di generazioni
int state=0;
//variabile globale indice riempimento
double perc=0;
// matrice con allocazione dinamica
int ** matrice=NULL;
//matrice di supporto con allocazione dinamica
int ** matrice_supp=NULL;

///////////////////////////////////////// prototipi funzioni/////////////////////////////////////

//verifica se il numero inserito e' intero
//return 1 se e' intero
//return 0 altrimenti
int is_int(double a);

//print di riga della griglia
void print_riga_griglia();

// stampa la matrice della prossima generazione
void print_stage(int w, int ** mat_supp);

//stampa la matrice originaria generata
void print_prima_matrice(int ** mat);

//conta il numero di celle vive vicine alla cella mat[r][c] e ritorna il contatore
int celle_vive_vicine(int **mat, int r, int c);

// calcola il numero di elementi che saranno vivi nella matrice a partire dall' indice di riempimento
int  perc_int_su_matrice();

// controlla se un file e' regolare oppure no
// return 0 se il file non e' regolare
// rerurn 1 se il file e' regolare
int file_regolare(const char *path);

//controlla se il file esiste
// return 0 se il file non esiste
// return 1 se il file esiste
int file_exist(const char* path);

// funzione per il controllo di scelte binarie
//int scelta_binaria(float scan_reale);

//funzione per controlli sulle stringhe
int scelta_str(char * path);

//funzione per controlo sulla variabile state 
int scelta_state(char * s );

// scelta per il controllo sulla riga
int scelta_rig(char * s);

//scrive nel file passato alla funzione l' ultimo stato della matrice
void scrittura_file(FILE ** fp , char *pathname_out, int ** mat);

//prende una stringa e controlla se e' un numero intero
int is_number_int(char * s);

// prende una stringa e controlla se e' un float
int is_number_float(char * s);

// prende una stringa e controlla l input per fare la scelta [1] o [2]
int scelta_binaria(char * s);

// prende una stringa e la percentuale intera 
int scelta_perc(char * s, int * perc_int);

// rilascia la memoria dinamica allocata dalle matrici
void free_matrice();

void genera_matrice(int p, int ***mat);

///////////////////////////////////////////main()//////////////////////////////////////////////////////
int main(int argc, char ** argv){
	//variabili per le scelte binarie
	int switch_=0;
	int switch_2=0;
	//variabili per i while
	int opt1=0;
	int opt2=0;
	int opt3=0;
	int opt=0;
	//variabile per errori 
	int err=0;
	//puntatore a file
	FILE *fp=NULL;
	//buffer per scrittura
	int buff=0;
	//indici per i for
	int i,j;
	//variabile che indica il numero di celle vive
	int celle_vive=0;
	//variabile che indica il numero di celle vive alla generazione
	int perc_int=0;
	// stringa contenete file in ingresso
	char pathname_in[PATHLEN];
	// stringa contenente file in uscita
	char pathname_out[PATHLEN];
	
	//puntatore a matrice che mi serve per fare la free della matrice di supporto
	int ** dealloca_matrice=NULL;
	// stringa per input
	char s[INPUTLEN];
	

////////////////////////////////////////avvio game of life///////////////////////////////////////////////
	printf("\nMENU' GAME OF LIFE\n\n");
	while(!opt){
		printf("Scegliere tra le seguenti opzioni:\n");
		printf("[1] Se vuoi caricare la griglia da file inserisci '1'\n");
		printf("[2] Se voui iniziare una nuova generazione inserisci '2'\n");
		scanf("%s",s);
		switch_=scelta_binaria(s);
		if(switch_==1){
			printf("Caricamento da file\n");
			while(!opt1){
			    opt=1;
				printf("Specificare il nome del file ed il suo percorso (Max 255 caratteri):\n");
				scanf("%s",pathname_in);
				switch_2 =scelta_str(pathname_in);
				if(switch_2 == 1){
					if(file_exist(pathname_in)==0 && file_regolare(pathname_in)==0){
						printf("Il file inserito non esiste oppure non e' un file regolare, riprovare\n");
					}
					else{
						printf("Inserimento corretto del file\n");
						printf("Hai inserito il file di nome : %s\n", pathname_in );
						opt1=1;
						while(!opt2){
							printf("Inserire il numero di generazioni:(Strettamente maggiore di 0)\n");
							scanf("%s",s);
							switch_2=scelta_state(s);
							if(switch_2){
								opt2=1;
								}
							}
						}
					}
				}
			}
		if(switch_ == 2){
			opt=1;
			printf("Nuova generazione\n");
			while(!opt1){
				printf("Inserire la dimensione del lato della matrice quadrata da generare:(Strettamente maggiore di 0)\n");
				scanf("%s",s);
				switch_2=scelta_rig(s);
				if(switch_2){
					opt1=1;
					while(!opt2){
						printf("Inserire il numero di generazioni:(Strettamente maggiore di 0)\n");
						scanf("%s",s);
						switch_2=scelta_state(s);
						if(switch_2){
							opt2=1;
							while(!opt3){
								printf("Inserire la percentuale di riempimento iniziale della matrice da generare\n");
								printf("Deve essere un numero appartenete all' insieme [0,1)\n");
								scanf("%s",s);
								switch_2=scelta_perc(s,&perc_int);
								if(switch_2)
									opt3=1;
							}
						}
					}
				}
			}
		}
	}

	// caso in cui la generazione viene presa da file
	if(switch_ == 1){
		if ((fp=fopen(pathname_in,"r"))==NULL){
			fprintf(stderr, "Errore fopen %s\n",pathname_in);
			exit(EXIT_FAILURE);
		}
		double a;

		if((err=fscanf(fp,"%lf", &a))==1){ // voglio leggere un carattere che sia intero
			if( is_int(a) && a>0){
				rig=a;
				col=rig;
			}
			else{
				printf("Il contenuto del file non corrisponde con le specifiche\n");
				printf("L'esecuzione termina\n");
				return 0;
			}
		}
		if(err==EOF){
			printf("Il file passato risulta vuoto, l' esecuzione termina\n");
			return 0;
		}
		if(err==0){
			printf("Il contenuto del file non corrisponde con le specifiche, leggere il Manuale\n");
			printf("L'esecuzione termina\n");
			return 0;
		}

		// alloco memoria dinamica per matrice e matrice di supporto;
		matrice = (int **)malloc(rig * sizeof(int*));
		matrice_supp = (int **) malloc (rig * sizeof(int*));
		if(matrice==NULL || matrice_supp==NULL){
			fprintf(stderr, "Errore malloc\n");
			exit(EXIT_FAILURE);
		}
		for(i = 0; i < rig; i++){
			matrice[i] = (int *)malloc(col * sizeof(int));
			matrice_supp[i] = (int *)malloc(col * sizeof(int));
			if(matrice[i]==NULL || matrice_supp[i]==NULL){
				fprintf(stderr, "Errore malloc\n");
				exit(EXIT_FAILURE);
			}
		}

		//riempio la matrice con -1
		for(i=0; i < rig; i++){
				for(j=0; j < col; j++ ){
					matrice[i][j]=-1;
				}
			}
		
		int ok=1;
		//scrivo nel file leggendo un intero alla volta
		for(i=0; i < rig && ok; i++){
			for(j=0; j < col && ok; j++){
				if((err=fscanf(fp,"%d", &buff)) == 1){
					if(buff==0 || buff==1){
						matrice[i][j]=buff;
					}
					else{
						printf("Il contenuto del file non corrisponde con le specifiche, leggere il Manuale\n");
						printf("L'esecuzione termina\n");
						free_matrice();
						exit(EXIT_FAILURE);
					}
				}
				if(err==EOF && !(j==col) && !(i==rig)){ // il file e' terminato prima di leggere l'intera matrice
					ok=0;
				}
				if(err==0){ // ho letto un carattere, che non voglio leggere, file in ingrsso compromesso
					printf("Il contenuto del file non corrisponde con le specifiche, leggere il Manuale\n");
					printf("L'esecuzione termina\n");
					free_matrice();
					exit(EXIT_FAILURE);
				}
			}
		}
		fclose(fp);
		if(ok==0){
			printf("La matrice contenuta nel file non e' completa\n");
			printf("La matrice verra' completata randomicamente\n");
			srand(time(NULL));
			for(i=0;i<rig;i++){
				for(j=0;j<col;j++){
					if(matrice[i][j]==-1){
						matrice[i][j]=( rand() %2);
					}
				}
			}
		}
		print_prima_matrice(matrice);

	}

	// caso in cui la matrice viene generata
	if(switch_ == 2){
		// alloco memoria dinamica per matrice e matrice di supporto;
		if((matrice = (int **)malloc(rig * sizeof(int*)))==NULL){
			fprintf(stderr, "Errore malloc\n");
			exit(EXIT_FAILURE);
		}
		if((matrice_supp = (int **) malloc (rig * sizeof(int*)))==NULL){
			fprintf(stderr, "Errore malloc\n");
			exit(EXIT_FAILURE);
		}
		for(i = 0; i < rig; i++){
			if((matrice[i] = (int *)malloc(rig * sizeof(int)))==NULL){
			fprintf(stderr, "Errore malloc\n");
			exit(EXIT_FAILURE);
			}
			if((matrice_supp[i] = (int *)malloc(rig * sizeof(int)))==NULL){
				fprintf(stderr, "Errore malloc\n");
				exit(EXIT_FAILURE);
			}
		}
		// riempimento matrice con 0
		for(i=0; i < rig; i++){
			for(j=0; j < col; j++ ){
				matrice[i][j]=0;
			}
		}
	
		//////////////// metodo di genrazione e swap di indici//////////////////////
		
		
//////////////// metodo di generazione ripetendo il ciclo/////////////////
		printf("\n%d\n\n",rig);
		int num=0;
		int dim= rig*col;
		int rapp=perc_int;
		while(perc_int>0){ // rinizio a scorrere la matrice finche' non ho raggiunto 0 numeri da inserire
			srand(time(NULL)); // cambio seme ogni volta che rinizio la matrice
			for(i=0;i<rig && perc_int>0;i++){
				for(j=0;j<col && perc_int>0;j++){
					num= rand() % dim; // 
					if(matrice[i][j]==0 && num < rapp ){ // controllo che la cella sia 0 ed il numero casuale sia minore di rapp
						matrice[i][j]=1;  // inserisco 1 nella cella
						perc_int--; // diminuisco il numero di celle mancanti da inserire
					}
				}
			}
		}
		///////////versione con generazione direttamente su matrice///////////////////
		
		// inserisco 1 in posizioni pseudorandomiche della matrice
		/*
		int ind_rig=0;
		int ind_col=0;
		while(perc_int>0){
			srand(time(NULL));
			ind_rig= rand() % rig;
			ind_col= rand() % col;
			if(matrice [ind_rig][ind_col] == 0){
				matrice [ind_rig][ind_col] = 1;
				perc_int--;
			}
		}
		*/
		print_prima_matrice(matrice);
	}
  /*Qualsiasi cella viva con meno di due celle vive adiacenti muore, come per effetto d’isolamento;
 	Qualsiasi cella viva con due o tre celle vive adiacenti sopravvive alla generazione successiva;
 	Qualsiasi cella viva con pi `u di tre celle vive adiacenti muore, come per effetto di sovrappopolazione;
 	Qualsiasi cella morta con esattamente tre celle vive adiacenti diventa una cella viva, come per effetto
	di riproduzione.”*/
	// inizio a creare le generazioni future
	for(int w=0 ; w<state ; w++){
		for(i=0; i<rig; i++){
			for(j=0;j<col;j++){
				celle_vive = celle_vive_vicine(matrice,i,j);
				if(matrice[i][j]==1 && (celle_vive==2 || celle_vive==3)){ // controllo che una cella viva abbia 2 o 3celle vive vicine
					matrice_supp[i][j]=1; // quindi la cella rimane viva
				}
				else if(matrice[i][j]==0 && celle_vive==3){ // controllo che una cella morta abbia 3 celle vicine vive 
					matrice_supp[i][j]=1; // quindi la cella rinasce
					}
				else{ // questo else sono il resto dei casi ovvero, in cui la cella muore con piu' di 3 vicine vive , la cella muore con meno di 2 celle vive vicine
					matrice_supp[i][j]=0; 
				}
			}
		}
		dealloca_matrice=matrice;
		matrice=matrice_supp; // faccio puntare matrice alla matrice di supporta
		matrice_supp=dealloca_matrice;
		sleep(SECOND);
		print_stage(w+1,matrice);
	}
	printf("\nGAME OVER\n\n");
	opt=0;
	opt1=0;
	opt2=0;;
	while(!opt){
		printf("Vuoi salvare lo stato della griglia in un file?\n");
		printf("[1] Si inserisci '1'\n");
		printf("[2] No inserisci '2'\n");
		scanf("%s",s);
		switch_=scelta_binaria(s);
		if(switch_==1){
			opt=1;
			printf("Salvare lo stato della griglia in un file\n");
			while(!opt1){
				printf("Indicare il nome del file in cui salvare la griglia (Max 255 caratteri)\n");
				scanf("%s",pathname_out);
				switch_2=scelta_str(pathname_out);
				if(switch_2){
					opt1=1;
					while(!opt2){
						if(file_exist(pathname_out)==1 && file_regolare(pathname_out)==1){
							printf("Il file esiste gia' ed e' un file regolare, Lo si vuole sovrascrivere?\n");
							printf("[1] Si, sovrascrivere il file\n");
							printf("[2] No, non sovrascrivere il file\n");
							scanf("%s",s);
							switch_2 =scelta_binaria(s);
							if(switch_2 ==1 ){
								opt2=1;
								printf("Si, sovrascrivere il file\n");
								printf("Il file : %s verra' sovrascritto\n",pathname_out);
								scrittura_file(&fp,pathname_out,matrice);
							}		
							if(switch_2 == 2 ){
								opt2=1;
								printf("No, non sovrascrivere il file\n");
							}
						}
						else{
							opt2=1;
							printf("Il file non esiste oppure e' un file non regolare, ne verra' creato uno nuovo\n");
							scrittura_file(&fp,pathname_out,matrice);
						}
					}
				}
			}
		}			
		if(switch_ == 2){
			opt=1;
			printf("Non salvare lo sato della griglia in un file\n");
			printf("Lo stato della matrice andra' perso\n");
		}
	}
	printf("\nPer giocare ancora lancia %s\n\n", argv[0]);
	// dealloco la memoria dinamica allocata
	free_matrice();
	return 0;
}

////////////////////////////////////// funzioni //////////////////////////////////////////////////////////
/*
void genera_matrice(int p, int ***mat){
		int i=0;
		int arrig[rig];
		int arcol[col];
		int swap=0;
		int ranrig=0;
		int rancol=0;
		for(i=0;i<rig ; i++){
			arrig[i]=i;
			printf("%d |",arrig[i]);
		}
		printf("\n");
		for(i=0;i<rig ; i++){
			arcol[i]=i;
			printf("%d |",arcol[i]);
		}
		printf("\n");
		printf("rig %d col %d i %d\n", rig, col, i);
		while(p > 0){
			srand(time(NULL));
			swap=rig-i;
			ranrig= i+ (rand() %(swap));
			swap=col-i;
			rancol= i+ (rand() %(swap));
			printf("%d,%d |\n",ranrig,rancol);
			*mat[ranrig] [rancol] = 0;
			print_prima_matrice(matrice);
			swap=arrig[ranrig];
			arrig[ranrig]=arrig[i];
			arrig[i]=swap;
			swap=arcol[rancol];
			arcol[rancol]=arcol[i];
			arcol[i]=swap;
			i++;
			p--;
		}
	}
*/
void free_matrice(){
	for(int i=0; i<rig ;i++){
		free(matrice[i]);
		free(matrice_supp[i]);
	}
	free(matrice);
	free(matrice_supp);
}

int is_number_int(char * s){
	int i;
	int ok=1;
	size_t len=strnlen(s,INPUTLEN);
	if(len==INPUTLEN && s[INPUTLEN-1]!='\0'){
		printf("Input troppo lungo, riprovare");
		return 0;
	}
	for(i=0; s[i]!='\0' && i< len && ok ; ++i){
		if( !(s[i] >= '0' && s[i] <='9' ) )
			ok=0;
	}
	if(ok)
		return 1;
	else 
		return 0;
}

int is_number_float(char * s){
	int i;
	int ok =0;
	int ok2= 0;
	size_t len=strnlen(s,INPUTLEN);
	if(len==INPUTLEN && s[INPUTLEN-1]!='\0'){
		printf("Input troppo lungo, riprovare");
		return 0;
	}
	for(i=0; s[i]!='\0' && i< len && ok && ok2<2 ; ++i){
		if(!(s[i] >= '0' && s[i] <='9'))
			ok=0;
		if(s[i] == '.')
			ok2++;
	}
	if(ok && ok2 < 2)
		return 1;
	else 
		return 0;
}


int scelta_str(char * path){
	size_t pathname_len=strnlen(path,PATHLEN);
	if(pathname_len<PATHLEN){
		printf("Inserimento corretto del file\n");
		printf("Hai inserito il nome : %s\n", path );
		return 1;
	}
	if(pathname_len==PATHLEN && path[PATHLEN]!='\0'){
		printf("Nome file troppo lungo, riprovare l' inserimento\n");
		return 0;
	}
	if(pathname_len>PATHLEN){
		printf("Nome file troppo lungo, riprovare l' inserimento\n");
		return 0;
	}
	if(pathname_len==0){
		printf("Nome file non inserito, riprovare l'inserimento\n");
		return 0;
	}
	return 0;
}

void scrittura_file(FILE ** fp , char *pathname_out ,int ** mat){
	if ((*fp=fopen(pathname_out,"w"))==NULL){
		fprintf(stderr, "Errore fopen %s\n",pathname_out);
		free_matrice();
		exit(EXIT_FAILURE);
	}
	int i,j;
	if(fprintf(*fp,"%d%s",rig,"\n")==EOF){
		fprintf(stderr, "Errore scrittura file %s\n",pathname_out);
		free_matrice();
		exit(EXIT_FAILURE);
	}
	for(i=0; i < rig; i++){
		for(j=0; j < col; j++){
			if(fprintf(*fp,"%d%c",mat[i][j],' ')==EOF){
				fprintf(stderr, "Errore scrittura file %s\n",pathname_out);
				free_matrice();
				exit(EXIT_FAILURE);
			}
		}
		if(fprintf(*fp,"%s","\n")==EOF)	{
			fprintf(stderr, "Errore scrittura file %s\n",pathname_out);
			free_matrice();
			exit(EXIT_FAILURE);
		}
	}
	fclose(*fp);
	return;
}

int scelta_perc(char * s, int * perc_int){
	double check=atof(s);
	if(( !is_number_float(s)) &&( check< 0 || check >=1)){
		printf("Inserimento non corretto numero non compreso in [0,1), riprovare\n");
		return 0;
	}
	else{
		perc=check;
		printf("La matrice verra' riempita con l'indice di riempimento %f\n",perc);
		*perc_int=perc_int_su_matrice();
		printf("Numero di elementi che saranno vivi nella matrice %d\n",*perc_int);
		return 1;
	}
}

int scelta_rig(char * s){
	int check=atoi(s);
	if(( !is_number_int(s)) && check<= 0){
		printf("La Granzezza inserita non e' strettamente maggiore di 0, riprovare\n");
		return 0;
	}
	else{
		rig=check;
		col=rig;
		printf("Verra' generata una matrice %d*%d\n",rig,col);
		return 1;
	}
}

int scelta_state(char * s){
	int check=atoi(s);
	if(( !is_number_int(s)) && check<= 0){
		printf("Il numero di generazioni inserito non e' strettamente maggiore di 0 oppure non e' un numero intero,riprovare\n");
		return 0;
	}
	else{
		state=check;
		printf("Verranno eseguite %d generazioni\n",state);
		return 1;
	}
}

int scelta_binaria(char * s){
	if(! is_number_int(s) ){
		printf("Numero non riconosciuto tra le opzioni, riprovare\n");
		return 0;
	}	
	int check=atoi(s);
	if(check==1){
		printf("Hai scelto l' opzione [1]: ");
		return 1;
		}
	if(check==2){
		printf("Hai scelto l' opzione [2]: ");
		return 2;
	}
	printf("Numero non riconosciuto tra le opzioni, riprovare\n");
	return 0;
}

int is_int(double a){
	int b= (int) a;
	a=a-b;
	if(a==0)
		return 1;
	else
		return 0;
}

void print_riga_griglia(){
	printf("\n");
	for(int i=0; i<col; i++)
        printf(" ---");
	printf("\n");
}

void print_stage(int w,int ** mat){
	int i ,j;
	printf("\nGenerazione numero %d\n",w);
    print_riga_griglia();
	for(i=0; i<rig; i++){
		printf("|");
		for(j=0;j<col;j++){
			//printf(" %d |",mat[i][j]);
			if(mat[i][j]==1)
				printf(" %c |",'#');
			else
				printf(" %c |",' ');
		}
		print_riga_griglia();
	}
}

void print_prima_matrice(int ** mat){
	int i,j;
	printf("\nSTART GAME OF LIFE\n");
	printf("\nGenerazione di partenza:\n");
	print_riga_griglia();
	for(i=0; i<rig; i++){
		printf("|");
		for(j=0;j<col;j++){
			//printf(" %d |",mat[i][j]);
			if(mat[i][j]==1)
				printf(" %c |",'#');
			else
				printf(" %c |",' ');
		}
		print_riga_griglia();
	}
}

int celle_vive_vicine(int **mat, int r, int c){
	int i, j, count=0;
	for(i=r-1 ; i<=r+1 ; i++){
		for(j=c-1 ; j<=c+1 ; j++){
			if((i==r && j==c) || (i<0 || j<0) || (i>=rig || j>=col)){
				continue;
			}
			if(mat[i][j] == 1){
				count++;
			}
		}
	}
	return count;
}

int  perc_int_su_matrice(){
	float perc_reale=0;
	float num_el_reale=0;
	int perc_int=0;
	int dim=rig*col;
	perc_reale=perc;
	num_el_reale=dim*perc_reale;
	perc_int=(int)num_el_reale;
	return perc_int;
}

int file_regolare(const char *path){
    struct stat pathstat;
    stat(path, &pathstat);
    if(S_ISREG(pathstat.st_mode)==0)
		return 0;
	else
		return 1;
}

int file_exist(const char* path){
    FILE *file;
    if ((file = fopen(path, "r"))!=NULL){
        fclose(file);
        return 1;
    }
    else{
		return 0;
    }
}