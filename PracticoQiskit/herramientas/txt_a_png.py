# Convierte un archivo .txt con la salida REAL de la consola en una captura estilo terminal (PNG).
# Uso: python3 txt_a_png.py entrada.txt salida.png "comando ejecutado" [prompt]
# El texto de la salida se copia tal cual del archivo; solo se dibuja la ventana y la línea del comando.
import sys
from PIL import Image, ImageDraw, ImageFont

FUENTE = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"
entrada, salida, comando = sys.argv[1], sys.argv[2], sys.argv[3]
prompt = sys.argv[4] if len(sys.argv) > 4 else "(.venv) $ "

import textwrap
lineas = []
for l in open(entrada, encoding="utf-8").read().rstrip("\n").split("\n"):
    # solo se parten líneas muy largas para que quepan en la imagen; el texto no se modifica
    lineas.extend(textwrap.wrap(l, 110, drop_whitespace=False, replace_whitespace=False) or [""])
if comando:
    lineas = [prompt + comando] + lineas
tam = 18
fuente = ImageFont.truetype(FUENTE, tam)
negrita = ImageFont.truetype(FUENTE.replace("Mono.ttf", "Mono-Bold.ttf"), tam)
ancho_car = fuente.getlength("M")
alto_lin = int(tam * 1.35)
cols = max(len(l) for l in lineas)
W = int(max(cols * ancho_car + 60, 700))
barra = 38
H = barra + 20 + alto_lin * len(lineas) + 20
img = Image.new("RGB", (W, H), "#1e1e1e")
d = ImageDraw.Draw(img)
d.rectangle([0, 0, W, barra], fill="#3a3a3a")
for i, c in enumerate(["#ff5f56", "#ffbd2e", "#27c93f"]):
    d.ellipse([14 + i * 24, 12, 28 + i * 24, 26], fill=c)
d.text((W // 2 - 90, 9), "Terminal - laboratorio Qiskit", font=fuente, fill="#cccccc")
y = barra + 14
for n, l in enumerate(lineas):
    if comando and n == 0:
        d.text((24, y), prompt, font=negrita, fill="#7ee787")
        d.text((24 + fuente.getlength(prompt), y), comando, font=fuente, fill="#ffffff")
    else:
        color = "#ff7b72" if ("Error" in l or "error" in l or "Warning" in l or "ERROR" in l) else "#d4d4d4"
        d.text((24, y), l, font=fuente, fill=color)
    y += alto_lin
img.save(salida)
print("captura:", salida, img.size)
