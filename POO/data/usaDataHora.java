package data;

public class usaDataHora {
    public static void main(String[] args){
        DataHora datahora1 = new DataHora();
        DataHora datahora2 = new DataHora();
        DataHora datahora3 = new DataHora();

        datahora1.incializaDataHora((byte)20, (byte)12, (short)40, (byte)5, (byte)12, (byte)4);
        datahora2.incializaDataHora((byte)7, (byte)10, (short)20, (byte)14, (byte)11, (byte)20);
        datahora3.incializaDataHora((byte)20, (byte)12, (short)40, (byte)5, (byte)12, (byte)4);

        System.out.println(datahora1.eIgualDataHora(datahora3));
        System.out.println(datahora2.eIgualDataHora(datahora3));

        System.out.println("Data 1\n" + datahora1);
        System.out.println("Data 2\n" + datahora2);


    }


}
