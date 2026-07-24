package pilhas2;

public class Cliente {
    private String nome;
    private char sexo;

    public Cliente(){
        this.nome = "";
        this.sexo = ' ';
    }

    public void inicializaCliente(String nome, char sexo){
        this.nome = nome;
        this.sexo = sexo;
    }

    public String toString() {
        return "Nome: " + nome + ", Sexo: " + sexo;
    }
}
