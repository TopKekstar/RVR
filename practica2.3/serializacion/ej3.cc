#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {						//tamaño de nombre + tamaño de x e y
		
		alloc_data(sizeof(char) * 80 + sizeof(int16_t) * 2);
		int32_t offset = 0;
		
		memcpy(_data,&_size,sizeof(int32_t));
		offset+=sizeof(int32_t);
		
		
		memcpy(_data+offset, name, sizeof(char)*80);
		offset+=sizeof(char)*80;
		
		
		
		memcpy(_data+offset, &x, sizeof(int16_t));	
		offset+=sizeof(int16_t);
		
		
		
		memcpy(_data+offset, &y, sizeof(int16_t));		
		offset+=sizeof(int16_t);
		
    }

    int from_bin(char * data)
    {
    	alloc_data(sizeof(char) * 80 + sizeof(int16_t) * 2);
	int32_t offset = 0;
		
	memcpy(&_size,data,sizeof(int32_t));
	offset+=sizeof(int32_t);
		
	
	memcpy(name,data+offset, sizeof(char)*80);
	offset+=sizeof(char)*80;
		
		
		
	memcpy(&x,data+offset, sizeof(int16_t));	
	offset+=sizeof(int16_t);
		
		
		
	memcpy(&y,data+offset, sizeof(int16_t));		
	offset+=sizeof(int16_t);
    	return 1;
    	
    }

public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
	if(argc == 4){
	Jugador one(argv[1],atoi(argv[2]),atoi(argv[3]));
	one.to_bin();

	int fichero = open("player.txt", O_CREAT | O_RDWR);
	write(fichero, one.data(), one.size());
	close(fichero);
	
	Jugador b("", 0, 0); 
	char * data;
	data = (char *)malloc(one.size());

	fichero = open("player.txt", O_RDONLY);
	read(fichero, data, one.size());
	close(fichero);
	
	b.from_bin(data);
	std::cout << b.name << "\n"<< b.x << "\n" << b.y << std::endl;

}
else std::cout << "./exec name, int, int" << std::endl;		

}
