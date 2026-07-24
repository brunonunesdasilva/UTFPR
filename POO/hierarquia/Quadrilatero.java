package hierarquia;

public class Quadrilatero{

    private float x1, y1, x2, y2, x3, y3, x4, y4;

    Quadrilatero(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
        this.x1 = x1;
        this.y1 = y1;
        this.x2 = x2;
        this.y2 = y2;
        this.x3 = x3;
        this.y3 = y3;
        this.x4 = x4;
        this.y4 = y4;
    }

    public double calcularAreaQuadrilatero() {
        return 0.0;
    }
}

class Trapezio extends Quadrilatero {

    private float altura, baseMaior, baseMenor;

    Trapezio(float x1, float y1, float x2, int y2, float x3, float y3, float x4, float y4) {
        super(x1, y1, x2, y2, x3, y3, x4, y4);
        this.altura = Math.abs(y2 - y3);
        float a = x2 - x1;
        float b = x4 - x3;
        if(a > b){
            this.baseMaior = Math.abs(a);
            this.baseMenor = Math.abs(b);
        }
        else{
            this.baseMaior = Math.abs(b);
            this.baseMenor = Math.abs(a);
        }
    }

    public float calcularAreaTrapezio() {
        return ((this.baseMaior + this.baseMenor) / 2) * this.altura;
    }
}

class Paralelogramo extends Quadrilatero {
    private float base, altura;
    Paralelogramo(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
        super(x1, y1, x2, y2, x3, y3, x4, y4);
        this.base = Math.abs(x2 - x1);
        this.altura = Math.abs(y2 - y3);
    }

    public float calcularAreaParalelogramo() {
        return (this.base * this.altura);
    }
}

class Retangulo extends Paralelogramo {
    Retangulo(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
        super(x1, y1, x2, y2, x3, y3, x4, y4);
    }
}

class Quadrado extends Retangulo {
    Quadrado(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
        super(x1, y1, x2, y2, x3, y3, x4, y4);
    }
}
