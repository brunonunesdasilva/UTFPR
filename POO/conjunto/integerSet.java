public class integerSet{
    private boolean[] a;

    public integerSet(){
        a = new boolean[101];
    }

    public void insertElement(int k){
        if(k<=100 && k>=0){
            a[k] = true;
        }
    }

    public integerSet union(integerSet c1){
        integerSet c2 = new integerSet();

        for(int i=0;i<101;i++){
            if(a[i] == true || c1.a[i] == true){
                c2.a[i] = true;
            }
            else{
                c2.a[i] = false;
            }
        }
        return c2;
    }

    public integerSet intersection(integerSet c1){
        integerSet c2  = new integerSet();

        for(int i = 0; i < 101; i++){
            if(a[i] == false || c1.a[i] == false){
                c2.a[i] = false;
            }
            else{
                c2.a[i] = true;
            }

        }
        return c2;
    }

    public void deleteElement(int m){
        for(int i = 0; i<101; i++){
            if(a[i] == true && i == m){
                a[i] = false;
            }
        }
    }

    public String toSetString(){
        String str = "";
        for(int i = 0; i<101; i++){
            if(a[i] == true){
                str = str + i +" ";
            }
            else{
                 str = str + "-- ";
            }
        }

        return str;
    }

    public boolean isEqualTo(integerSet c1){
        for(int i = 0; i<101; i++){
            if(a[i] == true && c1.a[i] == false){
                return false;
            }
        }
        return true;
    }
}