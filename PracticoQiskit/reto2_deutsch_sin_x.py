# Reto práctico 2: ¿qué ocurre en el algoritmo de Deutsch si NO se aplica X sobre la ancilla q1?
# Se ejecutan los 4 oráculos de lab3_algoritmo_deutsch.py en dos versiones:
#   (a) CON X en q1 (ancilla en |->  -> hay Phase Kickback)
#   (b) SIN X en q1 (ancilla en |+>  -> NO hay Phase Kickback)
from qiskit import QuantumCircuit
from qiskit_aer import AerSimulator


def construir_oraculo(tipo: str) -> QuantumCircuit:
    oraculo = QuantumCircuit(2, name=f"U_f ({tipo})")
    if tipo == "constante_0":
        pass  # f(x) = 0
    elif tipo == "constante_1":
        oraculo.x(1)  # f(x) = 1
    elif tipo == "balanceada_identidad":
        oraculo.cx(0, 1)  # f(x) = x
    elif tipo == "balanceada_inversa":
        oraculo.x(0); oraculo.cx(0, 1); oraculo.x(0)  # f(x) = NOT x
    return oraculo


def ejecutar_deutsch(tipo: str, con_x: bool):
    qc = QuantumCircuit(2, 1)
    if con_x:
        qc.x(1)  # ancilla en |1>; sin esta línea la ancilla queda en |0>
    qc.h(0)
    qc.h(1)
    qc.compose(construir_oraculo(tipo), inplace=True)
    qc.h(0)
    qc.measure(0, 0)
    counts = AerSimulator().run(qc, shots=1000).result().get_counts()
    return qc, counts


casos = ["constante_0", "constante_1", "balanceada_identidad", "balanceada_inversa"]
for con_x in (True, False):
    print("=== " + ("CON X en la ancilla (correcto)" if con_x else "SIN X en la ancilla (Reto 2)") + " ===")
    for caso in casos:
        _, res = ejecutar_deutsch(caso, con_x)
        bit = max(res, key=res.get)
        veredicto = "CONSTANTE" if bit == "0" else "BALANCEADA"
        tipo_real = "CONSTANTE" if caso.startswith("constante") else "BALANCEADA"
        marca = "OK" if veredicto == tipo_real else "ERROR (veredicto incorrecto)"
        print(f"Oráculo: {caso:22s} | conteos q0: {dict(res)} -> {veredicto:10s} | real: {tipo_real:10s} | {marca}")
    print()
