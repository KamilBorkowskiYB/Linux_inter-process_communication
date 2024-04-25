
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/wait.h>

sem_t *semafor21;
sem_t *semafor23;
sem_t *semafor32;
sem_t *semaforPocz;//pilnuje by program uruchomil sie po utworzeniu wszystkich procesow potomnych

int *pidkM;//
int *pidk1;//
int *pidk2;//
int *pidk3;//
    int *w;

struct buffer { //struktura wiadomosci w kolejce
	long typ;
	int mesg; 
	int pidNad;
} message; 
sigset_t maska;//
int msgid;

struct sigaction sign;
struct sigaction sign2;
struct sigaction sign3;

void handle_signal(int signum, siginfo_t *si,void *ignored)//handler sygnalow miedzy P3 - PM
{
		//wyslij ten sam sygnal do PM
		if(signum == SIGTSTP)//ctrl + z = stop
		{
			kill(getppid(),SIGUSR1);
		}
		if(signum == SIGCONT)// znow
		{
			kill(getppid(),	SIGABRT);
		}
		if(signum == SIGTERM)// wyjdz
		{
			kill(getppid(),	SIGFPE);
		}
}

void handle_signal2(int signum, siginfo_t *si,void *ignored)//handler sygnalow miedzy PM - KK
{
	if (si->si_pid == *pidk3){
		message.mesg = signum;//tu przypiszemy wartosc sygnalu
			message.pidNad = getpid();//
			message.typ = 1;// 
		msgsnd(msgid, &message, sizeof(message), 0); //wysylanie wiadomosci	x3po kazdym dla procesu potomnego
				message.pidNad = getpid();//
				message.typ = 2;// 
		msgsnd(msgid, &message, sizeof(message), 0);
				message.pidNad = getpid();//
				message.typ = 3;// 
		msgsnd(msgid, &message, sizeof(message), 0);
		//wyslij sygnal_odt do P1
				
				kill(*pidk1,SIGUSR2);//sigusr2 = signal_odczyt///
			
			}
}
int *k;
int *pomk1;
int *pomk2;
int *pomk3; 
int *koniec1;
int *koniec2;
int *koniec3;

void odczyt()
{
	if(message.mesg==10)//ctrl + z
	{
		if(getpid()==*pidk1)
		{
			*pomk1=0;
			//sigprocmask(SIG_UNBLOCK, &maska, NULL);
			kill(*pidk2,SIGUSR2);
			//sigprocmask(SIG_BLOCK, &maska, NULL);
		}
		if(getpid()==*pidk2)
		{
			*pomk2=0;
			kill(*pidk3,SIGUSR2);
		}
		if(getpid()==*pidk3)*pomk3=0;
	}
	
	if(message.mesg==6)//ctrl + '\'
	{
		if(getpid()==*pidk1)
		{
			*pomk1=1;
			kill(*pidk2,SIGUSR2);
		}
		if(getpid()==*pidk2)
		{
			*pomk2=1;
			kill(*pidk3,SIGUSR2);
		}
		if(getpid()==*pidk3)*pomk3=1;
	}

	if(message.mesg==8)//ctrl + c
	{
    if(*k == 0)
    {

    	if(getpid()==*pidk1)
		{
			printf("P1(%d) zakonczono\n", getpid());
			kill(*pidk2,SIGUSR2);
			exit(0);
		}
		if(getpid()==*pidk2)
		{
			printf("P2(%d) zakonczono\n", getpid());
			kill(*pidk3,SIGUSR2);
			exit(0);

		}
		if(getpid()==*pidk3)
		{
			printf("P3(%d) zakonczono\n", getpid());
			exit(0);		
		}
    }
		if(getpid()==*pidk1)
		{
			*koniec1=0;
			kill(*pidk2,SIGUSR2);
		}
		if(getpid()==*pidk2)
		{
			*koniec2=0;
			kill(*pidk3,SIGUSR2);
		}
		if(getpid()==*pidk3)
		{
			*koniec3=0;		
		}
	}
}
void handle_signal3(int signum, siginfo_t *si,void *ignored)//sygnal_odczyt
{
	
			if(getpid()==*pidk1)
			{
				if (si->si_pid == *pidkM){
				msgrcv(msgid, &message, sizeof(message), 1, 0);//
				if(message.pidNad == *pidkM) odczyt();
				else
				{
					while(message.pidNad != *pidkM)msgrcv(msgid, &message, sizeof(message), 1, 0);//
				}
				}
			}
			if(getpid()==*pidk2)
			{
				if (si->si_pid == *pidk1){
				msgrcv(msgid, &message, sizeof(message), 2, 0);//
				if(message.pidNad == *pidkM) odczyt();
				else
				{
					while(message.pidNad != *pidkM)msgrcv(msgid, &message, sizeof(message), 2, 0);//
				}
				}
			}
			if(getpid()==*pidk3)
			{
				if (si->si_pid == *pidk2){
				msgrcv(msgid, &message, sizeof(message), 3, 0);//
				if(message.pidNad == *pidkM) odczyt();
				else
				{
					while(message.pidNad != *pidkM)msgrcv(msgid, &message, sizeof(message), 3, 0);//
				}
			}
			}
}

