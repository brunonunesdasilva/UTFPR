import java.util.Scanner;

public class Jogador {
    private String Nome;
    private String TipoDeJogador;
    private JogoGeneral jogoG;

    public Jogador(String nome, String tipo){
        this.Nome = nome;
        this.TipoDeJogador = tipo;
        this.jogoG = new JogoGeneral();

    }

    public void jogarDados(){
        jogoG.rolarDados();
    }

    public void escolherJogadas(){
        String escolha = "\nEscolha qual jogada:\n1 2 3 4 5 6 7(T) 8(Q) 9(F) 10(S+) 11(S-) 12(G) 13(X)";
        Scanner Scanner = new Scanner(System.in);
        jogoG.pontuarJogada(Scanner.nextInt());
    }

}
