int func()
{
	int a = 1;
	float b = 2.0;
	float c = 3.0;
	return (a && b < c);
}

int main()
{
	int a;
	for(a = 5; a > 0; a = a - 1){
		int b;
		for(b = a + 1; b > 0; b = b - 1){
			write(a);
			write("\t");
			write(b);
			write("\n");
		}
	}
	write("\n\n");
	for(a = 0; a < 5; a = a + 1){
		if( a > 2)
			write(a + !func());
		else
			write(a + func());
	}
	return 0;
}
