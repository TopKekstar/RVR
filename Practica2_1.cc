#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include <iostream>


void pruebaError();
void showTime();
void checkLoop();


int main(int argc, char ** argv){
	
	pruebaError();
	showTime();
	checkLoop();
	return 0;
}

//Ejercicio 1 y 2
void pruebaError(){
	
	int errcode = setuid(-1);
	//La función retorna -1 cuando algo ha salido mal. EL numero de error
	//Está guardado en errno.	
	if(errcode == -1){
		//Guardamos el valor de errno rapidamente, porque errno es variable del sistema
		//y puede cambiar su valor
		int errnosave = errno;
		//Imprime el ultimo error guardado por el sistema (errno)
		perror ("Error ocurred with function setuid()");
		char error [80]; 
		//imprimimos el error relativo a errnosave, que nos es devuelto en strerror_r
		char * a = strerror_r (errnosave, error, 80);
		std::cout << "Error displayed by strerror: " << a << std::endl;
	
	}
	else printf("Process priviledges changed\n");
	
}
//Ejercicios 3, 4 y 6
void showTime() {
	
	//Obtenemos el tiempo con la función time, del sistema
	time_t tAct;
	time(&tAct);
	//Imprimimos el tiempo sin parsear
	std::cout <<"Not-parsed time: "<< tAct << std::endl;
	std::cout << "Parsed time:\n" << ctime(&tAct)<<std::endl;	
		
	struct tm * timeinfo;
	//Parseamos el tiempo a un struct timeinfo con la función localtime
	timeinfo = localtime(&tAct);
	
	char buffer[80];
	//Strftime plasma en el string buffer la hora formateada como tu quieras desde timeinfo. 
	//Mas info en man strftime
	strftime(buffer, 80, "What a great %A!, %d of %B of %G.\nThe time is: %H:%M:%S",  timeinfo); 
	
	std::cout << buffer << std::endl;
	
}

//Ejercicio 5 
void checkLoop(){

	struct timespec start, end;	
	int k = 1;
	if(clock_gettime(CLOCK_REALTIME, &start) == -1){
		perror("Error ocurred while measuring time: ");
	}
	else{
		
		for(int i = 0; i < 10000; i++)k++;
		
		clock_gettime(CLOCK_REALTIME, &end);
		
		double elapsed = 
		(end.tv_sec - start.tv_sec) + ((end.tv_nsec-start.tv_nsec)*pow(10, -9));
		
		std::cout << "Elapsed time in the loop was: "<< elapsed << std::endl;
		
	
	}
}


