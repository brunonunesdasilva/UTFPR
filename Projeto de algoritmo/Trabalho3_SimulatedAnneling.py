import sys
import csv
import math
import random
from pathlib import Path
from time import perf_counter

# --- FUNÇÕES DE CARREGAMENTO (PADRÃO AFG) ---
def carregar_instancia(caminho_arquivo):
    with open(caminho_arquivo, "r", encoding="utf-8") as f:
        linhas = [linha.strip() for linha in f if linha.strip()]
    n = int(linhas[0])
    matriz = [[float(v) for v in l.split()] for l in linhas[1:1+n]]
    janelas = []
    for i in range(1+n, 1 + 2*n):
        ini, fim = linhas[i].split()
        janelas.append((float(ini), float(fim)))
    return matriz, janelas

def carregar_otimos(caminho_csv):
    otimos = {}
    caminho = Path(caminho_csv)
    if not caminho.exists(): return otimos
    with open(caminho, "r", encoding="utf-8") as f:
        leitor = csv.DictReader(f)
        for linha in leitor:
            inst = (linha.get("instancia") or linha.get("Instance") or "").strip()
            # Garante consistência com a extensão .tw do AFG
            if inst and not inst.lower().endswith(".tw"): inst += ".tw"
            cost = linha.get("otimo") or linha.get("Cost") or "0"
            if inst:
                try: otimos[inst] = float(cost)
                except: otimos[inst] = None
    return otimos


# --- CÁLCULO DE CUSTO COM PENALIZAÇÃO DE INVIABILIDADE ---
def calcular_custo_sa(rota, matriz, janelas, penalidade_atraso=10000.0):
    """
    Calcula o tempo total e penaliza severamente se houver violação de janelas.
    Isso permite que o Simulated Annealing navegue por soluções inviáveis para consertá-las.
    """
    tempo = max(0.0, janelas[rota[0]][0])
    atraso_total = 0.0
    
    for i in range(len(rota) - 1):
        u, v = rota[i], rota[i+1]
        chegada = tempo + matriz[u][v]
        
        if chegada > janelas[v][1]: # Se chegar após o fim da janela, acumula o atraso para penalização
            atraso_total += (chegada - janelas[v][1])
            
        tempo = max(chegada, janelas[v][0])
        
    # O custo final é o tempo total de viagem + (atraso acumulado * multa pesada)
    return tempo + (atraso_total * penalidade_atraso), atraso_total


# --- HEURÍSTICA GULOSA ---
def tsptw_guloso(matriz, janelas):
    n = len(matriz)
    deposito = 0
    nao_visitados = list(range(1, n))
    rota = [deposito]
    tempo_atual = max(0.0, janelas[deposito][0])
    
    while nao_visitados:
        melhor_no = None
        melhor_custo = float('inf')
        
        for candidato in nao_visitados:
            chegada = tempo_atual + matriz[rota[-1]][candidato]
            if chegada <= janelas[candidato][1]:
                inicio_servico = max(chegada, janelas[candidato][0])
                custo = inicio_servico - tempo_atual
                if custo < melhor_custo:
                    melhor_custo = custo
                    melhor_no = candidato
        
        if melhor_no is None: return None
        rota.append(melhor_no)
        nao_visitados.remove(melhor_no)
        
    return rota + [deposito]


# --- OPERADOR DE VIZINHANÇA PARA O ANNEALING ---
def gerar_vizinho_1shift(rota):
    """Remove um nó aleatório e o insere em outra posição aleatória."""
    nova_rota = list(rota)
    # Não movemos o depósito inicial (0) nem o final (último)
    i = random.randint(1, len(nova_rota) - 2) # Escolhe um nó aleatório para remover (exceto os depósitos)
    no = nova_rota.pop(i)
    
    j = random.randint(1, len(nova_rota) - 1) # Escolhe uma posição aleatória para inserir
    nova_rota.insert(j, no)
    return nova_rota


