#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_PERIODS_DISPLAY 240  


void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void wait_enter() {
    printf("\nTekan Enter untuk melanjutkan...");
    clear_stdin();
}

void print_logo() {
    printf("============================================================\n");
    printf("        APLIKASI PENGHITUNG BUNGA TUNGGAL DAN MAJEMUK\n");
    printf("                     ---  BUNGAKU  ---\n");
    printf("============================================================\n");
    printf("                  Dibuat oleh Kelompok 4\n");
    printf("------------------------------------------------------------\n\n");
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
        printf("\nJangka waktu:\n");
        printf("1) Per Bulan\n");
        printf("2) Per Tahun\n");
        printf("   Pilihan (1-2): ");

        if (scanf("%d", &choice) != 1) {
            clear_stdin();
        } else {
            clear_stdin();
            if (choice == 1 || choice == 2)
                return choice;
        }
        printf("Pilihan tidak valid. Coba lagi.\n");
    }
}


void print_currency(double value) {
    long long intPart = (long long) value;
    int frac = (int) round((value - intPart) * 100);

    if (frac == 100) {
        intPart += 1;
        frac = 0;
    }

    char temp[64];
    char formatted[64];
    int len = 0;

    if (intPart == 0) {
        temp[len++] = '0';
    } else {
        int count = 0;
        while (intPart > 0) {
            if (count == 3) {
                temp[len++] = '.';
                count = 0;
            }
            temp[len++] = (intPart % 10) + '0';
            intPart /= 10;
            count++;
        }
    }
    temp[len] = '\0';

    int n = strlen(temp);
    for (int i = 0; i < n; ++i)
        formatted[i] = temp[n - i - 1];
    formatted[n] = '\0';

    if (frac > 0)
        printf("Rp %s,%02d", formatted, frac);
    else
        printf("Rp %s", formatted);
}


void calc_simple(double P, double r, int n) {
    double totalInterest = P * r * n;
    double totalToPay = P + totalInterest;

    printf("\n---------------- Hasil: Bunga Tunggal ----------------\n");
    printf("Jumlah hutang awal     : "); print_currency(P); printf("\n");
    printf("Periode                : %d\n", n);
    printf("Persentase per periode : %.6g%%\n", r * 100.0);
    printf("Total bunga            : "); print_currency(totalInterest); printf("\n");
    printf("Total yang dibayar     : "); print_currency(totalToPay); printf("\n");
}


void calc_compound(double P, double r, int n) {
    double amount = P * pow(1.0 + r, n);

    printf("\n---------------- Hasil: Bunga Majemuk ----------------\n");
    printf("Jumlah hutang awal     : "); print_currency(P); printf("\n");
    printf("Periode                : %d\n", n);
    printf("Persentase per periode : %.6g%%\n", r * 100.0);
    printf("Jumlah total (akhir)   : "); print_currency(amount); printf("\n");
    printf("Total bunga            : "); print_currency(amount - P); printf("\n");

    if (n > 0) {
        printf("\nRincian saldo per periode:\n");
        if (n > MAX_PERIODS_DISPLAY)
            printf("(Menampilkan hanya %d periode pertama)\n", MAX_PERIODS_DISPLAY);

        printf("------------------------------------------------------------\n");
        printf("%5s | %10s | %30s\n", "Per", "Saldo Akhir", "Bunga Per Periode");
        printf("------+----------------------+------------------------------\n");

        double prev = P;
        int limit = (n > MAX_PERIODS_DISPLAY) ? MAX_PERIODS_DISPLAY : n;

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

        if (limit < n)
            printf("... (%d periode lagi tidak ditampilkan)\n", n - limit);
    }
}


int main(void) {
    int mainChoice;

    while (1) {
        system("");  
        print_logo();

        printf("Menu Utama:\n");
        printf("1) Menu\n");
        printf("2) Keluar\n");
        printf("   Pilihan (1-2): ");

        if (scanf("%d", &mainChoice) != 1) {
            clear_stdin();
            printf("Input tidak valid.\n");
            continue;
        }
        clear_stdin();

        if (mainChoice == 2) {
            printf("Terima asih Telah Menggunakan Aplikasi Penghitung BUNGAKU.\n");
            break;
        }

        else if (mainChoice == 1) {
            int menu2;
            while (1) {
                printf("\n--------------- Pilih Jenis Bunga ---------------\n");
                printf("1) Bunga Tunggal\n");
                printf("2) Bunga Majemuk\n");
                printf("3) Kembali\n");
                printf("   Pilihan (1-3): ");

                if (scanf("%d", &menu2) != 1) {
                    clear_stdin();
                    printf("Input tidak valid.\n");
                    continue;
                }
                clear_stdin();

                if (menu2 == 3)
                    break;
                if (menu2 != 1 && menu2 != 2) {
                    printf("Pilihan tidak valid.\n");
                    continue;
                }

                int unit = choose_unit();
                int n = input_int_positive("   Masukkan jangka waktu (per periode): ");
                double P = input_double_positive("   Masukkan jumlah hutang (contoh 1000000): ");

                printf("   Masukkan persentase bunga per periode (contoh 1.5 untuk 1.5%%): ");
                double rate;
                while (1) {
                    if (scanf("%lf", &rate) == 1 && rate >= 0) {
                        clear_stdin();
                        break;
                    } else {
                        printf("Input tidak valid. Masukkan angka >= 0: ");
                        clear_stdin();
                    }
                }
                double r = rate / 100.0;

                int proceed;
                while (1) {
                    printf("\n1) Hitung\n");
                    printf("2) Kembali ke Menu Jenis\n");
                    printf("   Pilih (1-2): ");
                    if (scanf("%d", &proceed) != 1) {
                        clear_stdin();
                        printf("Input tidak valid.\n");
                        continue;
                    }
                    clear_stdin();
                    if (proceed == 1 || proceed == 2) break;
                    printf("Pilihan tidak valid.\n");
                }

                if (proceed == 2) continue;

                if (menu2 == 1)
                    calc_simple(P, r, n);
                else
                    calc_compound(P, r, n);

                int finalChoice;
                while (1) {
                    printf("\nPilihan:\n");
                    printf("  1) Kembali ke Input\n");
                    printf("  2) Kembali ke Menu Utama\n");
                    printf("  3) Keluar\n");
                    printf("Pilih (1-3): ");

                    if (scanf("%d", &finalChoice) != 1) {
                        clear_stdin();
                        printf("Input tidak valid.\n");
                        continue;
                    }
                    clear_stdin();

                    if (finalChoice >= 1 && finalChoice <= 3)
                        break;
                    printf("Pilihan tidak valid.\n");
                }

                if (finalChoice == 1)
                    continue;
                else if (finalChoice == 2)
                    break;
                else {
                    printf("Terima Kasih Telah Menggunakan Aplikasi Penghitung BUNGAKU.\n");
                    exit(0);
                }
            }
        } 
        else {
            printf("Pilihan tidak dikenal. Coba lagi.\n");
        }
    }

    return 0;
}
