package data;

public class Hora{
    private byte segundo;
    private byte minuto;
    private byte hora;

    public Hora(){
        this.segundo = 0;
        this.minuto = 0;
        this.hora = 0;
    }

    public void inicializaHora(byte seg, byte min, byte h){
        if(validarHora(seg, min, h) == true){
            segundo = seg;
            minuto = min;
            hora = h;
        }
        else{
            System.out.println("Valores invalidos para Hora\n");
        }
    }

    public boolean validarHora(byte seg, byte min, byte h){
        if((seg >=1) && (seg < 60) && (min >= 1) && (min < 60) && (h >=1) && (h <= 24)){
            return true;
        }
        else{
            return false;
        }
    }

    public boolean eIgualHora(Hora hora1){
        if((hora1.hora == this.hora) && (hora1.minuto == this.minuto) && (hora1.segundo == this.segundo)){
            return true;
        }
        else{
            return false;
        }
    }

    public String toStringHora(){
        String Hora = "";
        Hora += hora + ":" + minuto + ":" + segundo;
        return Hora;
    }
}
