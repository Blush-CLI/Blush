import sys
import os
import platform
import json
import zipfile
import tempfile
import shutil
import subprocess
import locale
from pathlib import Path
from PySide6.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                               QHBoxLayout, QLabel, QPushButton, QTextEdit, 
                               QProgressBar, QStackedWidget, QCheckBox, QMessageBox, QRadioButton, QButtonGroup, QComboBox, QGraphicsOpacityEffect)
from PySide6.QtCore import Qt, QThread, Signal, QTimer, QPropertyAnimation, QEasingCurve, Property, QEventLoop
from PySide6.QtGui import QFont, QPalette, QColor, QPainter, QLinearGradient, QPen, QPixmap, QIcon
from PySide6.QtNetwork import QNetworkAccessManager, QNetworkRequest, QNetworkReply
from PySide6.QtCore import QUrl

TRANSLATIONS = {
    "en": {
        "title": "Blush Installer",
        "welcome": "Welcome to Blush Installer",
        "description": "This wizard will guide you through the installation of Blush CLI.",
        "next": "Next",
        "back": "Back",
        "install": "Install",
        "finish": "Finish",
        "cancel": "Cancel",
        "license": "License Agreement",
        "accept": "I accept the terms in the License Agreement",
        "installing": "Installing Blush...",
        "download_progress": "Downloading: {progress}%",
        "install_progress": "Installing: {progress}%",
        "complete": "Installation Complete!",
        "complete_msg": "Blush has been successfully installed.",
        "error": "Installation Error",
        "select_lang": "Select Language:",
        "options": "Installation Options",
        "desktop_shortcut": "Create desktop shortcut",
        "start_menu_shortcut": "Create Start Menu shortcut",
        "reinstall": "Reinstall (redownload and replace files)",
        "select_action": "Select an action:",
        "install_reinstall": "Install / Reinstall",
        "modify": "Modify",
        "uninstall": "Uninstall",
        "modification_complete": "Modification Complete!",
        "modification_msg": "Changes have been applied successfully.",
        "uninstallation_complete": "Uninstallation Complete!",
        "uninstallation_msg": "Blush has been successfully uninstalled.",
        "uninstalling": "Uninstalling Blush...",
        "desktop_tooltip": "Creates a shortcut icon on your desktop for quick access.",
        "startmenu_tooltip": "Adds Blush to the Windows Start Menu.",
        "reinstall_tooltip": "Redownloads and replaces the existing installation files.",
        "applying": "Applying changes...",
        "preparing": "Preparing...",
        "extracting": "Extracting files...",
        "installing_components": "Installing components...",
        "finalizing": "Finalizing installation...",
        "connecting": "Connecting to download server...",
        "downloading": "Downloading",
        "download_complete": "Download complete. Installing...",
        "installation_complete": "Installation complete!",
        "uninstallation_complete_status": "Uninstallation complete!",
        "modification_complete_status": "Modification complete!",
        "confirm_uninstall_title": "Confirm Uninstall",
        "confirm_uninstall_msg": "Are you sure you want to uninstall Blush? This will remove all files and shortcuts.",
        "error_prefix": "An error occurred:",
    },
    "es": {
        "title": "Instalador de Blush",
        "welcome": "Bienvenido al Instalador de Blush",
        "description": "Este asistente le guiará a través de la instalación de Blush CLI.",
        "next": "Siguiente",
        "back": "Atrás",
        "install": "Instalar",
        "finish": "Finalizar",
        "cancel": "Cancelar",
        "license": "Acuerdo de Licencia",
        "accept": "Acepto los términos del Acuerdo de Licencia",
        "installing": "Instalando Blush...",
        "download_progress": "Descargando: {progress}%",
        "install_progress": "Instalando: {progress}%",
        "complete": "¡Instalación Completa!",
        "complete_msg": "Blush se ha instalado correctamente.",
        "error": "Error de Instalación",
        "select_lang": "Seleccionar Idioma:",
        "options": "Opciones de Instalación",
        "desktop_shortcut": "Crear acceso directo en el escritorio",
        "start_menu_shortcut": "Crear acceso directo en el menú de inicio",
        "reinstall": "Reinstalar (volver a descargar y reemplazar archivos)",
        "select_action": "Seleccione una acción:",
        "install_reinstall": "Instalar / Reinstalar",
        "modify": "Modificar",
        "uninstall": "Desinstalar",
        "modification_complete": "¡Modificación Completa!",
        "modification_msg": "Los cambios se han aplicado correctamente.",
        "uninstallation_complete": "¡Desinstalación Completa!",
        "uninstallation_msg": "Blush se ha desinstalado correctamente.",
        "uninstalling": "Desinstalando Blush...",
        "desktop_tooltip": "Crea un icono de acceso directo en su escritorio para un acceso rápido.",
        "startmenu_tooltip": "Agrega Blush al menú de inicio de Windows.",
        "reinstall_tooltip": "Vuelve a descargar y reemplaza los archivos de instalación existentes.",
        "applying": "Aplicando cambios...",
        "preparing": "Preparando...",
        "extracting": "Extrayendo archivos...",
        "installing_components": "Instalando componentes...",
        "finalizing": "Finalizando instalación...",
        "connecting": "Conectando al servidor de descarga...",
        "downloading": "Descargando",
        "download_complete": "Descarga completa. Instalando...",
        "installation_complete": "¡Instalación completa!",
        "uninstallation_complete_status": "¡Desinstalación completa!",
        "modification_complete_status": "¡Modificación completa!",
        "confirm_uninstall_title": "Confirmar Desinstalación",
        "confirm_uninstall_msg": "¿Está seguro de que desea desinstalar Blush? Esto eliminará todos los archivos y accesos directos.",
        "error_prefix": "Ocurrió un error:",
    },
    "pl": {
        "title": "Instalator Blush",
        "welcome": "Witamy w Instalatorze Blush",
        "description": "Ten kreator poprowadzi Cię przez instalację Blush CLI.",
        "next": "Dalej",
        "back": "Wstecz",
        "install": "Instaluj",
        "finish": "Zakończ",
        "cancel": "Anuluj",
        "license": "Umowa Licencyjna",
        "accept": "Akceptuję warunki Umowy Licencyjnej",
        "installing": "Instalowanie Blush...",
        "download_progress": "Pobieranie: {progress}%",
        "install_progress": "Instalowanie: {progress}%",
        "complete": "Instalacja Zakończona!",
        "complete_msg": "Blush został pomyślnie zainstalowany.",
        "error": "Błąd Instalacji",
        "select_lang": "Wybierz Język:",
        "options": "Opcje Instalacji",
        "desktop_shortcut": "Utwórz skrót na pulpicie",
        "start_menu_shortcut": "Utwórz skrót w menu Start",
        "reinstall": "Reinstaluj (pobierz ponownie i zastąp pliki)",
        "select_action": "Wybierz akcję:",
        "install_reinstall": "Instaluj / Reinstaluj",
        "modify": "Modyfikuj",
        "uninstall": "Odinstaluj",
        "modification_complete": "Modyfikacja Zakończona!",
        "modification_msg": "Zmiany zostały pomyślnie zastosowane.",
        "uninstallation_complete": "Odinstalowanie Zakończone!",
        "uninstallation_msg": "Blush został pomyślnie odinstalowany.",
        "uninstalling": "Odinstalowywanie Blush...",
        "desktop_tooltip": "Tworzy ikonę skrótu na pulpicie dla szybkiego dostępu.",
        "startmenu_tooltip": "Dodaje Blush do menu Start w systemie Windows.",
        "reinstall_tooltip": "Pobiera ponownie i zastępuje istniejące pliki instalacyjne.",
        "applying": "Zastosowanie zmian...",
        "preparing": "Przygotowywanie...",
        "extracting": "Wyodrębnianie plików...",
        "installing_components": "Instalowanie komponentów...",
        "finalizing": "Finalizowanie instalacji...",
        "connecting": "Łączenie z serwerem pobierania...",
        "downloading": "Pobieranie",
        "download_complete": "Pobieranie zakończone. Instalowanie...",
        "installation_complete": "Instalacja zakończona!",
        "uninstallation_complete_status": "Odinstalowanie zakończone!",
        "modification_complete_status": "Modyfikacja zakończona!",
        "confirm_uninstall_title": "Potwierdź Odinstalowanie",
        "confirm_uninstall_msg": "Czy na pewno chcesz odinstalować Blush? To usunie wszystkie pliki i skróty.",
        "error_prefix": "Wystąpił błąd:",
    }
}

