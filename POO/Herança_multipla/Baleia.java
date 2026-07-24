package Herança_multipla;

public class Baleia extends Animal implements Nadar{
    public Baleia (String nome){
        super(nome);
    }

    public void nadar(){
        System.out.println("A baleia nada.");
    }
}
