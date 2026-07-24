package Herança_multipla;

public class Arara extends Animal implements Voar{
    public Arara(String nome){
        super(nome);
    }

    public void voar(){
        System.out.println("A arara voa");
    }
}
