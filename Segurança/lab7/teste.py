import hashlib

with open("exemplo_malicioso_por_hash.bin", "rb") as f:
    dados = f.read()
    print(hashlib.sha256(dados).hexdigest())
