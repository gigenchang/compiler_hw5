float test(int aa, float bb)
{
	float cc = 4.0;
	float dd;
	dd = aa || bb;
	write(cc);
	return dd;
}

int main()
{
	int c = 3;
	float d = 5;
	write(test(6, 3.0));
	write("\n");
	write(test(6,3));
	write("\n");
	write(test(6, c));
	write("\n");
	write(test(6, c + d));
	write("\n");
	return 0;
}
