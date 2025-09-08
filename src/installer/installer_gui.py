import sys
import json
import os
import platform
import subprocess
import tempfile
import zipfile
import shutil
import winreg
import requests
import locale
from pathlib import Path
from PySide6.QtWidgets import *
from PySide6.QtCore import *
from PySide6.QtGui import *
from PySide6.QtNetwork import *
import math

class TranslationFetcher(QThread):
    translations_loaded = Signal(dict, list)
    error_occurred = Signal(str)
    
    def __init__(self):
        super().__init__()
        
    def run(self):
        try:
            script_dir = Path(__file__).parent
            local_translations_path = script_dir / "translations.json"
            
            if local_translations_path.exists():
                print(f"[BLUSH LOG] Loading local translations from {local_translations_path}")
                with open(local_translations_path, 'r', encoding='utf-8') as f:
                    translations_data = json.load(f)
                available_languages = list(translations_data.keys())
                print(f"[BLUSH LOG] Successfully loaded local translations with {len(available_languages)} languages")
                self.translations_loaded.emit(translations_data, available_languages)
                return
            
            print(f"[BLUSH LOG] Local translations not found, fetching from CDN")
            session = requests.Session()
            headers = {
                'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
                'Accept': 'application/json,text/plain,*/*',
                'Accept-Language': 'en-US,en;q=0.9',
                'Accept-Encoding': 'gzip, deflate, br',
                'DNT': '1',
                'Connection': 'keep-alive',
                'Cache-Control': 'no-cache'
            }
            
            response = session.get("https://cdn.getblush.xyz/installer/translations.json", 
                                 headers=headers, timeout=15)
            response.raise_for_status()
            
            print(f"[BLUSH LOG] Response status: {response.status_code}")
            
            content = response.text
            translations_data = json.loads(content)
            available_languages = list(translations_data.keys())
            
            print(f"[BLUSH LOG] Successfully parsed JSON with {len(available_languages)} languages")
            
            self.translations_loaded.emit(translations_data, available_languages)
            
        except requests.exceptions.RequestException as e:
            print(f"[BLUSH LOG] Network error fetching translations: {str(e)}")
            self._emit_fallback_translations()
        except json.JSONDecodeError as e:
            print(f"[BLUSH LOG] JSON decode error: {str(e)}")
            self._emit_fallback_translations()
        except Exception as e:
            print(f"[BLUSH LOG] Unexpected error fetching translations: {str(e)}")
            self._emit_fallback_translations()
    
    def _emit_fallback_translations(self):
        # fallback when cdn is down or whatever
        fallback_translations = {
            "en": {
                "title": "Blush Installer",
                "welcome": "Blush - A powerful CLI for developers and power users",
                "description": "Advanced command-line interface with modular commands, extensions, and workflow tools designed for terminal control.",
                "support": "Need help? Contact: 4raynixx on Discord",
                "language": "Language",
                "downloading": "Downloading Blush...",
                "installing": "Installing Blush...",
                "success": "Installation Successful!",
                "error": "Installation Error",
                "license_agreement": "License Agreement",
                "accept_license": "I accept the license agreement",
                "install": "Install",
                "cancel": "Cancel",
                "next": "Next",
                "close": "Close",
                "auto_launch": "Launch Blush when installer closes",
                "installation_complete": "Blush has been installed successfully!\nYou can now use 'blush' command in terminal.",
                "license_required": "You must accept the license agreement to continue",
                "adding_to_path": "Adding to PATH...",
                "debug_mode": "Debug Mode",
                "flag_img": "https://flagcdn.com/64x48/us.png"
            }
        }
        self.translations_loaded.emit(fallback_translations, ["en"])

class BlurWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setAttribute(Qt.WA_TranslucentBackground)
        
    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        
        # rounded rectangle with blur effect
        rect = self.rect()
        path = QPainterPath()
        path.addRoundedRect(rect, 20, 20)
        
        painter.fillPath(path, QColor(8, 8, 8, 200))
        
        gradient = QLinearGradient(0, 0, rect.width(), rect.height())
        gradient.setColorAt(0, QColor(64, 64, 64, 80))
        gradient.setColorAt(1, QColor(32, 32, 32, 80))
        
        pen = QPen(QBrush(gradient), 1)
        painter.setPen(pen)
        painter.drawPath(path)

class FadeWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.opacity_effect = QGraphicsOpacityEffect()
        self.setGraphicsEffect(self.opacity_effect)
        self.opacity_effect.setOpacity(1.0)
        
    def fade_in(self, duration=300):
        self.animation = QPropertyAnimation(self.opacity_effect, b"opacity")
        self.animation.setDuration(duration)
        self.animation.setStartValue(0.0)
        self.animation.setEndValue(1.0)
        self.animation.setEasingCurve(QEasingCurve.InOutQuad)
        self.animation.start()
        
    def fade_out(self, duration=300):
        self.animation = QPropertyAnimation(self.opacity_effect, b"opacity")
        self.animation.setDuration(duration)
        self.animation.setStartValue(1.0)
        self.animation.setEndValue(0.0)
        self.animation.setEasingCurve(QEasingCurve.InOutQuad)
        self.animation.start()

class AnimatedStatusWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.setFixedSize(40, 40)
        self.rotation = 0
        self.scale = 1.0
        self.pulse = 0
        self.status = "idle"  # idle, downloading, installing, success, error
        
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_animation)
        
        self.scale_animation = QPropertyAnimation(self, b"scale_factor")
        self.scale_animation.setDuration(1000)
        self.scale_animation.setLoopCount(-1)
        
    def set_status(self, status):
        self.status = status
        if status in ["downloading", "installing"]:
            self.timer.start(30)  # smoother animation at 33 fps
            self.scale_animation.setStartValue(0.9)
            self.scale_animation.setEndValue(1.1)
            self.scale_animation.setEasingCurve(QEasingCurve.InOutSine)
            self.scale_animation.start()
        else:
            self.timer.stop()
            self.scale_animation.stop()
            self.scale = 1.0
        self.update()
        
    def update_animation(self):
        self.rotation = (self.rotation + 8) % 360  # faster rotation
        self.pulse = (self.pulse + 0.1) % (2 * 3.14159)  # pulse effect
        self.update()
        
    def get_scale_factor(self):
        return self.scale
        
    def set_scale_factor(self, value):
        self.scale = value
        self.update()
        
    scale_factor = Property(float, get_scale_factor, set_scale_factor)
        
    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        
        center = self.rect().center()
        painter.translate(center)
        painter.scale(self.scale, self.scale)
        
        if self.status == "downloading":
            painter.rotate(self.rotation)
            gradient = QRadialGradient(0, 0, 20)
            gradient.setColorAt(0, QColor(102, 126, 234, 200))
            gradient.setColorAt(1, QColor(102, 126, 234, 100))
            painter.setBrush(QBrush(gradient))
            painter.setPen(QPen(QColor(102, 126, 234), 3))
            painter.drawEllipse(-15, -15, 30, 30)
            
            # animated download arrow
            painter.setPen(QPen(QColor(255, 255, 255), 3))
            painter.drawLine(0, -10, 0, 10)
            painter.drawLine(-5, 5, 0, 10)
            painter.drawLine(5, 5, 0, 10)
            
        elif self.status == "installing":
            painter.rotate(self.rotation)
            colors = [QColor(118, 75, 162), QColor(102, 126, 234), QColor(76, 175, 80)]
            for i in range(12):
                color = colors[i % 3]
                color.setAlpha(int(150 + 50 * abs(math.sin(self.pulse + i * 0.5))))
                painter.setPen(QPen(color, 2))
                painter.drawLine(10, 0, 16, 0)
                painter.rotate(30)
            
            # center circle with pulse
            pulse_radius = 6 + 2 * abs(math.sin(self.pulse))
            painter.setBrush(QBrush(QColor(118, 75, 162, 150)))
            painter.setPen(Qt.NoPen)
            painter.drawEllipse(-pulse_radius, -pulse_radius, pulse_radius*2, pulse_radius*2)
            
        elif self.status == "success":
            glow_color = QColor(34, 197, 94, int(100 + 50 * abs(math.sin(self.pulse * 2))))
            painter.setPen(QPen(glow_color, 6))
            painter.drawLine(-8, 0, -2, 6)
            painter.drawLine(-2, 6, 10, -6)
            
            painter.setPen(QPen(QColor(34, 197, 94), 4))
            painter.drawLine(-8, 0, -2, 6)
            painter.drawLine(-2, 6, 10, -6)
            
        elif self.status == "error":
            shake_offset = 2 * math.sin(self.pulse * 4)
            painter.translate(shake_offset, 0)
            
            glow_color = QColor(239, 68, 68, int(100 + 50 * abs(math.sin(self.pulse * 2))))
            painter.setPen(QPen(glow_color, 6))
            painter.drawLine(-8, -8, 8, 8)
            painter.drawLine(-8, 8, 8, -8)
            
            painter.setPen(QPen(QColor(239, 68, 68), 4))
            painter.drawLine(-8, -8, 8, 8)
            painter.drawLine(-8, 8, 8, -8)

class AnimatedProgressWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.progress = 0
        self.animation_offset = 0
        self.rotation_angle = 0
        self.pulse_scale = 1.0
        self.pulse_direction = 1
        self.setFixedHeight(120)  # bigger height for better animation space
        
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_animation)
        self.timer.start(30)  # smoother 33 fps animation
        
    def set_progress(self, value):
        self.progress = value
        self.update()
        
    def update_animation(self):
        self.animation_offset = (self.animation_offset + 3) % 60  # faster animation
        self.rotation_angle = (self.rotation_angle + 4) % 360  # smooth rotation
        
        self.pulse_scale += 0.02 * self.pulse_direction
        if self.pulse_scale >= 1.2:
            self.pulse_direction = -1
        elif self.pulse_scale <= 0.8:
            self.pulse_direction = 1
            
        self.update()
        
    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        
        center = self.rect().center()
        spinner_radius = 25
        
        # animated spinner
        painter.save()
        painter.translate(center)
        painter.rotate(self.rotation_angle)
        
        # multiple rotating circles for better effect
        for i in range(8):
            angle = i * 45
            painter.save()
            painter.rotate(angle)
            
            # gradient effect based on position
            alpha = int(255 * (1 - i / 8.0))
            color = QColor(59, 130, 246, alpha)
            
            painter.setBrush(QBrush(color))
            painter.setPen(Qt.NoPen)
            
            # pulsing circles
            radius = 4 * self.pulse_scale
            painter.drawEllipse(QPointF(spinner_radius, 0), radius, radius)
            painter.restore()
        
        painter.restore()
        
        # progress bar at bottom
        rect = QRect(20, self.height() - 40, self.width() - 40, 20)
        
        painter.setBrush(QBrush(QColor(8, 8, 8, 180)))
        painter.setPen(QPen(QColor(64, 64, 64, 100), 1))
        painter.drawRoundedRect(rect, 10, 10)
        
        # progress fill with enhanced animation
        if self.progress > 0:
            progress_width = int((rect.width() - 4) * self.progress / 100)
            progress_rect = QRect(rect.x() + 2, rect.y() + 2, progress_width, rect.height() - 4)
            
            gradient = QLinearGradient(0, 0, progress_rect.width(), 0)
            gradient.setColorAt(0, QColor(34, 197, 94, 220))   # green
            gradient.setColorAt(0.3, QColor(59, 130, 246, 220)) # blue
            gradient.setColorAt(0.7, QColor(168, 85, 247, 220)) # purple
            gradient.setColorAt(1, QColor(236, 72, 153, 220))   # pink
            
            painter.setBrush(QBrush(gradient))
            painter.setPen(Qt.NoPen)
            painter.drawRoundedRect(progress_rect, 8, 8)
            
            if self.progress < 100:
                wave_offset = (self.animation_offset * 3) % (progress_rect.width() + 60)
                shine_x = progress_rect.x() + wave_offset - 30
                shine_rect = QRect(shine_x, progress_rect.y(), 60, progress_rect.height())
                
                shine_gradient = QLinearGradient(shine_rect.x(), 0, shine_rect.x() + shine_rect.width(), 0)
                shine_gradient.setColorAt(0, QColor(255, 255, 255, 0))
                shine_gradient.setColorAt(0.3, QColor(255, 255, 255, 100))
                shine_gradient.setColorAt(0.7, QColor(255, 255, 255, 100))
                shine_gradient.setColorAt(1, QColor(255, 255, 255, 0))
                
                painter.setBrush(QBrush(shine_gradient))
                painter.drawRoundedRect(shine_rect.intersected(progress_rect), 8, 8)
        
        # progress text with glow effect
        painter.setPen(QPen(QColor(255, 255, 255, 200)))
        painter.setFont(QFont("Segoe UI", 11, QFont.Bold))
        text_rect = QRect(rect.x(), rect.y() - 25, rect.width(), 20)
        painter.drawText(text_rect, Qt.AlignCenter, f"{self.progress}%")

