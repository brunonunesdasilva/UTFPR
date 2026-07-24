import java.util.Scanner;


public class Humano extends Jogador implements JogarComoHumano{
    //private String cpf, agencia, conta;
    private double saldo;
    private JogoGeneral jogoG;
    private JogoAzar jogoA;

    // método construtor 
    public Humano(String nome, double saldo, char tipo){
        super(nome, saldo, tipo);
        this.saldo = saldo;
        this.jogoA = new JogoAzar();
        this.jogoG = new JogoGeneral();
    }
    
    // responsável por jogar, pontuar jogadas, definir seu estado  e o saldo
    public void jogarDados(double aposta){
        int jogoEscolhido = escolherJogo();

        // define o valor que foi apostado
        this.setApostas(aposta);

        System.out.println("JOGADOR(A) DA RODADA" + getRodadas() +": " + this.getNome() + " (H)");

        if(jogoEscolhido == 2){ //jogoGeneral
            jogoG.resetarRodadas();

            // itera-se por todas as jogadas do jogo general
            for(int i=0; i<13; i++){
                jogoG.pontuarRodada(escolherJogadas());
                System.out.println(mostrarJogadasExecutadas(jogoG));
                System.out.println("==================================================================");
            }

            // é calculado o resultado, se o jogador ganhou ou perdeu
            jogoG.resultado();

            // o jogo (e seus resultados) é setado no array de jogos na superclasse Jogador
            setJogo(jogoG, getRodadas());

            // também na classe Jogador, é setado o resultado se o jogador ganhou ou perdeu
            setVetorDeResultados(jogoG);

            // e então é finalmente mostrado para o jogador se ele ganhou este respectivo jogo
            if(jogoG.getResultadoFinal() == true){
                System.out.println("O jogador venceu o Jogo General :)\n");
                this.saldo += aposta;
            }
            else{
                System.out.println("O jogador perdeu o Jogo General :(\n");
                this.saldo -= aposta;
            }
            
            // o saldo é atualizado e exibido
            setSaldo(this.saldo); 
            System.out.println("\nSaldo: "+ getSaldo());
        }
        else if(jogoEscolhido == 1){
            
            jogoA.resultado();

            // o jogo (e seus resultados) é setado no array de jogos na superclasse Jogador
            setJogo(jogoA, getRodadas());

            // também na classe Jogador, é setado o resultado se o jogador ganhou ou perdeu
            setVetorDeResultados(jogoA);

            // e então é finalmente mostrado para o jogador se ele ganhou este respectivo jogo
            if(jogoA.getResultadoFinal() == true){
                this.saldo += aposta;
            }
            else{
                this.saldo -= aposta;
            }
          
            // o saldo é atualizado 
            setSaldo(this.saldo);
            System.out.println("Saldo: "+ getSaldo());
        }
        else if(jogoEscolhido == 3){ // saindo do jogo 
            System.out.println("Saindo do jogo ...");
            return;
        }
        else{ //caso seja escolhido um numero aleatorio
            System.out.println("Escolha um jogo válido");
        }
        
    }


    // responsável por mostrar os valores sorteados dos dados, as opções de jogadas existentes de 1 a 13 
    // e captura do valor da jogada que o jogador deseja executar 
    public int escolherJogadas(){
        int value = 0;
        Scanner scanner = new Scanner(System.in);

        jogoG.rolarDados();
        System.out.print(jogoG);
        System.out.println("\nEscolha uma jogada:\n1 2 3 4 5 6 7(T) 8(Q) 9(F) 10(S+) 11(S-) 12(G) 13(X)");
        value = scanner.nextInt();
        // scanner.close();
        
        while(value<1 || value > 13){//value escolha uma opção que não seja uma jogada
            System.out.println("Jogada invalida, escolha um número entre 1 e 13");
            value = scanner.nextInt();
        }
        while ((value<1 || value > 13) || !jogoG.validarRodada(value)){//verificar se a jogada é valida
            System.out.println("Jogada inválida, escolha outro número:");
            value = scanner.nextInt();
        }

        return value;
    }
}