public class integerSetDemo {
    public static void main(String[] args) {
        integerSet c1 = new integerSet();
        integerSet c2 = new integerSet();
        integerSet c3 = new integerSet();
        integerSet c4 = new integerSet();

        c1.insertElement(4);
        c1.insertElement(9);
        c1.insertElement(101);

        c2.insertElement(4);
        c2.insertElement(9);
        c2.insertElement(87);

        c3 = c1.union(c2);
        c4 = c1.intersection(c2);

        System.out.println(c3.toSetString()+ "\n");
        System.out.println(c4.toSetString() + "\n");

        c2.deleteElement(87);

        System.out.println(c1.toSetString() + "\n");
        System.out.println(c2.toSetString() + "\n");

        System.out.println(c1.isEqualTo(c2) + "\n");
    }
}
