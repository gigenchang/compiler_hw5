int a = 0;
int a1 = 1;
int a2 = 2;
float b = 1.0;
float c = 2.0;

int main()
{
	if(a < 1 || a1 > 0 ){
		if(a2 > 1)
			write("CORRECT\n");
		else
			write("WRONG\n");
	}
	else{
		write("WRONG\n");

	}
	if(b > c){
		write("WROND\n");
	}
	else if(b + c > c){
		write("CORRECT\n");
	}
	else
		write("WRONG\n");
}
