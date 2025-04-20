# EM-SideChannel-Attack-AES128
Electromagnetic side-channel analysis of AES-128 on Raspberry Pi 4 using a bare-metal implementation. Key recovery through trace alignment and statistical analysis.

# 🔒 Bare-Metal AES-128 Side-Channel Attack on Raspberry Pi 4 (DRDO Internship Project)

This project demonstrates an **electromagnetic (EM) side-channel attack** on a **bare-metal implementation of AES-128** encryption, conducted as part of an internship at **DRDO’s SAG Lab**. The aim was to show the feasibility of recovering secret encryption keys by analyzing EM radiation emitted from a Raspberry Pi 4 during cryptographic operations.

---

## 📌 Objective

To implement **AES-128 encryption in bare-metal C** on Raspberry Pi 4, collect EM traces during encryption, and apply **statistical side-channel analysis techniques** (specifically **Correlation Power Analysis**, CPA) to extract the secret AES key.

---

## ⚙️ Hardware and Tools Used

| Component | Description |
|----------|-------------|
| **Target** | Raspberry Pi 4 Model B |
| **Probe** | ELV Elektronik EM Probe Set |
| **Oscilloscope** | Rigol DS1054Z (for trace capture) |
| **Dev Machine** | PC running Ubuntu via VirtualBox with GCC-ARM toolchain |
| **Other** | MicroSD, USB keyboard, monitor, HDMI cable, power supply |

---

## 🧰 Software Stack

- **Bare-metal C** (No OS)
- **GCC ARM Cross-Compiler**
- **Python / MATLAB** for trace processing
- **CSV** format for storing EM traces
- **UART** communication (for plaintext/ciphertext transfer)

---

## 📜 Project Structure

