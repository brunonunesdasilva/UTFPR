package herança;

public class Aluno extends pessoa {
    private int registro;
    private String curso;

    public Aluno(String nome, int i, int j, String curso){
        super(nome, i);
        this.registro = j;
        this.curso = curso;
    }

    public String getNome(){
        return this.nome;
    }
}
