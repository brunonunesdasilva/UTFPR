package Herança_multipla;

public class Leão extends Animal implements Correr{
    public Leão(String nome){
        super(nome);
    }
    public void correr(){
        System.out.println("o leao corre.");
    }
}
