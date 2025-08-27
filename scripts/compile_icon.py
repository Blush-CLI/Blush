import os

ICO_DIR = os.path.join(os.path.dirname(__file__), "..", "assets")
ICO_FILE = os.path.abspath(os.path.join(ICO_DIR, "blush.ico")).replace("\\", "/")
RC_PATH = os.path.abspath(os.path.join(ICO_DIR, "ico.rc")).replace("\\", "/")
RES_PATH = os.path.abspath(os.path.join(ICO_DIR, "icon.res")).replace("\\", "/")

if not os.path.exists(ICO_DIR):
    os.makedirs(ICO_DIR)

if not os.path.exists(ICO_FILE):
    raise FileNotFoundError(f"Nie znaleziono pliku ikony: {ICO_FILE}")

with open(RC_PATH, "w") as f:
    f.write(f'IDI_ICON1 ICON "{ICO_FILE}"')

os.system(f'windres "{RC_PATH}" -O coff -o "{RES_PATH}"')
os.remove(RC_PATH)