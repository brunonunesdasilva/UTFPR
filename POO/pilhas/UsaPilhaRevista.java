package pilhas;

public class UsaPilhaRevista{
    public static void main(String[] args){
        Revista r1 = new Revista();
        Revista r2 = new Revista();
        Revista r3 = new Revista();
        Revista r4 = new Revista();
        Revista r5 = new Revista();

        PilhaRevista p1 = new PilhaRevista(); 

        r1.inicializaRevista("java1", 01, (byte)5, (short)2012);
        r2.inicializaRevista("java2", 02, (byte)6, (short)2012);
        r3.inicializaRevista("java3", 03, (byte)7, (short)2012);
        r4.inicializaRevista("java4", 04, (byte)8, (short)2012);
        r5.inicializaRevista("java5", 05, (byte)9, (short)2012);

        p1.empilharRevista(r1);
        p1.empilharRevista(r2);
        p1.empilharRevista(r3);
        p1.empilharRevista(r4);
        p1.empilharRevista(r5);

        System.out.println(p1.imprimirPilha());
        p1.desempilharRevista();
        System.out.println(p1.imprimirPilha());
        p1.desempilharRevista();
        System.out.println(p1.imprimirPilha());

    }
}