class DownloadThread(QThread):
    progress = Signal(int)
    finished = Signal(bool, str)
    status = Signal(str)
    
    def __init__(self, url, destination):
        super().__init__()
        self.url = url
        self.destination = destination
        
    def run(self):
        try:
            self.status.emit("Connecting to server...")
            
            session = requests.Session()
            headers = {
                'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
                'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7',
                'Accept-Language': 'en-US,en;q=0.9',
                'Accept-Encoding': 'gzip, deflate, br',
                'DNT': '1',
                'Connection': 'keep-alive',
                'Upgrade-Insecure-Requests': '1',
                'Sec-Fetch-Dest': 'document',
                'Sec-Fetch-Mode': 'navigate',
                'Sec-Fetch-Site': 'none',
                'Sec-Fetch-User': '?1',
                'Cache-Control': 'max-age=0'
            }
            
            response = session.get(self.url, stream=True, headers=headers, timeout=30, allow_redirects=True)
            
            if response.status_code == 403:
                self.finished.emit(False, f"Access forbidden (403). Server may be blocking the request.")
                return
            elif response.status_code == 404:
                self.finished.emit(False, f"File not found (404). The requested file may not exist.")
                return
            
            response.raise_for_status()
            
            total_size = int(response.headers.get('content-length', 0))
            self.status.emit(f"Downloading... ({total_size // 1024} KB)")
            
            with open(self.destination, 'wb') as file:
                downloaded = 0
                for chunk in response.iter_content(chunk_size=8192):
                    if chunk:
                        file.write(chunk)
                        downloaded += len(chunk)
                        if total_size > 0:
                            progress = int((downloaded / total_size) * 100)
                            self.progress.emit(progress)
                            self.status.emit(f"Downloading... {downloaded // 1024}/{total_size // 1024} KB")
            
            file_size = os.path.getsize(self.destination)
            
            if file_size < 1024 * 50:
                self.finished.emit(False, f"Downloaded file is too small ({file_size} bytes). This might be corrupted.")
                return
                
            self.finished.emit(True, f"Download completed! ({file_size // 1024} KB)")
        except Exception as e:
            self.finished.emit(False, f"Download failed: {str(e)}")

