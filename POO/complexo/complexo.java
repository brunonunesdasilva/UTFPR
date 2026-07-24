public class complexo{
    private double real;
    private double imaginario;

    public complexo(){
        real = 0;
        imaginario = 0;
    }

    public complexo(double r){
        real = r;
        imaginario = 0;
    }

    public void inicializaNumero(double r, double i){
        real = r;
        imaginario = i;
    }

    public void imprimeNumero(){
        if (imaginario >= 0) {
            System.out.println(real + " + " + imaginario + "i\n");
        } else {
            System.out.println(real + " - " + (-imaginario) + "i\n");
        }
    }

    public boolean elgual(complexo b){
        if(real == b.real && imaginario == b.imaginario){
            return true;
        }
        else{
            return false;
        }
    }

    public complexo soma(complexo b){
        complexo c = new complexo();
        c.real = real + b.real;
        c.imaginario = imaginario + b.imaginario;
        return c;
    }

    public complexo subtrai(complexo b){
        complexo c = new complexo();
        c.real = real - b.real;
        c.imaginario = imaginario - b.imaginario;
        return c;
    }

    public complexo multiplica(complexo b){
        complexo c = new complexo();
        c.real = (real * b.real) - (imaginario * b.imaginario);
        c.imaginario = (real * b.imaginario) + (imaginario * b.real);
        return c;
    }

    public complexo divide(complexo b){
        complexo c = new complexo();
        c.real = ((real * b.real) + (imaginario * b.imaginario)) / ((b.real * b.real) + (b.imaginario * b.imaginario));
        c.imaginario = ((imaginario * b.real) - (real * b.imaginario)) / ((b.real * b.real) + (b.imaginario * b.imaginario));
        return c;
    }
}