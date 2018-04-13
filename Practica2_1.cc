#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <iostream>


void pruebaError();
void showTime();


int main(int argc, char ** argv){
	
	pruebaError();
	showTime();
	sleep(6);
	return 0;
}

//Ejercicio 1 
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
//Ejercicio 2
void showTime() {
	
	//Obtenemos el tiempo con la función time, del sistema
	time_t tAct;
	time(&tAct);
	//Imprimimos el tiempo sin parsear
	std::cout <<"Not-parsed time since epoch: "<< tAct << std::endl;
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


