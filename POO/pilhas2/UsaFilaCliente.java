package pilhas2;

public class UsaFilaCliente {
    public static void main(String[] args){
        Cliente c1 = new Cliente();
        Cliente c2 = new Cliente();
        Cliente c3 = new Cliente();
        Cliente c4 = new Cliente();
        Cliente c5 = new Cliente();

        c1.inicializaCliente("Jose", 'H');
        c2.inicializaCliente("Maria", 'M');
        c3.inicializaCliente("Claudia", 'M');
        c4.inicializaCliente("André", 'H');
        c5.inicializaCliente("Fabi", 'M');

        FilaCliente f1 = new FilaCliente();

        f1.enfileirar(c1);
        f1.enfileirar(c2);
        f1.enfileirar(c3);
        f1.enfileirar(c4);
        System.out.println(f1.imprimirFila());
        f1.desenfileirar();
        f1.desenfileirar();
        f1.enfileirar(c5);
        System.out.println(f1.imprimirFila());


    }
}
