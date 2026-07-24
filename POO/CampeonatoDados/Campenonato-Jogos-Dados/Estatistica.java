public interface Estatistica {

    // o número de faces que o dado possui é definido como uma constante
    public static final short numFaces = (short) 6;

    // um método abstrato de soma das faces é definido, para que seja sempre construído nas classes que implementarem
    // a interface
    public abstract void somarFacesSorteadas(int[] estatistica);    
}