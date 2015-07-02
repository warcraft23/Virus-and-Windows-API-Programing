int main (int argc, char *argv[])
{
int i=0;
char a[20]={'0'};
char *ch="hello\n";
int addr=(int)ch;

for(i=7;i>=0;i--)
{
	int m=1;
	int j;
	for(j=0;j<i;j++)
	{
      m*=10;
	}
	a[7-i]=(char)(addr/m+48);
	addr=addr%m;
}
strcpy(a+8,ch);
	printf("\nµØÖ·Îª£º%d\n",ch);
	 my_print(a);
    return 0;
}