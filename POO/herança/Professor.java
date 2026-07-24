package herança;

public class Professor extends pessoa{
    private String materia;
    private int salario;

    public Professor(String nome, int cpf, int salario, String materia){
        super(nome, cpf);
        this.materia = materia;
        this.salario = salario;
    }

    public String getNome(){
        return this.nome;
    }
}
