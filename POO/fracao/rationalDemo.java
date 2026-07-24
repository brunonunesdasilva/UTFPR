package fracao;

public class rationalDemo {
    public static void main(String[] args) {
        rational f1 = new rational(3, 5);
        rational f2 = new rational();
        rational f3 = new rational();

        f1.imprimir();
        f2.imprimir();
        f3 = f1.somar(f2);
        f3.imprimir();
        f3 = f1.subtrair(f2);
        f3.imprimir();
        f3 = f1.multiplicar(f2);
        f3.imprimir();
        f3 = f1.dividir(f2);
        f3.imprimir();
        f3.imprimirFlutuante(4);
    }
}
