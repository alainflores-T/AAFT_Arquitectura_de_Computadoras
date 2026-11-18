# Evidencia del problema "UserWarning: FigureCanvasAgg is non-interactive" (tabla 7 de la guía).
# Se ejecuta en un servidor SIN entorno gráfico (backend Agg) e intenta mostrar el circuito con plt.show().
import matplotlib
matplotlib.use("Agg")  # backend no interactivo: lo que ocurre en Ubuntu Server / WSL sin X11
import matplotlib.pyplot as plt
from qiskit import QuantumCircuit

qc = QuantumCircuit(2, 2)
qc.h(0)
qc.cx(0, 1)
qc.measure([0, 1], [0, 1])
qc.draw(output="mpl")
plt.show()  # <- aquí aparece la advertencia: no hay ventana donde mostrar la figura
print("plt.show() no abrió ninguna ventana (backend:", matplotlib.get_backend() + ")")
