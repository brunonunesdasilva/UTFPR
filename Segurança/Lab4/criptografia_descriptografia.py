def criptografar(texto):
    # 1. Aplicar Run-length (RLE)
    rle = []
    count = 1
    for i in range(1, len(texto)):
        if texto[i] == texto[i-1]:
            count += 1
        else:
            rle.append(str(count) + texto[i-1])
            count = 1
    rle.append(str(count) + texto[-1])
    rle_str = ''.join(rle)
    
    # 2. Aplicar substituição simples
    substituicao = {
        '0': 'a', '1': 'b', '2': 'c', '3': 'd', '4': 'e',
        '5': 'f', '6': 'g', '7': 'h', '8': 'i', '9': 'j',
        'a': 'k', 'b': 'l', 'c': 'm', 'd': 'n', 'e': 'o',
        'f': 'p', 'g': 'q', 'h': 'r', 'i': 's', 'j': 't',
        'k': 'u', 'l': 'v', 'm': 'w', 'n': 'x', 'o': 'y',
        'p': 'z', 'q': '0', 'r': '1', 's': '2', 't': '3',
        'u': '4', 'v': '5', 'w': '6', 'x': '7', 'y': '8',
        'z': '9'
    }
    substituido = ''.join([substituicao.get(c, c) for c in rle_str.lower()])
    
    # 3. Inverter a string
    invertido = substituido[::-1]
    
    return invertido

def descriptografar(texto_criptografado):
    # 1. Reverter a inversão
    invertido = texto_criptografado[::-1]
    
    # 2. Aplicar substituição inversa
    substituicao_inversa = {
        'a': '0', 'b': '1', 'c': '2', 'd': '3', 'e': '4',
        'f': '5', 'g': '6', 'h': '7', 'i': '8', 'j': '9',
        'k': 'a', 'l': 'b', 'm': 'c', 'n': 'd', 'o': 'e',
        'p': 'f', 'q': 'g', 'r': 'h', 's': 'i', 't': 'j',
        'u': 'k', 'v': 'l', 'w': 'm', 'x': 'n', 'y': 'o',
        'z': 'p', '0': 'q', '1': 'r', '2': 's', '3': 't',
        '4': 'u', '5': 'v', '6': 'w', '7': 'x', '8': 'y',
        '9': 'z'
    }
    substituido = ''.join([substituicao_inversa.get(c, c) for c in invertido.lower()])
    
    # 3. Decodificar RLE
    rle_decodificado = []
    i = 0
    while i < len(substituido):
        num_str = ''
        while i < len(substituido) and substituido[i].isdigit():
            num_str += substituido[i]
            i += 1
        if i < len(substituido):
            char = substituido[i]
            rle_decodificado.append(int(num_str) * char)
            i += 1
        else:
            break
    
    return ''.join(rle_decodificado)

texto_original = "aaabbbcc"
print("Texto original:", texto_original)

texto_criptografado = criptografar(texto_original)
print("Texto criptografado:", texto_criptografado)

texto_descriptografado = descriptografar(texto_criptografado)
print("Texto descriptografado:", texto_descriptografado)