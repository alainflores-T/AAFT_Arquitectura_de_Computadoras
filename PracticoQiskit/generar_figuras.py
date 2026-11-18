# Genera las figuras (PNG) del laboratorio usando el backend NO interactivo de matplotlib (Agg).
# Todas las figuras salen de simulaciones reales con qiskit-aer; no se usa plt.show().
import math
import os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from qiskit import QuantumCircuit
from qiskit.quantum_info import Statevector
from qiskit.visualization import plot_histogram, plot_bloch_multivector
from qiskit_aer import AerSimulator

DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "figuras")
os.makedirs(DIR, exist_ok=True)
sim = AerSimulator()


def guardar(fig, nombre):
    fig.savefig(f"{DIR}/{nombre}", dpi=150, bbox_inches="tight", facecolor="white")
    plt.close(fig)
    print("guardada:", nombre)


# ---------- Lab 1: circuito de Bell + histograma ----------
qc1 = QuantumCircuit(2, 2)
qc1.h(0); qc1.cx(0, 1); qc1.measure([0, 1], [0, 1])
guardar(qc1.draw(output="mpl"), "lab1_circuito.png")
c1 = sim.run(qc1, shots=10000).result().get_counts()
guardar(plot_histogram(c1, title="Lab 1 - Estado de Bell |Φ+> (10000 disparos)"), "lab1_histograma.png")

# ---------- Lab 2: curva P(0)=cos^2(theta/2) con puntos simulados + esferas de Bloch ----------
def p0_sim(theta):
    qc = QuantumCircuit(1, 1); qc.ry(theta, 0); qc.measure(0, 0)
    c = sim.run(qc, shots=10000).result().get_counts()
    return c.get("0", 0) / 10000

fig, ax = plt.subplots(figsize=(7, 4))
xs = [i * math.pi / 100 for i in range(101)]
ax.plot(xs, [math.cos(t / 2) ** 2 for t in xs], label="teoría cos²(θ/2)")
pts = [0, math.pi / 4, math.pi / 2, math.pi]
ax.scatter(pts, [p0_sim(t) for t in pts], color="red", zorder=3, label="simulación (10000 disparos)")
ax.set_xlabel("θ (rad)"); ax.set_ylabel("P(|0>)"); ax.grid(alpha=0.3); ax.legend()
ax.set_title("Lab 2 - Rotación Ry(θ): probabilidad de medir |0>")
guardar(fig, "lab2_curva_probabilidad.png")

# plot_bloch_multivector crea su propia figura: una por ángulo, luego se unen con imágenes
import io
from PIL import Image
nombres = ["θ = 0", "θ = π/4", "θ = π/2", "θ = π"]
imgs = []
for t, n in zip(pts, nombres):
    q = QuantumCircuit(1); q.ry(t, 0)
    f = plot_bloch_multivector(Statevector.from_instruction(q), title=n)
    buf = io.BytesIO(); f.savefig(buf, format="png", dpi=120, bbox_inches="tight", facecolor="white"); plt.close(f)
    buf.seek(0); imgs.append(Image.open(buf).convert("RGB"))
h = max(i.height for i in imgs); w = sum(i.width for i in imgs)
lienzo = Image.new("RGB", (w, h), "white"); x = 0
for i in imgs:
    lienzo.paste(i, (x, 0)); x += i.width
lienzo.save(f"{DIR}/lab2_esferas_bloch.png"); print("guardada: lab2_esferas_bloch.png")

# ---------- Lab 3: circuito de Deutsch (balanceada_identidad) + histogramas de los 4 oráculos ----------
def oraculo(tipo):
    o = QuantumCircuit(2, name=f"U_f ({tipo})")
    if tipo == "constante_1": o.x(1)
    elif tipo == "balanceada_identidad": o.cx(0, 1)
    elif tipo == "balanceada_inversa": o.x(0); o.cx(0, 1); o.x(0)
    return o

def deutsch(tipo, con_x=True):
    qc = QuantumCircuit(2, 1)
    if con_x: qc.x(1)
    qc.h(0); qc.h(1)
    qc.compose(oraculo(tipo), inplace=True)
    qc.h(0); qc.measure(0, 0)
    return qc, sim.run(qc, shots=1000).result().get_counts()

casos = ["constante_0", "constante_1", "balanceada_identidad", "balanceada_inversa"]
qc3, _ = deutsch("balanceada_identidad")
guardar(qc3.draw(output="mpl"), "lab3_circuito_balanceada_identidad.png")
res3 = {c: deutsch(c)[1] for c in casos}
guardar(plot_histogram(list(res3.values()), legend=casos, title="Lab 3 - Deutsch: medición de q0 (1000 disparos)"), "lab3_histograma.png")

# ---------- Lab 4: circuito de teletransportación + histograma de Bob ----------
qc4 = QuantumCircuit(3, 3)
qc4.ry(math.pi / 3, 0); qc4.barrier()
qc4.h(1); qc4.cx(1, 2); qc4.barrier()
qc4.cx(0, 1); qc4.h(0); qc4.barrier()
qc4.measure(0, 0); qc4.measure(1, 1); qc4.barrier()
qc4.cx(1, 2); qc4.cz(0, 2); qc4.barrier()
qc4.measure(2, 2)
guardar(qc4.draw(output="mpl"), "lab4_circuito.png")
c4 = sim.run(qc4, shots=10000).result().get_counts()
bob = {"0": 0, "1": 0}
for b, n in c4.items(): bob[b[0]] += n
fig, ax = plt.subplots(figsize=(5, 4))
ax.bar(["|0>", "|1>"], [bob["0"] / 100, bob["1"] / 100], color=["#4c72b0", "#dd8452"], label="simulación")
ax.scatter(["|0>", "|1>"], [75, 25], color="black", zorder=3, marker="_", s=900, label="teoría 75% / 25%")
ax.set_ylabel("Probabilidad (%)"); ax.set_title("Lab 4 - Cúbit de Bob (q2) tras teletransportar"); ax.legend()
guardar(fig, "lab4_histograma_bob.png")

# ---------- Reto 1: |Ψ+> ----------
r1 = QuantumCircuit(2, 2)
r1.h(0); r1.cx(0, 1); r1.x(1); r1.measure([0, 1], [0, 1])
guardar(r1.draw(output="mpl"), "reto1_circuito.png")
guardar(plot_histogram(sim.run(r1, shots=10000).result().get_counts(), title="Reto 1 - Estado de Bell |Ψ+> (10000 disparos)"), "reto1_histograma.png")

# ---------- Reto 2: Deutsch sin X en la ancilla ----------
res_sin = {c: deutsch(c, con_x=False)[1] for c in casos}
guardar(plot_histogram(list(res_sin.values()), legend=casos, title="Reto 2 - Deutsch SIN X en la ancilla: q0 siempre '0'"), "reto2_histograma_sin_x.png")
q2, _ = deutsch("balanceada_identidad", con_x=False)
guardar(q2.draw(output="mpl"), "reto2_circuito_sin_x.png")
