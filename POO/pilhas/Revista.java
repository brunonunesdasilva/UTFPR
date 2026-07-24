package pilhas;

public class Revista{
    private String nome;
    private int numero;
    private byte mes;
    private short ano;

    public Revista(){
        this.nome = null;
        this.numero = 0;
        this.mes = 0;
        this.ano = 0; 
    }

    public void inicializaRevista(String nome, int numero, byte mes, short ano){
        this.nome = nome;
        this.numero = numero;
        this.mes = mes;
        this.ano = ano;
    }

    public String toString(){
        return "Nome: " + this.nome + "\nEdicao: " + this.numero + "\nMes: " + this.mes + "\nAno: " + this.ano;
    }
}
