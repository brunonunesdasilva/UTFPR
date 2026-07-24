package Herança_multipla;

public interface Habilidade {
    void executarHabilidade();
}

public interface Nadar extends Habilidade {
    void nadar();
}

public interface Voar extends Habilidade{
    void voar();
}

public interface Correr extends Habilidade{
    void correr();
}