import random

def eh_primo(n):
    if n <= 1:
        return False
    if n <= 3:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

def gerar_primo(min=100, max=300):
    while True:
        candidato = random.randint(min, max)
        if eh_primo(candidato):
            return candidato

def mdc(a, b):
    while b:
        a, b = b, a % b
    return a

def inverso_modular(a, m):
    m0 = m
    x0, x1 = 0, 1
    while a > 1:
        q = a // m
        a, m = m, a % m
        x0, x1 = x1 - q * x0, x0
    return x1 + m0 if x1 < 0 else x1

def gerar_chaves():
    p = gerar_primo()
    q = gerar_primo()
    while q == p:
        q = gerar_primo()

    print(f"Primos p = {p}, q = {q}")

    n = p * q
    phi = (p - 1) * (q - 1)

    # Escolher e tal que mdc(e, phi) = 1
    e = 3
    while mdc(e, phi) != 1:
        e += 2

    d = inverso_modular(e, phi)

    print(f"Chave pública: (n = {n}, e = {e})")
    print(f"Chave privada: (n = {n}, d = {d})")

    return ((n, e), (n, d))

def criptografar(mensagem, chave_publica):
    n, e = chave_publica
    return pow(mensagem, e, n)

def descriptografar(cifra, chave_privada):
    n, d = chave_privada
    return pow(cifra, d, n)

def main():
    print("=== Algoritmo RSA sem bibliotecas externas ===")
    pub, priv = gerar_chaves()

    mensagem = int(input("\nDigite um número inteiro para criptografar: "))
    cifra = criptografar(mensagem, pub)
    print(f"Mensagem criptografada: {cifra}")

    mensagem_recuperada = descriptografar(cifra, priv)
    print(f"Mensagem descriptografada: {mensagem_recuperada}")

    if mensagem == mensagem_recuperada:
        print("\n Sucesso! Mensagem foi corretamente criptografada e recuperada.")
    else:
        print("\n Erro: mensagem recuperada não bate com a original.")

if __name__ == "__main__":
    main()
