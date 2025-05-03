#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FRAMES 1024

int is_lost_or_damaged() {
    return rand() % 5 == 0;
}


int min(int a, int b) {
    return (a < b) ? a : b;
}

int main() {
    srand(time(NULL));

    int m, n_frames, sender_window_size, receiver_buffer_size;
    printf("Enter number of bits for sequence number (m): ");
    scanf("%d", &m);

    int max_seq = (1 << m);
    printf("Enter number of frames to send: ");
    scanf("%d", &n_frames);

    sender_window_size = max_seq - 1;
    printf("Sender window size is %d (2^m - 1)\n", sender_window_size);

    printf("Enter receiver buffer size (≤ %d): ", sender_window_size);
    scanf("%d", &receiver_buffer_size);

    int next_frame = 0;
    int acked[MAX_FRAMES] = {0};

    while (next_frame < n_frames) {
        int send_count = 0;
        printf("\nSender sending window: ");

        for (int i = next_frame; i < min(next_frame + sender_window_size, n_frames); ++i) {
            if (acked[i]) continue; 
            if (send_count >= receiver_buffer_size) break;

            printf("%d ", i % max_seq);
            send_count++;
        }
        printf("\n");

        for (int i = next_frame; i < next_frame + send_count && i < n_frames; ++i) {
            if (acked[i]) continue;

            if (is_lost_or_damaged()) {
                printf("Receiver: Frame %d lost/damaged. No ACK.\n", i % max_seq);
            } else {
                acked[i] = 1;
                printf("Receiver: ACK %d | Free buffer slots: %d\n",
                       i % max_seq, receiver_buffer_size - 1);
                receiver_buffer_size--; 
            }
        }

       
        while (acked[next_frame] && next_frame < n_frames)
            next_frame++;

        
        receiver_buffer_size = min(receiver_buffer_size + send_count, sender_window_size);
    }

    printf("\nAll frames sent and acknowledged.\n");
    return 0;
}
