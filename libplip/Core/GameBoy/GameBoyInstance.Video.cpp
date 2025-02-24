/* GameBoyInstance.Video.cpp
 *
 * Handles functions related to emulating the GameBoy's PPU.
 */

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::Plot(int color, const int pos) const {
    if(m_lcdOff) color = 0b00;

    switch(color) {
        case 0b00:  // White
            m_videoFormat.plot(m_videoBuffer, pos, 255, 255, 255);
            break;

        case 0b01:  // Light Grey
            m_videoFormat.plot(m_videoBuffer, pos, 172, 172, 172);
            break;

        case 0b10:  // Dark Gray
            m_videoFormat.plot(m_videoBuffer, pos, 86, 86, 86);
            break;

        case 0b11:  // Black
            m_videoFormat.plot(m_videoBuffer, pos, 0, 0, 0);
            break;

        default:  // wtf? Red, I guess?
            m_videoFormat.plot(m_videoBuffer, pos, 255, 0, 0);
            break;
    }
}

void GameBoyInstance::VideoCycle() {
    m_video->BeginDraw();
    m_video->Draw(m_videoBuffer);
    m_video->EndDraw();
}
