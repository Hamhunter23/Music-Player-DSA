#include "progressbar.h"
#include <stdio.h>
#include <unistd.h>

void progress_bar(int minutes, int seconds) {
    int total_seconds = minutes * 60 + seconds;
    int progress = 0;

    while (progress <= total_seconds) {
        int remaining_seconds = total_seconds - progress;
        int remaining_minutes = remaining_seconds / 60;
        remaining_seconds = remaining_seconds % 60;

        printf("\rProgress: [");
        int bar_width = (progress * 50) / total_seconds;
        for (int i = 0; i < bar_width; i++) {
            printf("=");
        }
        for (int i = bar_width; i < 50; i++) {
            printf(" ");
        }
        printf("] %02d:%02d", remaining_minutes, remaining_seconds);
        fflush(stdout);
        sleep(1);
        progress++;
    }

    printf("\n");
}