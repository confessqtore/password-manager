# **Password Manager**
![Language](https://img.shields.io/badge/Language-C-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Encryption](https://img.shields.io/badge/Encryption-libsodium-red)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS-lightgrey)
## Un gestore password sicuro da linea di comando
### Caratteristiche:
  - Interfaccia interattiva da terminale
  - Crittografia con libsodium
  - Password memorizzate su file binari crittografati
  - Archiviazione in locale
### Download/Installazione
### Requisiti:
  - Installare libsodium
  - Installare lncurses

    **Debian/Ubuntu**: 

    ```
    sudo apt-get install libsodium-dev libncurses5-dev libncursesw5-dev
    ```

    **Fedora/RHEL**: 

    ```
    sudo dnf install libsodium-devel ncurses-devel
    ```

    **Arch Linux**: 

    ```
    sudo pacman -S libsodium ncurses
    ```

    **MacOs(con HomeBrew)**: 

    ```
    brew install libsodium ncurses
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
  - Non ti dimenticare la password che inserisci - senza di quella no potrai accedere e di conseguenza recuperare i tuoi dati
  - Le informazioni sulle password sono salvate in locale nei corrispettivi file:
    - nomi.txt
    - pass.bin
    - passwordUtente.bin
## Enjoy!