class BlushInstaller(QMainWindow):
    def __init__(self):
        super().__init__()
        print(f"[BLUSH LOG] Initializing Blush Installer")
        
        self.language = self.detect_system_language()
        self.system_info = self.detect_system()
        self.license_accepted = False
        self.license_text = ""
        self.auto_launch = False
        self.installation_path = None
        self.debug_mode = False
        self.translations = {}
        self.available_languages = []
        
        self.init_ui()
        self.load_translations()
        self.load_license()

    def detect_system_language(self):
        """detect system language and return appropriate language code"""
        try:
            # get system locale
            system_locale = locale.getdefaultlocale()[0]
            if system_locale:
                # extract language code (first 2 characters)
                lang_code = system_locale[:2].lower()
                
                # map language codes to supported languages
                language_mapping = {
                    'en': 'en',  # english
                    'pl': 'pl',  # polish
                    'es': 'es',  # spanish
                    'fr': 'fr',  # french
                    'de': 'de',  # german
                    'it': 'it',  # italian
                    'pt': 'pt',  # portuguese
                    'ru': 'ru',  # russian
                    'zh': 'zh',  # chinese
                    'ja': 'ja'   # japanese
                }
                
                detected_lang = language_mapping.get(lang_code, 'en')
                print(f"[BLUSH LOG] Detected system language: {system_locale} -> {detected_lang}")
                return detected_lang
                
        except Exception as e:
            print(f"[BLUSH LOG] Error detecting system language: {e}")
        
        # default to english if detection fails
        print(f"[BLUSH LOG] Using default language: en")
        return 'en'
        
    def detect_system(self):
        system = platform.system().lower()
        machine = platform.machine().lower()
        
        if machine in ['x86_64', 'amd64', 'x64']:
            arch = 'x64'
        elif machine in ['aarch64', 'arm64']:
            arch = 'arm64'
        elif machine in ['i386', 'i686', 'x86']:
            arch = 'x86'
        else:
            arch = 'x64'
            
        if system == 'windows':
            system = 'win'
        elif system == 'linux':
            system = 'linux'
        elif system == 'darwin':
            system = 'mac'
            
        result = {'os': system, 'arch': arch, 'full_os': platform.system(), 'full_arch': platform.machine()}
        return result
    
    def load_translations(self):
        self.translation_fetcher = TranslationFetcher()
        self.translation_fetcher.translations_loaded.connect(self.on_translations_loaded)
        self.translation_fetcher.error_occurred.connect(self.on_translation_error)
        self.translation_fetcher.start()
    
    def load_license(self):
        try:
            print("[BLUSH LOG] Loading license from GitHub...")
            response = requests.get("https://raw.githubusercontent.com/Blush-CLI/Blush/refs/heads/master/LICENSE", timeout=10)
            response.raise_for_status()
            self.license_text = response.text
            self.license_text_edit.setText(self.license_text)
            print("[BLUSH LOG] License loaded successfully")
        except Exception as e:
            print(f"[BLUSH LOG] Error loading license: {str(e)}")
            self.license_text = f"Could not load license from GitHub. Please check your internet connection.\n\nError: {str(e)}\n\nYou can view the license at:\nhttps://raw.githubusercontent.com/Blush-CLI/Blush/refs/heads/master/LICENSE"
            self.license_text_edit.setText(self.license_text)
    
    def on_translations_loaded(self, translations, available_languages):
        print(f"[BLUSH LOG] Translations loaded: {len(translations)} languages")
        print(f"[BLUSH LOG] Available languages: {available_languages}")
        
        self.translations = translations
        self.available_languages = available_languages
        
        if self.language not in available_languages:
            if available_languages:
                self.language = 'en' if 'en' in available_languages else available_languages[0]
            print(f"[BLUSH LOG] Detected language not supported, using: {self.language}")
        else:
            print(f"[BLUSH LOG] Using detected language: {self.language}")
        
        self.update_language_combo()
        self.update_translations()
        
    def on_translation_error(self, error):
        print(f"[BLUSH LOG] Translation error: {error}")
        
    def update_language_combo(self):
        self.lang_combo.blockSignals(True)
        self.lang_combo.clear()
        
        current_index = 0
        for i, lang_code in enumerate(self.available_languages):
            lang_data = self.translations.get(lang_code, {})
            flag_url = lang_data.get('flag_img', '')
            
            lang_names = {
                'en': 'English',
                'pl': 'Polski', 
                'es': 'Español',
                'fr': 'Français',
                'de': 'Deutsch',
                'it': 'Italiano',
                'pt': 'Português',
                'ru': 'Русский',
                'zh': '中文',
                'ja': '日本語'
            }
            lang_name = lang_names.get(lang_code, lang_code.upper())
            
            if flag_url:
                try:
                    print(f"[BLUSH LOG] Loading flag for {lang_code}: {flag_url}")
                    session = requests.Session()
                    headers = {
                        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
                        'Accept': 'image/webp,image/apng,image/*,*/*;q=0.8',
                        'Accept-Language': 'en-US,en;q=0.9',
                        'DNT': '1',
                        'Connection': 'keep-alive'
                    }
                    
                    response = session.get(flag_url, headers=headers, timeout=5)
                    if response.status_code == 200:
                        pixmap = QPixmap()
                        pixmap.loadFromData(response.content)
                        
                        rounded_pixmap = QPixmap(36, 27)
                        rounded_pixmap.fill(Qt.transparent)
                        
                        painter = QPainter(rounded_pixmap)
                        painter.setRenderHint(QPainter.Antialiasing)
                        
                        # add subtle shadow
                        shadow_path = QPainterPath()
                        shadow_path.addRoundedRect(2, 2, 32, 23, 6, 6)
                        painter.fillPath(shadow_path, QColor(0, 0, 0, 50))
                        
                        # main flag
                        path = QPainterPath()
                        path.addRoundedRect(0, 0, 32, 23, 6, 6)
                        painter.setClipPath(path)
                        
                        scaled_pixmap = pixmap.scaled(32, 23, Qt.KeepAspectRatioByExpanding, Qt.SmoothTransformation)
                        painter.drawPixmap(0, 0, scaled_pixmap)
                        
                        # add border
                        painter.setClipping(False)
                        painter.setPen(QPen(QColor(102, 126, 234, 100), 1))
                        painter.drawPath(path)
                        painter.end()
                        
                        icon = QIcon(rounded_pixmap)
                        self.lang_combo.addItem(icon, f"  {lang_name}")
                        print(f"[BLUSH LOG] Successfully loaded flag for {lang_code}")
                    else:
                        print(f"[BLUSH LOG] Failed to load flag for {lang_code}: HTTP {response.status_code}")
                        self.lang_combo.addItem(f"🌐 {lang_name}")
                except Exception as e:
                    print(f"[BLUSH LOG] Could not load flag for {lang_code}: {e}")
                    self.lang_combo.addItem(f"🌐 {lang_name}")
            else:
                print(f"[BLUSH LOG] No flag URL found for {lang_code}")
                self.lang_combo.addItem(f"🌐 {lang_name}")
            
            if lang_code == self.language:
                current_index = i
        
        self.lang_combo.setCurrentIndex(current_index)
        self.lang_combo.blockSignals(False)

    def init_ui(self):
        self.setWindowTitle("Blush Installer")
        self.setFixedSize(900, 700)
        
        # massive stylesheet block - keeping it as is since it's just styling
        self.setStyleSheet("""
            QMainWindow {
                background-color: #0a0a0a;
            }
            QLabel {
                color: #fafafa;
                font-family: 'Segoe UI', Arial, sans-serif;
            }
            QPushButton {
                background-color: #18181b;
                color: #fafafa;
                border: 1px solid #27272a;
                padding: 12px 24px;
                font-size: 14px;
                font-weight: 500;
                border-radius: 8px;
                min-width: 120px;
            }
            QPushButton:hover {
                background-color: #27272a;
                border: 1px solid #3f3f46;
            }
            QPushButton:pressed {
                background-color: #09090b;
            }
            QPushButton:disabled {
                background-color: #09090b;
                color: #71717a;
                border: 1px solid #18181b;
            }
            QPushButton#primaryBtn {
                background-color: #3b82f6;
                border: 1px solid #2563eb;
                color: white;
            }
            QPushButton#primaryBtn:hover {
                background-color: #2563eb;
                border: 1px solid #1d4ed8;
            }
            QPushButton#primaryBtn:pressed {
                background-color: #1d4ed8;
            }
            QPushButton#cancelBtn {
                background-color: #dc2626;
                border: 1px solid #b91c1c;
                color: white;
            }
            QPushButton#cancelBtn:hover {
                background-color: #b91c1c;
                border: 1px solid #991b1b;
            }
            QPushButton#debugBtn {
                background-color: #f59e0b;
                border: 1px solid #d97706;
                color: white;
                padding: 8px 16px;
                font-size: 12px;
                min-width: 80px;
                border-radius: 6px;
            }
            QPushButton#debugBtn:hover {
                background-color: #d97706;
                border: 1px solid #b45309;
            }
            QTextEdit {
                background-color: #09090b;
                color: #e4e4e7;
                border: 1px solid #27272a;
                border-radius: 8px;
                padding: 12px;
                font-family: 'Consolas', 'Courier New', monospace;
                font-size: 12px;
            }
            QCheckBox {
                color: #fafafa;
                font-size: 14px;
                spacing: 8px;
                padding: 8px;
            }
            QCheckBox::indicator {
                width: 20px;
                height: 20px;
                border: 1px solid #3f3f46;
                border-radius: 4px;
                background-color: #18181b;
                margin-right: 8px;
            }
            QCheckBox::indicator:hover {
                border: 1px solid #52525b;
                background-color: #27272a;
            }
            QCheckBox::indicator:checked {
                background-color: #3b82f6;
                border: 1px solid #2563eb;
            }
            QComboBox {
                background-color: #18181b;
                color: #fafafa;
                border: 1px solid #27272a;
                padding: 8px 12px;
                border-radius: 8px;
                min-width: 160px;
                font-size: 14px;
                font-weight: 500;
            }
            QComboBox:hover {
                background-color: #27272a;
                border: 1px solid #3f3f46;
            }
            QComboBox:on {
                background-color: #27272a;
            }
            QComboBox::drop-down {
                border: none;
                width: 30px;
                background: transparent;
            }
            QComboBox::down-arrow {
                image: none;
                border-left: 5px solid transparent;
                border-right: 5px solid transparent;
                border-top: 6px solid #a1a1aa;
                margin-right: 8px;
            }
            QComboBox QAbstractItemView {
                background-color: #18181b;
                color: #fafafa;
                selection-background-color: #3b82f6;
                border: 1px solid #27272a;
                border-radius: 8px;
                padding: 4px;
                outline: none;
            }
            QComboBox QAbstractItemView::item {
                background: transparent;
                padding: 8px 12px;
                border-radius: 4px;
                margin: 2px;
                border: none;
                outline: none;
            }
            QComboBox QAbstractItemView::item:hover {
                background-color: #27272a;
            }
            QComboBox QAbstractItemView::item:selected {
                background-color: #3b82f6;
                outline: none;
            }
            QGroupBox {
                color: #fafafa;
                font-size: 16px;
                font-weight: 600;
                border: 1px solid #27272a;
                border-radius: 8px;
                padding-top: 16px;
                margin-top: 12px;
                background-color: #09090b;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 16px;
                padding: 0 8px 0 8px;
                background-color: #0a0a0a;
            }
        """)
        
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        self.stacked_widget = QStackedWidget()
        layout = QVBoxLayout(central_widget)
        layout.addWidget(self.stacked_widget)
        
        self.welcome_page = self.create_welcome_page()
        self.license_page = self.create_license_page()
        self.install_page = self.create_install_page()
        self.success_page = self.create_success_page()
        
        self.stacked_widget.addWidget(self.welcome_page)
        self.stacked_widget.addWidget(self.license_page)
        self.stacked_widget.addWidget(self.install_page)
        self.stacked_widget.addWidget(self.success_page)
        
        self.stacked_widget.setCurrentIndex(0)
        
    def create_welcome_page(self):
        page = FadeWidget()  # use fadewWidget for smooth transitions
        layout = QVBoxLayout(page)
        layout.setSpacing(30)
        layout.setContentsMargins(50, 50, 50, 50)
        
        lang_layout = QHBoxLayout()
        lang_layout.addStretch()
        
        lang_container = QWidget()
        lang_container.setStyleSheet("""
            QWidget {
                background: rgba(8, 8, 8, 0.6);
                border: 1px solid rgba(64, 64, 64, 0.3);
                border-radius: 8px;
                padding: 5px;
            }
        """)
        lang_container_layout = QHBoxLayout(lang_container)
        lang_container_layout.setContentsMargins(10, 8, 10, 8)
        
        self.lang_combo = QComboBox()
        self.lang_combo.currentIndexChanged.connect(self.change_language)
        self.lang_combo.setStyleSheet("""
            QComboBox {
                background: transparent;
                border: none;
                color: rgba(255, 255, 255, 0.9);
                font-size: 14px;
                font-weight: 500;
                padding: 5px 25px 5px 10px;
                min-width: 120px;
            }
            QComboBox::drop-down {
                border: none;
                width: 20px;
            }
            QComboBox::down-arrow {
                image: none;
                border-left: 5px solid transparent;
                border-right: 5px solid transparent;
                border-top: 6px solid rgba(255, 255, 255, 0.7);
                margin-right: 5px;
            }
            QComboBox::down-arrow:hover {
                border-top-color: rgba(59, 130, 246, 0.9);
            }
            QComboBox QAbstractItemView {
                background: rgba(8, 8, 8, 0.95);
                border: 1px solid rgba(64, 64, 64, 0.5);
                border-radius: 6px;
                selection-background-color: rgba(59, 130, 246, 0.3);
                color: rgba(255, 255, 255, 0.9);
                padding: 2px;
            }
            QComboBox QAbstractItemView::item {
                padding: 8px 12px;
                border: none;
                border-radius: 4px;
                margin: 1px;
            }
            QComboBox QAbstractItemView::item:hover {
                background: rgba(59, 130, 246, 0.2);
            }
            QComboBox QAbstractItemView::item:selected {
                background: rgba(59, 130, 246, 0.4);
            }
        """)
        
        lang_container_layout.addWidget(self.lang_combo)
        lang_layout.addWidget(lang_container)
        layout.addLayout(lang_layout)
        
        layout.addStretch()
        
        title_label = QLabel("BLUSH INSTALLER")
        title_label.setAlignment(Qt.AlignCenter)
        title_label.setStyleSheet("""
            font-size: 64px; 
            font-weight: bold; 
            color: #667eea; 
            text-shadow: 0px 0px 20px rgba(102, 126, 234, 0.8), 
                         0px 0px 40px rgba(102, 126, 234, 0.4),
                         3px 3px 6px rgba(0,0,0,0.7); 
            margin: 50px;
            letter-spacing: 4px;
        """)
        layout.addWidget(title_label)
        
        self.description_label = QLabel(self.tr("description"))
        self.description_label.setAlignment(Qt.AlignCenter)
        self.description_label.setWordWrap(True)
        self.description_label.setStyleSheet("""
            font-size: 16px; 
            margin: 10px 40px; 
            color: rgba(255, 255, 255, 0.8); 
            line-height: 1.6;
            text-shadow: 0px 0px 10px rgba(0, 0, 0, 0.5);
        """)
        layout.addWidget(self.description_label)

        layout.addStretch()
        
        button_layout = QHBoxLayout()
        button_layout.addStretch()
        
        self.cancel_btn = QPushButton("Cancel")
        self.cancel_btn.setObjectName("cancelBtn")
        self.cancel_btn.clicked.connect(self.close)
        
        self.next_btn = QPushButton("Next →")
        self.next_btn.clicked.connect(lambda: self.stacked_widget.setCurrentIndex(1))
        
        button_layout.addWidget(self.cancel_btn)
        button_layout.addWidget(self.next_btn)
        layout.addLayout(button_layout)
        
        return page
        
    def create_license_page(self):
        page = QWidget()
        layout = QVBoxLayout(page)
        layout.setSpacing(25)
        layout.setContentsMargins(50, 50, 50, 50)
        
        self.license_title = QLabel("License Agreement")
        self.license_title.setAlignment(Qt.AlignCenter)
        self.license_title.setStyleSheet("""
            font-size: 32px; 
            font-weight: bold; 
            margin-bottom: 25px;
            color: #667eea;
            text-shadow: 0px 0px 15px rgba(102, 126, 234, 0.6);
        """)
        layout.addWidget(self.license_title)
        
        self.license_text_edit = QTextEdit()
        self.license_text_edit.setReadOnly(True)
        layout.addWidget(self.license_text_edit)
        
        self.accept_checkbox = QCheckBox("I accept the license agreement")
        self.accept_checkbox.stateChanged.connect(self.on_license_accept)
        layout.addWidget(self.accept_checkbox)
        
        button_layout = QHBoxLayout()
        button_layout.addStretch()
        
        self.back_btn = QPushButton("← Cancel")
        self.back_btn.setObjectName("cancelBtn")
        self.back_btn.clicked.connect(lambda: self.stacked_widget.setCurrentIndex(0))
        
        self.install_btn = QPushButton("Install →")
        self.install_btn.setEnabled(False)
        self.install_btn.clicked.connect(self.start_installation)
        
        button_layout.addWidget(self.back_btn)
        button_layout.addWidget(self.install_btn)
        layout.addLayout(button_layout)
        
        return page
        
    def create_install_page(self):
        page = QWidget()
        layout = QVBoxLayout(page)
        layout.setSpacing(40)
        layout.setContentsMargins(80, 60, 80, 60)
        
        self.install_title = QLabel("Installing Blush")
        self.install_title.setAlignment(Qt.AlignCenter)
        self.install_title.setStyleSheet("""
            font-size: 28px; 
            font-weight: 600; 
            color: #fafafa;
            margin-bottom: 20px;
        """)
        layout.addWidget(self.install_title)
        
        layout.addStretch()
        
        # simple status display
        self.status_label = QLabel("Preparing installation...")
        self.status_label.setAlignment(Qt.AlignCenter)
        self.status_label.setStyleSheet("""
            font-size: 16px; 
            color: #a1a1aa; 
            margin-bottom: 30px;
        """)
        layout.addWidget(self.status_label)
        
        self.status_icon = QLabel("●")
        self.status_icon.setAlignment(Qt.AlignCenter)
        self.status_icon.setStyleSheet("""
            QLabel {
                font-size: 24px;
                color: #71717a;
                margin: 10px 0;
            }
        """)
        layout.addWidget(self.status_icon)
        
        # clean progress bar
        progress_container = QWidget()
        progress_container.setFixedHeight(60)
        progress_layout = QVBoxLayout(progress_container)
        progress_layout.setContentsMargins(0, 0, 0, 0)
        
        self.progress_widget = QWidget()
        self.progress_widget.setFixedHeight(8)
        self.progress_widget.setStyleSheet("""
            QWidget {
                background-color: #27272a;
                border-radius: 4px;
            }
        """)
        
        self.progress_fill = QWidget(self.progress_widget)
        self.progress_fill.setFixedHeight(8)
        self.progress_fill.setFixedWidth(0)
        self.progress_fill.setStyleSheet("""
            QWidget {
                background-color: #3b82f6;
                border-radius: 4px;
            }
        """)
        
        progress_layout.addWidget(self.progress_widget)
        
        # progress percentage
        self.progress_label = QLabel("0%")
        self.progress_label.setAlignment(Qt.AlignCenter)
        self.progress_label.setStyleSheet("""
            font-size: 14px; 
            color: #71717a; 
            margin-top: 10px;
        """)
        progress_layout.addWidget(self.progress_label)
        
        layout.addWidget(progress_container)
        
        layout.addStretch()
        
        # debug section
        debug_layout = QHBoxLayout()
        debug_layout.addStretch()
        
        self.debug_btn = QPushButton("Debug Mode")
        self.debug_btn.setObjectName("debugBtn")
        self.debug_btn.clicked.connect(self.toggle_debug_mode)
        debug_layout.addWidget(self.debug_btn)
        layout.addLayout(debug_layout)
        
        self.details_text = QTextEdit()
        self.details_text.setReadOnly(True)
        self.details_text.setMaximumHeight(120)
        self.details_text.setVisible(False)
        layout.addWidget(self.details_text)
        
        return page
        
    def create_success_page(self):
        page = QWidget()
        layout = QVBoxLayout(page)
        layout.setSpacing(30)
        layout.setContentsMargins(50, 50, 50, 50)
        
        layout.addStretch()
        
        success_icon = QLabel("🎉")
        success_icon.setAlignment(Qt.AlignCenter)
        success_icon.setStyleSheet("""
            font-size: 120px; 
            margin: 30px;
            text-shadow: 0px 0px 30px rgba(102, 126, 234, 0.8);
        """)
        layout.addWidget(success_icon)
        
        self.success_title = QLabel("Installation Successful!")
        self.success_title.setAlignment(Qt.AlignCenter)
        self.success_title.setStyleSheet("""
            font-size: 36px; 
            font-weight: bold; 
            margin: 25px; 
            color: #667eea;
            text-shadow: 0px 0px 20px rgba(102, 126, 234, 0.6);
        """)
        layout.addWidget(self.success_title)
        
        self.success_message = QLabel(self.tr("installation_complete"))
        self.success_message.setAlignment(Qt.AlignCenter)
        self.success_message.setStyleSheet("""
            font-size: 16px; 
            margin: 25px; 
            line-height: 1.6;
            color: rgba(255, 255, 255, 0.9);
            text-shadow: 0px 0px 10px rgba(0, 0, 0, 0.5);
        """)
        self.success_message.setWordWrap(True)
        layout.addWidget(self.success_message)
        
        self.auto_launch_checkbox = QCheckBox("Launch Blush when installer closes")
        self.auto_launch_checkbox.setStyleSheet("font-size: 18px; margin: 25px;")
        self.auto_launch_checkbox.stateChanged.connect(self.on_auto_launch_changed)
        checkbox_layout = QHBoxLayout()
        checkbox_layout.addStretch()
        checkbox_layout.addWidget(self.auto_launch_checkbox)
        checkbox_layout.addStretch()
        layout.addLayout(checkbox_layout)
        
        layout.addStretch()
        
        self.close_btn = QPushButton("Close")
        self.close_btn.clicked.connect(self.on_close_clicked)
        close_layout = QHBoxLayout()
        close_layout.addStretch()
        close_layout.addWidget(self.close_btn)
        close_layout.addStretch()
        layout.addLayout(close_layout)
        
        return page
    
    def toggle_debug_mode(self):
        self.debug_mode = not self.debug_mode
        self.details_text.setVisible(self.debug_mode)
        self.debug_btn.setText("Hide Debug" if self.debug_mode else "Debug Mode")

    def change_language(self, index):
        if index < len(self.available_languages):
            old_language = self.language
            self.language = self.available_languages[index]
            print(f"[BLUSH LOG] Language changed from {old_language} to {self.language}")
            
            current_page = self.stacked_widget.currentWidget()
            if isinstance(current_page, FadeWidget):
                current_page.fade_out(150)
                QTimer.singleShot(150, lambda: [self.update_translations(), current_page.fade_in(150)])
            else:
                self.update_translations()

    def tr(self, key):
        if not self.translations:
            print(f"[BLUSH LOG] No translations loaded, returning key: {key}")
            return key
            
        if self.language not in self.translations:
            print(f"[BLUSH LOG] Language {self.language} not found in translations, returning key: {key}")
            return key
            
        translation = self.translations[self.language].get(key, key)
        if translation == key:
            print(f"[BLUSH LOG] Translation not found for key '{key}' in language '{self.language}'")
        
        return translation
        
    def update_translations(self):
        if not self.translations or self.language not in self.translations:
            print(f"[BLUSH LOG] Cannot update translations - translations: {bool(self.translations)}, language: {self.language}")
            return
            
        print(f"[BLUSH LOG] Updating translations for language: {self.language}")
        
        self.setWindowTitle(self.tr("title"))
        
        self.description_label.setText(self.tr("description"))
        self.cancel_btn.setText(self.tr("cancel"))
        self.next_btn.setText(self.tr("next") + " →")
        
        self.license_title.setText(self.tr("license_agreement"))
        self.accept_checkbox.setText(self.tr("accept_license"))
        self.back_btn.setText("← " + self.tr("cancel"))
        self.install_btn.setText(self.tr("install") + " →")
        self.success_title.setText(self.tr("success"))
        self.success_message.setText(self.tr("installation_complete"))
        self.auto_launch_checkbox.setText(self.tr("auto_launch"))
        self.close_btn.setText(self.tr("close"))

    def on_license_accept(self, state):
        self.license_accepted = (state == 2)
        self.install_btn.setEnabled(self.license_accepted)
        
    def start_installation(self):
        if not self.license_accepted:
            QMessageBox.warning(self, self.tr("error"), self.tr("license_required"))
            return
            
        self.stacked_widget.setCurrentIndex(2)
        QTimer.singleShot(100, self.perform_installation)
        
    def create_download_screen(self):
        download_widget = QWidget()
        download_widget.setStyleSheet("""
            QWidget {
                background-color: #0a0a0a;
            }
        """)
        
        layout = QVBoxLayout(download_widget)
        layout.setContentsMargins(60, 60, 60, 60)
        layout.setSpacing(30)
        
        # title
        title = QLabel("Installing Blush")
        title.setAlignment(Qt.AlignCenter)
        title.setStyleSheet("""
            QLabel {
                font-size: 28px;
                font-weight: 600;
                color: #fafafa;
                margin-bottom: 20px;
            }
        """)
        layout.addWidget(title)
        
        # status label
        self.status_label = QLabel("Preparing installation...")
        self.status_label.setAlignment(Qt.AlignCenter)
        self.status_label.setStyleSheet("""
            QLabel {
                font-size: 16px;
                color: #a1a1aa;
                margin-bottom: 30px;
            }
        """)
        layout.addWidget(self.status_label)
        
        # progress container
        progress_container = QWidget()
        progress_container.setFixedHeight(60)
        progress_layout = QVBoxLayout(progress_container)
        progress_layout.setContentsMargins(0, 0, 0, 0)
        progress_layout.setSpacing(10)
        
        # progress bar background
        self.progress_widget = QWidget()
        self.progress_widget.setFixedHeight(8)
        self.progress_widget.setStyleSheet("""
            QWidget {
                background-color: #27272a;
                border-radius: 4px;
            }
        """)
        
        # progress bar fill
        self.progress_fill = QWidget(self.progress_widget)
        self.progress_fill.setFixedHeight(8)
        self.progress_fill.setFixedWidth(0)
        self.progress_fill.setStyleSheet("""
            QWidget {
                background-color: #3b82f6;
                border-radius: 4px;
            }
        """)
        
        progress_layout.addWidget(self.progress_widget)
        
        # progress percentage
        self.progress_label = QLabel("0%")
        self.progress_label.setAlignment(Qt.AlignCenter)
        self.progress_label.setStyleSheet("""
            QLabel {
                font-size: 14px;
                color: #71717a;
                font-weight: 500;
            }
        """)
        progress_layout.addWidget(self.progress_label)
        
        layout.addWidget(progress_container)
        
        self.status_icon = QLabel("●")
        self.status_icon.setAlignment(Qt.AlignCenter)
        self.status_icon.setStyleSheet("""
            QLabel {
                font-size: 24px;
                color: #71717a;
                margin: 20px 0;
            }
        """)
        layout.addWidget(self.status_icon)
        
        layout.addStretch()
        
        # details text area for debug mode
        self.details_text = QTextEdit()
        self.details_text.setVisible(False)
        self.details_text.setStyleSheet("""
            QTextEdit {
                background-color: #18181b;
                border: 1px solid #27272a;
                border-radius: 8px;
                color: #a1a1aa;
                font-family: 'Consolas', 'Monaco', monospace;
                font-size: 12px;
                padding: 10px;
            }
        """)
        layout.addWidget(self.details_text)
        
        return download_widget

    def perform_installation(self):
        try:
            self.status_icon.setStyleSheet("""
                QLabel {
                    font-size: 24px;
                    color: #3b82f6;
                    margin: 20px 0;
                }
            """)
            self.status_label.setText("Detecting system...")
            self.update_progress(10)
            
            if self.debug_mode:
                self.details_text.append(f"[LOG] Detected: {self.system_info['full_os']} {self.system_info['full_arch']}")
                self.details_text.append(f"[LOG] Using architecture: {self.system_info['arch']}")
            
            QTimer.singleShot(500, self.start_download)
            
        except Exception as e:
            if self.debug_mode:
                self.details_text.append(f"[LOG] Error: {str(e)}")
            QMessageBox.critical(self, self.tr("error"), str(e))

    def start_download(self):
        try:
            if self.system_info['os'] == 'win':
                if self.system_info['arch'] == 'x64':
                    filename = "blush-win-x64.exe"
                elif self.system_info['arch'] == 'arm64':
                    filename = "blush-win-arm64.exe"
                else:
                    filename = "blush-win-x64.exe"
                url = f"https://cdn.getblush.xyz/{filename}"
                local_filename = "blush.exe"
            elif self.system_info['os'] == 'mac':
                if self.system_info['arch'] == 'arm64':
                    filename = "blush-mac-arm64"
                else:
                    filename = "blush-mac-x64"
                url = f"https://cdn.getblush.xyz/{filename}"
                local_filename = "blush"
            else:  # linux
                if self.system_info['arch'] == 'x64':
                    filename = "blush-linux-x64"
                elif self.system_info['arch'] == 'arm64':
                    filename = "blush-linux-arm64"
                else:
                    filename = "blush-linux-x64"
                url = f"https://cdn.getblush.xyz/{filename}"
                local_filename = "blush"
                
            if self.debug_mode:
                self.details_text.append(f"[LOG] Downloading: {filename}")
                self.details_text.append(f"[LOG] From: {url}")

            self.status_icon.setStyleSheet("""
                QLabel {
                    font-size: 24px;
                    color: #f59e0b;
                    margin: 20px 0;
                }
            """)
            self.status_label.setText("Starting download...")
            
            temp_path = Path.home() / "Downloads" / local_filename
            
            self.download_thread = DownloadThread(url, str(temp_path))
            self.download_thread.progress.connect(self.update_progress)
            self.download_thread.status.connect(self.update_status)
            self.download_thread.finished.connect(self.on_download_finished)
            self.download_thread.start()
            
        except Exception as e:
            if self.debug_mode:
                self.details_text.append(f"[LOG] Error: {str(e)}")
            QMessageBox.critical(self, self.tr("error"), str(e))
            
    def update_progress(self, value):
        progress_width = int((value / 100.0) * self.progress_widget.width())
        self.progress_fill.setFixedWidth(progress_width)
        self.progress_label.setText(f"{value}%")
        
    def update_status(self, message):
        self.status_label.setText(message)
        if self.debug_mode:
            self.details_text.append(message)
        
    def on_download_finished(self, success, message):
        if success:
            if self.debug_mode:
                self.details_text.append(message)
            self.status_icon.setStyleSheet("""
                QLabel {
                    font-size: 24px;
                    color: #10b981;
                    margin: 20px 0;
                }
            """)
            self.status_label.setText(self.tr("adding_to_path"))
            QTimer.singleShot(500, self.install_to_system)
        else:
            if self.debug_mode:
                self.details_text.append(message)
            QMessageBox.critical(self, self.tr("error"), message)
            
    def on_auto_launch_changed(self, state):
        self.auto_launch = (state == 2)
    
    def on_close_clicked(self):
        if self.auto_launch:
            try:
                if self.system_info['os'] == 'win':
                    if self.installation_path and self.installation_path.exists():
                        blush_exe_path = str(self.installation_path)
                        subprocess.Popen(f'start cmd /k "{blush_exe_path}"', shell=True)
                    else:
                        subprocess.Popen('start cmd /k blush', shell=True)
                else:
                    if self.installation_path and self.installation_path.exists():
                        subprocess.Popen([str(self.installation_path)])
                    else:
                        subprocess.Popen(['blush'])
            except Exception as e:
                if self.debug_mode:
                    self.details_text.append(f"[LOG] Could not launch Blush: {e}")
        self.close()

    def install_to_system(self):
        try:
            self.update_progress(75)
            
            if self.system_info['os'] == 'win':
                local_filename = "blush.exe"
                temp_path = Path.home() / "Downloads" / local_filename
                
                program_files = Path(os.environ.get('PROGRAMFILES', 'C:\\Program Files'))
                blush_dir = program_files / "Blush"
                use_system_path = False
                
                try:
                    blush_dir.mkdir(exist_ok=True)
                    final_path = blush_dir / local_filename
                    use_system_path = True
                except PermissionError:
                    app_data = Path(os.environ.get('LOCALAPPDATA', ''))
                    if not app_data:
                        app_data = Path.home() / "AppData" / "Local"
                    blush_dir = app_data / "Blush"
                    use_system_path = False
                    blush_dir.mkdir(exist_ok=True)
                    final_path = blush_dir / local_filename
                
                shutil.move(str(temp_path), str(final_path))
                os.chmod(str(final_path), 0o755)
                
                self.installation_path = final_path
                
                try:
                    import winreg
                    
                    # try to add to system path first
                    if use_system_path:
                        try:
                            with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, 
                                              r"SYSTEM\CurrentControlSet\Control\Session Manager\Environment",
                                              0, winreg.KEY_ALL_ACCESS) as key:
                                current_path, _ = winreg.QueryValueEx(key, "Path")
                                if str(blush_dir) not in current_path:
                                    new_path = current_path + ";" + str(blush_dir)
                                    winreg.SetValueEx(key, "Path", 0, winreg.REG_EXPAND_SZ, new_path)
                                    if self.debug_mode:
                                        self.details_text.append("[LOG] Added to system PATH")
                        except PermissionError:
                            use_system_path = False
                    
                    # fallback to user path
                    if not use_system_path:
                        with winreg.OpenKey(winreg.HKEY_CURRENT_USER, "Environment", 0, winreg.KEY_ALL_ACCESS) as key:
                            try:
                                current_path, _ = winreg.QueryValueEx(key, "Path")
                            except FileNotFoundError:
                                current_path = ""
                            
                            if str(blush_dir) not in current_path:
                                new_path = current_path + ";" + str(blush_dir) if current_path else str(blush_dir)
                                winreg.SetValueEx(key, "Path", 0, winreg.REG_EXPAND_SZ, new_path)
                                if self.debug_mode:
                                    self.details_text.append("[LOG] Added to user PATH")
                    
                    # notify system of path change
                    import ctypes
                    from ctypes import wintypes
                    HWND_BROADCAST = 0xFFFF
                    WM_SETTINGCHANGE = 0x001A
                    SMTO_ABORTIFHUNG = 0x0002
                    result = ctypes.windll.user32.SendMessageTimeoutW(
                        HWND_BROADCAST, WM_SETTINGCHANGE, 0, "Environment", 
                        SMTO_ABORTIFHUNG, 5000, ctypes.byref(wintypes.DWORD())
                    )
                    if self.debug_mode:
                        self.details_text.append(f"[LOG] PATH change notification result: {result}")
                        
                except Exception as e:
                    if self.debug_mode:
                        self.details_text.append(f"[LOG] Could not modify PATH: {e}")

            else:  # linux/mac
                local_filename = "blush"
                temp_path = Path.home() / "Downloads" / local_filename
                
                try:
                    system_bin = Path("/usr/local/bin")
                    
                    if system_bin.exists() and os.access(system_bin, os.W_OK):
                        final_path = system_bin / local_filename
                        shutil.move(str(temp_path), str(final_path))
                        os.chmod(str(final_path), 0o755)
                        
                        self.installation_path = final_path
                        if self.debug_mode:
                            self.details_text.append("[LOG] Installed to /usr/local/bin")
                        
                    else:
                        raise PermissionError("No system write access")
                        
                except (PermissionError, OSError):
                    # install to user directory
                    blush_dir = Path.home() / ".local" / "bin"
                    blush_dir.mkdir(parents=True, exist_ok=True)
                    
                    final_path = blush_dir / local_filename
                    shutil.move(str(temp_path), str(final_path))
                    os.chmod(str(final_path), 0o755)
                    
                    self.installation_path = final_path
                    if self.debug_mode:
                        self.details_text.append(f"[LOG] Installed to {final_path}")

                    shell_configs = [
                        Path.home() / ".bashrc",
                        Path.home() / ".zshrc",
                        Path.home() / ".profile",
                        Path.home() / ".bash_profile"
                    ]
                    
                    path_export = f'export PATH="$HOME/.local/bin:$PATH"'
                    
                    for config_file in shell_configs:
                        if config_file.exists():
                            try:
                                with open(config_file, 'r') as f:
                                    content = f.read()
                                if '.local/bin' not in content and 'PATH' in content:
                                    with open(config_file, 'a') as f:
                                        f.write(f'\n# added by blush installer\n{path_export}\n')
                                    if self.debug_mode:
                                        self.details_text.append(f"[LOG] Added to PATH in {config_file.name}")
                                elif '.local/bin' not in content:
                                    with open(config_file, 'a') as f:
                                        f.write(f'\n# added by blush installer\n{path_export}\n')
                                    if self.debug_mode:
                                        self.details_text.append(f"[LOG] Added PATH export to {config_file.name}")
                            except Exception as e:
                                if self.debug_mode:
                                    self.details_text.append(f"[LOG] Could not modify {config_file.name}: {e}")
                
            self.update_progress(100)
            self.status_icon.setStyleSheet("""
                QLabel {
                    font-size: 24px;
                    color: #10b981;
                    margin: 20px 0;
                }
            """)
            self.status_label.setText(self.tr("success"))
            QTimer.singleShot(1500, lambda: self.stacked_widget.setCurrentIndex(3))
            
        except Exception as e:
            if self.debug_mode:
                self.details_text.append(f"[LOG] Installation error: {str(e)}")
            QMessageBox.critical(self, self.tr("error"), str(e))

def main():
    app = QApplication(sys.argv)
    app.setStyle('Fusion')
    
    installer = BlushInstaller()
    installer.show()
    
    sys.exit(app.exec())

if __name__ == "__main__":
    main()
