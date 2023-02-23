int a;
int b;
int foo(int a){
	a = a - 1;
	return a;
}

int main(){
	int b;
	b = 2;
	if (b > 0){
		b = foo(b);
	}
output(b);
}
