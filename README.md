# **Password Manager**
## Un gestore di password sicuro da linea di comando
![Language](https://img.shields.io/badge/Language-C-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Encryption](https://img.shields.io/badge/Encryption-libsodium-red)
![Platform](https://img.shields.io/badge/Platform-Linux%20-lightgrey)
### Caratteristiche:
  - Interfaccia interattiva da terminale
  - Crittografia con libsodium
  - Password memorizzate su file binari crittografati
  - Archiviazione in locale
### Requisiti:
  - Installare libsodium
  - Installare lncurses
  - Installare xclip

    **Debian/Ubuntu**: 

    ```
    sudo apt update
    sudo apt install xclip
    sudo apt-get install libsodium-dev libncurses5-dev libncursesw5-dev
    ```

    **Fedora/RHEL**: 

    ```
    sudo dnf install xclip
    sudo dnf install libsodium-devel ncurses-devel
    ```

    **Arch Linux**: 

    ```
    sudo pacman -S xclip
    sudo pacman -S libsodium ncurses
    ```

### Installazione: 
  ```
    git clone https://github.com/confessqtore/password-manager.git
    cd password-manager
    chmod +x setup
    ./setup
  ```

  Lo script setup crea automaticamente tutto il necessario in modo che il programma funzioni correttamente.

  Il file passwordManager.c verrà compilato a sua volta automaticamente dallo script setup.

### Utilizzo:
  Una volta eseguito il file ./setup per avviare il Password Manager basterà digitare il comando ./passwordManager.

### Note:
  - Non ti dimenticare la password che inserisci - senza di quella non potrai accedere e di conseguenza recuperare i tuoi dati
  - Le informazioni sulle password sono salvate in locale nei corrispettivi file cifrati:
    - nomi.txt
    - pass.bin
    - passwordUtente.bin
  - La funzione copia non potrebbe funzionare correttamente se si usa Wayland
## Enjoy! ☺️
