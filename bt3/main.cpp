#include <stdio.h>
#include <string.h>

#define MAX_ZONES 10
#define MAX_QRS 20

#define SUCCESS 0
#define ERR_INVALID_QTY -1
#define ERR_OVERBOOKING -2
#define ERR_ZONE_NOT_FOUND -3
#define ERR_DOUBLE_CHECKIN -4
#define ERR_EMPTY_LIST -5
#define ERR_QR_NOT_FOUND -6

typedef struct {
    int zone_id;
    char zone_name[30];
    float ticket_price;
    int total_capacity;
    int sold_tickets;
} TicketZone;

typedef struct {
    char qr_code[20];
    int zone_id;
    int is_used;
} QrCheckIn;

int findZoneIndex(const TicketZone zones[], int total_zones, int zone_id);
int findQrIndex(const QrCheckIn qrs[], int total_qrs, const char *qr_code);
int datVe(TicketZone zones[], int total_zones, int zone_id, int quantity, int is_early_bird, float *total_cost);
int checkInQR(QrCheckIn qrs[], int total_qrs, const char *qr_code);
void hienThiKhuVuc(const TicketZone zones[], int total_zones);

int main() {
    TicketZone zones[MAX_ZONES] = {
        {1, "VIP Zone", 2000000.0f, 10, 8},
        {2, "GA Zone", 800000.0f, 50, 20}
    };
    int total_zones = 2;

    QrCheckIn qrs[MAX_QRS] = {
        {"QR101", 1, 0},
        {"QR102", 1, 1}
    };
    int total_qrs = 2;

    float total_cost = 0.0f;
    int res = 0;

    printf("=== HE THONG DAT VE & CHECK-IN (REFRACTORED MODULE) ===\n\n");
    hienThiKhuVuc(zones, total_zones);

    printf("\n--- KIEM THU KHU VUC DAT VE ---\n");

    res = datVe(zones, total_zones, 1, 5, 0, &total_cost);
    if (res == ERR_INVALID_QTY) {
        printf("[TEST PASS] Ban vuot han muc (1-4 ve/lan) bi tu choi.\n");
    }

    res = datVe(zones, total_zones, 1, 3, 1, &total_cost);
    if (res == ERR_OVERBOOKING) {
        printf("[TEST PASS] Overbooking khu vuc bi tu choi thanh cong.\n");
    }

    res = datVe(zones, total_zones, 99, 2, 0, &total_cost);
    if (res == ERR_ZONE_NOT_FOUND) {
        printf("[TEST PASS] Ma khu vuc khong ton tai bi tu choi.\n");
    }

    res = datVe(zones, total_zones, 1, 2, 1, &total_cost);
    if (res == SUCCESS) {
        printf("[TEST PASS] Dat 2 ve VIP thanh cong! Tong tien (giam 15%%): %.0f VND\n", total_cost);
    }

    printf("\n--- KIEM THU KHU VUC CHECK-IN QR ---\n");

    res = checkInQR(qrs, total_qrs, "QR102");
    if (res == ERR_DOUBLE_CHECKIN) {
        printf("[TEST PASS] Canh bao quet trung lap QR102 thanh cong.\n");
    }

    res = checkInQR(qrs, total_qrs, "QR101");
    if (res == SUCCESS) {
        printf("[TEST PASS] Check-in QR101 thanh cong.\n");
    }

    res = datVe(zones, 0, 1, 1, 0, &total_cost);
    if (res == ERR_EMPTY_LIST) {
        printf("[TEST PASS] Phat hien va chan loi thao tac tren danh sach rong.\n");
    }

    printf("\n--- TRANG THAI CUOI CUNG ---\n");
    hienThiKhuVuc(zones, total_zones);

    return 0;
}

int findZoneIndex(const TicketZone zones[], int total_zones, int zone_id) {
    if (total_zones <= 0) return ERR_EMPTY_LIST;
    for (int i = 0; i < total_zones; i++) {
        if (zones[i].zone_id == zone_id) {
            return i;
        }
    }
    return ERR_ZONE_NOT_FOUND;
}

int findQrIndex(const QrCheckIn qrs[], int total_qrs, const char *qr_code) {
    if (total_qrs <= 0) return ERR_EMPTY_LIST;
    for (int i = 0; i < total_qrs; i++) {
        if (strcmp(qrs[i].qr_code, qr_code) == 0) {
            return i;
        }
    }
    return ERR_QR_NOT_FOUND;
}

int datVe(TicketZone zones[], int total_zones, int zone_id, int quantity, int is_early_bird, float *total_cost) {
    if (total_zones <= 0) return ERR_EMPTY_LIST;

    if (quantity < 1 || quantity > 4) {
        return ERR_INVALID_QTY;
    }

    int idx = findZoneIndex(zones, total_zones, zone_id);
    if (idx < 0) return idx;

    if (zones[idx].sold_tickets + quantity > zones[idx].total_capacity) {
        return ERR_OVERBOOKING;
    }

    float price = zones[idx].ticket_price;
    if (is_early_bird == 1) {
        price *= 0.85f;
    }

    *total_cost = price * quantity;
    zones[idx].sold_tickets += quantity;

    return SUCCESS;
}

int checkInQR(QrCheckIn qrs[], int total_qrs, const char *qr_code) {
    if (total_qrs <= 0) return ERR_EMPTY_LIST;

    int idx = findQrIndex(qrs, total_qrs, qr_code);
    if (idx < 0) return idx;

    if (qrs[idx].is_used == 1) {
        return ERR_DOUBLE_CHECKIN;
    }

    qrs[idx].is_used = 1;
    return SUCCESS;
}

void hienThiKhuVuc(const TicketZone zones[], int total_zones) {
    printf("DANH SACH KHU VUC VE (%d khu vuc):\n", total_zones);
    for (int i = 0; i < total_zones; i++) {
        printf("ID: %d | Ten: %-10s | Gia: %.0f | Da ban: %d/%d\n",
               zones[i].zone_id, zones[i].zone_name, zones[i].ticket_price,
               zones[i].sold_tickets, zones[i].total_capacity);
    }
}
