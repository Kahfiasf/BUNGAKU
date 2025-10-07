/* bungaku.c
   Program console sederhana untuk menghitung:
   - Bunga Tunggal (simple interest)
   - Bunga Majemuk (compound interest)
   Build: gcc -std=c99 -O2 -o bungaku bungaku.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PERIODS_DISPLAY 240  /* Batasi tampilan baris untuk bunga majemuk */

void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void wait_enter() {
    printf("\nTekan Enter untuk melanjutkan...");
    clear_stdin();
}

void print_logo() {
    printf("========================================\n");
    printf("               _ _                     \n");
    printf("             ( (B) )  Bungaku           \n");
    printf("             ( ( ) ) Aplikasi Hitung    \n");
    printf("             / | /  Bunga & Keuangan   \n");
    printf("               /|/                      \n");
    printf("               ^-^                       \n");
    printf("========================================\n\n");
}

double input_double_positive(const char *prompt) {
    double x;
    int rc;
    while (1) {
        printf("%s", prompt);
        rc = scanf("%lf", &x);
        if (rc == 1 && x > 0) {
            clear_stdin();
            return x;
        } else {
            printf("Input tidak valid. Masukkan angka > 0.\n");
            clear_stdin();
        }
    }
}

int input_int_positive(const char *prompt) {
    int n;
    int rc;
    while (1) {
        printf("%s", prompt);
        rc = scanf("%d", &n);
        if (rc == 1 && n > 0) {
            clear_stdin();
            return n;
        } else {
            printf("Input tidak valid. Masukkan bilangan bulat > 0.\n");
            clear_stdin();
        }
    }
}

int choose_unit() {
    int choice;
    while (1) {
        printf("Pilih unit jangka waktu:\n");
        printf(" 1) Per Bulan\n");
        printf(" 2) Per Tahun\n");
        printf("Pilihan (1-2): ");
        if (scanf("%d", &choice) != 1) { clear_stdin(); }
        else {
            clear_stdin();
            if (choice == 1 || choice == 2) return choice;
        }
        printf("Pilihan tidak valid. Coba lagi.\n");
    }
}

void print_currency(double x) {
    /* Tampilkan dengan 2 desimal dan pemisah ribuan sederhana */
    long long intPart = (long long) x;
    int frac = (int) round((x - intPart) * 100);
    if (frac == 100) { intPart += 1; frac = 0; }

    /* Format ribuan */
    char buf[64];
    int pos = 0;
    long long tmp = intPart;
    if (tmp == 0) {
        buf[pos++] = '0';
    } else {
        char rev[64];
        int rpos = 0;
        while (tmp > 0) {
            int digit = tmp % 10;
            rev[rpos++] = '0' + digit;
            tmp /= 10;
            if (tmp > 0 && rpos % 3 == 0) rev[rpos++] = ','; /* titik/komma pemisah */
        }
        /* balik */
        for (int i = rpos - 1; i >= 0; --i) buf[pos++] = rev[i];
    }
    buf[pos] = '\0';
    printf("Rp %s.%02d", buf, frac);
}

void calc_simple(double P, double r, int n) {
    /* r = desimal per periode, n = jumlah periode */
    double totalInterest = P * r * n;
    double totalToPay = P + totalInterest;

    printf("\n--- Hasil: Bunga Tunggal ---\n");
    printf("Jumlah hutang awal: "); print_currency(P); printf("\n");
    printf("Periode: %d\n", n);
    printf("Persentase per periode: %.6g%%\n", r * 100.0);
    printf("Total bunga: "); print_currency(totalInterest); printf("\n");
    printf("Total yang harus dibayar: "); print_currency(totalToPay); printf("\n");
}

void calc_compound(double P, double r, int n) {
    /* A = P * (1 + r)^n */
    double amount = P * pow(1.0 + r, n);

    printf("\n--- Hasil: Bunga Majemuk ---\n");
    printf("Jumlah hutang awal: "); print_currency(P); printf("\n");
    printf("Periode: %d\n", n);
    printf("Persentase per periode: %.6g%%\n", r * 100.0);
    printf("Jumlah total (akhir): "); print_currency(amount); printf("\n");
    printf("Total bunga (akhir - pokok): "); print_currency(amount - P); printf("\n");

    if (n > 0) {
        printf("\nRincian saldo per periode:\n");
        if (n > MAX_PERIODS_DISPLAY) {
            printf("(Jumlah periode terlalu banyak untuk ditampilkan; hanya tampilkan %d periode pertama)\n", MAX_PERIODS_DISPLAY);
        }
        double prev = P;
        int limit = (n > MAX_PERIODS_DISPLAY) ? MAX_PERIODS_DISPLAY : n;
        printf("%5s | %20s | %15s\n", "Per", "Saldo Akhir Periode", "Bunga Periode");
        printf("------+----------------------+-----------------\n");
        for (int i = 1; i <= limit; ++i) {
            double bal = P * pow(1.0 + r, i);
            double interestThis = bal - prev;
            printf("%5d | ", i);
            print_currency(bal);
            printf(" | ");
            print_currency(interestThis);
            printf("\n");
            prev = bal;
        }
        if (limit < n) {
            printf("... (%d periode lagi tidak ditampilkan)\n", n - limit);
        }
    }
}

