#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h> 

#define MAX_PERIODS_DISPLAY 240  

void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void running_text(const char *text, int delay_ms) {
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        fflush(stdout);
        Sleep(delay_ms); 
    }
    printf("\n");
}

void wait_enter() {
    printf("\nTekan Enter untuk melanjutkan...");
    clear_stdin();
}

void print_logo() {
    printf("\033[34m============================================================\033[0m\n");
    running_text("\033[35m       APLIKASI PENGHITUNG BUNGA TUNGGAL DAN MAJEMUK\033[0m", 40);
        printf("\n");
    running_text("\033[95m                     ---  BungaKu  ---\033[0m", 40);
        printf("\n");
    printf("\033[34m============================================================\033[0m\n");
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
        running_text("\n\033[36mJangka waktu:\n\033[0m", 30);
        printf("1) Per Bulan\n");
        printf("2) Per Tahun\n");
        printf("\033[32m   Pilihan (1-2): \033[0m");

        if (scanf("%d", &choice) != 1) {
            clear_stdin();
        } else {
            clear_stdin();
            if (choice == 1 || choice == 2)
                return choice;
        }
        printf("\033[91mPilihan tidak valid. Coba lagi.\n\033[0m");
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

    running_text("\n\033[36m---------------- Hasil : Bunga Tunggal ----------------\n\033[0m", 30);
    printf("\033[32mJumlah hutang awal     : \033[0m"); print_currency(P); printf("\n");
    printf("\033[32mPeriode                : \033[0m%d\n", n);
    printf("\033[32mPersentase per periode : \033[0m%.6g%%\n", r * 100.0);
    printf("\033[32mTotal bunga            : \033[0m"); print_currency(totalInterest); printf("\n");
    printf("\033[32mTotal yang dibayar     : \033[0m"); print_currency(totalToPay); printf("\n");
}

void calc_compound(double P, double r, int n) {
    double amount = P * pow(1.0 + r, n);

    running_text("\033[36m\n---------------- Hasil : Bunga Majemuk ----------------\n\033[0m", 30);
    printf("\033[32mJumlah hutang awal     : \033[0m"); print_currency(P); printf("\n");
    printf("\033[32mPeriode                : \033[0m%d\n", n);
    printf("\033[32mPersentase per periode : \033[0m%.6g%%\n", r * 100.0);
    printf("\033[32mJumlah total (akhir)   : \033[0m"); print_currency(amount); printf("\n");
    printf("\033[32mTotal bunga            : \033[0m"); print_currency(amount - P); printf("\n");

    if (n > 0) {
        printf("\nRincian saldo per periode:\n");
        if (n > MAX_PERIODS_DISPLAY)
            printf("(Menampilkan hanya %d periode pertama)\n", MAX_PERIODS_DISPLAY);

        printf("\033[36m------------------------------------------------------------\n\033[0m");
        printf("\033[32m%5s | %10s | %30s\n", "Per", "Saldo Akhir", "Bunga Per Periode");
        printf("\033[36m------+----------------------+------------------------------\n\033[0m");

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

        running_text("\033[33mSelamat datang di aplikasi BungaKu..... \033[0m", 40);
        printf("\n");

        running_text("\033[36mMenu Utama:\033[0m\n", 30);
        printf("1) Menu\n");
        printf("2)\033[91m Keluar\n\033[0m");
        printf("\033[32m   Pilihan (1-2): \033[0m");

        if (scanf("%d", &mainChoice) != 1) {
            clear_stdin();
            printf("Input tidak valid.\n");
            continue;
        }
        clear_stdin();

        if (mainChoice == 2) {
            running_text("\033[38;2;255;80;1mTerima Kasih Telah Menggunakan Aplikasi BUNGAKU.\033[0m", 40);
        break;
        }

        else if (mainChoice == 1) {
            int menu2;
            while (1) {
                running_text("\033[36m\n--------------- Pilih Jenis Bunga ---------------\n\033[0m", 30);
                printf("1) Bunga Tunggal\n");
                printf("2) Bunga Majemuk\n");
                printf("\033[91m3) Kembali\n\033[0m");
                printf("\033[32m   Pilihan (1-3): \033[0m");

                if (scanf("%d", &menu2) != 1) {
                    clear_stdin();
                    printf("Input tidak valid.\n");
                    continue;
                }
                clear_stdin();

                if (menu2 == 3)
                    break;
                if (menu2 != 1 && menu2 != 2) {
                    printf("\033[91mPilihan tidak valid.\n\033[0m");
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
                    printf("\033[32m   Pilih (1-2): \033[0m");
                    if (scanf("%d", &proceed) != 1) {
                        clear_stdin();
                        printf("Input tidak valid.\n");
                        continue;
                    }
                    clear_stdin();
                    if (proceed == 1 || proceed == 2) break;
                    printf("\033[91mPilihan tidak valid.\n\033[0m");
                }

                if (proceed == 2) continue;

                if (menu2 == 1)
                    calc_simple(P, r, n);
                else
                    calc_compound(P, r, n);

                int finalChoice;
                while (1) {
                    printf("\033[36m\nPilihan:\n\033[0m");
                    printf("  1) Kembali ke menu jenis \n");
                    printf("  2) Kembali ke Menu Utama\n");
                    printf("\033[91m  3) Keluar\n\033[0m");
                    printf("\033[32mPilih (1-3): \033[0m");

                    if (scanf("%d", &finalChoice) != 1) {
                        clear_stdin();
                        printf("Input tidak valid.\n");
                        continue;
                    }
                    clear_stdin();

                    if (finalChoice >= 1 && finalChoice <= 3)
                        break;
                    printf("\033[91mPilihan tidak valid.\n\033[0m");
                }

                if (finalChoice == 1)
                    continue;
                else if (finalChoice == 2)
                    break;
                else {
                    running_text("\033[38;2;255;80;1mTerima Kasih Telah Menggunakan Aplikasi BUNGAKU.\033[0m", 40);
                    exit(0);
                }
            }
        } 
        else {
            printf("\033[91mPilihan tidak dikenal. Coba lagi.\n\033[0m");
        }
    }

    return 0;
}

