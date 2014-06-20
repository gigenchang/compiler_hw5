int FA(int a, float b){
	float x, z[10][20];
	int y, i;
	x = a;
	y = x * x;
	if(a > y) x = 0.0;
	else x = 1.0;
	for(i = 0; i < a; i = i + 1)
	{
		z[i][i] = x + 10;
	}
	return (y + b);
}
int main()
{
	write(FA(2,0.4));
	return 0;
}
