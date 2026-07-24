import java.util.Random;
import java.io.Serializable;

public class Dado implements Serializable{
    private int sideUP;

    //metodo construtor
    public Dado(){
        sideUP = 1;
    }

    //retorna o valor da face superior do dado
    public int getSideUp(){
        return sideUP;
    }

    //utiliza a biblioteca Random para gerar números aleatórios entre 1 e 6. O valor obtido 
    //corresponde a face superior do dado
    public void roll(){
        Random x = new Random();
        sideUP = x.nextInt(6) + 1;  
    }

    // retorna a string com o valor obtido ao rolar os dados 
    public String toString(){
        String valores = "\nRolando dados para o JOGADOR ... \n";
        return valores;
    }

}