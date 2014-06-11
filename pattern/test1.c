void main()
{
	int a, b, c;
	a = 0;
	b = 0;
	c = 0;
	while(a < 5){
		b = 0;
		c = 0;
		while(b < 5){
			write(a);
			write("\t");
			write(b);
			write("\t");
			write(c);
			write("\n");
			b = b + 1;
		}
		while(c < 5){
			write(a);
			write("\t");
			write(b);
			write("\t");
			write(c);
			write("\n");
			c = c + 1;
		}
		a = a + 1;
	}
}
