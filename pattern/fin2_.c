int inttest(int aa, int bb)
{
	write(aa);
	write("\n");
	return 0;
}

float test(int aa, float bb)
{
	float dd;
	dd = aa + bb;
	write(aa);
	write("\n");
	return dd;
}

int main()
{
	write(test(3, 3));
	write("\n");
	write(test(6, 3.0));
	write("\n");
	write(test(6,3));
	write("\n");
	return 0;
}
