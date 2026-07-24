import java.util.Random;

public class Maquina extends Jogador implements JogarComoMaquina{
    // Declaração de variáveis de instância
    private JogoAzar jogoA;
    private JogoGeneral jogoG;
    private double saldo;
    private Random random;

    // Construtor que inicializa a máquina com um nome, saldo, e chama o construtor da superclasse 
    public Maquina(String nome, double saldo, char tipo){
        super(nome, saldo, tipo);
        this.saldo = saldo;
        jogoA = new JogoAzar();
        jogoG = new JogoGeneral();
        random = new Random();
    }

    // Método para simular os jogos de dados 
    public void jogarDados(double aposta) {
        // Escolhe aleatoriamente entre os dois jogos disponíveis
        int jogoEscolhido = random.nextInt(2) + 1;

        // o valor apostado é identificado na classe JogoDados por meio do método set, para ser armazenado e utilizado 
        // posteriormente 
        this.setApostas(aposta);

        // o nome do jogador responsável pelo jogo da vez é impresso
        System.out.println("JOGADOR(A) DA RODADA" + getRodadas() + ": " + this.getNome() + " (M)");

        // Se o jogo escolhido for JogoGeneral (2)
        if (jogoEscolhido == 2) {
            // reseta as jogadas do Jogo General
            jogoG.resetarRodadas();
            // Aplica a estratégia do JogoGeneral durante 13 rodadas
            for (int i = 0; i < 13; i++){
                aplicarEstrategia();
            }

            // Verifica se a máquina ganhou ou perdeu o JogoGeneral e ajusta o saldo
            jogoG.resultado();
            this.setVetorDeResultados(jogoG);
            setJogo(jogoG, getRodadas());
            if (jogoG.getResultadoFinal() == true){
                System.out.println("O jogador venceu o Jogo General :)\n");
                this.saldo += aposta;
            } else {
                System.out.println("O jogador perdeu o Jogo General :(\n");
                this.saldo -= aposta;
            }

            // Atualiza o saldo 
            setSaldo(this.saldo);

            // Imprime estatísticas do JogoGeneral e o saldo atual

            System.out.println("\nSaldo: "+ String.format("%.2f", getSaldo()));
        }
        // Se o jogo escolhido for JogoAzar (1)
        else if (jogoEscolhido == 1){
            // Simula o resultado do JogoAzar e ajusta o saldo
            jogoA.resultado();
            setJogo(jogoA, getRodadas());
            setVetorDeResultados(jogoA);
            
            if (jogoA.getResultadoFinal() == true){
                this.saldo += aposta;
            } else {
                this.saldo -= aposta;
            }

            // Atualiza o saldo e imprime estatísticas do JogoAzar
            setSaldo(this.saldo);
            System.out.println("\nSaldo: "+ String.format("%.2f", getSaldo()));
        }
    } 

    // Método para aplicar a estratégia no JogoGeneral
    public void aplicarEstrategia(){
        int jogadaDeMaiorPontuacao;
        int maiorPontuacao = -1;

        // Simula uma jogada do JogoGeneral
        jogadaDeMaiorPontuacao = 0;
        maiorPontuacao = -1;
        jogoG.rolarDados();
        System.out.println(jogoG);

        // Itera sobre as 12 rodadas possíveis
        for (int i = 1; i <= 12; i++){
            // Verifica se a jogada é válida
            if (jogoG.validarRodada(i)){
                // Verifica se a pontuação da rodada atual é a maior
                if (jogoG.pontuarRodada(i) > maiorPontuacao){
                    jogadaDeMaiorPontuacao = i;
                    maiorPontuacao = jogoG.pontuarRodada(jogadaDeMaiorPontuacao);
                }
                // Reseta a jogada no JogoGeneral (simula a jogada da máquina), para que as outras jogadas possam ser testadas
                jogoG.resetarJogadaDaMaquina(i);
            }
        }

        // Imprime informações sobre a jogada escolhida pela máquina
        if (jogadaDeMaiorPontuacao != 0){
            System.out.println("Jogada escolhida pela maquina: " + jogadaDeMaiorPontuacao);
            System.out.println("Pontuação obtida: " + jogoG.pontuarRodada(jogadaDeMaiorPontuacao));
            System.out.println(mostrarJogadasExecutadas(jogoG));
            System.out.println("==================================================================");
        } else {
            System.out.println("==================================================================");
            System.out.println("Jogada escolhida pela maquina: " + 13);
            System.out.println("Pontuação obtida: " + jogoG.pontuarRodada(13));
            System.out.println(mostrarJogadasExecutadas(jogoG));
            System.out.println("==================================================================");
        }
    }
}