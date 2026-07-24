public class JogoGeneral {
    private Dado[] dados = new Dado[5];
    private int[] jogadas = new int[13];
    JogoGeneral jogador = new JogoGeneral();

    public JogoGeneral(){
        for(int i=0; i<5; i++){
        this.dados[i] = new Dado();
        }
        for(int i=0; i<13; i++){
            this.jogadas[i] = 0;
        }
    }

    public void rolarDados(){

        for(Dado dado : jogador.dados){
            dado.roll();
        }
    }

    public String toString(){
        String valores = "Os valores obtidos são: ";
        for (int i=0;i<5;i++){
             valores += jogador.dados[i].getSideUp() + " - ";
        }
       // valores += "\nEscolha qual jogada:\n1 2 3 4 5 6 7(T) 8(Q) 9(F) 10(S+) 11(S-) 12(G) 13(X)"; 

        return valores;
    }

    public int validarJogada(int jogadaAtual, int situacaoDasJogadas[]){
        int resultado = 0;

        if(situacaoDasJogadas[jogadaAtual] != 1){
            situacaoDasJogadas[jogadaAtual] = 1;
            resultado = pontuarJogada(jogadaAtual);
        }
        
        return resultado;
    }

    public int pontuarJogada(int escolha){
        int soma = 0, cont;
        soma = 0;

        switch (escolha){
            case 1:
                for (Dado dado: jogador.dados){
                    if(dado.getSideUp() == 1)
                        soma += 1;
                } 
                if(soma == 0) return 0;
                break;
                
            case 2:
                for (Dado dado: jogador.dados){
                    if(dado.getSideUp() == 2)
                        soma += 2;
                } 
                if(soma == 0) return 0;
                break;
            case 3:
                for (Dado dado: jogador.dados){
                    if(dado.getSideUp() == 3)
                        soma += 3;
                } 
                if(soma == 0) return 0;
                break;
            case 4:
                for (Dado dado: jogador.dados){
                    if(dado.getSideUp() == 4)
                        soma += 4;
                }
                if(soma == 0) return 0;
                break;
            case 5:
                for (Dado dado: jogador.dados){
                    if(dado.getSideUp() == 5)
                        soma += 5;
                }
                if(soma == 0) return 0;
                break;
            case 6:
                for (Dado dado: jogador.dados){
                    if(dado.getSideUp() == 6)
                        soma += 6;
                }
                if(soma == 0) return 0;
                break;
            case 7:
                for(int i=1; i<=6; i++){
                    cont = 0;
                    for (Dado dado: jogador.dados){
                    if(dado.getSideUp() == i)
                        cont++;
                    }
                    if(cont >= 3){
                       for(Dado dado : jogador.dados){
                            soma += dado.getSideUp();
                       } 
                       break;}
                }break;
            case 8:
                for(int i=1; i<=6; i++){
                    cont = 0;
                    for (Dado dado : jogador.dados){
                    if(dado.getSideUp() == i)
                        cont++;
                    }
                    if(cont >= 4){
                       for(Dado dado : jogador.dados){
                            soma += dado.getSideUp();
                       } 
                       break; }

                    } break;
            case 9: //arrumar essa parte, a lógica esta errada
                int cont2, aux;
                for(int i=1; i<=6; i++){
                    cont2 = cont = 0;
                    for (Dado dado : jogador.dados){
                        
                    if(dado.getSideUp() == i)
                        cont++;
                    else cont2++;
                    }
                    if((cont == 3 && cont2 == 2) || (cont == 2 && cont2 == 3)){
                       for(Dado dado : jogador.dados){
                            soma += dado.getSideUp();
                       } 
                       break; }
                }
            case 10:{
                for (Dado dado : jogador.dados)
                    soma += dado.getSideUp();
            }


        }

        return soma;
    }
}