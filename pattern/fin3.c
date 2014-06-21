float floatfunc(int para1, float para2)
{
	int a[3][4]; 
	float b[3][3];
	a[2][1] = 6;
	b[1][2] = para1 + para2;
	return a[2][1] + b[1][2];
}
int main()
{
	write(floatfunc(4, floatfunc(3, 6)));
	write("\n");
	return 0;
}
