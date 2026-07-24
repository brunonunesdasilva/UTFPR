package herança;

public class Principla_polimorfismo {
    public static void main(String[] args) {
        pessoa[] pes = new pessoa[10];

        for (int i = 0; i < 10; i++) {
            if (i < 5) {
                pes[i] = new Aluno("Jose" +  i , i, i, "Engenharia");
            } else {
                pes[i] = new Professor("João" + i, i, i, "Agronomia");
            }
        }

        for (int i = 0; i < 10; i++) {
            String nome = pes[i].getNome();
            System.out.println("Nome: " + nome);
        }
    }
}
