#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#define DISKSTATS_FILE "/proc/diskstats"

int main(void) {

  /* Наши ID процесса и сессии */
  pid_t pid, sid;

  /* Ответвляемся от родительского процесса */
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  /* Если с PID'ом все получилось, то родительский процесс можно завершить. */
  if (pid > 0) {
      clrscr();
    while(1) {	
	printf("%8s","NameDev");
	printf("%8s", "NumRe");/*kol-vo*/	
	printf("%8s", "SecRe");/*razmer*/
	printf("%8s", "NumWr");/*kol-vo*/	
	printf("%8s \n", "SecWr");/*razmer*/
      	struct device {
		int majorNumber;
		int minorNumber;
		char deviceName[6];
		int readsCompletedSuccessfully;
		int readsMerged;
		int sectorsRead;
		int timeSpentReading;
		int writesCompleted;
		int writesMerged;
		int sectorsWritten;
		int timeSpentWriting;
		int IOsCurrentlyInProgress;
		int timeSpentDoingIOs;
		int weightedTimeSpentDoingIOs;	
	};
	FILE * file = fopen(DISKSTATS_FILE,"r");
	FILE * fileCopy = fopen(DISKSTATS_FILE,"r");
	int lines_count = 0;
	//s4itaem kol-vo strok
	if (file==NULL)
		printf("Trouble of reading file!");
	else {
	while(!feof(file))
	{
		if(fgetc(file)=='\n')
			lines_count++;
	}
	lines_count++;
        fclose(file);	
	//kajdaia stroka iz faila eto structura soderjawaia polia
	struct device block[lines_count];
	char i=0;
	while(fscanf (fileCopy, "%d%d%s%d%d%d%d%d%d%d%d%d%d%d", &(block[i].majorNumber), &(block[i].minorNumber), block[i].deviceName, &(block[i].readsCompletedSuccessfully), &(block[i].readsMerged), &(block[i].sectorsRead), &(block[i].timeSpentReading), &(block[i].writesCompleted),  &(block[i].writesMerged), &(block[i].sectorsWritten), &(block[i].timeSpentWriting), &(block[i].IOsCurrentlyInProgress), &(block[i].timeSpentDoingIOs), &(block[i].weightedTimeSpentDoingIOs)) != EOF) {
		printf("%6s %8d %8d %6d %6d \n", block[i].deviceName, block[i].readsCompletedSuccessfully, block[i].sectorsRead, block[i].writesCompleted, block[i].sectorsWritten);
		i++;
	}
      fclose(fileCopy);
	printf("\n");
	printf("Note: Program shows you the number of hits and the number of used sectors.\n");
	printf("One sector is 512 bytes\n");
      sleep(5);
      clrscr();
     }
    }
    exit(1);
  }

  /* Изменяем файловую маску */
  umask(0);

  /* Здесь можно открывать любые журналы */       
  /* Создание нового SID для дочернего процесса */
  sid = setsid();
  if (sid < 0) {
    /* Журналируем любой сбой */
    exit(EXIT_FAILURE);
  }

  /* Изменяем текущий рабочий каталог */
  if ((chdir("/")) < 0) {
    /* Журналируем любой сбой */
    exit(EXIT_FAILURE);
  }

  /* Закрываем стандартные файловые дескрипторы */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  exit(EXIT_SUCCESS);
}

void clrscr(void){
	printf("\033[2J"); /*clear the entire screen*/
	printf("\033[0;0f"); /*Move cursor to the top*/
}
