package hierarquia;

class A {
    int a;

    A(int a) {
        this.a = a;
    }
}

class B {
    int b;

    B(int b) {
        this.b = b;
    }
}

class C extends A {
    B b;

    C(int a, int b) {
        super(a);
        this.b = new B(b);
    }

    public static void main(String[] args) {
        C c = new C(1, 2);
        System.out.println(c.a);
        System.out.println(c.b.b);
    }
}
