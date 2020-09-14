#ifndef FADING_BLINKER_H
#define FADING_BLINKER_H

#include "Arduino.h"
#include "fadingblinker_data.hpp"

class FadingBlinker
{
public:

	FadingBlinker(uint8_t ledLeftPin, uint8_t ledRightPin) :m_leftPin(ledLeftPin), m_rightPin(ledRightPin), m_currState(INACTIVE), m_currTimerDirUp(true)
	{
		// set direction registers
		pinMode(m_leftPin, OUTPUT);
		pinMode(m_rightPin, OUTPUT);

		// set initial state
		digitalWrite(m_leftPin, HIGH);
		digitalWrite(m_rightPin, HIGH);

		m_maxOCR1A = pgm_read_word_near(fadingblinker_data + 255);
	}

	inline void activateLeft()
	{
		m_currState = LEFT_ACTIVE;
		m_setupTimer();
	}

	inline void activateRight()
	{
		m_currState = RIGHT_ACTIVE;
		m_setupTimer();
	}

	inline void activateBoth()
	{
		m_currState = BOTH_ACTIVE;
		m_setupTimer();
	}

	inline void deactivate()
	{
		m_currState = INACTIVE;
	}

	void inline timerCallbackCOMPA()
	{
		// turn LEDs on if required
		switch (m_currState)
		{
		case LEFT_ACTIVE:
			digitalWrite(m_leftPin, HIGH);
			m_advanceTimer();
			break;

		case RIGHT_ACTIVE:
			digitalWrite(m_rightPin, HIGH);
			m_advanceTimer();
			break;

		case BOTH_ACTIVE:
			digitalWrite(m_leftPin, HIGH);
			digitalWrite(m_rightPin, HIGH);
			m_advanceTimer();
			break;

		default:
			break;
		}
	}

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PA__)
	void inline timerCallbackCOMPB()
	{
		// turn LEDs off
		if (OCR1B < m_maxOCR1A)
		{
			digitalWrite(m_leftPin, LOW);
			digitalWrite(m_rightPin, LOW);
		}
	}
#elif defined (__AVR_ATmega4809__)
	void inline timerCallbackCOMPB()
	{
		// turn LEDs off
		if (TCA0.SINGLE.CMP0 < m_maxOCR1A | m_currState == INACTIVE)
		{
			digitalWrite(m_leftPin, LOW);
			digitalWrite(m_rightPin, LOW);
		}
	}
#endif

	// states
	static const uint8_t INACTIVE = 0;
	static const uint8_t LEFT_ACTIVE = 1;
	static const uint8_t RIGHT_ACTIVE = 2;
	static const uint8_t BOTH_ACTIVE = 3;

	// minimum timer value for glitch-free operation
	// FIXME: This is later on used as a table index rather than an actual timer value
	static const uint8_t MIN_VAL = 0x1A;

private:

	// register names are dependent on platform
#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PA__)
	inline void m_setupTimer()
	{
		m_currVal = MIN_VAL;
		m_currTimerDirUp = true;

		// interrupts for COMPA and COMPB
		TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B);

		// prescaler: 1
		TCCR1B = (1 << CS10);// | (1 << CS10);

		// CTC mode
		TCCR1A = 0x0000;
		TCCR1B |= (1 << WGM12);

		// CTC Limit
		OCR1A = m_maxOCR1A;
	}

	inline void m_advanceTimer()
	{
		// calculate new value
		if (m_currTimerDirUp)
		{
			if (m_currVal < 0xFF)
			{
				m_currVal++;
			}
			else
			{
				m_currTimerDirUp = false;
			}
		}

		if (!m_currTimerDirUp)
		{
			if (m_currVal > MIN_VAL)
			{
				m_currVal--;
			}
			else
			{
				m_currTimerDirUp = true;
				m_currVal++; // necessary because the first if() was missed
			}
		}

		// set timer value
		OCR1B = pgm_read_word_near(fadingblinker_data + m_currVal);
	}

#elif defined (__AVR_ATmega4809__)

	inline void m_setupTimer()
	{
		m_currVal = MIN_VAL;
		m_currTimerDirUp = true;

		// Reverse PORTMUX setting
		PORTMUX.TCAROUTEA &= ~(PORTMUX_TCA0_PORTB_gc);

		// Period setting
		// TODO: Is -1 still necessary?
		TCA0.SINGLE.PER = m_maxOCR1A - 1;

		// Interrupts
		TCA0.SINGLE.INTCTRL = (TCA_SINGLE_CMP0_bm | TCA_SINGLE_OVF_bm);
	}

	inline void m_advanceTimer()
	{
		// calculate new value
		if (m_currTimerDirUp)
		{
			if (m_currVal < 0xFF)
			{
				m_currVal++;
			}
			else
			{
				m_currTimerDirUp = false;
			}
		}

		if (!m_currTimerDirUp)
		{
			if (m_currVal > MIN_VAL)
			{
				m_currVal--;

			}
			else
			{
				m_currTimerDirUp = true;
				m_currVal++; // necessary because the first if() was missed
			}
		}

		// set timer value
		TCA0.SINGLE.CMP0 = pgm_read_word_near(fadingblinker_data + m_currVal);
	}
#endif

	// private members
	uint8_t m_leftPin;
	uint8_t m_rightPin;

	// blinking state
	// TODO: use an enum
	uint8_t m_currState;

	bool m_currTimerDirUp;
	uint8_t m_currVal;
	uint16_t m_maxOCR1A;
};
#endif