int main(void) {
    int mainChoice;
    while (1) {
        system(""); /* no-op untuk kompatibilitas */
        print_logo();
        printf("Menu Utama:\n");
        printf(" 1) Klik Menu\n");
        printf(" 2) Keluar\n");
        printf("Pilih (1-2): ");
        if (scanf("%d", &mainChoice) != 1) { clear_stdin(); printf("Input tidak valid.\n"); continue; }
        clear_stdin();

        if (mainChoice == 2) {
            printf("Terima kasih. Keluar...\n");
            break;
        } else if (mainChoice == 1) {
            /* Menu kedua: pilih jenis bunga */
            int menu2;
            while (1) {
                printf("\n--- Menu Pilih Jenis Bunga ---\n");
                printf(" 1) Bunga Tunggal\n");
                printf(" 2) Bunga Majemuk\n");
                printf(" 3) Kembali (Back)\n");
                printf("Pilihan (1-3): ");
                if (scanf("%d", &menu2) != 1) { clear_stdin(); printf("Input tidak valid.\n"); continue; }
                clear_stdin();

                if (menu2 == 3) break; /* kembali ke main menu */
                if (menu2 != 1 && menu2 != 2) { printf("Pilihan tidak valid.\n"); continue; }

                int unit = choose_unit(); /* 1=bulan, 2=tahun */
                int n = input_int_positive("Masukkan jangka waktu (jumlah periode, bilangan bulat > 0): ");
                double P = input_double_positive("Masukkan jumlah hutang (contoh 1000000): ");
                printf("Masukkan persentase bunga per periode (misal 1.5 untuk 1.5%% per periode): ");
                double rate;
                while (1) {
                    if (scanf("%lf", &rate) == 1 && rate >= 0) { clear_stdin(); break; }
                    else { printf("Input tidak valid. Masukkan angka >= 0: "); clear_stdin(); }
                }
                double r = rate / 100.0; /* desimal per periode */
                /* Nota: program mengasumsikan rate yang dimasukkan sesuai unit pilihan */

                /* Tampilan input selesai: beri pilihan Back atau Lanjutkan */
                int proceed;
                while (1) {
                    printf("\n 1) Lanjutkan (hitung)\n 2) Kembali ke menu jenis (Back)\nPilih (1-2): ");
                    if (scanf("%d", &proceed) != 1) { clear_stdin(); printf("Input tidak valid.\n"); continue; }
                    clear_stdin();
                    if (proceed == 1 || proceed == 2) break;
                    printf("Pilihan tidak valid.\n");
                }
                if (proceed == 2) continue; /* kembali pilih jenis */

                /* Lakukan perhitungan dan tampilkan hasil */
                if (menu2 == 1) {
                    calc_simple(P, r, n);
                } else {
                    calc_compound(P, r, n);
                }

                /* Tombol keluar di tampilan akhir: Back atau Keluar */
                int finalChoice;
                while (1) {
                    printf("\nPilihan:\n 1) Kembali ke input (Back)\n 2) Kembali ke menu utama\n 3) Keluar\nPilih (1-3): ");
                    if (scanf("%d", &finalChoice) != 1) { clear_stdin(); printf("Input tidak valid.\n"); continue; }
                    clear_stdin();
                    if (finalChoice >= 1 && finalChoice <= 3) break;
                    printf("Pilihan tidak valid.\n");
                }
                if (finalChoice == 1) {
                    /* kembali ke input untuk jenis yang sama */
                    continue;
                } else if (finalChoice == 2) {
                    break; /* kembali ke menu jenis */
                } else {
                    printf("Terima kasih. Keluar...\n");
                    exit(0);
                }
            } /* akhir while menu2 */
        } else {
            printf("Pilihan tidak dikenal. Coba lagi.\n");
        }
    } /* akhir main while */

    return 0;
}

