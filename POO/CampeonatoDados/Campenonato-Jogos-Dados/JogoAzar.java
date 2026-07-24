import java.io.Serializable;
// Classe que representa um jogo de azar, herdando de JogoDados
public class JogoAzar extends JogoDados{
    
    // Construtor que chama o construtor da classe pai (JogoDados) com parâmetros específicos
    public JogoAzar(){
        super(2, "JogoAzar");
    }

    // Método para simular o resultado do jogo de azar
    public void resultado(){
        System.out.println("==================================================================");
        this.rolarDados(); // Rola os dados

        int res;
        System.out.print(this); // Exibe o estado atual dos dados
        res = dados[0].getSideUp() + dados[1].getSideUp(); // Calcula a soma dos valores dos dados
        System.out.println("\nA soma é:" + res);

        // Verifica se o jogador ganhou
        if (res == 7 || res == 11){
            System.out.println("O jogador ganhou :)");
            this.setResultadoFinal(true);
            return;
        } 
        // Verifica se o jogador perdeu
        else if (res == 2 || res == 3 || res == 12){
            System.out.println("O jogador perdeu :(");
            this.setResultadoFinal(false);
            return;
        } 
        // Se a soma não é imediatamente ganhadora ou perdedora, continua jogando
        else{
            int res1 = res;
            res = 0;

            // Continua jogando até obter o mesmo valor inicial ou um valor perdedor
            while (res != res1 && res != 2 && res != 3 && res != 12){
                System.out.println("Jogar Dados.\n");
                this.rolarDados(); // Rola os dados novamente

                res = dados[0].getSideUp() + dados[1].getSideUp(); // Calcula a nova soma
                System.out.println(this); // Exibe o estado atual dos dados
                System.out.println("A soma é:" + res);

                // Verifica se o jogador ganhou
                if (res == res1){
                    System.out.println("O jogador ganhou :)");
                    // atribui o valor true a variavel resutadoFinal, que sera utilizada na montagem do extrato
                    this.setResultadoFinal(true);

                } 
                // Verifica se o jogador perdeu
                else if (res == 2 || res == 3 || res == 12){
                    System.out.println("O jogador perdeu :(");
                     // atribui o valor true a variavel resutadoFinal, que sera utilizada na montagem do extrato
                    this.setResultadoFinal(false);

                }
            } 
        }

    }
}