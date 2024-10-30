| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
(Erstlellungsdatum: 30.10.2024)
# _Norvi SmartPlug NBIOT_

## Lizenz: CC BY-NC-ND 4.0
https://creativecommons.org/licenses/by-nc-nd/4.0/
[![License](https://img.shields.io/badge/LICENSE%20CC%20BY-NC-ND%20)](https://github.com/idleberg/Creative-Commons-Markdown/blob/master/4.0/by-nc-nd.markdown)



## Hardware-Anforderung
Norvi SMP-R11SMP-R11

## Hardware Komponenten 
- ESP32 Wrover 4MB
- Quectel Modem Support: BC28JA, BC28JB

## Überblick

**Norvi SmartPlug NBIOT** ist ein Projekt, das in C entwickelt wird. Es verwendet CMake als Build-System und ist mit CLion 2024.2.3 entwickelt worden. Dieses Projekt läuft auf einer Linux-Umgebung (amd64) mit dem Pop!_OS GNOME Desktop Environment.

Um CMake zu nutzen wird das ESP-IDF (Espressif IoT Development Framework) vorausgesetzt.
## Voraussetzungen

Um dieses Projekt zu bauen und auszuführen, benötigen Sie folgende Software:

- **CMake** (Version ≥ 3.x)
- **GCC** oder **Clang**
- **CLion IDE** (empfohlen: Version 2024.2.3)
- **Git** (für Versionskontrolle)
- **GitKraken** (GIT GUI Client)
- **ESP_IDF** (v5.3.1)
- **Python** (3.10.12)

## Installation

1. **Projekt klonen**:
    ```sh
    git clone https://github.com/IhrBenutzername/sample_project.git
    cd sample_project
    ```

2. **Abhängigkeiten installieren** (optional, je nach verwendeten Bibliotheken):

    - Für Debian-basierte Systeme:
      ```sh
      sudo apt-get update
      sudo apt-get install build-essential cmake
      ```

    - Für RedHat-basierte Systeme:
      ```sh
      sudo yum groupinstall 'Development Tools'
      sudo yum install cmake
      ```

## Bauen und Ausführen

1. **Build-Verzeichnis erstellen**:
    ```sh
    mkdir build
    cd build
    ```

2. **Projekt konfigurieren und bauen**:
    ```sh
    cmake ..
    make
    ```

3. **Programm ausführen**:
    ```sh
    ./IhrProgramName
    ```

## Projektstruktur

```plaintext
sample_project/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   └── ...
└── include/
    ├── ...
```

- **CMakeLists.txt**: CMake-Build-Skripte.
- **src/**: Verzeichnis mit Quellcode-Dateien.
- **include/**: Verzeichnis mit Header-Dateien.

## Mitmachen

Wenn Sie zum Projekt beitragen möchten:

1. Erstellen Sie einen Fork des Projekts.
2. Erstellen Sie einen Feature-Branch (`git checkout -b feature/NeuesFeature`).
3. Committen Sie Ihre Änderungen (`git commit -am 'Fügen Sie ein neues Feature hinzu'`).
4. Pushen Sie den Branch (`git push origin feature/NeuesFeature`).
5. Öffnen Sie einen Pull Request.

## Lizenz

Dieses Projekt ist lizenziert unter der MIT-Lizenz - siehe die [LICENSE](LICENSE) Datei für Details.

## Kontakt

Wenn Sie Fragen haben, können Sie uns unter [IhreEmail@example.com](mailto:IhreEmail@example.com) erreichen.