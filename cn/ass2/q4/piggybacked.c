#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FRAMES 100

int damaged_lost() {
    return rand() % 5 == 0;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int main() {
    srand(time(NULL));

    int n1, n2, w1, w2;
    printf("Enter frames for Sender1: ");
    scanf("%d", &n1);
    printf("Enter window size for Sender1: ");
    scanf("%d", &w1);

    printf("Enter frames for Sender2: ");
    scanf("%d", &n2);
    printf("Enter window size for Sender2: ");
    scanf("%d", &w2);

    int ack1[MAX_FRAMES] = {0}, ack2[MAX_FRAMES] = {0};
    int ptr1 = 0, ptr2 = 0;

    while (ptr1 < n1 || ptr2 < n2) {
        printf("\n\nSender1 Window: ");
        for (int i = ptr1; i < ptr1 + w1 && i < n1; i++)
            if (!ack1[i]) printf("%da ", i);

        for (int i = ptr1; i < ptr1 + w1 && i < n1; i++) {
            if (!ack1[i]) {
                if (damaged_lost()) {
                    printf("\nSender2 (Receiver): Frame %da lost", i);
                } else {
                    ack1[i] = 1;
                    printf("\nSender2 (Receiver): ACK piggybacked for Frame %da", i);
                }
            }
        }

        while (ack1[ptr1] && ptr1 < n1) ptr1++;

        printf("\nSender2 Window: ");
        for (int i = ptr2; i < ptr2 + w2 && i < n2; i++)
            if (!ack2[i]) printf("%db ", i);

        for (int i = ptr2; i < ptr2 + w2 && i < n2; i++) {
            if (!ack2[i]) {
                if (damaged_lost()) {
                    printf("\nSender1 (Receiver): Frame %db lost", i);
                } else {
                    ack2[i] = 1;
                    printf("\nSender1 (Receiver): ACK piggybacked for Frame %db", i);
                }
            }
        }

        while (ack2[ptr2] && ptr2 < n2) ptr2++;
    }

    printf("\n\nBoth senders completed transmission.\n");
    return 0;
}