STATIC_LICENSE_TEXT = """MIT License

Copyright (c) 2024 Blush

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE."""

class SpinnerWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.angle = 0
        self.setFixedSize(80, 80)
        
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.rotate)
        self.timer.start(16)
    
    def rotate(self):
        self.angle = (self.angle + 6) % 360
        self.update()
    
    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        
        center_x = self.width() / 2
        center_y = self.height() / 2
        radius = 30
        
        for i in range(12):
            angle = self.angle + i * 30
            opacity = 1.0 - (i / 12.0) * 0.7
            
            painter.save()
            painter.translate(center_x, center_y)
            painter.rotate(angle)
            
            color = QColor(100, 150, 255)
            color.setAlphaF(opacity)
            
            pen = QPen(color, 4.0, Qt.SolidLine, Qt.RoundCap)
            painter.setPen(pen)
            
            painter.drawLine(radius - 10, 0, radius, 0)
            painter.restore()
        
        painter.end()

class GradientProgressBar(QProgressBar):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setTextVisible(False)
        self.setFixedHeight(12)
        self._shine_pos = 0
        
        self.shine_timer = QTimer(self)
        self.shine_timer.timeout.connect(self._update_shine)
        self.shine_timer.start(30)
    
    def _update_shine(self):
        self._shine_pos = (self._shine_pos + 2) % (self.width() + 100)
        self.update()
    
    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        
        bg_rect = self.rect()
        painter.fillRect(bg_rect, QColor(40, 40, 50))
        
        if self.value() > 0:
            progress_width = int((self.value() / self.maximum()) * self.width())
            progress_rect = self.rect()
            progress_rect.setWidth(progress_width)
            
            gradient = QLinearGradient(0, 0, self.width(), 0)
            gradient.setColorAt(0, QColor(80, 120, 255))
            gradient.setColorAt(0.5, QColor(100, 150, 255))
            gradient.setColorAt(1, QColor(120, 180, 255))
            
            painter.fillRect(progress_rect, gradient)
            
            if progress_width > 0:
                shine_gradient = QLinearGradient(self._shine_pos - 50, 0, self._shine_pos + 50, 0)
                shine_gradient.setColorAt(0, QColor(255, 255, 255, 0))
                shine_gradient.setColorAt(0.5, QColor(255, 255, 255, 80))
                shine_gradient.setColorAt(1, QColor(255, 255, 255, 0))
                
                painter.fillRect(progress_rect, shine_gradient)
        
        painter.end()

class ModernButton(QPushButton):
    def __init__(self, text, parent=None, primary=False):
        super().__init__(text, parent)
        self.primary = primary
        self.setFixedHeight(45)
        self.setMinimumWidth(120)
        self.setCursor(Qt.PointingHandCursor)
        self._update_style()
    
    def _update_style(self):
        if self.primary:
            self.setStyleSheet("""
                QPushButton {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #5078ff, stop:1 #6090ff);
                    color: white;
                    border: none;
                    border-radius: 22px;
                    font-size: 14px;
                    font-weight: 600;
                    padding: 0 24px;
                }
                QPushButton:hover {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #6088ff, stop:1 #70a0ff);
                }
                QPushButton:pressed {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #4068ef, stop:1 #5080ef);
                }
                QPushButton:disabled {
                    background: #3a3a4a;
                    color: #666;
                }
            """)
        else:
            self.setStyleSheet("""
                QPushButton {
                    background: #2a2a3a;
                    color: #ccc;
                    border: 1px solid #3a3a4a;
                    border-radius: 22px;
                    font-size: 14px;
                    font-weight: 500;
                    padding: 0 24px;
                }
                QPushButton:hover {
                    background: #3a3a4a;
                    border-color: #4a4a5a;
                }
                QPushButton:pressed {
                    background: #1a1a2a;
                }
            """)

class DownloadThread(QThread):
    progress = Signal(int, int, int)
    finished = Signal(str)
    error = Signal(str)
    
    def __init__(self, url, destination):
        super().__init__()
        self.url = url
        self.destination = destination
        self.manager = QNetworkAccessManager()
        self.reply = None
        self.file = None
        
    def run(self):
        try:
            request = QNetworkRequest(QUrl(self.url))
            self.reply = self.manager.get(request)
            
            self.file = open(self.destination, 'wb')
            
            self.reply.downloadProgress.connect(self._on_progress)
            self.reply.finished.connect(self._on_finished)
            self.reply.readyRead.connect(self._on_ready_read)
            
            self.exec()
            
        except Exception as e:
            self.error.emit(str(e))
    
    def _on_progress(self, received, total):
        if total > 0:
            percentage = int((received / total) * 100)
            self.progress.emit(percentage, received, total)
    
    def _on_ready_read(self):
        if self.file and self.reply:
            self.file.write(self.reply.readAll().data())
    
    def _on_finished(self):
        if self.file:
            self.file.close()
        
        if self.reply.error() == QNetworkReply.NoError:
            self.finished.emit(self.destination)
        else:
            error_msg = self.reply.errorString()
            self.error.emit(error_msg)
        
        self.quit()

