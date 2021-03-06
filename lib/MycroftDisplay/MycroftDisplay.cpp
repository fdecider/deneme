/**                                                                         
#Copyright 2017, Mycroft AI, Inc.
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
**/
#include "MycroftDisplay.h"

MycroftDisplay::MycroftDisplay(int pinCS1, int pinWR, int pinDATA) {
	ht1632.begin(pinCS1, pinWR, pinDATA);
	ht1632.clear();
	ht1632.render();
}

void MycroftDisplay::drawFrame(const char (&IMG)[4][16]) {
	for (byte panel = 0; panel < NUM_PANELS; ++panel) {
		ht1632.drawImage(IMG[panel], PANEL_SX, PANEL_SY, PANEL_SX * panel, 0);
	}
	render();
}

void MycroftDisplay::drawFramePgm(byte index, const char (*IMG)[16]) {
	index *= NUM_PANELS;
	for (byte panel = 0; panel < NUM_PANELS; ++panel) {
		drawPanelPgm(PANEL_SX * panel, index + panel, IMG);
	}
	render();
}

void MycroftDisplay::drawIconPgm(byte pos, byte index, const char (*ICONS)[16]) {
	drawPanelPgm(pos, index, ICONS);
}

void MycroftDisplay::drawTextCentered(const String& text, bool small) {
	int xPixTotal = PANEL_SX*NUM_PANELS;	// 4 8x8 matrix displays
	int xWidth = ht1632.getTextWidth(text.c_str(), FONT_5X4_WIDTH, FONT_5X4_HEIGHT);
	int xStart = (xPixTotal - xWidth)/2;	// fonts are 4 pix wide

	drawText(text, xStart, small);
}

void MycroftDisplay::drawText(const String& text, int8_t pos, bool small) {
	if (small) {
		ht1632.drawTextPgm(text.c_str(), pos, 2, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_HEIGHT, FONT_5X4_STEP_GLYPH);
	} else {
		ht1632.drawTextPgm(text.c_str(), pos, 0, FONT_8X4, FONT_8X4_WIDTH, FONT_8X4_HEIGHT, FONT_8X4_STEP_GLYPH);
	}
}

void MycroftDisplay::setBrightness(int iLevel) {
	// level must be between 1-15
	if (iLevel < 1 || iLevel > 15)
		return;
	ht1632.setBrightness(iLevel);
}

void MycroftDisplay::slideText() {
	ht1632.transition(TRANSITION_BUFFER_SWAP);
}

void MycroftDisplay::clear() {
	ht1632.clear();
}

void MycroftDisplay::render() {
	ht1632.render();
}

void MycroftDisplay::drawPanelPgm(byte pos, byte index, const char (*IMG)[16]) {
	readBuffer(index, IMG);
	ht1632.drawImage(currentImage, PANEL_SX, PANEL_SY, pos, 0);
}

void MycroftDisplay::readBuffer(byte idx, const char (*IMG)[16]) {
	byte size = sizeof (currentImage);
	for (byte j = 0; j < size; j++) {
		currentImage[j] = (char) pgm_read_byte(&(IMG[idx][j]));
	}
}
