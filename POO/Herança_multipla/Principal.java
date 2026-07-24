package Herança_multipla;

public class Principal {
    public static void main(String[] args) {
        Arara arara = new Arara("Blue");
        Baleia baleia = new Baleia("Lagun");
        Leão leao = new Leão("Simba");

        arara.voar();
        baleia.nadar();
        leao.correr();
    }
}
