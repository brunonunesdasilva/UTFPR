package fracao;

public class rational {
    private int numerator;
    private int denominator;

    private int mdc (int a, int b){
        if(b==0){
            return a;
        }
        return mdc(b, a % b);
    }

    public void reduzir(){
        if(numerator == denominator){
            numerator = 1;
            denominator = 1;
        }
        else{
            int maxdiv = mdc(numerator, denominator);
            numerator = numerator / maxdiv;
            denominator = denominator / maxdiv;
          
        }
    }
    
    public rational (int n, int d){
        int maxdiv = mdc(n,d);
        numerator = n / maxdiv;
        denominator = d / maxdiv;
    }

    public rational(){
        numerator = 3;
        denominator = 5;
    }

    public rational somar(rational a){
        rational resultante = new rational();

        if(denominator == a.denominator){
            resultante.numerator = numerator + a.numerator;
            resultante.denominator = denominator;
        }

        else{
            resultante.denominator = denominator * a.denominator;
            resultante.numerator = (numerator * a.denominator) + (a.numerator * denominator);
        }

        resultante.reduzir();

        return resultante;
    }

    public rational subtrair(rational a){
        rational resultante = new rational();

        if(denominator == a.denominator){
            resultante.numerator = numerator - a.numerator;
            resultante.denominator = denominator;
        }

        else{
            resultante.denominator = denominator * a.denominator;
            resultante.numerator = (numerator * a.denominator) - (a.numerator * denominator);
        }

        resultante.reduzir();

        return resultante;
    }

    public rational multiplicar(rational a){
        rational resultante = new rational();

        resultante.numerator = numerator * a.numerator;
        resultante.denominator = denominator * a.denominator;

        resultante.reduzir();

        return resultante;
    }

    public rational dividir(rational a){
        rational resultante = new rational();

        resultante.numerator = numerator *a.denominator;
        resultante.denominator = denominator * a.numerator;

        resultante.reduzir();
        
        return resultante;
    }

    public void imprimir(){
        if(numerator == 0 || numerator == 1){
            System.out.println(numerator + "\n");
        }
        else{
            System.out.println(numerator + "/" + denominator + "\n");
        }
    }

    public void imprimirFlutuante(int n){
        float valor = (float)numerator/(float)denominator;

        System.out.printf("%."+n+"f",valor); 

    }
}
