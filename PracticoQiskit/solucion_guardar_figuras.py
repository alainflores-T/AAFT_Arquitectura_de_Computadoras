# Solución al problema anterior: guardar a disco con savefig en vez de plt.show().
import os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from qiskit import QuantumCircuit

qc = QuantumCircuit(2, 2)
qc.h(0)
qc.cx(0, 1)
qc.measure([0, 1], [0, 1])
fig = qc.draw(output="mpl")
os.makedirs("figuras", exist_ok=True)
fig.savefig("figuras/circuito_solucion_savefig.png", dpi=150, bbox_inches="tight")
print("Figura guardada con savefig en figuras/circuito_solucion_savefig.png (sin advertencias)")
