package pilhas;

public class PilhaRevista {
    private Revista[] pilha;
    private int tamanho; // Variável para rastrear o tamanho atual da pilha

    public PilhaRevista() {
        pilha = new Revista[50];
        tamanho = 0; // Inicialmente, a pilha está vazia
    }

    public void empilharRevista(Revista revista) {
        if (tamanho < pilha.length) {
            pilha[tamanho] = revista;
            tamanho++;
        } else {
            System.out.println("A pilha está cheia. Não é possível empilhar mais revistas.");
        }
    }

    public void desempilharRevista() {
        if (tamanho > 0) {
            pilha[tamanho - 1] = null;
            tamanho--;
        } else {
            System.out.println("A pilha está vazia. Não é possível desempilhar.");
        }
    }

    public String imprimirPilha() {
        String pilhaStr = "Base\n";

        for (int i = 0; i < tamanho; i++) {
            if (pilha[i] != null) {
                pilhaStr += pilha[i] + "\n---------------------------------\n";
            }
        }

        pilhaStr +="Topo";
        return pilhaStr;
    }
}