int *p1_skonczyl;
int *p2_skonczyl;
int main()
{
    pid_t k1, k2, k3;
	int fd[2];//0 czyta z ; 1 pisze do
	if (pipe(fd) == -1) {
        printf("error");
        return 1;
    }

    
    msgctl(msgid, IPC_RMID, NULL);//profilaktyczne czyszczenie kolejki
    
    key_t key; 
	key = ftok("progfile", 65); //generacja klucza
	msgid = msgget(key, 0666 | IPC_CREAT); //podpiecie sie pod kk

    int semafor21_shmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);//tworzenie pamieci wspoldzielonej
    semafor21 = (sem_t *)shmat(semafor21_shmid, NULL, 0);//przypisanie semafora do pamieci
    sem_init(semafor21, 1, 1);//inicjalizacja semafora
    
    int semafor23_shmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);//tworzenie pamieci wspoldzielonej
    semafor23 = (sem_t *)shmat(semafor23_shmid, NULL, 0);//przypisanie semafora do pamieci
    sem_init(semafor23, 1, 0);//inicjalizacja semafora
    
    int semafor32_shmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);//tworzenie pamieci wspoldzielonej
    semafor32 = (sem_t *)shmat(semafor32_shmid, NULL, 0);//przypisanie semafora do pamieci
    sem_init(semafor32, 1, 1);//inicjalizacja semafora

    int semaforPocz_shmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);//tworzenie pamieci wspoldzielonej
    semaforPocz = (sem_t *)shmat(semaforPocz_shmid, NULL, 0);//przypisanie semafora do pamieci
    sem_init(semaforPocz, 1, 0);//inicjalizacja semafora
    
    //dana miedzy p2 - p3
    int w_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    w = (int*)shmat(w_shmid, NULL, 0);//przypisanie buffera do pamieci

    int k_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    k = (int*)shmat(k_shmid, NULL, 0);//przypisanie buffera do pamieci
    *k = 0;


    int koniec1_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    koniec1 = (int*)shmat(koniec1_shmid, NULL, 0);//przypisanie buffera do pamieci
    *koniec1 = 1;

    int koniec2_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    koniec2 = (int*)shmat(koniec2_shmid, NULL, 0);//przypisanie buffera do pamieci
    *koniec2 = 1;

    int koniec3_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    koniec3 = (int*)shmat(koniec3_shmid, NULL, 0);//przypisanie buffera do pamieci
    *koniec3 = 1;

    int pomk1_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    pomk1 = (int*)shmat(pomk1_shmid, NULL, 0);//przypisanie buffera do pamieci
	*pomk1 =1;

	int pomk2_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    pomk2 = (int*)shmat(pomk2_shmid, NULL, 0);//przypisanie buffera do pamieci
	*pomk2 =1;

    int pomk3_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    pomk3 = (int*)shmat(pomk3_shmid, NULL, 0);//przypisanie buffera do pamieci
	*pomk3 =1;
	
	int p1_skonczyl_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    p1_skonczyl = (int*)shmat(p1_skonczyl_shmid, NULL, 0);//przypisanie buffera do pamieci
	*p1_skonczyl =0;

	int p2_skonczyl_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    p2_skonczyl = (int*)shmat(p2_skonczyl_shmid, NULL, 0);//przypisanie buffera do pamieci
	*p2_skonczyl =0;
	
     int pidkM_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    pidkM = (int*)shmat(pidkM_shmid, NULL, 0);//przypisanie buffera do pamieci
    
       int pidk1_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    pidk1 = (int*)shmat(pidk1_shmid, NULL, 0);//przypisanie buffera do pamieci
    
       int pidk2_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    pidk2 = (int*)shmat(pidk2_shmid, NULL, 0);//przypisanie buffera do pamieci
    
      int pidk3_shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);
    pidk3 = (int*)shmat(pidk3_shmid, NULL, 0);//przypisanie buffera do pamieci
  
 

    if ((k1 = fork()) == 0)
    { 
		*pidk1=getpid();///

		sign3.sa_flags = SA_RESTART | SA_SIGINFO;
		sign3.sa_sigaction = handle_signal3;
		sigaction(SIGUSR2,&sign3,NULL);
		sigemptyset(&maska);
		sigaddset(&maska, SIGUSR1);//
		sigaddset(&maska, SIGABRT);//
		sigaddset(&maska, SIGFPE);//
		sigaddset(&maska, SIGCONT);
		sigaddset(&maska, SIGTSTP);
		sigaddset(&maska, SIGTERM);
		sigaddset(&maska, SIGINT);
		sigaddset(&maska, SIGQUIT);
		sigprocmask(SIG_BLOCK, &maska, NULL);  
		printf("Tutaj proces potomny 1(%d)\n",getpid());
		sem_wait(semaforPocz);
		close(fd[0]);//ten proces nie czyta

	    FILE * fp;
		char * line = NULL;
		size_t len = 0;
        int n;

		while(1)
		{
	    if(!*koniec1)
			    {
			    	line = "/0";
			    	write(fd[1],line, strlen(line));
					printf("P1(%d) zakonczono\n",getpid());
					sem_post(semafor32);
					*p1_skonczyl=1;
					exit(0);
			    }
	    *k = 0;
		printf("Wybor wejscia: \n1. Z klawiatury\n2. Z pliku \n");
	    scanf("%d", &n);
	    getchar();
        	    if(!*koniec1)
	    {
			printf("P1(%d) zakonczono\n",getpid());
			line = "";
			write(fd[1], line, strlen(line));
			sem_post(semafor32);
			*p1_skonczyl=1;
			exit(0);
	    }
	    *k = 1;
		if (n == 1) 
		{
            //czytanie z klawiatury
            char buffer[100];
            while (1) 
			{ 	    
			fflush(stdout);
			if(!*koniec1) *w = 0;

				if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
					if (buffer[0] == '.') {
						sem_post(semafor21);
						break; // Zakończ pętlę, jeśli odczytano kropkę
					}
					write(fd[1], buffer, strlen(buffer));
				}
			}
        } 
        if (n == 2)
        { 	
        	fp = fopen("doc", "r");
				while ((getline(&line, &len, fp)) != -1)
				{   
					//sem_wait(semafor21);//tu ladniej wyglada wstrzymanie, chociaz nie najlepiej
					if(*pomk1)
					{
						sem_wait(semafor21);//tu dziala
						//printf("P1: odczytal: %s", line);
						write(fd[1],line, strlen(line));
					}	
				  while(*pomk1 == 0)
				  {}
			    }
			    if(!*koniec1)
			    {
			    	
			    	write(fd[1],line, strlen(line));
			    	close(fd[1]);
					fclose(fp);
					if (line)
					{
						free(line);
					}
					printf("P1(%d) zakonczono\n",getpid());
					sem_post(semafor32);
					*p1_skonczyl=1;
					line = "koniec";
					exit(0);
			    }
        
		}
	}
}
		
 


