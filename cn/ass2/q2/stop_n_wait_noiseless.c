#include <stdio.h>
#include <stdlib.h>

typedef enum {frame_arrival} event_type;
typedef enum {data, acknowledgement} frame_kind;

int current_seq_number = 1;

typedef struct {
    frame_kind kind;
    int seq;
    int ack;
    int info;
} frame;

frame *interface_buffer;
event_type global_event = frame_arrival;

void wait_for_event(event_type *event) {
    while(global_event != frame_arrival) {};
    *event = global_event;
}

int get_data() {
    int data;
    printf("\nEnter packet info (an integer): ");
    scanf("%d", &data);
    return data;
}

frame* make_frame(frame_kind kind, int info, int acknowledgement) {
    frame* f = (frame*)malloc(sizeof(frame));
    f->kind = kind;
    if(f->kind == data)
        f->seq = current_seq_number++;
    f->ack = acknowledgement;
    f->info = info;
    return f;
}

void set_interface_buffer(frame* f) {
    int receiver();
    interface_buffer = f;
    if(f->kind == data)
        receiver();
}

void send_frame(frame* f) {
    set_interface_buffer(f);
}

frame* receive_frame() {
    frame *temp = interface_buffer;
    interface_buffer = NULL;
    return temp;
}

int extract_data(frame* f) {
    return f->info;
}

int sender(void) {
    int info_buffer;
    frame *f;
    info_buffer = get_data();
    f = make_frame(data, info_buffer, 0);
    send_frame(f);
    f = receive_frame();
    if (f->kind == acknowledgement)
        printf("Sender: Acknowledgement received for packet number %d.\n", f->ack);
    free(f);
}

int receiver(void) {
    event_type event;
    frame* received;
    int info_buffer;
    int ack_no;
    wait_for_event(&event);
    received = receive_frame();
    info_buffer = extract_data(received);
    ack_no = received->seq;
    printf("Receiver: %d has been extracted.\n", info_buffer);
    free(received);
    frame* dummy = make_frame(acknowledgement, info_buffer, ack_no);
    send_frame(dummy);
}

int main() {
    int n_frames;
    printf("Enter the number of frames: ");
    scanf("%d", &n_frames);
    int counter = 0;
    while(counter < n_frames) {
        sender();
        ++counter;
    }
}
