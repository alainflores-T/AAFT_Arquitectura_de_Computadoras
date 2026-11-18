# Reto práctico 1: generar el estado de Bell |Ψ+> = (|01> + |10>)/√2
# Modificación de lab1_hola_mundo.py: se agrega una compuerta Pauli-X en el cúbit 1
# justo después de la CNOT (equivale a ponerla antes). Así |Φ+> = (|00> + |11>)/√2 pasa a (|01> + |10>)/√2.
from qiskit import QuantumCircuit
from qiskit.quantum_info import Statevector
from qiskit_aer import AerSimulator

qc = QuantumCircuit(2, 2)
qc.h(0)       # superposición en q0
qc.cx(0, 1)   # entrelazamiento: |Φ+> = (|00> + |11>)/√2
qc.x(1)       # Pauli-X sobre q1: invierte el cúbit 1 en cada rama -> (|01> + |10>)/√2

# Verificación teórica ANTES de medir (vector de estado sin mediciones)
sv = Statevector.from_instruction(qc)
print("VECTOR DE ESTADO (orden Qiskit |q1 q0>):")
for i, amp in enumerate(sv.data):
    print(f"  |{i:02b}>: {amp.real:+.4f}{amp.imag:+.4f}j")

qc.measure([0, 1], [0, 1])
print("\nCIRCUITO CUÁNTICO (Reto 1):")
print(qc.draw(output="text"))

shots = 10000
counts = AerSimulator().run(qc, shots=shots).result().get_counts(qc)
print(f"\nRESULTADOS DE LA MEDICIÓN ({shots} disparos):")
for bitstring, count in sorted(counts.items()):
    print(f"Estado |{bitstring}⟩: {count} ocurrencias ({count / shots * 100:.2f}%)")
