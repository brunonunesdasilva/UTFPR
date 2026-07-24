import hashlib

# Lista de assinaturas de bytes maliciosos conhecidas (em binário)
MALICIOUS_SIGNATURES = [
    b'malware!',  # Forma ASCII
    b'\x68\x61\x63\x6B\x65\x64',  # "hacked"
    b'\x90\x90\x90\x90',  # NOP sled (opcional)
]

# Lista de hashes SHA-256 de arquivos maliciosos conhecidos
MALICIOUS_HASHES = {
    '5d41402abc4b2a76b9719d911017c592': 'Virus.Simple.1',
    '9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08': 'Trojan.Generic.2',
    'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855': 'Empty.Malicious.File'
}

def verificar_arquivo(caminho_arquivo):
    try:
        with open(caminho_arquivo, "rb") as f:
            dados = f.read()

        # Verificação por assinatura de bytes
        for assinatura in MALICIOUS_SIGNATURES:
            if assinatura in dados:
                print("⚠ Arquivo malicioso detectado por assinatura de bytes!")
                print(f"Assinatura encontrada: {assinatura}")
                return

        # Verificação por hash SHA-256
        hash_arquivo = hashlib.sha256(dados).hexdigest()
        if hash_arquivo in MALICIOUS_HASHES:
            print("⚠ Arquivo malicioso detectado por hash!")
            print(f"Hash malicioso: {hash_arquivo}")
            print(f"Nome da ameaça: {MALICIOUS_HASHES[hash_arquivo]}")
            return

        print("✅ Arquivo limpo.")
    except FileNotFoundError:
        print("❌ Arquivo não encontrado.")
    except PermissionError:
        print("❌ Permissão negada para ler o arquivo.")
    except Exception as e:
        print(f"❌ Erro ao verificar o arquivo: {e}")

# ===== Preparação dos arquivos de teste =====

# Arquivo 1: Malicioso por assinatura
with open("exemplo_malicioso_por_assinatura.bin", "wb") as f:
    f.write(b"Dados inocentes \x90\x90\x90\x90 mais dados inocentes")

# Arquivo 2: Malicioso por hash
with open("exemplo_malicioso_por_hash.bin", "wb") as f:
    f.write(b"")  # Arquivo vazio (hash conhecido)

# Arquivo 3: Limpo
with open("exemplo_limpo.txt", "wb") as f:
    f.write(b"Este arquivo esta completamente limpo e seguro")

# ===== Execução dos testes =====
print("\n--- Teste 1: Arquivo malicioso (por assinatura) ---")
verificar_arquivo("exemplo_malicioso_por_assinatura.bin")

print("\n--- Teste 2: Arquivo malicioso (por hash) ---")
verificar_arquivo("exemplo_malicioso_por_hash.bin")

print("\n--- Teste 3: Arquivo limpo ---")
verificar_arquivo("exemplo_limpo.txt")