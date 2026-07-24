package data;

public class DataHora{
    private Data estaData;
    private Hora estaHora;

    public DataHora(){
        estaData = new Data();
        estaHora = new Hora();
    }

    public void incializaDataHora(byte dia, byte mes, short ano, byte hora, byte minuto, byte segundo){
        estaData.inicializaData(dia, mes, ano);
        estaHora.inicializaHora(segundo, minuto, hora);
    }

    public boolean eIgualDataHora(DataHora datahora){
        if((datahora.estaData.eIgualData(estaData) == this.estaData.eIgualData(estaData)) && (datahora.estaHora.eIgualHora(estaHora) == datahora.estaHora.eIgualHora(estaHora))){
            return true;
        }
        else{
            return false;
        }
    }

    public String toString(){
        String datahora = "";
        datahora += estaData.toStringData() + "\n" + estaHora.toStringHora();
        return datahora;
    }

}
    