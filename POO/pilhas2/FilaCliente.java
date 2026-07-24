package pilhas2;

public class FilaCliente {
    private Cliente[] fila;
    private int tamanho;

    public FilaCliente(){
        fila = new Cliente[10];
        tamanho = 0;
    }

    public void enfileirar(Cliente cliente){
        if(tamanho < fila.length){
            fila[tamanho] = cliente;
            tamanho ++;
        }
        else{
            System.out.println("Fila cheia");
        }
    }

    public void desenfileirar() {
        if (tamanho > 0) {
            for (int i = 0; i < tamanho - 1; i++) {
                fila[i] = fila[i + 1];
            }
            fila[tamanho - 1] = null;
            tamanho--;
        } 
        else {
            System.out.println("A fila está vazia. Não é possível desenfileirar.");
        }
    }  

    public String imprimirFila() {
        String filaStr = "Fila de Clientes:\n";
    
        for (int i = 0; i < tamanho; i++) {
            if (fila[i] != null) {
                filaStr += fila[i].toString() + "\n---------------------------------\n";
            }
        }
    
        return filaStr;
    }

}
