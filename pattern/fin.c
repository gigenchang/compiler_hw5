int func(int a, int b, int c)
{
	a = 1;
	b = 2;
	c = 3;
	return b;
}

int main()
{
	write(func(0, 0, 0));
	write("\n");
}
