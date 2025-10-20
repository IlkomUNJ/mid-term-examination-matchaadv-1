# Laporan Analisis: Ukuran Jendela dan Pola Segmen

## Tujuan
Menganalisis ukuran jendela yang optimal untuk mengidentifikasi segmen garis lurus dalam citra biner (canvas Qt) dan menentukan pola jendela yang valid sebagai kandidat segmen.

## Metodologi
- Menggunakan canvas berlatar putih (`0xffffffff`).
- Piksel aktif = warna selain putih (misalnya biru dari titik, merah dari garis).
- Melakukan sliding window 3×3 di seluruh area canvas (kecuali tepi).
- Mengumpulkan semua jendela **non-kosong** (minimal 1 piksel aktif) untuk analisis.

## Temuan

### 1. Ukuran Jendela Ideal: **3×3**
Alasan:
- **2×2**: Terlalu kecil untuk menentukan arah garis (tidak cukup informasi spasial).
- **3×3**: Cukup untuk merepresentasikan arah dasar (horizontal, vertikal, diagonal) dengan minimal 3 piksel aktif.
- **≥4×4**: Berisiko mencakup noise, persimpangan, atau lebih dari satu segmen → mengurangi akurasi deteksi lokal.
- Kompatibel dengan resolusi canvas dan efisien secara komputasi.

### 2. Pola Jendela Valid untuk Segmen
Sebuah jendela 3×3 dianggap sebagai **kandidat segmen** jika:
- Memiliki **tepat 3 piksel aktif**.
- Piksel aktif membentuk **garis lurus kontinu** dalam salah satu konfigurasi berikut:

| Arah         | Pola (1 = aktif, 0 = latar) |
|--------------|-----------------------------|
| Horizontal   | `0 0 0`<br>`1 1 1`<br>`0 0 0` |
| Vertikal     | `0 1 0`<br>`0 1 0`<br>`0 1 0` |
| Diagonal ↘   | `1 0 0`<br>`0 1 0`<br>`0 0 1` |
| Diagonal ↙   | `0 0 1`<br>`0 1 0`<br>`1 0 0` |

Pola lain (misalnya bentuk L, T, cluster acak, atau >3 piksel) **tidak dianggap segmen** karena tidak merepresentasikan garis lurus sederhana.

## Kesimpulan
Ukuran jendela **3×3** dan keempat pola di atas digunakan sebagai dasar untuk mengembangkan algoritma deteksi otomatis pada Tujuan 3.