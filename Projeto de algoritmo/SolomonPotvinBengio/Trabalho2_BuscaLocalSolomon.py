import sys
import csv
from pathlib import Path
from time import perf_counter

# --- FUNÇÕES DE CARREGAMENTO (PADRÃO LANGEVIN) ---
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
            if inst and not inst.lower().endswith(".txt"): inst += ".txt"
            cost = linha.get("otimo") or linha.get("Cost") or "0"
            if inst:
                try: otimos[inst] = float(cost)
                except: otimos[inst] = None
    return otimos

# --- CÁLCULO DE CUSTO E VIABILIDADE ---
def calcular_tempo_total(rota, matriz, janelas):
    """Calcula o tempo de conclusão da rota considerando janelas e esperas."""
    tempo = max(0.0, janelas[rota[0]][0]) # Início no depósito, espera se chegar cedo
    for i in range(len(rota) - 1):
        u, v = rota[i], rota[i+1] # Nó atual e próximo
        chegada = tempo + matriz[u][v] # Tempo atual + tempo de viagem
        if chegada > janelas[v][1]: # Condicao de inviabilidade: chegou após o fim da janela
            return float('inf')
        tempo = max(chegada, janelas[v][0]) # Espera se chegar cedo
    return tempo

# --- HEURÍSTICA GULOSA (CONSTRUÇÃO) ---
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
        
    chegada_final = calcular_tempo_total(rota + [deposito], matriz, janelas)
    if chegada_final == float('inf'): return None
    return rota + [deposito]

# --- BUSCA LOCAL 1-SHIFT (FORWARD & BACKWARD) ---
def busca_local_1shift(rota_inicial, matriz, janelas):
    melhor_rota = list(rota_inicial)
    melhor_tempo = calcular_tempo_total(melhor_rota, matriz, janelas)
    melhorou = True

    while melhorou: #executa enquanto houver melhoria
        melhorou = False
        # Percorre todas as cidades, exceto os depósitos (início e fim)
        for i in range(1, len(melhor_rota) - 1):
            rota_aux = list(melhor_rota)
            no_removido = rota_aux.pop(i)
            
            # Tenta reinserir o nó em todas as posições j possíveis
            # Isso cobre movimentos para trás (j < i) e para frente (j > i)
            for j in range(1, len(rota_inicial) - 1):
                if i == j: continue
                
                nova_rota = list(rota_aux)
                nova_rota.insert(j, no_removido)
                
                tempo_atual = calcular_tempo_total(nova_rota, matriz, janelas)# Verifica se a nova rota é melhor que a melhor encontrada até agora
                
                if tempo_atual < melhor_tempo:
                    melhor_tempo = tempo_atual
                    melhor_rota = nova_rota
                    melhorou = True
                    break # Estratégia First Improvement
            if melhorou: break
            
    return melhor_rota, melhor_tempo

# --- EXPERIMENTOS ---
def executar():
    pasta = Path("SolomonPotvinBengio")
    otimos = carregar_otimos("otimos_conhecidosSolomon.csv")
    resultados = []

    print(f"{'Instância':<15} | {'Ótimo':<8} | {'1-Shift':<8} | {'Gap %':<8} | {'Tempo(ms)':<8}")
    print("-" * 65)

    arquivos = sorted(pasta.glob("*.txt"))
    for arq in arquivos:
        matriz, janelas = carregar_instancia(str(arq))
        otimo = otimos.get(arq.name)
        
        start_t = perf_counter()
        rota_inicial = tsptw_guloso(matriz, janelas)
        
        if rota_inicial:
            rota_final, tempo_final = busca_local_1shift(rota_inicial, matriz, janelas)
            tempo_proc = (perf_counter() - start_t) * 1000
            
            gap = ((tempo_final - otimo) / otimo * 100) if otimo else 0.0
            
            res_str = f"{tempo_final:.2f}"
            gap_str = f"{gap:.2f}%"
        else:
            res_str = "Inviável"
            gap_str = "-"
            tempo_proc = 0

        print(f"{arq.name:<15} | {str(otimo):>8} | {res_str:>8} | {gap_str:>8} | {tempo_proc:>8.2f}")
        
        resultados.append({
            "instancia": arq.name,
            "otimo": otimo,
            "tempo_1shift": res_str,
            "gap": gap_str,
            "tempo_exec_ms": f"{tempo_proc:.3f}"
        })

    with open("resultado_1shiftSolomon.csv", "w", encoding="utf-8", newline="") as f:
        campos = ["instancia", "otimo", "tempo_1shift", "gap", "tempo_exec_ms"]
        writer = csv.DictWriter(f, fieldnames=campos)
        writer.writeheader()
        writer.writerows(resultados)

if __name__ == "__main__":
    executar()