# --- ALGORITMO: SIMULATED ANNEALING ---
def simulated_annealing(rota_inicial,matriz, janelas ):
    # Configurações de hiperparâmetros
    t_inicial = 1609.50077 # Temperatura inicial alta para permitir exploração ampla
    t_final = 0.03686 # Temperatura final baixa para convergência
    alfa = 0.99455  # Taxa de resfriamento
    iter_por_temp = 144  # Quantas tentativas de mudança fazemos em cada temperatura
    
    solucao_atual = list(rota_inicial)
    custo_atual, atraso_atual = calcular_custo_sa(solucao_atual, matriz, janelas)
    
    melhor_solucao = list(solucao_atual)
    melhor_custo = custo_atual
    melhor_atraso = atraso_atual
    
    t = t_inicial
    while t > t_final:
        for _ in range(iter_por_temp): #roda iter_por_temp vezes para cada temperatura
            # Gera perturbação na rota (Vizinho)
            vizinho = gerar_vizinho_1shift(solucao_atual)
            custo_vizinho, atraso_vizinho = calcular_custo_sa(vizinho, matriz, janelas)
            
            diff = custo_vizinho - custo_atual
            
            # Critério de Aceitação de Metropolis
            # math.exp(-diff / t) é a probabilidade de aceitar uma solução pior, que diminui à medida que a temperatura cai
            if diff < 0 or random.random() < math.exp(-diff / t): # Aceita a solução vizinha se for melhor ou com uma probabilidade que diminui com o tempo
                solucao_atual = vizinho
                custo_atual = custo_vizinho
                atraso_atual = atraso_vizinho
                
                # Guarda a melhor solução estritamente viável encontrada
                # Se a atual for viável (atraso == 0) e melhor que a nossa melhor viável conhecida
                if atraso_atual == 0:
                    if (melhor_atraso > 0 or custo_atual < melhor_custo): # caso nao tenha uma solução sem atraso ou seja melhor que a melhor rota sem atraso
                        melhor_solucao = list(solucao_atual)
                        melhor_custo = custo_atual
                        melhor_atraso = 0.0
                # Se nunca achamos uma viável, aceita a que tiver o menor atraso/custo ponderado
                elif melhor_atraso > 0 and custo_atual < melhor_custo:
                    melhor_solucao = list(solucao_atual)
                    melhor_custo = custo_atual
                    melhor_atraso = atraso_atual
                    
        t *= alfa  # Resfria a temperatura
        
    return melhor_solucao, melhor_custo, melhor_atraso


# --- EXECUÇÃO DOS EXPERIMENTOS ---
def executar():
    pasta = Path("AFG")
    otimos = carregar_otimos("otimos_conhecidosAFG.csv")
    resultados = []

    print(f"{'Instância':<15} | {'Ótimo':<8} | {'SA-Result':<9} | {'Gap %':<8} | {'Status':<8} | {'Tempo(ms)':<8}")
    print("-" * 78)

    arquivos = sorted(pasta.glob("*.tw"))
    for arq in arquivos:
        matriz, janelas = carregar_instancia(str(arq))
        otimo = otimos.get(arq.name)
        
        start_t = perf_counter()
        
        # 1. Tenta o guloso
        rota_inicial = tsptw_guloso(matriz, janelas)
        
        # 2. Se o guloso falhar, gera uma rota padrão trivial para o SA consertar
        if not rota_inicial:
            n = len(matriz)
            rota_inicial = list(range(n)) + [0]
            
        # 3. Executa o Simulated Annealing
        rota_final, custo_final, atraso_final = simulated_annealing(rota_inicial, matriz, janelas)
        tempo_proc = (perf_counter() - start_t) * 1000
        
        # Define se a solução final respeitou as restrições
        if atraso_final == 0:
            status = "Viável"
            res_str = f"{custo_final:.2f}"
            gap = ((custo_final - otimo) / otimo * 100) if otimo else 0.0
            gap_str = f"{gap:.2f}%"
        else:
            status = "Inviável"
            res_str = "Falhou"
            gap_str = "-"

        print(f"{arq.name:<15} | {str(otimo):>8} | {res_str:>9} | {gap_str:>8} | {status:<8} | {tempo_proc:>8.2f}")
        
        resultados.append({
            "instancia": arq.name,
            "otimo": otimo,
            "tempo_sa": res_str,
            "gap": gap_str,
            "status": status,
            "tempo_exec_ms": f"{tempo_proc:.3f}"
        })

    with open("resultado_SimulatedAnnealingAFG.csv", "w", encoding="utf-8", newline="") as f:
        campos = ["instancia", "otimo", "tempo_sa", "gap", "status", "tempo_exec_ms"]
        writer = csv.DictWriter(f, fieldnames=campos)
        writer.writeheader()
        writer.writerows(resultados)

if __name__ == "__main__":
    executar()