public class complexoDemo{
    public static void main(String[] args){
        complexo n1 = new complexo();
        complexo n2 = new complexo();
        complexo n3 = new complexo();

        n1.inicializaNumero(3, 2);
        n1.imprimeNumero();
        n2.inicializaNumero(3, 2);
        System.out.print(n1.elgual(n2) + "\n");
        n3 = n1.soma(n2);
        n3.imprimeNumero();
        n3 = n1.subtrai(n2);
        n3.imprimeNumero();
        n3 = n1.multiplica(n2);
        n3.imprimeNumero();
        n3 = n1.divide(n2);
        n3.imprimeNumero();
    }
}
