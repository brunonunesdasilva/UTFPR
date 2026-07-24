package hierarquia;

public class AreaDeQuadrilateros {
    public static void main(String[] args) {
        Trapezio trapezio = new Trapezio(0, 2, 5, 0, 1, 6, 0, 4);
        Paralelogramo paralelogramo = new Paralelogramo(1, 5, 2, 0, 3, 1, 0, 4);
        Retangulo retangulo = new Retangulo(3, 2, 1, 4, 5, 0, 1, 3);
        Quadrado quadrado = new Quadrado(2, 3, 6, 4, 5, 3, 0, 4);

        System.out.println("Área do Trapézio: " + trapezio.calcularAreaTrapezio());
        System.out.println("Área do Paralelogramo: " + paralelogramo.calcularAreaParalelogramo());
        System.out.println("Área do Retângulo: " + retangulo.calcularAreaQuadrilatero());
        System.out.println("Área do Quadrado: " + quadrado.calcularAreaQuadrilatero());
    }
}
