struct datos{
double dato1;
double dato2;
};
program CALCULADORA {
version VERSION {
double suma(datos)=1;
double resta(datos)=2;
double multiplicacion(datos)=3;
double division(datos)=4;
double potencia(datos)=5;
double raiz(double)=6;
int n_llamadas()=7;
char menu()=8;
}=1;
}=0x20000001;