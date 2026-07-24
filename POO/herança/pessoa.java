package herança;

public abstract class pessoa {
    protected String nome;
    private int cpf;

    public pessoa(String nome, int cpf){
        this.nome = nome;
        this.cpf = cpf;
    }

    public abstract String getNome();

    public int getCpf(){
        return this.cpf;
    }
}
