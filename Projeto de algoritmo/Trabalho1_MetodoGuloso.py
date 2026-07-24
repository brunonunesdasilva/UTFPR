import sys
import csv
from pathlib import Path
from time import perf_counter

# padroniza o nome da instancia para facilitar a busca do otimo conhecido, adicionando a extensão .tw se necessário e removendo espaços em branco
def padronizar_nome_instancia(nome):
    nome = nome.strip()
    if not nome:
        return nome
    if not nome.lower().endswith(".tw"):
        nome = f"{nome}.tw"
    return nome

# Carrega a instancia do teste AFG, onde a primeira linha é o numero de nos, seguida por n linhas de matriz de tempo e n linhas de janelas de tempo
def carregar_instancia(caminho_arquivo):
    with open(caminho_arquivo, "r", encoding="utf-8") as f:
        linhas = [linha.strip() for linha in f if linha.strip()]

    n = int(linhas[0])

    matriz = []
    for i in range(1, 1 + n):
        matriz.append([float(valor) for valor in linhas[i].split()])

    janelas = []
    for i in range(1 + n, 1 + 2 * n):
        inicio, fim = linhas[i].split()
        janelas.append((float(inicio), float(fim)))

    return matriz, janelas

# algoritmo guloso para o problema do caixeiro viajante com janelas de tempo (TSPTW).
def tsptw_guloso(matriz_tempo, janelas, deposito=0):
    n = len(matriz_tempo)
    nao_visitados = set(range(n))
    nao_visitados.remove(deposito)

    rota = [deposito]
    tempos_rota = [0.0]
    tempo_atual = max(0.0, janelas[deposito][0])
    atual = deposito
    # deposito ponto de partida e chegada, deve ser visitado no inicio e no final da rota.

    while nao_visitados:
        melhor_no = None
        melhor_custo = float("inf")
        melhor_chegada = 0.0
        melhor_inicio = 0.0 # melhor inicio do serviço considerando a janela de tempo do cliente

        # percorre os nos nao visitados para encontrar o proximo no mais "barato" de visitar.
        for candidato in nao_visitados:
            viagem = matriz_tempo[atual][candidato]
            chegada = tempo_atual + viagem
            abertura, fechamento = janelas[candidato]

            if chegada > fechamento:
                continue

            # o serviço inicia no maximo entre a chegada e a abertura da janela.
            inicio_servico = max(chegada, abertura)
            espera = inicio_servico - chegada
            custo = viagem + espera

            # atualiza o melhor no encontrado considerando o custo total (viagem + espera) e a chegada dentro da janela de tempo.
            if custo < melhor_custo:
                melhor_custo = custo
                melhor_no = candidato
                melhor_chegada = chegada
                melhor_inicio = inicio_servico

        if melhor_no is None:
            return None, None, "Inviavel: nao ha cliente viavel dentro da janela de tempo."

        # adiciona o melhor no encontrado na rota, atualiza os tempos e remove o no dos nao visitados.
        rota.append(melhor_no)
        tempos_rota.append(melhor_chegada)
        nao_visitados.remove(melhor_no)
        atual = melhor_no
        tempo_atual = melhor_inicio

    # Retorna ao deposito e valida a janela do deposito.
    chegada_deposito = tempo_atual + matriz_tempo[atual][deposito]
    if chegada_deposito > janelas[deposito][1]:
        return None, None, "Inviavel: retorno ao deposito fora da janela de tempo."

    # adiciona o deposito no final da rota e registra o tempo de chegada.
    rota.append(deposito)
    tempos_rota.append(chegada_deposito)
    return rota, tempos_rota, None


def imprimir_resultado(rota, tempos_rota):
    print("--- ROTA GERADA (indices dos nos) ---")
    print(" -> ".join(str(no) for no in rota))
    print("Tempo final:", f"{tempos_rota[-1]:.2f}")
    print("\nChegada por no visitado:")
    for no, tempo in zip(rota, tempos_rota):
        print(f"No {no:>2}: {tempo:.2f}")

# funcao para carregar otimos conhecidos a partir de um arquivo.
def carregar_otimos_conhecidos(caminho_arquivo):
    otimos = {}
    caminho = Path(caminho_arquivo)
    if not caminho.exists():
        return otimos

    with caminho.open("r", encoding="utf-8") as f:
        linhas = [linha.strip() for linha in f if linha.strip()]

    if not linhas:
        return otimos

    # CSV com header (instancia, otimo) ou similar:
    if "," in linhas[0]:
        with caminho.open("r", encoding="utf-8", newline="") as f:
            leitor = csv.DictReader(f)
            for linha in leitor:
                instancia = padronizar_nome_instancia(
                    linha.get("instancia")
                    or linha.get("Instance")
                    or linha.get("instance")
                    or ""
                )
                otimo = (
                    linha.get("otimo")
                    or linha.get("Cost")
                    or linha.get("cost")
                    or ""
                ).strip()
                if instancia:
                    otimos[instancia] = otimo if otimo else "N/D"
        return otimos

#automatiza a execucao dos experimentos para todas as instancias na pasta "Langevin" e salva os resultados em "resultados_experimentos.csv"
#def rodar_experimentos(pasta_instancias="Langevin", csv_otimos="otimos_conhecidos.csv", csv_saida="resultados_experimentos.csv"):