class InstallThread(QThread):
    progress = Signal(int)
    finished = Signal()
    error = Signal(str)
    
    def __init__(self, zip_path, system_type, create_desktop, create_startmenu):
        super().__init__()
        self.zip_path = zip_path
        self.system_type = system_type
        self.create_desktop = create_desktop
        self.create_startmenu = create_startmenu
    
    def run(self):
        try:
            home = Path.home()
            blush_dir = home / ".blush"
            blush_dir.mkdir(exist_ok=True)
            
            self.progress.emit(20)
            
            temp_dir = Path(tempfile.gettempdir()) / "blushinstaller"
            temp_dir.mkdir(exist_ok=True)
            
            self.progress.emit(40)
            
            with zipfile.ZipFile(self.zip_path, 'r') as zip_ref:
                zip_ref.extractall(temp_dir)
            
            self.progress.emit(60)
            
            for item in temp_dir.iterdir():
                dest = blush_dir / item.name
                if dest.exists():
                    if dest.is_dir():
                        shutil.rmtree(dest)
                    else:
                        dest.unlink()
                shutil.move(str(item), str(dest))
            
            self.progress.emit(80)
            
            if self.system_type == "Windows":
                exe_path = blush_dir / "blush.exe"
                if not exe_path.exists():
                    for file in blush_dir.iterdir():
                        if file.suffix == '.exe':
                            file.rename(exe_path)
                            break
                
                if exe_path.exists():
                    self._add_to_windows_path(str(blush_dir))
                    if self.create_startmenu:
                        self._create_start_menu_shortcut(str(blush_dir), str(exe_path))
                    if self.create_desktop:
                        self._create_desktop_shortcut_windows(str(blush_dir), str(exe_path))
            else:
                binary_path = blush_dir / "blush"
                if not binary_path.exists():
                    for file in blush_dir.iterdir():
                        if file.is_file() and os.access(file, os.X_OK):
                            file.rename(binary_path)
                            break
                
                if binary_path.exists():
                    os.chmod(binary_path, 0o755)
                    self._add_to_unix_path(str(blush_dir))
                    if self.create_desktop and self.system_type in ["Linux", "Darwin"]:
                        self._create_desktop_shortcut_unix(str(blush_dir), str(binary_path))
            
            self.progress.emit(100)
            
            shutil.rmtree(temp_dir, ignore_errors=True)
            
            self.finished.emit()
            
        except Exception as e:
            self.error.emit(str(e))
    
    def _add_to_windows_path(self, directory):
        try:
            import ctypes
            from ctypes import byref, wintypes
            import winreg
            key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, 'Environment', 0, winreg.KEY_ALL_ACCESS)
            try:
                current_path, _ = winreg.QueryValueEx(key, 'Path')
            except FileNotFoundError:
                current_path = ''
            
            if directory not in current_path.split(';'):
                new_path = current_path + ';' + directory if current_path else directory
                winreg.SetValueEx(key, 'Path', 0, winreg.REG_EXPAND_SZ, new_path)
            
            winreg.CloseKey(key)
            
            HWND_BROADCAST = 0xFFFF
            WM_SETTINGCHANGE = 0x1A
            SMTO_NORMAL = 0x0
            result = wintypes.DWORD()
            ctypes.windll.user32.SendMessageTimeoutW(
                HWND_BROADCAST,
                WM_SETTINGCHANGE,
                0,
                'Environment',
                SMTO_NORMAL,
                2000,
                byref(result)
            )
        except:
            pass
    
    def _create_start_menu_shortcut(self, blush_dir, exe_path):
        try:
            from win32com.client import Dispatch
            appdata = os.environ['APPDATA']
            programs_dir = os.path.join(appdata, 'Microsoft', 'Windows', 'Start Menu', 'Programs')
            blush_folder = os.path.join(programs_dir, 'Blush')
            if not os.path.exists(blush_folder):
                os.mkdir(blush_folder)
            shortcut_path = os.path.join(blush_folder, 'Blush.lnk')
            create_shortcut = True
            if os.path.exists(shortcut_path):
                shell = Dispatch('WScript.Shell')
                existing_shortcut = shell.CreateShortCut(shortcut_path)
                if existing_shortcut.TargetPath == exe_path:
                    create_shortcut = False
                else:
                    os.remove(shortcut_path)
            if create_shortcut:
                shell = Dispatch('WScript.Shell')
                shortcut = shell.CreateShortCut(shortcut_path)
                shortcut.TargetPath = exe_path
                shortcut.WorkingDirectory = blush_dir
                shortcut.save()
        except:
            pass
    
    def _create_desktop_shortcut_windows(self, blush_dir, exe_path):
        try:
            from win32com.client import Dispatch
            desktop = os.path.join(os.environ['USERPROFILE'], 'Desktop')
            shortcut_path = os.path.join(desktop, 'Blush.lnk')
            create_shortcut = True
            if os.path.exists(shortcut_path):
                shell = Dispatch('WScript.Shell')
                existing_shortcut = shell.CreateShortCut(shortcut_path)
                if existing_shortcut.TargetPath == exe_path:
                    create_shortcut = False
                else:
                    os.remove(shortcut_path)
            if create_shortcut:
                shell = Dispatch('WScript.Shell')
                shortcut = shell.CreateShortCut(shortcut_path)
                shortcut.TargetPath = exe_path
                shortcut.WorkingDirectory = blush_dir
                shortcut.save()
        except:
            pass
    
    def _add_to_unix_path(self, directory):
        try:
            home = Path.home()
            shell_configs = ['.bashrc', '.zshrc', '.profile']
            
            export_line = f'\nexport PATH="$PATH:{directory}"\n'
            
            for config in shell_configs:
                config_path = home / config
                if config_path.exists():
                    with open(config_path, 'r') as f:
                        content = f.read()
                    
                    if directory not in content:
                        with open(config_path, 'a') as f:
                            f.write(export_line)
        except:
            pass
    
    def _create_desktop_shortcut_unix(self, blush_dir, binary_path):
        try:
            home = Path.home()
            desktop = home / "Desktop"
            desktop.mkdir(exist_ok=True)
            shortcut_path = desktop / "Blush.desktop"
            with open(shortcut_path, 'w') as f:
                f.write("[Desktop Entry]\n")
                f.write("Name=Blush\n")
                f.write(f"Exec={binary_path}\n")
                f.write("Type=Application\n")
                f.write("Terminal=true\n")
            os.chmod(shortcut_path, 0o755)
        except:
            pass

