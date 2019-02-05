#include <spede/stdio.h>
#include <spede/flames.h>

void DisplayMsg(long);

int main(void)
{
	long i;
	i = 111;
        for(; i < 116; i++)
        {
		DisplayMsg(i);
	}
	return 0;
}

void DisplayMsg(long i)
{
  
        printf("%d Hello World %d \nECS\n", i, 2 * i);
	cons_printf("--> Hello World <--\nCPE/CSC\n");
}