def rodar_experimentos(pasta_instancias="AFG", csv_otimos="otimos_conhecidosAFG.csv", csv_saida="resultados_experimentosAFG.csv"):
    pasta = Path(pasta_instancias)
    if not pasta.exists() or not pasta.is_dir():
        print(f"Pasta de instancias invalida: {pasta_instancias}")
        sys.exit(1)

    arquivos = sorted(pasta.glob("*.tw"))
    if not arquivos:
        print(f"Nenhum arquivo .tw encontrado em: {pasta_instancias}")
        sys.exit(1)

    otimos = carregar_otimos_conhecidos(csv_otimos)
    resultados = []

    for arquivo in arquivos:
        matriz, janelas_tempo = carregar_instancia(str(arquivo))
        inicio = perf_counter()
        rota_resultado, chegadas, erro = tsptw_guloso(matriz, janelas_tempo, deposito=0)
        # calcula o tempo de execucao de cada instancia em milissegundos
        tempo_execucao_ms = (perf_counter() - inicio) * 1000.0

        if erro:
            solucao_gulosa = "INVIAVEL"
        else:
            solucao_gulosa = f"{chegadas[-1]:.2f}"

        resultados.append(
            {
                "instancia": arquivo.name,
                "otimo_conhecido": otimos.get(arquivo.name, "N/D"),
                "solucao_gulosa": solucao_gulosa,
                "tempo_execucao_ms": f"{tempo_execucao_ms:.3f}",
            }
        )

    with open(csv_saida, "w", encoding="utf-8", newline="") as f:
        campos = ["instancia", "otimo_conhecido", "solucao_gulosa", "tempo_execucao_ms"]
        escritor = csv.DictWriter(f, fieldnames=campos)
        escritor.writeheader()
        escritor.writerows(resultados)

    print(f"Tabela de resultados salva em: {csv_saida}")
    print("\nPrimeiras linhas:")
    for linha in resultados[:10]:
        print(
            f"{linha['instancia']:>12} | otimo={linha['otimo_conhecido']:>8} | "
            f"guloso={linha['solucao_gulosa']:>10} | tempo_ms={linha['tempo_execucao_ms']:>8}"
        )


def main():
    if len(sys.argv) == 1:
        #rodar_experimentos("Langevin", "otimos_conhecidos.csv", "resultados_experimentos.csv")
        rodar_experimentos("AFG", "otimos_conhecidosAFG.csv", "resultados_experimentosAFG.csv")

        return

    if len(sys.argv) >= 2 and sys.argv[1] == "--experimentos":
        if len(sys.argv) > 5:
            print("Uso: python Trabalho1.py --experimentos [pasta_instancias] [csv_otimos] [csv_saida]")
            sys.exit(1)

        #pasta_instancias = sys.argv[2] if len(sys.argv) >= 3 else "Langevin"
        pasta_instancias = sys.argv[2] if len(sys.argv) >= 3 else "AFG"
        csv_otimos = sys.argv[3] if len(sys.argv) >= 4 else "otimos_conhecidosAFG.csv"
        csv_saida = sys.argv[4] if len(sys.argv) >= 5 else "resultados_experimentosAFG.csv"
        rodar_experimentos(pasta_instancias, csv_otimos, csv_saida)
        return

    if len(sys.argv) >= 2 and sys.argv[1] == "--instancia":
        if len(sys.argv) != 3:
            print("Uso: python Trabalho1.py --instancia caminho_da_instancia")
            sys.exit(1)

        arquivo = sys.argv[2]
        try:
            matriz, janelas_tempo = carregar_instancia(arquivo)
        except FileNotFoundError:
            print(f"Arquivo nao encontrado: {arquivo}")
            sys.exit(1)
        except (ValueError, IndexError):
            print("Formato de instancia invalido para o parser AFG.")
            sys.exit(1)

        rota_resultado, chegadas, erro = tsptw_guloso(matriz, janelas_tempo, deposito=0)
        if erro:
            print(erro)
        else:
            imprimir_resultado(rota_resultado, chegadas)
        return

    if len(sys.argv) == 2 and not sys.argv[1].startswith("--"):
        arquivo = sys.argv[1]
        try:
            matriz, janelas_tempo = carregar_instancia(arquivo)
        except FileNotFoundError:
            print(f"Arquivo nao encontrado: {arquivo}")
            sys.exit(1)
        except (ValueError, IndexError):
            print("Formato de instancia invalido para o parser AFG.")
            sys.exit(1)

        rota_resultado, chegadas, erro = tsptw_guloso(matriz, janelas_tempo, deposito=0)
        if erro:
            print(erro)
        else:
            imprimir_resultado(rota_resultado, chegadas)
        return

    print("Uso padrao: python Trabalho1.py")
    print("Uso: python Trabalho1.py --experimentos [pasta_instancias] [csv_otimos] [csv_saida]")
    print("Uso: python Trabalho1.py --instancia caminho_da_instancia")
    print("Uso antigo (ainda suportado): python Trabalho1.py caminho_da_instancia")
    sys.exit(1)


if __name__ == "__main__":
    main()