#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <iostream>


void pruebaError();
void showTime();


int main(int argc, char ** argv){
	
	pruebaError();
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
	
	sleep(6);
}
//Ejercicio 2
void showTime() {





}


