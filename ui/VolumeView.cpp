#include <stdlib.h>
#include <stdio.h>
#include "VolumeView.h"
#include "peripherals/LCD.h"
#include "colors.h"


#define TEXT_Y 100
#define SPEAKER_Y 130
#define VOLBAR_Y 220
#define VOLBAR_WEIGHT 6
#define PADDING_X 100
#define PADDING_Y 70

VolumeView::VolumeView() : View(WIDTH, HEIGHT) {

}

void VolumeView::setValue(double volume) {

    renderRect(PADDING_X - 20, PADDING_Y, width - PADDING_X * 2 + 40, height - PADDING_Y * 2, DARK_GRAY);
    char text[15];
    sprintf(text, "VOLUME %d%%", (unsigned int) (volume * 100.0));
    renderText(text, CENTER, TEXT_Y, WHITE, DARK_GRAY);
    renderIcon(SPEAKER, CENTER, SPEAKER_Y, 2, WHITE, DARK_GRAY);

    renderRect(PADDING_X, VOLBAR_Y, WIDTH - PADDING_X * 2, VOLBAR_WEIGHT, DARK_GRAY);
    renderRect(PADDING_X, VOLBAR_Y, volume * (WIDTH - PADDING_X * 2), VOLBAR_WEIGHT, BLUE );

    View::updateScreen();
}

void VolumeView::update() {
    View::updateScreen();
}