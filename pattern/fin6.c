float floatfunc(int para1, float para2)
{
	float b[3][3];
	b[1][2] = para1 + para2;
	return b[1][2];
}

int main()
{
	float a[3][4];
	int c = 6;
	a[1][3] = 7;
	write(floatfunc(c, a[1][3]));
	write("\n");
	return 0;
}
