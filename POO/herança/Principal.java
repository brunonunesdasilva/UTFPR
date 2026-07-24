package herança;

public class Principal {
    public static void main(String[] args){
        Aluno a1 = new Aluno("julia", 1245679, 242325, "EngenhariaEletrica");
        Professor p1 = new Professor("Astolfo", 1450987, 7000, "Circuitos");

        System.out.println(a1.getCpf());
        System.out.println(a1.getNome());
        System.out.println(p1.getCpf());
        System.out.println(p1.getNome());
    }
}