class ModifyThread(QThread):
    progress = Signal(int)
    finished = Signal()
    error = Signal(str)
    
    def __init__(self, system_type, create_desktop, create_startmenu):
        super().__init__()
        self.system_type = system_type
        self.create_desktop = create_desktop
        self.create_startmenu = create_startmenu
    
    def run(self):
        try:
            home = Path.home()
            blush_dir = home / ".blush"
            if not blush_dir.exists():
                self.error.emit("Blush is not installed.")
                return
            
            self.progress.emit(20)
            
            if self.system_type == "Windows":
                exe_path = blush_dir / "blush.exe"
                if exe_path.exists():
                    if self.create_startmenu:
                        self._create_start_menu_shortcut(str(blush_dir), str(exe_path))
                    else:
                        self._remove_start_menu_shortcut()
                    if self.create_desktop:
                        self._create_desktop_shortcut_windows(str(blush_dir), str(exe_path))
                    else:
                        self._remove_desktop_shortcut_windows()
            else:
                binary_path = blush_dir / "blush"
                if binary_path.exists():
                    if self.create_desktop and self.system_type in ["Linux", "Darwin"]:
                        self._create_desktop_shortcut_unix(str(blush_dir), str(binary_path))
                    else:
                        self._remove_desktop_shortcut_unix()
            
            self.progress.emit(100)
            
            self.finished.emit()
            
        except Exception as e:
            self.error.emit(str(e))
    
    def _create_start_menu_shortcut(self, blush_dir, exe_path):
        try:
            from win32com.client import Dispatch
            appdata = os.environ['APPDATA']
            programs_dir = os.path.join(appdata, 'Microsoft', 'Windows', 'Start Menu', 'Programs')
            blush_folder = os.path.join(programs_dir, 'Blush')
            if not os.path.exists(blush_folder):
                os.mkdir(blush_folder)
            shortcut_path = os.path.join(blush_folder, 'Blush.lnk')
            create_shortcut = True
            if os.path.exists(shortcut_path):
                shell = Dispatch('WScript.Shell')
                existing_shortcut = shell.CreateShortCut(shortcut_path)
                if existing_shortcut.TargetPath == exe_path:
                    create_shortcut = False
                else:
                    os.remove(shortcut_path)
            if create_shortcut:
                shell = Dispatch('WScript.Shell')
                shortcut = shell.CreateShortCut(shortcut_path)
                shortcut.TargetPath = exe_path
                shortcut.WorkingDirectory = blush_dir
                shortcut.save()
        except:
            pass
    
    def _remove_start_menu_shortcut(self):
        try:
            appdata = os.environ['APPDATA']
            programs_dir = os.path.join(appdata, 'Microsoft', 'Windows', 'Start Menu', 'Programs')
            blush_folder = os.path.join(programs_dir, 'Blush')
            shortcut_path = os.path.join(blush_folder, 'Blush.lnk')
            if os.path.exists(shortcut_path):
                os.remove(shortcut_path)
            if os.path.exists(blush_folder) and not os.listdir(blush_folder):
                os.rmdir(blush_folder)
        except:
            pass
    
    def _create_desktop_shortcut_windows(self, blush_dir, exe_path):
        try:
            from win32com.client import Dispatch
            desktop = os.path.join(os.environ['USERPROFILE'], 'Desktop')
            shortcut_path = os.path.join(desktop, 'Blush.lnk')
            create_shortcut = True
            if os.path.exists(shortcut_path):
                shell = Dispatch('WScript.Shell')
                existing_shortcut = shell.CreateShortCut(shortcut_path)
                if existing_shortcut.TargetPath == exe_path:
                    create_shortcut = False
                else:
                    os.remove(shortcut_path)
            if create_shortcut:
                shell = Dispatch('WScript.Shell')
                shortcut = shell.CreateShortCut(shortcut_path)
                shortcut.TargetPath = exe_path
                shortcut.WorkingDirectory = blush_dir
                shortcut.save()
        except:
            pass
    
    def _remove_desktop_shortcut_windows(self):
        try:
            desktop = os.path.join(os.environ['USERPROFILE'], 'Desktop')
            shortcut_path = os.path.join(desktop, 'Blush.lnk')
            if os.path.exists(shortcut_path):
                os.remove(shortcut_path)
        except:
            pass
    
    def _create_desktop_shortcut_unix(self, blush_dir, binary_path):
        try:
            home = Path.home()
            desktop = home / "Desktop"
            desktop.mkdir(exist_ok=True)
            shortcut_path = desktop / "Blush.desktop"
            with open(shortcut_path, 'w') as f:
                f.write("[Desktop Entry]\n")
                f.write("Name=Blush\n")
                f.write(f"Exec={binary_path}\n")
                f.write("Type=Application\n")
                f.write("Terminal=true\n")
            os.chmod(shortcut_path, 0o755)
        except:
            pass
    
    def _remove_desktop_shortcut_unix(self):
        try:
            home = Path.home()
            desktop = home / "Desktop"
            shortcut_path = desktop / "Blush.desktop"
            if shortcut_path.exists():
                shortcut_path.unlink()
        except:
            pass

class UninstallThread(QThread):
    progress = Signal(int)
    finished = Signal()
    error = Signal(str)
    
    def __init__(self, system_type):
        super().__init__()
        self.system_type = system_type
    
    def run(self):
        try:
            home = Path.home()
            blush_dir = home / ".blush"
            if not blush_dir.exists():
                self.error.emit("Blush is not installed.")
                return
            
            self.progress.emit(20)
            
            if self.system_type == "Windows":
                self._remove_start_menu_shortcut()
                self._remove_desktop_shortcut_windows()
                self._remove_from_windows_path(str(blush_dir))
            else:
                self._remove_desktop_shortcut_unix()
                self._remove_from_unix_path(str(blush_dir))
            
            self.progress.emit(60)
            
            shutil.rmtree(blush_dir, ignore_errors=True)
            
            self.progress.emit(100)
            
            self.finished.emit()
            
        except Exception as e:
            self.error.emit(str(e))
    
    def _remove_start_menu_shortcut(self):
        try:
            appdata = os.environ['APPDATA']
            programs_dir = os.path.join(appdata, 'Microsoft', 'Windows', 'Start Menu', 'Programs')
            blush_folder = os.path.join(programs_dir, 'Blush')
            shortcut_path = os.path.join(blush_folder, 'Blush.lnk')
            if os.path.exists(shortcut_path):
                os.remove(shortcut_path)
            if os.path.exists(blush_folder) and not os.listdir(blush_folder):
                os.rmdir(blush_folder)
        except:
            pass
    
    def _remove_desktop_shortcut_windows(self):
        try:
            desktop = os.path.join(os.environ['USERPROFILE'], 'Desktop')
            shortcut_path = os.path.join(desktop, 'Blush.lnk')
            if os.path.exists(shortcut_path):
                os.remove(shortcut_path)
        except:
            pass
    
    def _remove_desktop_shortcut_unix(self):
        try:
            home = Path.home()
            desktop = home / "Desktop"
            shortcut_path = desktop / "Blush.desktop"
            if shortcut_path.exists():
                shortcut_path.unlink()
        except:
            pass
    
    def _remove_from_windows_path(self, directory):
        try:
            import ctypes
            from ctypes import byref, wintypes
            import winreg
            key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, 'Environment', 0, winreg.KEY_ALL_ACCESS)
            current_path, _ = winreg.QueryValueEx(key, 'Path')
            paths = current_path.split(';')
            paths = [p for p in paths if p != directory]
            new_path = ';'.join(paths)
            winreg.SetValueEx(key, 'Path', 0, winreg.REG_EXPAND_SZ, new_path)
            winreg.CloseKey(key)
            
            HWND_BROADCAST = 0xFFFF
            WM_SETTINGCHANGE = 0x1A
            SMTO_NORMAL = 0x0
            result = wintypes.DWORD()
            ctypes.windll.user32.SendMessageTimeoutW(
                HWND_BROADCAST,
                WM_SETTINGCHANGE,
                0,
                'Environment',
                SMTO_NORMAL,
                2000,
                byref(result)
            )
        except:
            pass
    
    def _remove_from_unix_path(self, directory):
        try:
            home = Path.home()
            shell_configs = ['.bashrc', '.zshrc', '.profile']
            export_line = f'export PATH="$PATH:{directory}"'
            for config in shell_configs:
                config_path = home / config
                if config_path.exists():
                    with open(config_path, 'r') as f:
                        lines = f.readlines()
                    new_lines = [line for line in lines if export_line not in line]
                    with open(config_path, 'w') as f:
                        f.writelines(new_lines)
        except:
            pass

class InstallerWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        locale.setlocale(locale.LC_ALL, '')
        default_locale = locale.getlocale()
        sys_lang = default_locale[0][:2] if default_locale[0] else 'en'
        self.current_lang = sys_lang if sys_lang in TRANSLATIONS else 'en'
        self.system_type = platform.system()
        self.download_thread = None
        self.install_thread = None
        self.modify_thread = None
        self.uninstall_thread = None
        self.mode = None
        self.blush_dir = Path.home() / ".blush"
        self.installed = self.blush_dir.exists()
        self.lang_to_code = {}
        self.lang_to_country = {"en": "us", "es": "es", "pl": "pl"}
        
        self.setWindowTitle(self.tr("title"))
        self.setFixedSize(700, 550)
        
        self._setup_ui()
        self._apply_styles()
        self._populate_language_combo()
        self.update_ui()
        self.fetch_license()
        self.fetch_translations()
    
    def tr(self, key, **kwargs):
        text = TRANSLATIONS[self.current_lang].get(key, TRANSLATIONS['en'].get(key, key))
        if kwargs:
            return text.format(**kwargs)
        return text
    
    def get_lang_name(self, lang):
        names = {"en": "English", "es": "Español", "pl": "Polski"}
        return names.get(lang, lang.upper())
    
    def _change_language(self, item_text):
        for name, code in self.lang_to_code.items():
            if name in item_text:
                self.current_lang = code
                break
        self.update_ui()
    
    def _populate_language_combo(self):
        self.lang_combo.clear()
        self.lang_to_code.clear()
        for lang in sorted(TRANSLATIONS.keys()):
            name = self.get_lang_name(lang)
            country = self.lang_to_country.get(lang, lang.lower())
            pixmap = self._fetch_flag_pixmap(country)
            icon = QIcon(pixmap) if pixmap else QIcon()
            self.lang_combo.addItem(icon, name)
            self.lang_to_code[name] = lang
        current_display = self.get_lang_name(self.current_lang)
        index = self.lang_combo.findText(current_display)
        if index != -1:
            self.lang_combo.setCurrentIndex(index)
    
    def update_ui(self):
        self.setWindowTitle(self.tr("title"))
        self.title_label.setText(self.tr("title"))
        self.welcome_label.setText(self.tr("welcome"))
        self.desc_label.setText(self.tr("description"))
        self.action_label.setText(self.tr("select_action"))
        self.radio_install.setText(self.tr("install_reinstall"))
        self.radio_modify.setText(self.tr("modify"))
        self.radio_uninstall.setText(self.tr("uninstall"))
        self.license_label.setText(self.tr("license"))
        self.accept_checkbox.setText(self.tr("accept"))
        self.options_label.setText(self.tr("options"))
        self.desktop_checkbox.setText(self.tr("desktop_shortcut"))
        if self.startmenu_checkbox:
            self.startmenu_checkbox.setText(self.tr("start_menu_shortcut"))
        self.reinstall_checkbox.setText(self.tr("reinstall"))
        self.complete_title.setText(self.tr("complete"))
        self.complete_msg.setText(self.tr("complete_msg"))
        self.back_btn.setText(self.tr("back"))
        self.next_btn.setText(self.tr("next"))
        self.cancel_btn.setText(self.tr("cancel"))
        self._update_buttons()
    
    def _setup_ui(self):
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        main_layout = QVBoxLayout(central_widget)
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)
        
        header = QWidget()
        header.setFixedHeight(80)
        header.setStyleSheet("""
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #1a1a2e, stop:1 #2a2a3e);
        """)
        header_layout = QHBoxLayout(header)
        header_layout.setContentsMargins(30, 20, 30, 20)
        
        self.title_label = QLabel(self.tr("title"))
        self.title_label.setStyleSheet("color: white; font-size: 24px; font-weight: 700;")
        header_layout.addWidget(self.title_label)
        
        header_layout.addStretch()
        
        self.lang_combo = QComboBox()
        self.lang_combo.currentTextChanged.connect(self._change_language)
        self.lang_combo.setFixedWidth(150)
        self.lang_combo.setFixedHeight(35)
        self.lang_combo.setStyleSheet("""
            QComboBox {
                background: #2a2a3a;
                color: white;
                border: 1px solid #3a3a4a;
                border-radius: 18px;
                padding: 8px 12px;
                font-size: 13px;
                min-width: 140px;
                font-weight: 500;
            }
            QComboBox:hover {
                background: #3a3a4a;
                border-color: #4a4a5a;
            }
            QComboBox:focus {
                border-color: #5078ff;
                background: #3a3a4a;
            }
            QComboBox::drop-down {
                border: none;
                width: 25px;
                subcontrol-origin: padding;
                subcontrol-position: top right;
                left: 5px;
            }
            QComboBox::down-arrow {
                image: none;
                border-left: 5px solid transparent;
                border-right: 5px solid transparent;
                border-top: 6px solid white;
                width: 0;
                height: 0;
            }
            QComboBox QAbstractItemView {
                background: #2a2a3a;
                color: white;
                border: 1px solid #3a3a4a;
                border-radius: 8px;
                selection-background-color: #5078ff;
                selection-color: white;
                font-size: 13px;
            }
            QComboBox QAbstractItemView::item {
                padding: 8px 12px;
                border-radius: 4px;
            }
            QComboBox QAbstractItemView::item:selected {
                background: #5078ff;
            }
        """)
        header_layout.addWidget(self.lang_combo)
        
        self.opacity_effect = QGraphicsOpacityEffect(self.title_label)
        self.title_label.setGraphicsEffect(self.opacity_effect)
        self.opacity_effect.setOpacity(0.0)
        self.title_animation = QPropertyAnimation(self.opacity_effect, b"opacity")
        self.title_animation.setDuration(800)
        self.title_animation.setStartValue(0.0)
        self.title_animation.setEndValue(1.0)
        self.title_animation.setEasingCurve(QEasingCurve.OutQuad)
        self.title_animation.start()
        
        main_layout.addWidget(header)
        
        self.stack = QStackedWidget()
        main_layout.addWidget(self.stack)
        
        self._create_welcome_page()
        self._create_license_page()
        self._create_options_page()
        self._create_install_page()
        self._create_complete_page()
        
        footer = QWidget()
        footer.setFixedHeight(80)
        footer.setStyleSheet("background: #1a1a2a; border-top: 1px solid #2a2a3a;")
        footer_layout = QHBoxLayout(footer)
        footer_layout.setContentsMargins(30, 0, 30, 0)
        
        self.back_btn = ModernButton(self.tr("back"))
        self.back_btn.clicked.connect(self._go_back)
        self.back_btn.setEnabled(False)
        
        self.next_btn = ModernButton(self.tr("next"), primary=True)
        self.next_btn.clicked.connect(self._go_next)
        
        self.cancel_btn = ModernButton(self.tr("cancel"))
        self.cancel_btn.clicked.connect(self.close)
        
        footer_layout.addWidget(self.back_btn)
        footer_layout.addStretch()
        footer_layout.addWidget(self.cancel_btn)
        footer_layout.addWidget(self.next_btn)
        
        main_layout.addWidget(footer)
    
    def _create_welcome_page(self):
        page = QWidget()
        layout = QVBoxLayout(page)
        layout.setContentsMargins(50, 40, 50, 40)
        layout.setSpacing(20)
        
        self.welcome_label = QLabel(self.tr("welcome"))
        self.welcome_label.setStyleSheet("color: white; font-size: 28px; font-weight: 700;")
        layout.addWidget(self.welcome_label)
        
        self.desc_label = QLabel(self.tr("description"))
        self.desc_label.setStyleSheet("color: #aaa; font-size: 15px; line-height: 1.6;")
        self.desc_label.setWordWrap(True)
        layout.addWidget(self.desc_label)
        
        self.action_label = QLabel(self.tr("select_action"))
        self.action_label.setStyleSheet("color: white; font-size: 18px; font-weight: 600;")
        layout.addWidget(self.action_label)
        
        button_style = """
            QRadioButton {
                color: white;
                font-size: 14px;
                font-weight: 500;
                padding: 12px 25px;
                border-radius: 22px;
                background: #2a2a3a;
                border: 1px solid #3a3a4a;
                text-align: center;
            }
            QRadioButton:hover {
                background: #3a3a4a;
                border-color: #4a4a5a;
            }
            QRadioButton:pressed {
                background: #1a1a2a;
            }
            QRadioButton:checked {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #5078ff, stop:1 #6090ff);
                border-color: #5078ff;
            }
            QRadioButton:disabled {
                background: #1a1a2a;
                color: #666;
                border-color: #2a2a3a;
            }
            QRadioButton::indicator {
                width: 0px;
                height: 0px;
            }
        """
        
        self.action_group = QButtonGroup(self)
        
        self.radio_install = QRadioButton(self.tr("install_reinstall"))
        self.radio_install.setChecked(True)
        self.radio_install.setStyleSheet(button_style)
        self.radio_install.setFixedHeight(45)
        self.radio_install.setMinimumWidth(500)
        self.action_group.addButton(self.radio_install)
        layout.addWidget(self.radio_install)
        
        self.radio_modify = QRadioButton(self.tr("modify"))
        self.radio_modify.setEnabled(self.installed)
        self.radio_modify.setStyleSheet(button_style)
        self.radio_modify.setFixedHeight(45)
        self.radio_modify.setMinimumWidth(500)
        self.action_group.addButton(self.radio_modify)
        layout.addWidget(self.radio_modify)
        
        self.radio_uninstall = QRadioButton(self.tr("uninstall"))
        self.radio_uninstall.setEnabled(self.installed)
        self.radio_uninstall.setStyleSheet(button_style)
        self.radio_uninstall.setFixedHeight(45)
        self.radio_uninstall.setMinimumWidth(500)
        self.action_group.addButton(self.radio_uninstall)
        layout.addWidget(self.radio_uninstall)
        
        layout.addStretch()
        
        self.stack.addWidget(page)
    
    def _create_license_page(self):
        page = QWidget()
        layout = QVBoxLayout(page)
        layout.setContentsMargins(50, 40, 50, 40)
        layout.setSpacing(20)
        
        self.license_label = QLabel(self.tr("license"))
        self.license_label.setStyleSheet("color: white; font-size: 20px; font-weight: 600;")
        layout.addWidget(self.license_label)
        
        self.license_text = QTextEdit()
        self.license_text.setPlainText("Loading license...")
        self.license_text.setReadOnly(True)
        self.license_text.setStyleSheet("""
            QTextEdit {
                background: #1a1a2a;
                color: #ccc;
                border: 1px solid #2a2a3a;
                border-radius: 8px;
                padding: 15px;
                font-family: 'Courier New', monospace;
                font-size: 12px;
            }
        """)
        layout.addWidget(self.license_text)
        
        self.accept_checkbox = QCheckBox(self.tr("accept"))
        self.accept_checkbox.setStyleSheet("""
            QCheckBox {
                color: white;
                font-size: 14px;
                spacing: 10px;
            }
            QCheckBox::indicator {
                width: 20px;
                height: 20px;
                border: 2px solid #3a3a4a;
                border-radius: 4px;
                background: #1a1a2a;
            }
            QCheckBox::indicator:checked {
                background: #5078ff;
                border-color: #5078ff;
            }
        """)
        self.accept_checkbox.stateChanged.connect(self._on_accept_changed)
        layout.addWidget(self.accept_checkbox)
        
        self.license_note = QLabel("")
        self.license_note.setOpenExternalLinks(True)
        self.license_note.setStyleSheet("color: #aaa; font-size: 12px;")
        layout.addWidget(self.license_note)
        
        self.stack.addWidget(page)
    
    def _create_options_page(self):
        page = QWidget()
        layout = QVBoxLayout(page)
        layout.setContentsMargins(50, 40, 50, 40)
        layout.setSpacing(20)
        
        self.options_label = QLabel(self.tr("options"))
        self.options_label.setStyleSheet("color: white; font-size: 20px; font-weight: 600;")
        layout.addWidget(self.options_label)
        
        self.desktop_checkbox = QCheckBox(self.tr("desktop_shortcut"))
        self.desktop_checkbox.setChecked(True)
        self.desktop_checkbox.setToolTip(self.tr("desktop_tooltip"))
        self.desktop_checkbox.setStyleSheet("""
            QCheckBox {
                color: white;
                font-size: 14px;
                spacing: 10px;
            }
            QCheckBox::indicator {
                width: 20px;
                height: 20px;
                border: 2px solid #3a3a4a;
                border-radius: 4px;
                background: #1a1a2a;
            }
            QCheckBox::indicator:checked {
                background: #5078ff;
                border-color: #5078ff;
            }
        """)
        layout.addWidget(self.desktop_checkbox)
        
        self.startmenu_checkbox = None
        if self.system_type == "Windows":
            self.startmenu_checkbox = QCheckBox(self.tr("start_menu_shortcut"))
            self.startmenu_checkbox.setChecked(True)
            self.startmenu_checkbox.setToolTip(self.tr("startmenu_tooltip"))
            self.startmenu_checkbox.setStyleSheet("""
                QCheckBox {
                    color: white;
                    font-size: 14px;
                    spacing: 10px;
                }
                QCheckBox::indicator {
                    width: 20px;
                    height: 20px;
                    border: 2px solid #3a3a4a;
                    border-radius: 4px;
                    background: #1a1a2a;
                }
                QCheckBox::indicator:checked {
                    background: #5078ff;
                    border-color: #5078ff;
                }
            """)
            layout.addWidget(self.startmenu_checkbox)
        
        self.reinstall_checkbox = QCheckBox(self.tr("reinstall"))
        self.reinstall_checkbox.setChecked(False)
        self.reinstall_checkbox.setToolTip(self.tr("reinstall_tooltip"))
        self.reinstall_checkbox.setStyleSheet("""
            QCheckBox {
                color: white;
                font-size: 14px;
                spacing: 10px;
            }
            QCheckBox::indicator {
                width: 20px;
                height: 20px;
                border: 2px solid #3a3a4a;
                border-radius: 4px;
                background: #1a1a2a;
            }
            QCheckBox::indicator:checked {
                background: #5078ff;
                border-color: #5078ff;
            }
        """)
        self.reinstall_checkbox.setVisible(False)
        layout.addWidget(self.reinstall_checkbox)
        
        layout.addStretch()
        
        self.stack.addWidget(page)
    
    def _create_install_page(self):
        page = QWidget()
        layout = QVBoxLayout(page)
        layout.setContentsMargins(50, 60, 50, 60)
        layout.setSpacing(30)
        
        layout.addStretch()
        
        spinner_container = QWidget()
        spinner_layout = QHBoxLayout(spinner_container)
        spinner_layout.setContentsMargins(0, 0, 0, 0)
        self.spinner = SpinnerWidget()
        spinner_layout.addStretch()
        spinner_layout.addWidget(self.spinner)
        spinner_layout.addStretch()
        layout.addWidget(spinner_container)
        
        self.install_label = QLabel(self.tr("installing"))
        self.install_label.setStyleSheet("color: white; font-size: 22px; font-weight: 600;")
        self.install_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.install_label)
        
        self.status_label = QLabel(self.tr("preparing"))
        self.status_label.setStyleSheet("color: #888; font-size: 14px;")
        self.status_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.status_label)
        
        progress_container = QWidget()
        progress_layout = QVBoxLayout(progress_container)
        progress_layout.setContentsMargins(0, 20, 0, 0)
        progress_layout.setSpacing(8)
        
        self.progress_bar = GradientProgressBar()
        self.progress_bar.setMaximum(100)
        progress_layout.addWidget(self.progress_bar)
        
        self.progress_label = QLabel("0%")
        self.progress_label.setStyleSheet("color: #aaa; font-size: 13px;")
        self.progress_label.setAlignment(Qt.AlignCenter)
        progress_layout.addWidget(self.progress_label)
        
        layout.addWidget(progress_container)
        
        layout.addStretch()
        
        self.stack.addWidget(page)
    
    def _create_complete_page(self):
        page = QWidget()
        layout = QVBoxLayout(page)
        layout.setContentsMargins(50, 40, 50, 40)
        layout.setSpacing(20)
        
        layout.addStretch()
        
        self.complete_icon = QLabel("✓")
        self.complete_icon.setStyleSheet("color: #4CAF50; font-size: 72px; font-weight: 700;")
        self.complete_icon.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.complete_icon)
        
        self.complete_title = QLabel(self.tr("complete"))
        self.complete_title.setStyleSheet("color: white; font-size: 26px; font-weight: 700;")
        self.complete_title.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.complete_title)
        
        self.complete_msg = QLabel(self.tr("complete_msg"))
        self.complete_msg.setStyleSheet("color: #aaa; font-size: 15px;")
        self.complete_msg.setAlignment(Qt.AlignCenter)
        self.complete_msg.setWordWrap(True)
        layout.addWidget(self.complete_msg)
        
        layout.addStretch()
        
        self.stack.addWidget(page)
    
    def _apply_styles(self):
        self.setStyleSheet("""
            QMainWindow {
                background: #16161e;
            }
            QWidget {
                background: #16161e;
                color: white;
            }
        """)
    
    def fetch_license(self):
        self.manager = QNetworkAccessManager(self)
        request = QNetworkRequest(QUrl("https://raw.githubusercontent.com/Blush-CLI/Blush/refs/heads/master/LICENSE"))
        reply = self.manager.get(request)
        reply.finished.connect(self._on_license_fetched)
    
    def _on_license_fetched(self):
        reply = self.sender()
        if reply.error() == QNetworkReply.NoError:
            text = bytes(reply.readAll()).decode('utf-8')
            self.license_text.setPlainText(text)
            self.license_note.setText("")
        else:
            self.license_text.setPlainText(STATIC_LICENSE_TEXT)
            note = "Note: This is a static license copy. It may not be up to date. Check the latest at: <a href=\"https://github.com/Blush-CLI/Blush?tab=License-1-ov-file\">GitHub Repo</a>"
            self.license_note.setText(note)
    
    def fetch_translations(self):
        self.trans_manager = QNetworkAccessManager(self)
        url = "https://cdn.getblush.xyz/translations_getblush.json"
        req = QNetworkRequest(QUrl(url))
        reply = self.trans_manager.get(req)
        reply.finished.connect(self._on_trans_fetched)
    
    def _on_trans_fetched(self):
        reply = self.sender()
        if reply.error() == QNetworkReply.NoError:
            try:
                data = json.loads(bytes(reply.readAll()).decode())
                for lang, trans in data.items():
                    if lang not in TRANSLATIONS:
                        TRANSLATIONS[lang] = {}
                    TRANSLATIONS[lang].update(trans)
                    if lang not in self.lang_to_country:
                        self.lang_to_country[lang] = lang.lower()
            except:
                pass
        self._populate_language_combo()
        current_display = self.get_lang_name(self.current_lang)
        index = self.lang_combo.findText(current_display)
        if index != -1:
            self.lang_combo.setCurrentIndex(index)
        self.update_ui()
    
    def _fetch_flag_pixmap(self, country):
        try:
            url = QUrl(f"https://flagcdn.com/w40/{country}.png")
            request = QNetworkRequest(url)
            reply = self.manager.get(request)
            loop = QEventLoop()
            reply.finished.connect(loop.quit)
            loop.exec()
            if reply.error() == QNetworkReply.NoError:
                data = bytes(reply.readAll())
                pixmap = QPixmap()
                if pixmap.loadFromData(data):
                    return pixmap
        except:
            pass
        return None
    
    def _on_accept_changed(self, state):
        is_checked = state == 2
        self.next_btn.setEnabled(is_checked)
    
    def _go_back(self):
        current = self.stack.currentIndex()
        if current > 0:
            self.stack.setCurrentIndex(current - 1)
            self._update_buttons()
    
    def _go_next(self):
        current = self.stack.currentIndex()
        
        if current == 0:
            if self.radio_install.isChecked():
                self.mode = "install"
                self.stack.setCurrentIndex(1)
            elif self.radio_modify.isChecked():
                self.mode = "modify"
                self._prepare_options_for_modify()
                self.stack.setCurrentIndex(2)
            elif self.radio_uninstall.isChecked():
                self.mode = "uninstall"
                self._confirm_uninstall()
            self._update_buttons()
        elif current == 1:
            self.stack.setCurrentIndex(2)
            self._update_buttons()
        elif current == 2:
            if self.mode == "install":
                self.stack.setCurrentIndex(3)
                self.next_btn.setEnabled(False)
                self.back_btn.setEnabled(False)
                self.cancel_btn.setEnabled(False)
                self._start_installation()
            elif self.mode == "modify":
                self.stack.setCurrentIndex(3)
                self.next_btn.setEnabled(False)
                self.back_btn.setEnabled(False)
                self.cancel_btn.setEnabled(False)
                self._apply_modify()
        elif current == 4:
            self.close()
    
    def _update_buttons(self):
        current = self.stack.currentIndex()
        
        self.back_btn.setEnabled(current > 0 and current < 3)
        
        if current == 0:
            self.next_btn.setText(self.tr("next"))
            self.next_btn.setEnabled(True)
        elif current == 1:
            self.next_btn.setText(self.tr("next"))
            self.next_btn.setEnabled(self.accept_checkbox.isChecked())
        elif current == 2:
            if self.mode == "install":
                self.next_btn.setText(self.tr("install"))
            else:
                self.next_btn.setText("Apply")
            self.next_btn.setEnabled(True)
        elif current == 4:
            self.next_btn.setText(self.tr("finish"))
            self.next_btn.setEnabled(True)
    
    def _prepare_options_for_modify(self):
        self.reinstall_checkbox.setVisible(True)
        self.desktop_checkbox.setChecked(self._shortcut_exists("desktop"))
        if self.startmenu_checkbox:
            self.startmenu_checkbox.setChecked(self._shortcut_exists("startmenu"))
    
    def _shortcut_exists(self, shortcut_type):
        if shortcut_type == "desktop":
            if self.system_type == "Windows":
                path = os.path.join(os.environ['USERPROFILE'], 'Desktop', 'Blush.lnk')
            else:
                path = Path.home() / "Desktop" / "Blush.desktop"
            return os.path.exists(path)
        elif shortcut_type == "startmenu":
            if self.system_type == "Windows":
                appdata = os.environ['APPDATA']
                programs_dir = os.path.join(appdata, 'Microsoft', 'Windows', 'Start Menu', 'Programs')
                blush_folder = os.path.join(programs_dir, 'Blush')
                path = os.path.join(blush_folder, 'Blush.lnk')
                return os.path.exists(path)
            return False
    
    def _confirm_uninstall(self):
        msg = QMessageBox(self)
        msg.setIcon(QMessageBox.Question)
        msg.setWindowTitle(self.tr("confirm_uninstall_title"))
        msg.setText(self.tr("confirm_uninstall_msg"))
        msg.setStandardButtons(QMessageBox.Yes | QMessageBox.No)
        msg.setDefaultButton(QMessageBox.No)
        reply = msg.exec()
        if reply == QMessageBox.Yes:
            self.stack.setCurrentIndex(3)
            self.install_label.setText(self.tr("uninstalling"))
            self.next_btn.setEnabled(False)
            self.back_btn.setEnabled(False)
            self.cancel_btn.setEnabled(False)
            self._start_uninstall()
        else:
            self.mode = None
    
    def _start_uninstall(self):
        self.status_label.setText(self.tr("connecting"))
        self.progress_bar.setValue(0)
        
        self.uninstall_thread = UninstallThread(self.system_type)
        self.uninstall_thread.progress.connect(self._on_install_progress)
        self.uninstall_thread.finished.connect(self._on_uninstall_finished)
        self.uninstall_thread.error.connect(self._handle_error)
        self.uninstall_thread.start()
    
    def _on_uninstall_finished(self):
        self.progress_bar.setValue(100)
        self.progress_label.setText("100%")
        self.status_label.setText(self.tr("uninstallation_complete_status"))
        
        QTimer.singleShot(500, lambda: self._show_complete_page("uninstall"))
    
    def _apply_modify(self):
        if self.reinstall_checkbox.isChecked():
            self._start_installation()
            return
        
        create_desktop = self.desktop_checkbox.isChecked()
        create_startmenu = self.startmenu_checkbox.isChecked() if self.startmenu_checkbox else False
        
        self.install_label.setText(self.tr("applying"))
        
        self.status_label.setText(self.tr("preparing"))
        self.progress_bar.setValue(0)
        
        self.modify_thread = ModifyThread(self.system_type, create_desktop, create_startmenu)
        self.modify_thread.progress.connect(self._on_install_progress)
        self.modify_thread.finished.connect(self._on_modify_finished)
        self.modify_thread.error.connect(self._handle_error)
        self.modify_thread.start()
    
    def _on_modify_finished(self):
        self.progress_bar.setValue(100)
        self.progress_label.setText("100%")
        self.status_label.setText(self.tr("modification_complete_status"))
        
        QTimer.singleShot(500, lambda: self._show_complete_page("modify"))
    
    def _start_installation(self):
        try:
            if self.system_type == "Windows":
                filename = "blush-win-x64.zip"
            elif self.system_type == "Linux":
                filename = "blush-linux-x64.zip"
            elif self.system_type == "Darwin":
                filename = "blush-darwin-x64.zip"
            else:
                self._handle_error("Unsupported operating system")
                return
            
            url = f"https://cdn.getblush.xyz/latest/{filename}"
            
            temp_dir = Path(tempfile.gettempdir())
            destination = temp_dir / filename
            
            self.status_label.setText(self.tr("connecting"))
            self.progress_bar.setValue(0)
            
            self.download_thread = DownloadThread(url, str(destination))
            self.download_thread.progress.connect(self._on_download_progress)
            self.download_thread.finished.connect(self._on_download_finished)
            self.download_thread.error.connect(self._handle_error)
            self.download_thread.start()
            
        except Exception as e:
            self._handle_error(str(e))
    
    def _on_download_progress(self, percentage, received, total):
        self.progress_bar.setValue(percentage)
        self.progress_label.setText(f"{percentage}%")
        
        received_mb = received / (1024 * 1024)
        total_mb = total / (1024 * 1024)
        self.status_label.setText(f"{self.tr('downloading')}: {received_mb:.1f} MB / {total_mb:.1f} MB")
    
    def _on_download_finished(self, file_path):
        self.status_label.setText(self.tr("download_complete"))
        self.progress_bar.setValue(0)
        self.progress_label.setText("0%")
        
        create_desktop = self.desktop_checkbox.isChecked()
        create_startmenu = self.startmenu_checkbox.isChecked() if self.startmenu_checkbox else False
        
        self.install_thread = InstallThread(file_path, self.system_type, create_desktop, create_startmenu)
        self.install_thread.progress.connect(self._on_install_progress)
        self.install_thread.finished.connect(self._on_install_finished)
        self.install_thread.error.connect(self._handle_error)
        self.install_thread.start()
    
    def _on_install_progress(self, percentage):
        self.progress_bar.setValue(percentage)
        self.progress_label.setText(f"{percentage}%")
        
        if percentage < 40:
            self.status_label.setText(self.tr("extracting"))
        elif percentage < 80:
            self.status_label.setText(self.tr("installing_components"))
        else:
            self.status_label.setText(self.tr("finalizing"))
    
    def _on_install_finished(self):
        self.progress_bar.setValue(100)
        self.progress_label.setText("100%")
        self.status_label.setText(self.tr("installation_complete"))
        
        QTimer.singleShot(500, lambda: self._show_complete_page("install"))
    
    def _show_complete_page(self, complete_mode):
        if complete_mode == "install":
            self.complete_title.setText(self.tr("complete"))
            self.complete_msg.setText(self.tr("complete_msg"))
        elif complete_mode == "modify":
            self.complete_title.setText(self.tr("modification_complete"))
            self.complete_msg.setText(self.tr("modification_msg"))
        elif complete_mode == "uninstall":
            self.complete_title.setText(self.tr("uninstallation_complete"))
            self.complete_msg.setText(self.tr("uninstallation_msg"))
        
        self.stack.setCurrentIndex(4)
        self.next_btn.setEnabled(True)
        self.next_btn.setText(self.tr("finish"))
        self.cancel_btn.setEnabled(False)
        self._update_buttons()
    
    def _handle_error(self, error_message):
        if self.system_type == "Windows":
            msg_box = QMessageBox(self)
            msg_box.setIcon(QMessageBox.Critical)
            msg_box.setWindowTitle(self.tr("error"))
            msg_box.setText(self.tr("error_prefix"))
            msg_box.setInformativeText(error_message)
            msg_box.setStandardButtons(QMessageBox.Ok)
            msg_box.setDefaultButton(QMessageBox.Ok)
            msg_box.exec()
        else:
            try:
                log_file = Path.home() / "blush_error.log"
                with open(log_file, 'w') as f:
                    f.write(f"Blush Error\n")
                    f.write(f"========================\n\n")
                    f.write(f"Error: {error_message}\n")
                
                try:
                    if self.system_type == "Linux":
                        subprocess.run(['xdg-open', str(log_file)])
                    elif self.system_type == "Darwin":
                        subprocess.run(['open', str(log_file)])
                except:
                    print(f"\nError: {error_message}")
                    print(f"Log saved to: {log_file}")
            except:
                print(f"\nError: {error_message}")
            
            QTimer.singleShot(3000, lambda: QApplication.quit())

def main():
    app = QApplication(sys.argv)
    
    app.setStyle('Fusion')
    
    palette = QPalette()
    palette.setColor(QPalette.Window, QColor(22, 22, 30))
    palette.setColor(QPalette.WindowText, Qt.white)
    palette.setColor(QPalette.Base, QColor(26, 26, 42))
    palette.setColor(QPalette.AlternateBase, QColor(42, 42, 58))
    palette.setColor(QPalette.Text, Qt.white)
    palette.setColor(QPalette.Button, QColor(42, 42, 58))
    palette.setColor(QPalette.ButtonText, Qt.white)
    app.setPalette(palette)
    
    font = QFont("Segoe UI", 10)
    app.setFont(font)
    
    window = InstallerWindow()
    window.show()
    
    sys.exit(app.exec())

if __name__ == "__main__":
    main()