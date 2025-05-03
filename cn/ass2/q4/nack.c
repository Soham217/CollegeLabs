#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FRAMES 100

int damaged_lost() {
    return rand() % 5 == 0;
}

int main() {
    srand(time(NULL));

    int total_frames, window_size;
    printf("Enter number of frames: ");
    scanf("%d", &total_frames);
    printf("Enter window size: ");
    scanf("%d", &window_size);

    int base = 0;
    int acked[MAX_FRAMES] = {0};

    while (base < total_frames) {
        printf("\nSending window: ");
        for (int i = base; i < base + window_size && i < total_frames; i++) {
            if (!acked[i])
                printf("%d ", i);
        }

        for (int i = base; i < base + window_size && i < total_frames; i++) {
            if (!acked[i]) {
                if (damaged_lost()) {
                    printf("\nNACK: Frame %d lost/damaged. Retransmit needed.\n", i);
                } else {
                    printf("\nACK: Frame %d received.\n", i);
                    acked[i] = 1;
                }
            }
        }

        while (acked[base] && base < total_frames) base++;
    }

    printf("\nAll frames sent successfully.\n");
    return 0;
}
