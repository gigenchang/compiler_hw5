int main()
{
	float f = 0;
	int i, j;
	i = 5;
	j = 0;
	if(i || j || i/j > 0){
		f = i + j;
		write(f);
		write("\n");
	}
	else{
		write("Great\n");
	}	
	return 0;
}
