package pontos_2D;

public class ArrayDePontos2D {
    private Ponto2D[] array;

    public ArrayDePontos2D(int numero){
        array = new Ponto2D[numero];
    }

    public int tamanho(){
        return array.length;
    }

    public void modifica(int posicao,Ponto2D instancia){
        if((posicao >= 0) && (posicao < array.length)){
            array[posicao] = instancia;
        }
    }

    public Ponto2D valor(int posicao){
        if((posicao >=0) && (posicao < array.length)){
            return array[posicao];
        }
        return null;
    }

    public String toString(){
        String valores = "O array possui os seguintes elementos:";
        for(int i=0;i<array.length;i++){
            valores +=array[i] + " ";
        }
        return valores;
    }

    public static void main(String[] args) {
        ArrayDePontos2D n1 = new ArrayDePontos2D(5);

        n1.tamanho();
        n1.modifica(0, new Ponto2D(2, 7));
        n1.modifica(1, new Ponto2D(3,9));
        n1.modifica(2, new Ponto2D(2,7));
        n1.valor(0);
        System.out.println(n1.toString());

    }
}