if ((k2 = fork()) == 0)
    {
		*pidk2=getpid();//
        printf("Tutaj proces potomny 2(%d)\n", getpid());
        close(fd[1]); // Ten proces nie pisze

		sign3.sa_flags = SA_RESTART | SA_SIGINFO;
		sign3.sa_sigaction = handle_signal3;
		sigaction(SIGUSR2,&sign3,NULL);
		sigemptyset(&maska);
		sigaddset(&maska, SIGUSR1);//
		sigaddset(&maska, SIGABRT);//
		sigaddset(&maska, SIGFPE);//
		sigaddset(&maska, SIGCONT);
		sigaddset(&maska, SIGTSTP);
		sigaddset(&maska, SIGTERM);
		sigaddset(&maska, SIGINT);
		sigaddset(&maska, SIGQUIT);
		sigprocmask(SIG_BLOCK, &maska, NULL);
        char buffer[100];
        ssize_t read_len;
        while (1)
        { 
        	
        	
            while(*pomk2)
            {
            	
				read_len = read(fd[0], buffer, sizeof(buffer));//czytanie z pipe
				if (read_len > 0)
				{
					if(!*koniec2 && *p1_skonczyl)
					{
						//close(fd[0]);
						printf("P2(%d) zakonczono\n",getpid());
						*p2_skonczyl=1;
						sem_post(semafor23);
						exit(0);
					}
					if(read_len>1) printf("P2(%d): otrzymal: %.*s", getpid(), (int)read_len, buffer);

					
					size_t char_count = 0;// zliczanie ilosci znakow
					for (ssize_t i = 1; i < read_len && buffer[i] != '\0'; i++)
					{
						char_count++;
					}
					//printf("DEBUG1\n");
					sem_wait(semafor32); // czekaj na podanie ostatniego w
					//printf("DEBUG2\n");
					*w = char_count;
					sem_post(semafor23); // pozwol podac w
					sem_post(semafor21);
				}
            }
            while(!*pomk2){ }
        }
            close(fd[0]);
			printf("P2(%d) zakonczono\n",getpid());
			*p2_skonczyl=1;
			*w = 0;
			exit(0);
    }

        if ((k3 = fork()) == 0)
    {   
		*pidk3=getpid();///
		printf("Tutaj proces potomny 3(%d)\n",getpid());

		sign3.sa_flags = SA_RESTART | SA_SIGINFO;
		sign3.sa_sigaction = handle_signal3;
		
		sign.sa_flags = SA_SIGINFO;
		sign.sa_sigaction = handle_signal;
		
		sigaction(SIGUSR2,&sign3,NULL);
		sigaction(SIGCONT,&sign,NULL);
		sigaction(SIGTSTP,&sign,NULL);
		sigaction(SIGTERM,&sign,NULL);
		sigaction(SIGQUIT,&sign,NULL);
		sigemptyset(&maska);
		sigaddset(&maska, SIGUSR1);//
		sigaddset(&maska, SIGABRT);//
		sigaddset(&maska, SIGFPE);//
		sigaddset(&maska, SIGINT);//
		sigprocmask(SIG_BLOCK, &maska, NULL);

		sem_post(semaforPocz);
		while(1)
		{
			while(1)
			{
			if(*pomk3){
				//if(*koniec2 == 0) exit(0);
				sem_wait(semafor23);//czekaj na wyznaczenie w
				if(!*koniec3 && *p2_skonczyl)
				{
					printf("P3(%d) zakonczono\n",getpid());
					exit(0);
				}
				if(*w != 0) 
				{
				printf("P3(%d): Znaki w linii: %d\n\n", getpid(),*w);
				}
				fflush(stdout);
				sem_post(semafor32);
			}
			while(!*pomk3)
			{}
		    }
          printf("P3(%d) zakonczono\n",getpid());
		  exit(0);

        }
    }

	printf("Tutaj proces macierzysty(%d)\n",getpid());
	*pidkM=getpid();///
	sign2.sa_flags = SA_RESTART | SA_SIGINFO;
	sign2.sa_sigaction = handle_signal2;
	sigaction(SIGUSR1,&sign2,NULL);
	sigaction(SIGABRT,&sign2,NULL);
	sigaction(SIGFPE,&sign2,NULL);
	sigemptyset(&maska);
	sigaddset(&maska, SIGCONT);
	sigaddset(&maska, SIGTSTP);
	sigaddset(&maska, SIGTERM);
	sigaddset(&maska, SIGINT);
	sigaddset(&maska, SIGQUIT);
	sigprocmask(SIG_BLOCK, &maska, NULL);


    wait(NULL);
    wait(NULL);
    wait(NULL);
	sem_destroy(semafor21);
	sem_destroy(semafor23);
	sem_destroy(semafor32);
	sem_destroy(semaforPocz);
	shmdt(w);
	shmdt(koniec1);
	shmdt(koniec2);
	shmdt(koniec3);
	shmdt(pomk1);
	shmdt(pomk2);
	shmdt(pomk3);
	shmdt(p1_skonczyl);
	shmdt(p2_skonczyl);
    shmctl(semafor21_shmid, IPC_RMID, NULL);//usuwanie pamieci wspodzielonej
    shmctl(semafor23_shmid, IPC_RMID, NULL);
    shmctl(semafor32_shmid, IPC_RMID, NULL);
    shmctl(semaforPocz_shmid, IPC_RMID, NULL);
    shmctl(w_shmid, IPC_RMID, NULL);
    shmctl(pomk1_shmid, IPC_RMID, NULL);
    shmctl(pomk2_shmid, IPC_RMID, NULL);
    shmctl(pomk3_shmid, IPC_RMID, NULL);
    shmctl(p1_skonczyl_shmid, IPC_RMID, NULL);
    shmctl(p2_skonczyl_shmid, IPC_RMID, NULL);
    msgctl(msgid, IPC_RMID, NULL); //niszczenie kk na koniec programu
    printf("PM(%d) zakonczono\n", getpid());
    return 0;
}
