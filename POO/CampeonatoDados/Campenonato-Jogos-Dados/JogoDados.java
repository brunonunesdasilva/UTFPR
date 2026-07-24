import java.io.Serializable;

public abstract class JogoDados implements Estatistica, Serializable{
    private int numDados;
    private boolean resultadoFinal;
    private int estatistica[];
    private String nomeJogo;
    Dado[] dados;

    // Método construtor 1
    public JogoDados(int numDados, String nomeJogo) {
        this.numDados = numDados;
        this.nomeJogo = nomeJogo;
        this.resultadoFinal = false;
        this.dados = new Dado[numDados];
        this.estatistica = new int[6];
        for (int i = 0; i < numDados; i++) {
            this.dados[i] = new Dado();
        }

        for(int i=0; i<6; i++){
            estatistica[i] = 0;
        }
    }

    // sobrecarga do método construtor da classe JogoDados. Agora, o jogo não utiliza a variável nomeJogo (String) para definir seu nome
    public JogoDados(int numDados){
        this.numDados = numDados;
        this.dados = new Dado[numDados];
        this.estatistica = new int[6];
        for (int i = 0; i < numDados; i++) {
            this.dados[i] = new Dado();
        }

        for(int i=0; i<6; i++){
            estatistica[i] = 0;
        }
    }

    // método abstrato para ser obrigatoriamente definido nas classes que herdam esta classe 
    public abstract void resultado();

    // o método a seguir exibirá a análise de quantas vezes cada face do dado foi sorteada durante este jogo
    public void analiseDeJogo(){
        for(int i=0; i < numFaces; i++){ 
            System.out.println("DADO DE NUMERO " + (i + 1) + ": " + this.estatistica[i]);
        }
    }

    // o método abstrato da interface Estatísitca é implementado a seguir 
    public void somarFacesSorteadas(int estatistica[]){
        for(Dado dado : dados){
            this.estatistica[dado.getSideUp() - 1] += 1;
        }
    }

    public void rolarDados(){//joga os dados e obtem o resultado
        System.out.println(dados[0]);
        
        for(Dado dado : this.dados){
            dado.roll();
        }
        // logo após o sorteio dos dados, o método somarFacesSorteadas é chamado para calcular a estatística do jogo
        this.somarFacesSorteadas(this.estatistica);
    }

    // método toString, responsável por imprimir os valores que foram obtidos por meio do sorteio dos dados
    // para então proceder com o jogo 
    public String toString(){
  
        String valores = "Os valores obtidos são: ";
        for (int i=0;i<numDados - 1;i++){
             valores += this.dados[i].getSideUp() + " - ";
        } 
        valores += this.dados[numDados - 1].getSideUp();
        return valores; 
    } 
 
    // obtém o vetor estatísica para ser utilizado em outras classes, de forma a proceder com a análise do jogo
    public int[] getEStatistica(){
        return estatistica;
    }

    // atribui um valor para a variável resultadoFinal; True: ganhou o jogo. False: perdeu o jogo.
    public void setResultadoFinal(boolean resultadoFinal) {
        this.resultadoFinal = resultadoFinal;
    }

    // retorna o valor de resultadoFinal para que a análise de ganho ou perda do jogo possa ser realizada
    // em outras classes
    public boolean getResultadoFinal(){
        return this.resultadoFinal;
    }
}