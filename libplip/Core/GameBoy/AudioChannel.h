/* AudioChannel.h
 *
 * Data structures that represent the DMG APU's audio channels.
 */

#pragma once

#include <array>
#include <cstdint>

namespace Plip::Core::GameBoy {
    // Common channel definition.
    // At least two channel types must share a property for it to appear here.
    struct AudioChannel {
        virtual ~AudioChannel() = default;

        constexpr static uint16_t MaxPeriod = 2048;
        constexpr static int MaxLength = 0;  // override me

        // Common values (settable)
        bool Enabled {};
        bool TimerEnabled {};
        bool PanLeft {};
        bool PanRight {};
        uint8_t InitialVolume {};
        uint8_t Volume {};

        // Common values (internal)
        uint16_t Length {};

        // Semi-common values :)
        uint16_t Period {};                 // P1 / P2 / W
        uint16_t PeriodDivider {};          // P1 / P2 / W
        bool EnvelopeIncrease {};           // P1 / P2 / N
        uint8_t EnvelopeSweepPace {};       // P1 / P2 / N

        int EnvelopeTimer {};               // P1 / P2 / N

        // Convenience methods.
        virtual void ClockChannel() { }

        virtual void DivApuPulse(const uint8_t divApu) {
            if(TimerEnabled && (divApu % 2 == 0) && Length > 0 && --Length == 0) {
                Enabled = false;
            }

            if(EnvelopeSweepPace > 0 && (divApu % 8 == 0)) {
                if(--EnvelopeTimer <= 0) {
                    EnvelopeTimer = EnvelopeSweepPace;
                    Volume += EnvelopeIncrease ? 1 : -1;

                    if(Volume == 255)  // wrap-around
                        Volume = 0;
                    else if(Volume > 15)
                        Volume = 15;
                }
            }
        }

        [[nodiscard]] uint8_t GetPanning() const {
            return (PanLeft ? 0x10 : 0) | (PanRight ? 0x01 : 0);
        }

        virtual void Reset() {
            Enabled = false;
            TimerEnabled = false;

            Period = PeriodDivider = 0;
            InitialVolume = Volume = 0;

            PanLeft = PanRight = false;

            EnvelopeIncrease = false;
            EnvelopeSweepPace = 0;
            EnvelopeTimer = 0;
        }

        void SetPanning(const uint8_t value) {
            PanLeft  = value & 0xF0;
            PanRight = value & 0x0F;
        }

    protected:
        bool IncrementPeriod() {
            if(++PeriodDivider == MaxPeriod) {
                PeriodDivider = Period;
                return true;
            }
            return false;
        }
    };

    struct PulseChannel : AudioChannel {
        static constexpr uint8_t Waveforms[4] = {
            0b11111110,  // 12.5% duty cycle
            0b01111110,  // 25% duty cycle
            0b01111000,  // 50% duty cycle
            0b10000001,  // 75% duty cycle
        };

        constexpr static int MaxLength = 63;

        uint8_t DutyCycle {};
        uint8_t DutyPosition {};

        // Period sweep. Only valid for channel 1 / pulse 1.
        bool Ch1SweepEnabled {};
        uint8_t Ch1SweepPace {};
        bool Ch1SweepSubtract {};
        uint8_t Ch1SweepStep {};
        uint8_t Ch1SweepTimer {};
        uint16_t Ch1PeriodShadow {};

        int Ch1SweepPaceCurrent {};

        // Convenience methods.
        void ClockChannel() override {
            AudioChannel::ClockChannel();

            if(IncrementPeriod()) {
                DutyPosition = (DutyPosition + 1) % 8;
            }
        }

        void DivApuPulse(const uint8_t divApu) override {
            AudioChannel::DivApuPulse(divApu);

            if(Ch1SweepEnabled && Ch1SweepPace > 0 && (divApu % 4) == 0) {
                ClockSweep();
            }
        }

        void ClockSweep() {
            if(--Ch1SweepPaceCurrent) return;
            Ch1SweepPaceCurrent = Ch1SweepPace;

            auto sweepFrequency = Ch1PeriodShadow >> Ch1SweepStep;
            if(Ch1SweepSubtract) sweepFrequency = -sweepFrequency;

            if(const auto newPeriod = Ch1PeriodShadow + sweepFrequency; newPeriod > 2047) {
                // Overflow has occurred. Disable the channel.
                Enabled = false;
            } else if(Ch1SweepStep != 0) {
                // Update the period.
                Ch1PeriodShadow = Period = newPeriod;
            }

            // Perform an immediate second overflow check. Don't write the value back this time.
            if(Ch1PeriodShadow + sweepFrequency > 2047) {
                Enabled = false;
            }
        }

        void Reset() override {
            AudioChannel::Reset();

            DutyCycle = DutyPosition = 0;

            Ch1SweepEnabled = Ch1SweepSubtract = false;
            Ch1SweepPace = Ch1SweepStep = Ch1SweepTimer = Ch1SweepPaceCurrent = 0;
        }
    };

    struct WaveChannel : AudioChannel {
        constexpr static int MaxLength = 256;

        bool DacEnabled {};
        uint8_t OutputLevel {};
        std::array<uint8_t, 16> WaveRam {};
        uint8_t WaveRamIndex {};

        // Convenience methods.
        void ClockChannel() override {
            AudioChannel::ClockChannel();

            if(IncrementPeriod()) {
                WaveRamIndex = (WaveRamIndex + 1) % 32;
            }
        }

        [[nodiscard]] uint8_t GetCurrentSample() const {
            // Wave RAM consists of 32 4-bit samples, stuffed into 16 bytes.
            // Extract the current sample and return it.
            const auto shift = (WaveRamIndex % 2 == 0) ? 4 : 0;
            return (WaveRam[WaveRamIndex / 2] >> shift) & 0b1111;
        }

        void Reset() override {
            AudioChannel::Reset();

            DacEnabled = false;
            OutputLevel = 0;
            WaveRamIndex = 0;
        }
    };

    struct NoiseChannel : AudioChannel {
        constexpr static int MaxLength = 63;
        constexpr static std::array<int, 8> Divisors = { 8, 16, 32, 48, 64, 80, 96, 112 };

        uint8_t ClockShift {};
        bool LFSRWidth7Bit {};  // false = 15-bit, true = 7-bit
        uint8_t ClockDivider {};
        uint16_t LFSRBits {};

        int NoiseTick {};

        // Convenience methods.
        void ClockChannel() override {
            AudioChannel::ClockChannel();

            if(++NoiseTick >= (Divisors[ClockDivider] << ClockShift) >> 1) {  // APU runs every 2 T-cycles
                NoiseTick = 0;
                TickLFSR();
            }
        }

        void TickLFSR() {
            const uint16_t lfsr0 = LFSRBits & 0b1;
            const uint16_t lfsr1 = (LFSRBits >> 1) & 0b1;
            LFSRBits |= ~(lfsr0 ^ lfsr1) << 15;  // bit 15 should already be clear
            if(LFSRWidth7Bit) {
                // Copy bit 15 to 7.
                LFSRBits = (LFSRBits & ~(1 << 7))       // remove bit 7
                         | ((LFSRBits & 0x8000) >> 8);  // isolate bit 15 and shift it to 7
            }
            LFSRBits >>= 1;
        }
    };
}
