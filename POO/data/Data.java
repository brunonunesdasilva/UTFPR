package data;

public class Data {
    private  byte dia;
    private  byte mes;
    private short ano;

    public Data(){
        dia = 0;
        mes = 0;
        ano = 0;
    }

    public void inicializaData(byte dia1, byte mes1, short ano1){
        if(validarData(dia1, mes1, ano1) == false){
            System.out.println("Data invalida\n");
        }
        else{
            dia = dia1;
            mes = mes1;
            ano = ano1; 
        }
          
    }

    public boolean validarData(byte d, byte m, short ano){
        if( (d >=1) && (d <= 31) && (m >= 1) && (m <= 12)){
            return true;
        }
        else{
            return false;
        }
    }

    public boolean eIgualData(Data data1){
        if(data1.dia == this.dia && data1.mes == this.mes && data1.ano == this.ano){
            return true;
        }
        else{
            return false;
        }
    }

    public String toStringData(){
        String Data = "";
        Data += dia + "/" + mes + "/" + ano;
        return Data;
    }
}
